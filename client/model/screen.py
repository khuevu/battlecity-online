
class Node(object): 

    def __init__(self, drawable): 
        self.drawable = drawable
        self.key = drawable.Z
        self.next = None
        self.prev = None


class DrawableSequence(object): 


    class SeqIterator(object): 

        def __init__(self, head): 
            self.cur = head

        def next(self):
            if self.cur: 
                node = self.cur
                self.cur = node.next
                return node
            else: 
                raise StopIteration

    def __init__(self): 
        self.list = None

    def __iter__(self): 
        return self.SeqIterator(self.list)

    def insert(self, drawable): 
        newNode = Node(drawable)
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
        self.drawables = DrawableSequence()

    def draw(self): 
        # clear old screen
        self.screen.fill([0, 0, 0])
        # draw active objects
        for node in self.drawables: 
            if node.drawble.state == Drawable.S_ACTIVE: 
                node.drawble.draw(self.screen)
            else: 
                self.drawables.remove(node)
        pygame.display.flip()

    def add(self, drawable): 
        self.drawables.insert(drawble)


