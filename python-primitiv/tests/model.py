from primitiv import Device
from primitiv import devices as D
from primitiv import Graph
from primitiv import Parameter
from primitiv import Model

import unittest
import tempfile


class TestChildModel(Model):
    def __init__(self):
        super().__init__()
        self.c1 = Parameter()
        self.c2 = Parameter()
        super().add_parameter("c1", self.c1)
        super().add_parameter("c2", self.c2)


class TestParentModel(Model):
    def __init__(self):
        super().__init__()
        self.p1 = Parameter()
        self.p2 = Parameter()
        self.sub1 = TestChildModel()
        super().add_parameter("p1", self.p1)
        super().add_parameter("p2", self.p2)
        super().add_submodel("sub1", self.sub1)


class ModelTest(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        pass

    @classmethod
    def tearDownClass(cls):
        pass

    def setUp(self):
        self.dev = D.Naive()
        Device.set_default(self.dev)
        self.g = Graph()
        Graph.set_default(self.g)

    def tearDown(self):
        pass

    def test_model(self):
        m = TestParentModel()
