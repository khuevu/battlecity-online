from model.screen import OrderedSequence
import unittest


class MockDrawable(object): 

    def __init__(self, z, name):
        self.Z = z # mock Z
        self.name = name


class TestOrderedSequence(unittest.TestCase): 
    """ Test whether a sequence of drawable objects can be 
    iterated in the correct order of their z coordinates. """


    def setUp(self): 
        self.drawables = OrderedSequence(key=lambda d: d.Z)
        self.seq = [MockDrawable(z=0, name=1), MockDrawable(z=0, name=2),
                MockDrawable(z=9, name=5), MockDrawable(z=5, name=4)]
        for mock in self.seq: 
            self.drawables.insert(mock)

    def tearDown(self): 
        self.drawables = None

    def test_insert_drawable(self): 
        self.seq.append(MockDrawable(z=12, name=7))
        self.seq.append(MockDrawable(z=8, name=4.5))
        self.drawables.insert(self.seq[-1])
        self.drawables.insert(self.seq[-2])
        
        expected_order = [o.name for o in sorted(self.seq, key=lambda d: d.Z)]
        actual_order = []
        for i, node in enumerate(self.drawables):
            #print i, node.drawable.name
            actual_order.append(node.elem.name)

        self.assertEqual(expected_order, actual_order)
