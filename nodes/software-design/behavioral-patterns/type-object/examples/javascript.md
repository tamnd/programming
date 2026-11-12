```javascript run id=breeds
class Breed {
  constructor(name, health, attack) {
    this.name = name;
    this.health = health;
    this.attack = attack;
  }
  newMonster() {
    return new Monster(this);
  }
}

class Monster {
  constructor(breed) {
    this.breed = breed;
    this.health = breed.health; // each monster has its own health
  }
  describe() {
    const b = this.breed;
    return `${b.name}: ${this.health} health, attacks with ${b.attack}`;
  }
}

// New kinds are data, for example from a JSON file.
const table = [
  ["goblin", 20, "a dagger"],
  ["dragon", 200, "fire"],
  ["wyvern", 120, "its tail"],
];
for (const row of table) {
  console.log(new Breed(...row).newMonster().describe());
}
```

```output for=breeds
goblin: 20 health, attacks with a dagger
dragon: 200 health, attacks with fire
wyvern: 120 health, attacks with its tail
```

- Verdict: idiom. A class with one instance for each kind.
