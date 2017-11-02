from libcpp.vector cimport vector

from primitiv._parameter cimport CppParameter, _Parameter


cdef class _Model:

    def __init__(self):
        if self.wrapped is not NULL:
            raise TypeError("__init__() has already been called.")
        self.wrapped = new CppModel()

    def __dealloc__(self):
        if self.wrapped is not NULL:
            del self.wrapped
            self.wrapped = NULL

    def add_parameter(self, str name, _Parameter param):
        self.wrapped.add_parameter(name.encode("utf-8"), param.wrapped[0])

    def add_submodel(self, str name, _Model model):
        self.wrapped.add_submodel(name.encode("utf-8"), model.wrapped[0])

    def get_trainable_parameters(self):
        cdef CppParameter *param
        return [_Parameter.get_wrapper(param) for param in self.wrapped.get_trainable_parameters()]

    def __copy__(self):
        raise NotImplementedError(type(self).__name__ + " does not support `__copy__` for now.")

    def __deepcopy__(self, memo):
        raise NotImplementedError(type(self).__name__ + " does not support `__deepcopy__` for now.")
