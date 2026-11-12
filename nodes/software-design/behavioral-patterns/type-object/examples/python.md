```python run id=breeds
class Breed:
    def __init__(self, name, health, attack):
        self.name, self.health, self.attack = name, health, attack

    def new_monster(self):
        return Monster(self)


class Monster:
    def __init__(self, breed):
        self.breed = breed
        self.health = breed.health  # each monster has its own health

    def describe(self):
        b = self.breed
        return f"{b.name}: {self.health} health, attacks with {b.attack}"


# New kinds are data, for example from a file. No new class is needed.
table = [
    ("goblin", 20, "a dagger"),
    ("dragon", 200, "fire"),
    ("wyvern", 120, "its tail"),
]
breeds = [Breed(*row) for row in table]
for breed in breeds:
    print(breed.new_monster().describe())
```

```output for=breeds
goblin: 20 health, attacks with a dagger
dragon: 200 health, attacks with fire
wyvern: 120 health, attacks with its tail
```

- Verdict: idiom. A class with one instance for each kind.
