#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include "npu_aol.h"

#define NPU_DEV_NAME "/dev/npu"
#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)

typedef struct {
  npu_aol_dev_handle_t device;
  int fd;
} npu_aol_linux_t;

/*
 * Modify the size of memory segment for system page size alignment
 */
unsigned long align_for_page_size(unsigned long size) {
  if (size % PAGE_SIZE) {
    return ((size / PAGE_SIZE) * PAGE_SIZE + PAGE_SIZE);
  }

  return size;
}

/* Attach NPU device and other IPs.
 * Input:
 *     id - npu core id: 0 - 3
 *     mode - Conventional the NPU scheduling mode. Only
 * NPU_SCHEDULE_MODE_SINGLE used currently. Return: The device handle that the
 * subsequent function needs to usd.
 */
npu_aol_dev_handle_t *npu_aol_attach(uint8_t id, uint32_t mode) {
  int fd;
  int ret;
  char device_name_buf[64];

  npu_aol_linux_t *p = NULL;

  if (mode != NPU_SCHEDULE_MODE_SINGLE) {
    return NULL;
  }

  p = (npu_aol_linux_t *)malloc(sizeof(npu_aol_linux_t));
  memset(p, 0, sizeof(npu_aol_linux_t));

  snprintf(device_name_buf, sizeof(device_name_buf), "%s%d", NPU_DEV_NAME, id);

  fd = open(device_name_buf, O_RDWR | O_SYNC, 0);
  if (fd < 0) {
    free(p);
    return NULL;
  }
  p->fd = fd;

  // Get NPU handle
  ret = ioctl(p->fd, REQ_GET_DEV_HANDLE, (void *)&p->device);
  if (ret < 0) {
    close(fd);
    free(p);
    return NULL;
  }

  p->device.aol_version = 0x0100;
  return &(p->device);
}

/* Detach NPU device and other IPs.
 * Input:
 *     dev - The device handle obtained by npu_aol_attach.
 * Return:
 *     The device handle that the subsequent function needs to usd.
 */
int npu_aol_detach(npu_aol_dev_handle_t *dev) {
  int ret;
  npu_aol_linux_t *p;

  if (dev == NULL) {
    return NPU_AOL_ERROR;
  }

  p = (npu_aol_linux_t *)dev;
  ret = close(p->fd);
  free(p);

  if (ret) {
    return NPU_AOL_ERROR;
  }

  return NPU_AOL_OK;
}

/* Read the NPU related IPs registers in word.
 * Input:
 *     dev - The device handle obtained by npu_aol_attach.
 *     phy_address - The physical address of the registers to be read.
 *     count - Byte lenght of the read register data.
 * Output:
 *     buf - The output buffer in word.
 * Return:
 *     NPU_AOL_OK for success, NPU_AOL_ERROR for failure.
 */
int npu_aol_read_regs(npu_aol_dev_handle_t *dev, uint64_t phy_address,
                      uint32_t *buf, uint32_t count) {
  int i, ret;
  npu_aol_linux_t *p = (npu_aol_linux_t *)dev;
  npu_aol_read_regs_t read_reg;

  if ((dev == NULL) || ((count >> 2) > READ_REG_DEFAULT_BUF_LEN)) {
    return -1;
  }

  // Get NPU handle
  read_reg.size = count;
  read_reg.phy_address = phy_address;
  ret = ioctl(p->fd, REQ_NPU_READ_REGS, (void *)&read_reg);
  if (ret < 0) {
    return -1;
  }
  for (i = 0; i < (int)(count >> 2); i++) {
    buf[i] = read_reg.out_buffer[i];
  }

  return 0;
}

/* Allocate physically contiguous DMA device memory.
 * Input:
 *     dev - The device handle obtained by npu_aol_attach.
 *     size - Byte length of the memory to be allocated.
 *     prot - Fixed to (NPU_AOL_MEM_PROT_READ|NPU_AOL_MEM_PROT_WRITE) in this
 * version. Return: The handle of the requedsted memory. NULL for failure.
 */
npu_aol_dev_mem_t *npu_aol_alloc_dev_mem(npu_aol_dev_handle_t *dev,
                                         uint64_t size, uint32_t prot) {
  int ret;
  void *map;
  npu_aol_linux_t *p;
  struct req_mem_alloc_t req_alloc;

  (void)prot;

  req_alloc.size = align_for_page_size(size);
  p = (npu_aol_linux_t *)dev;
  ret = ioctl(p->fd, REQ_NPU_MEM_ALLOC, (void *)&req_alloc);
  if (ret < 0) {
    return NULL;
  }

  npu_aol_dev_mem_t *mem =
      (npu_aol_dev_mem_t *)malloc(sizeof(npu_aol_dev_mem_t));
  mem->size = req_alloc.size;
  mem->addr_phy = req_alloc.addr_phy;
  map = mmap(NULL, mem->size, PROT_READ | PROT_WRITE, MAP_SHARED, p->fd,
             mem->addr_phy);
  if (map == MAP_FAILED) {
    free(mem);
    return NULL;
  }

  mem->addr_virt = (uint64_t)map;
  return mem;
}

