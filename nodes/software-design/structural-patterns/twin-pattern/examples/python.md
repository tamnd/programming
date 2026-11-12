```python run id=twin-ball
class Item:
    def __init__(self, name):
        self.name = name

    def show(self):  # template method: position() comes from the subclass
        print(f"{self.name} at {self.position()}")


class Ticker:
    def __init__(self):
        self.count = 0

    def tick(self):
        self.count += 1
        self.on_tick()


class BallItem(Item):
    def __init__(self, name):
        super().__init__(name)
        self.x = 0
        self.twin = BallTicker(self)

    def position(self):
        return self.x

    def move(self):
        self.x += 2


class BallTicker(Ticker):
    def __init__(self, twin):
        super().__init__()
        self.twin = twin

    def on_tick(self):
        self.twin.move()


ball = BallItem("ball")
ball.show()
for _ in range(2):
    ball.twin.tick()
    ball.show()
print(f"ticks: {ball.twin.count}")
```

```output
ball at 0
ball at 2
ball at 4
ticks: 2
```

- Verdict: idiom. Python has multiple inheritance, so `class Ball(Item, Ticker)` also works. The twin form keeps the names of the two parents apart.
