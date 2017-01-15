import pygame 




class OrderedSequence(object): 
    """ Container contains the children of a Screen.

    The children must be of type elem.

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

        newNode = self.Node(elem, self.key(elem))
        if not self.list: 
            self.list = newNode
        else: 
            prev = None
            cur = self.list
            # find position to insert the new node
            while cur and cur.key <= newNode.key: 
                prev = cur
                cur = cur.next
            
            if not prev: 
                # insert at the begining of the list
                newNode.next = self.list
                self.list.prev = newNode
                self.list = newNode

            elif not cur: 
                # insert at the end of the list
                prev.next = newNode
                newNode.prev = prev

            else: 
                # insert in between
                prev.next = newNode
                newNode.prev = prev
                newNode.next = cur
                cur.prev = newNode

        return newNode
                
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

    def __init__(self, screen): 
        self.screen = screen
        self.drawableSeq = OrderedSequence()

    def draw(self): 
        # Clear old screen
        self.screen.fill([0, 0, 0])
        # Draw active objects by iterating through the order of 
        # the OrderedSequence container so that the lower objects
        # are drawn first. 
        for node in self.drawableSeq: 
            if node.drawble.state == Drawable.S_ACTIVE: 
                node.drawble.draw(self.screen)
            else: 
                self.drawableSeq.remove(node)
        pygame.display.flip()

    def add(self, drawable): 
        self.drawableSeq.insert(drawble)


