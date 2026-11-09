class Point:
    __match_args__ = ("x", "y")

    def __init__(self, x, y):
        self.x = x
        self.y = y


match Point(1, 2):
    case Point(1, y):
        print(y)
