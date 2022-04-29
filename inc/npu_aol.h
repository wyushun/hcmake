#pragma once

#include <cstdint>

/*
 * define NPU AOL status code
 */
#define NPU_AOL_OK (0)
#define NPU_AOL_ERROR (-1)

// #define SUPPORT_IP_MAX_COUNT 16
// #define SUPPORT_CORE_MAX_COUNT 32

typedef struct {
  uint32_t size;
  uint64_t addr_phy;
  uint64_t addr_virt;
} npu_aol_dev_mem_t;

typedef struct {
  uint32_t
      aol_version; /*[Output] The version of AOL interface, fixed to 0x0100 */
  uint8_t core_count;     /*[Output] The core count of each related NPU IP. */
  uint64_t core_phy_addr; /*[Output] The physical address of each IP core. */
} npu_aol_dev_handle_t;

typedef struct {
  uint32_t offset;
  uint32_t value;
} npu_aol_reg_t;

#define NPU_AOL_REG_NUM 32
typedef struct {
  uint64_t time_start; /*[Output] The start timestamp in nano-second */
  uint64_t time_end;   /*[Output] The end timestamp in nano-second */
  uint32_t timeout; /*[Input] The timeout setting for IP computing in second */
  uint32_t
      profiler_en;    /*[Input] Specify the core profiler function to be used*/
  uint32_t reg_count; /*[Input] Specify the count of registers to be written. No
                         more than NPU_AOL_REG_NUM. */
  npu_aol_reg_t
      regs[NPU_AOL_REG_NUM]; /*[Input] The registers data buffer to be written.
                                The actual count is specified by reg_count. */
} npu_aol_run_t;

typedef struct {
  uint32_t reg_count; /*[Input] Specify the count of registers to be written. No
                         more than NPU_AOL_REG_NUM. */
  npu_aol_reg_t
      regs[NPU_AOL_REG_NUM]; /*[Input] The registers data buffer to be written.
                                The actual count is specified by reg_count. */
  uint32_t regs_delay_us[NPU_AOL_REG_NUM]; /*[Input] The delay time array in
                                              microsecond after writing each
                                              register specified by regs. */
} npu_aol_init_t;

struct req_mem_alloc_t {
  unsigned long size;     /* size of memory space to be allocated */
  unsigned long addr_phy; /* suv the start pyhsical address of allocated NPU
                             memory (RETURNED) */
};

struct req_mem_free_t {
  unsigned long
      addr_phy; /* the start pyhsical address of allocated NPU memory */
};

typedef struct {
  unsigned long addr_phy; /* physical address of memory range */
  unsigned long size;     /* size of memory range */
} req_cache_ctrl_t;

#define READ_REG_DEFAULT_BUF_LEN 64
typedef struct {
  uint64_t phy_address;
  uint32_t size;
  uint32_t out_buffer[READ_REG_DEFAULT_BUF_LEN];
} npu_aol_read_regs_t;

#define NPU_IOCTL_MAGIC 'N'

/* allocate NPU memory */
#define REQ_NPU_MEM_ALLOC _IOWR(NPU_IOCTL_MAGIC, 1, struct req_mem_alloc_t *)
/* free NPU memory */
#define REQ_NPU_MEM_FREE _IOWR(NPU_IOCTL_MAGIC, 2, struct req_mem_free_t *)
/* run NPU */
#define REQ_NPU_RUN _IOWR(NPU_IOCTL_MAGIC, 3, npu_aol_run_t *)
/* init npu registers */
#define REQ_NPU_INIT _IOWR(NPU_IOCTL_MAGIC, 4, npu_aol_init_t *)
/* Memory accessible from the CPU, synchronized to memory that the device can
 * access */
#define REQ_SYNC_TO_DEV _IOWR(NPU_IOCTL_MAGIC, 5, req_cache_ctrl_t *)
/* Memory accessible from the device, synchronized back to the memory that the
 * CPU can access */
#define REQ_SYNC_FROM_DEV _IOWR(NPU_IOCTL_MAGIC, 6, req_cache_ctrl_t *)
/* Get the cores physical address */
#define REQ_GET_DEV_HANDLE _IOWR(NPU_IOCTL_MAGIC, 7, npu_aol_dev_handle_t *)
/* read the registers of the IPs */
#define REQ_NPU_READ_REGS _IOWR(NPU_IOCTL_MAGIC, 8, npu_aol_read_regs_t *)

