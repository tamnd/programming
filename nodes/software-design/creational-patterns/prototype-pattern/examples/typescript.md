```typescript run id=spawner
interface Prototype {
  clone(): this;
  describe(): string;
}

class Monster implements Prototype {
  name: string;
  health: number;
  items: string[];
  constructor(name: string, health: number, items: string[]) {
    this.name = name;
    this.health = health;
    this.items = items;
  }
  clone(): this {
    // Keep the class of this object, copy the fields, then copy the array.
    const copy = Object.assign(Object.create(Object.getPrototypeOf(this)), this) as this;
    copy.items = [...this.items];
    return copy;
  }
  describe(): string {
    return `${this.name}: health ${this.health}, items ${this.items.join(", ")}`;
  }
}

class Trap implements Prototype {
  name: string;
  damage: number;
  constructor(name: string, damage: number) {
    this.name = name;
    this.damage = damage;
  }
  clone(): this {
    return Object.assign(Object.create(Object.getPrototypeOf(this)), this) as this;
  }
  describe(): string {
    return `${this.name}: damage ${this.damage}`;
  }
}

class Registry {
  private readonly prototypes = new Map<string, Prototype>();
  add(key: string, prototype: Prototype): void {
    this.prototypes.set(key, prototype);
  }
  get(key: string): Prototype {
    const prototype = this.prototypes.get(key);
    if (prototype === undefined) throw new Error(`no prototype ${key}`);
    return prototype;
  }
  spawn(key: string): Prototype {
    return this.get(key).clone();
  }
}

const registry = new Registry();
registry.add("goblin", new Monster("goblin", 30, ["dagger"]));
registry.add("spikes", new Trap("spikes", 5));

const first = registry.spawn("goblin");
if (first instanceof Monster) first.items.push("torch");
const second = registry.spawn("goblin");
const trap = registry.spawn("spikes");
for (const thing of [first, second, trap]) {
  console.log(thing.describe());
}
console.log("prototype", registry.get("goblin").describe());
console.log("the clone is a Monster:", first instanceof Monster);
```

```output for=spawner
goblin: health 30, items dagger, torch
goblin: health 30, items dagger
spikes: damage 5
prototype goblin: health 30, items dagger
the clone is a Monster: true
```

- Verdict: idiom. An interface with `clone(): this`. The polymorphic `this` type says that a clone has the type of the object it copies, so `monster.clone()` has the static type `Monster` when `monster` has that type.
- `Object.create(Object.getPrototypeOf(this))` keeps the class: the copy is an instance of `Monster`. `Object.assign` copies the own fields, which is a shallow copy, so the method copies the array by hand.
- A class with `#private` fields cannot use `Object.assign` for its clone, because private fields are not properties. Such a class calls its own constructor in `clone`.
- `structuredClone` gives a deep copy of the data, but the result is a plain object with no methods.
