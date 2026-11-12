```python run id=consultants
class Consultant:
    def __init__(self, name, home, available_at_home, location):
        self.name, self.home = name, home
        self.available_at_home, self.location = available_at_home, location
        self.location_changed, self.availability_changed = [], []

    def available_at(self, place):
        return self.available_at_home if place == self.home else True

    def move(self, place):
        print(f"{self.name} moves to {place}")
        if self.location != place:
            for h in self.location_changed:
                h(self)
            if self.available_at(place) != self.available_at(self.location):
                for h in self.availability_changed:
                    h(self)
            self.location = place


class EventAggregator:
    def __init__(self):
        self.changed, self.location, self.availability = [], [], []

    def listen(self, consultant):
        consultant.location_changed.append(lambda c: self._signal(self.location, c))
        consultant.availability_changed.append(lambda c: self._signal(self.availability, c))

    def _signal(self, specific, c):
        for h in specific:  # the specific event
            h(c)
        for h in self.changed:  # and the general one
            h(c)


agg = EventAggregator()
alice = Consultant("alice", "London", False, "London")
bob = Consultant("bob", "Paris", True, "Lyon")
for c in (alice, bob):
    agg.listen(c)

agg.changed.append(lambda c: print(f"screen: {c.name} changed"))
agg.availability.append(lambda c: print(f"phone: availability of {c.name} changed"))

alice.move("Leeds")
bob.move("Paris")
```

```output for=consultants
alice moves to Leeds
screen: alice changed
phone: availability of alice changed
screen: alice changed
bob moves to Paris
screen: bob changed
```

- Verdict: idiom. Lists of functions are the events.
