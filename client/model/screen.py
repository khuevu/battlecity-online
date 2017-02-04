import pygame 
from model import Drawable


class OrderedSequence(object): 
    """ Container contains the children of a Screen.

    The container ensure the elements are sorted in the order of its type 
    Z coordinate. 
    """

    class Node(object): 
        """ Node element of the container. """
    
        def __init__(self, elem, key): 
            self.elem = elem
            self.key = key
            self.next = None
            self.prev = None


    class SeqIterator(object): 
        """ Iterator of the container. """

        def __init__(self, head): 
            self.cur = head

        def next(self):
            if self.cur: 
                node = self.cur
                self.cur = node.next
                return node
            else: 
                raise StopIteration

    def __init__(self, key): 
        self.list = None
        self.key = key

    def __iter__(self): 
        return self.SeqIterator(self.list)

    def insert(self, elem): 
        """ Insert a elem element in the order based on its key. """
        new_node = self.Node(elem, self.key(elem))
        if not self.list: 
            self.list = new_node
        else: 
            prev = None
            cur = self.list
            # find position to insert the new node
            while cur and cur.key <= new_node.key: 
                prev = cur
                cur = cur.next
            
            if not prev: 
                # insert at the begining of the list
                new_node.next = self.list
                self.list.prev = new_node
                self.list = new_node

            elif not cur: 
                # insert at the end of the list
                prev.next = new_node
                new_node.prev = prev

            else: 
                # insert in between
                prev.next = new_node
                new_node.prev = prev
                new_node.next = cur
                cur.prev = new_node

        return new_node
                
    def remove(self, node): 
        """ Remove the node from the container. """
        p = node.prev
        n = node.next
        if p: 
            p.next = n
        if n:
            n.prev = p


class GameScreen(object): 
    """ Define the type of game's screens """

    def __init__(self, display): 
        self.display = display 
        self.drawableSeq = OrderedSequence(key=lambda d: d.Z)

    def draw(self): 
        """ Paint the GameScreen. """
        # Clear old screen
        self.display.fill([0, 0, 0])
        # Draw active objects by iterating through the order of 
        # the OrderedSequence container so that the lower objects
        # are drawn first. 
        for node in self.drawableSeq: 
            if node.elem.state == Drawable.S_ACTIVE: 
                node.elem.draw(self.display)
            else: 
                self.drawableSeq.remove(node)
        pygame.display.flip()

    def add(self, drawable): 
        """ Add Element to the screen. """
        self.drawableSeq.insert(drawable)
