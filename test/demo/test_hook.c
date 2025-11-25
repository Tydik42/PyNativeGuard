#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject* call_parse_tuple(PyObject* self, PyObject* args) {
    const char* str_arg;
    if (!PyArg_ParseTuple(args, "s", &str_arg)) {
        return NULL;
    }
    printf("[Extension] Inside C-module: got '%s'\n", str_arg);
    Py_RETURN_NONE;
}

static PyMethodDef TestMethods[] = {
    {"trigger", call_parse_tuple, METH_VARARGS, "Docs"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef testmodule = {
    PyModuleDef_HEAD_INIT, "test_hook", NULL, -1, TestMethods
};

PyMODINIT_FUNC PyInit_test_hook(void) {
    return PyModule_Create(&testmodule);
}