/* Attach NPU device and other IPs.
 * Input:
 *     mode - Conventional the NPU scheduling mode. Only
 * NPU_SCHEDULE_MODE_SINGLE used currently. Return: The device handle that the
 * subsequent function needs to usd.
 */
#define NPU_SCHEDULE_MODE_SINGLE 1
npu_aol_dev_handle_t *npu_aol_attach(uint8_t id, uint32_t mode);

/* Detach NPU device and other IPs.
 * Input:
 *     dev - The device handle obtained by npu_aol_attach.
 * Return:
 *     The device handle that the subsequent function needs to usd.
 */
int npu_aol_detach(npu_aol_dev_handle_t *dev);

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
                      uint32_t *buf, uint32_t count);

/* Initialize NPU or other IPs. It may be called when the IP first starts or
 * times out. Input: dev - The device handle obtained by npu_aol_attach.
 * Input/Output:
 *     data - The data required for this scheduling. See npu_aol_init_t for
 * detail. Return: NPU_AOL_OK for success, NPU_AOL_ERROR for failure.
 */
int npu_aol_init(npu_aol_dev_handle_t *dev, npu_aol_init_t *data);

/* Make a NPU or other IP schedule.
 * Input:
 *     dev - The device handle obtained by npu_aol_attach.
 * Input/Output:
 *     data - The data required for this scheduling. See npu_aol_run_t for
 * detail. Return: NPU_AOL_OK for success, NPU_AOL_ERROR for timeout.
 */
int npu_aol_run(npu_aol_dev_handle_t *dev, npu_aol_run_t *data);

/* Allocate physically contiguous DMA device memory.
 * Input:
 *     dev - The device handle obtained by npu_aol_attach.
 *     size - Byte length of the memory to be allocated.
 *     prot - Fxied to (NPU_AOL_MEM_PROT_READ|NPU_AOL_MEM_PROT_WRITE) in this
 * version. Return: The handle of the requedsted memory. NULL for failure.
 */
#define NPU_AOL_MEM_PROT_NONE 0
#define NPU_AOL_MEM_PROT_READ 1
#define NPU_AOL_MEM_PROT_WRITE 2
#define NPU_AOL_MEM_PROT_EXEC 4
npu_aol_dev_mem_t *npu_aol_alloc_dev_mem(npu_aol_dev_handle_t *dev,
                                         uint64_t size, uint32_t prot);

/* Free physically contiguous DMA device memory.
 * Input:
 *     dev - The device handle obtained by npu_aol_attach.
 *     mem - The memory handle obtained by npu_aol_alloc_dev_mem.
 * Return:
 *     NPU_AOL_OK for success, NPU_AOL_ERROR for failure.
 */
int npu_aol_free_dev_mem(npu_aol_dev_handle_t *dev, npu_aol_dev_mem_t *mem);

/* Memory accessible from the CPU, synchronized to memory that the device can
 * access. Input: dev - The device handle obtained by npu_aol_attach. mem - The
 * memory handle obtained by npu_aol_alloc_dev_mem. offset - The byte offset of
 * memory address from mem->addr_phy needs to be flushed. size - The byte length
 * of memory needs to be flushed. Return: NPU_AOL_OK for success, NPU_AOL_ERROR
 * for failure.
 */
int npu_aol_sync_to_dev(npu_aol_dev_handle_t *dev, const npu_aol_dev_mem_t *mem,
                        uint32_t offset, uint32_t size);

/* Memory accessible from the device, synchronized back to the memory that the
 * CPU can access. Input: dev - The device handle obtained by npu_aol_attach.
 *     mem - The memory handle obtained by npu_aol_alloc_dev_mem.
 *     offset - The byte offset of memory address from mem->addr_phy needs to be
 * flushed. size - The byte length of memory needs to be flushed. Return:
 *     NPU_AOL_OK for success, NPU_AOL_ERROR for failure.
 */
int npu_aol_sync_from_dev(npu_aol_dev_handle_t *dev, npu_aol_dev_mem_t *mem,
                          uint32_t offset, uint32_t size);
