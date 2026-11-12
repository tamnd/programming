```typescript run id=breeds
class Breed {
  name: string;
  health: number;
  attack: string;
  constructor(name: string, health: number, attack: string) {
    this.name = name;
    this.health = health;
    this.attack = attack;
  }
  newMonster(): Monster {
    return new Monster(this);
  }
}

class Monster {
  breed: Breed;
  health: number; // each monster has its own health
  constructor(breed: Breed) {
    this.breed = breed;
    this.health = breed.health;
  }
  describe(): string {
    const b = this.breed;
    return `${b.name}: ${this.health} health, attacks with ${b.attack}`;
  }
}

// New kinds are data, for example from a JSON file.
const table: [string, number, string][] = [
  ["goblin", 20, "a dagger"],
  ["dragon", 200, "fire"],
  ["wyvern", 120, "its tail"],
];
for (const [name, health, attack] of table) {
  console.log(new Breed(name, health, attack).newMonster().describe());
}
```

```output for=breeds
goblin: 20 health, attacks with a dagger
dragon: 200 health, attacks with fire
wyvern: 120 health, attacks with its tail
```

- Verdict: idiom. A class with one instance for each kind.
