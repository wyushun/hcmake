 pulls in the Python API
 #include <Python.h>

 // C function always has two arguments, conventionally named self and args
 // The args argument will be a pointer to a Python tuple object containing the arguments.
 // Each item of the tuple corresponds to an argument in the call’s argument list.
 static PyObject *
 demo_add(PyObject *self, PyObject *args)
 {
     const int a, b;
         // convert PyObject to C values
             if (!PyArg_ParseTuple(args, "ii", &a, &b))
                     return NULL;
                         return Py_BuildValue("i", a+b);
                         }

                         // module's method table
                         static PyMethodDef DemoMethods[] = {
                             {"add", demo_add, METH_VARARGS, "Add two integers"},
                                 {NULL, NULL, 0, NULL}
                                 };

                                 // module’s initialization function
                                 PyMODINIT_FUNC
                                 initdemo(void)
                                 {
                                     (void)Py_InitModule("demo", DemoMethods);
                                     }
                                 }
                         }
 }
