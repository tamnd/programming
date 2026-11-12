```python run id=guarded-suspension
import threading


class Tank:
    def __init__(self):
        self._cond = threading.Condition()
        self._level = 0
        self.lowest = 0  # the lowest level seen after a take
        self.filled = 0
        self.taken = 0
        self.served = 0

    def take(self, amount):
        with self._cond:
            # Guarded suspension: wait until the guard is true.
            self._cond.wait_for(lambda: self._level >= amount)
            self._level -= amount
            self.lowest = min(self.lowest, self._level)
            self.taken += amount
            self.served += 1

    def fill(self, amount):
        with self._cond:
            self._level += amount
            self.filled += amount
            self._cond.notify_all()  # each waiting taker checks its guard again

    def level(self):
        with self._cond:
            return self._level


tank = Tank()
amounts = [5, 3, 8]
takers = [threading.Thread(target=tank.take, args=(a,)) for a in amounts]
for t in takers:
    t.start()
for _ in range(10):
    tank.fill(2)
for t in takers:
    t.join()

print("units filled:", tank.filled)
print("units taken:", tank.taken)
print("takers served:", tank.served)
print("level never below 0:", tank.lowest >= 0)
print("units left:", tank.level())
```

```output
units filled: 20
units taken: 16
takers served: 3
level never below 0: True
units left: 4
```

- Verdict: stdlib. `threading.Condition` gives `wait_for`, which waits until the guard is true and tests it again after each wakeup.