/* Free physically contiguous DMA device memory.
 * Input:
 *     dev - The device handle obtained by npu_aol_attach.
 *     mem - The memory handle obtained by npu_aol_alloc_dev_mem.
 * Return:
 *     NPU_AOL_OK for success, NPU_AOL_ERROR for failure.
 */
int npu_aol_free_dev_mem(npu_aol_dev_handle_t *dev, npu_aol_dev_mem_t *mem) {
  int ret;
  npu_aol_linux_t *p;
  struct req_mem_free_t req_free;

  ret = munmap((void *)mem->addr_virt, mem->size);
  if (ret != 0) {
    return NPU_AOL_ERROR;
  }

  req_free.addr_phy = mem->addr_phy;
  p = (npu_aol_linux_t *)dev;
  ret = ioctl(p->fd, REQ_NPU_MEM_FREE, (void *)&req_free);
  if (ret < 0) {
    return NPU_AOL_ERROR;
  }

  return NPU_AOL_OK;
}

/* Memory accessible from the CPU, synchronized to memory that the device can
 * access. Input: dev - The device handle obtained by npu_aol_attach. mem - The
 * memory handle obtained by npu_aol_alloc_dev_mem. offset - The byte offset of
 * memory address from mem->addr_phy needs to be flushed. size - The byte length
 * of memory needs to be flushed. Return: NPU_AOL_OK for success, NPU_AOL_ERROR
 * for failure.
 */
int npu_aol_sync_to_dev(npu_aol_dev_handle_t *dev, const npu_aol_dev_mem_t *mem,
                        uint32_t offset, uint32_t size) {
  int ret;
  req_cache_ctrl_t cache_ctrl;
  npu_aol_linux_t *p = (npu_aol_linux_t *)dev;

  cache_ctrl.size = size;
  cache_ctrl.addr_phy = (unsigned long)(mem->addr_phy + offset);
  ret = ioctl(p->fd, REQ_SYNC_TO_DEV, (void *)&cache_ctrl);
  if (ret >= 0) {
    return NPU_AOL_OK;
  }

  return NPU_AOL_ERROR;
}

/* Memory accessible from the device, synchronized back to the memory that the
 * CPU can access. Input: dev - The device handle obtained by npu_aol_attach.
 *     mem - The memory handle obtained by npu_aol_alloc_dev_mem.
 *     offset - The byte offset of memory address from mem->addr_phy needs to be
 * flushed. size - The byte length of memory needs to be flushed. Return:
 *     NPU_AOL_OK for success, NPU_AOL_ERROR for failure.
 */
int npu_aol_sync_from_dev(npu_aol_dev_handle_t *dev, npu_aol_dev_mem_t *mem,
                          uint32_t offset, uint32_t size) {
  int ret;
  req_cache_ctrl_t cache_ctrl;
  npu_aol_linux_t *p = (npu_aol_linux_t *)dev;

  cache_ctrl.size = size;
  cache_ctrl.addr_phy = (unsigned long)(mem->addr_phy + offset);
  ret = ioctl(p->fd, REQ_SYNC_FROM_DEV, (void *)&cache_ctrl);
  if (ret >= 0) {
    return NPU_AOL_OK;
  }

  return NPU_AOL_ERROR;
}

/* Make a NPU or other IP schedule.
 * Input:
 *     dev - The device handle obtained by npu_aol_attach.
 * Input/Output:
 *     data - The data required for this scheduling. See npu_aol_run_t for
 * detail. Return: NPU_AOL_OK for success, NPU_AOL_ERROR for timeout.
 */
int npu_aol_run(npu_aol_dev_handle_t *dev, npu_aol_run_t *data) {
  int ret;
  npu_aol_linux_t *p = (npu_aol_linux_t *)dev;

  ret = ioctl(p->fd, REQ_NPU_RUN, (void *)data);
  if (ret < 0) {
    return NPU_AOL_ERROR;
  }

  return NPU_AOL_OK;
}

/* Initialize NPU or other IPs. It may be called when the IP first starts or
 * times out. Input: dev - The device handle obtained by npu_aol_attach.
 * Input/Output:
 *     data - The data required for this scheduling. See npu_aol_init_t for
 * detail. Return: NPU_AOL_OK for success, NPU_AOL_ERROR for failure.
 */
int npu_aol_init(npu_aol_dev_handle_t *dev, npu_aol_init_t *data) {
  int ret;
  npu_aol_linux_t *p = (npu_aol_linux_t *)dev;

  ret = ioctl(p->fd, REQ_NPU_INIT, (void *)data);
  if (ret < 0) {
    return ret;
  }

  return NPU_AOL_OK;
}
