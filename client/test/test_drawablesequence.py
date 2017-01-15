from model.screen import DrawableSequence
import unittest


class MockDrawable(object): 

    def __init__(self, z, name):
        self.Z = z # mock Z
        self.name = name


class TestDrawableSequence(unittest.TestCase): 


    def setUp(self): 
        self.drawables = DrawableSequence()
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
            actual_order.append(node.drawable.name)

        self.assertEqual(expected_order, actual_order)
