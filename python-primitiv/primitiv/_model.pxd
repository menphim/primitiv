from libcpp.vector cimport vector
from libcpp.string cimport string

from primitiv._parameter cimport CppParameter


cdef extern from "primitiv/model.h":
    cdef cppclass CppModel "primitiv::Model":
        CppModel() except +
        void add_parameter(string &name, CppParameter &param) except +
        void add_submodel(string &name, CppModel &model) except +
        vector[CppParameter*] get_trainable_parameters() except +


cdef class _Model:
    cdef CppModel *wrapped
