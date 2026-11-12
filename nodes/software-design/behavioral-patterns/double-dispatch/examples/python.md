```python run id=collisions
class Ship:
    name = "ship"

    def collide(self, other):
        return other.collide_with_ship(self)  # second dispatch, on other

    def collide_with_ship(self, ship):
        return "ships bounce"

    def collide_with_asteroid(self, asteroid):
        return "ship takes damage"


class Asteroid:
    name = "asteroid"

    def collide(self, other):
        return other.collide_with_asteroid(self)

    def collide_with_ship(self, ship):
        return "ship takes damage"

    def collide_with_asteroid(self, asteroid):
        return "asteroids break apart"


pairs = [(Ship(), Asteroid()), (Asteroid(), Ship()), (Asteroid(), Asteroid()), (Ship(), Ship())]
for a, b in pairs:
    print(f"{a.name} + {b.name}: {a.collide(b)}")
```

```output
ship + asteroid: ship takes damage
asteroid + ship: ship takes damage
asteroid + asteroid: asteroids break apart
ship + ship: ships bounce
```

- Verdict: idiom. Python dispatches a method call on the receiver only, so `collide` calls back the other object. `functools.singledispatch` dispatches on the first argument only, so it does not give double dispatch.
