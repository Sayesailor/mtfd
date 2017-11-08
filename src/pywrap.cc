// Copyright 2017-2018 SeetaTech

#ifdef SHARED_LIB

#include <Python.h>
#include <cstdio>
#include <string>
#include <vector>
#include "include/domtfd.h"

extern "C" {
static PyObject *mtfd_proc(PyObject *self, PyObject *args) {
    char *execname = nullptr;
    char *src_path = nullptr;
    char *crop_path = nullptr;
    char *dest_path = nullptr;
#ifdef IMG_SUFF
    char *img_suff = nullptr;
#endif
    char *thread_cnt = nullptr;
    char *multi_face = nullptr;
    char *enable_crop = nullptr;
    char *model_path = nullptr;
    char *log_path = nullptr;
    char *mean_shape = nullptr;
    char *final_sz = nullptr;
    char *pthfn_lst = nullptr;
    if (PyArg_ParseTuple(args
#ifdef IMG_SUFF
                         ,
                         "ssssssssssss"
#else
                         ,
                         "sssssssssss"
#endif
                         ,
                         &src_path, &crop_path, &dest_path
#ifdef IMG_SUFF
                         ,
                         &img_suff
#endif
                         ,
                         &thread_cnt, &multi_face, &enable_crop, &model_path,
                         &log_path, &mean_shape, &final_sz, &pthfn_lst)) {
        std::vector<char *> argv;
        argv.push_back(execname);
        if (src_path) argv.push_back(src_path);
        if (crop_path) argv.push_back(crop_path);
        if (dest_path) argv.push_back(dest_path);
#ifdef IMG_SUFF
        if (img_suff) argv.push_back(img_suff);
#endif
        if (thread_cnt) argv.push_back(thread_cnt);
        if (multi_face) argv.push_back(multi_face);
        if (enable_crop) argv.push_back(enable_crop);
        if (model_path) argv.push_back(model_path);
        if (log_path) argv.push_back(log_path);
        if (mean_shape) argv.push_back(mean_shape);
        if (final_sz) argv.push_back(final_sz);
        if (pthfn_lst) argv.push_back(pthfn_lst);
        show_usage(argv.size(), &(argv[0]));
        domtfd();
    }
    Py_RETURN_TRUE;
}

#ifdef SUPPORT_PYTHON3
#define PYFUNC(name) \
    { #name, (PyCFunction) name, METH_VARARGS, "" }
#define PYENDFUNC \
    { nullptr, nullptr, 0, nullptr }

PyMethodDef *get_all_methods() {
    static PyMethodDef g_python_methods[]{
        PYFUNC(mtfd_proc), PYENDFUNC,
    };
    return g_python_methods;
}

static struct PyModuleDef libmtfd = {PyModuleDef_HEAD_INIT, "libmtfd", "", -1,
                                     get_all_methods()};

PyMODINIT_FUNC PyInit_libmtfd(void) { return PyModule_Create(&libmtfd); }

#else  // PYTHON2

#define PYFUNC(name) \
    { #name, name, METH_VARARGS, "" }
#define PYENDFUNC \
    { nullptr, nullptr, 0, nullptr }

PyMethodDef *get_all_methods() {
    static PyMethodDef g_python_methods[]{
        PYFUNC(mtfd_proc), PYENDFUNC,
    };
    return g_python_methods;
}

PyMODINIT_FUNC initlibmtfd() {
    PyObject *module = Py_InitModule("libmtfd", get_all_methods());
    if (module == nullptr) return;
}
#endif  // SUPPORT_PYTHON3
}

#endif
