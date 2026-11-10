```javascript run id=spawner
const monster = {
  describe() {
    return `${this.name}: health ${this.health}, items ${this.items.join(", ")}`;
  },
};

const trap = {
  describe() {
    return `${this.name}: damage ${this.damage}`;
  },
};

// The prototypes are objects whose prototype is monster or trap.
const goblin = Object.assign(Object.create(monster), {
  name: "goblin",
  health: 30,
  items: ["dagger"],
});
const spikes = Object.assign(Object.create(trap), { name: "spikes", damage: 5 });

const registry = new Map([
  ["goblin", goblin],
  ["spikes", spikes],
]);

function spawn(key) {
  const prototype = registry.get(key);
  // The new object delegates to the prototype. It gets its own copy of each array.
  const thing = Object.create(prototype);
  for (const [field, value] of Object.entries(prototype)) {
    if (Array.isArray(value)) thing[field] = [...value];
  }
  return thing;
}

const first = spawn("goblin");
first.items.push("torch");
const second = spawn("goblin");
const third = spawn("spikes");
for (const thing of [first, second, third]) {
  console.log(thing.describe());
}
console.log("prototype", registry.get("goblin").describe());
console.log("own fields of second:", Object.keys(second).join(", "));
```

```output for=spawner
goblin: health 30, items dagger, torch
goblin: health 30, items dagger
spikes: damage 5
prototype goblin: health 30, items dagger
own fields of second: items
```

- Verdict: native. Each object has an internal link to a prototype object. `Object.create(p)` makes an object whose prototype is `p`, and a property read that does not find an own property follows the chain to `p`.
- A clone made with `Object.create` copies nothing. It delegates to the prototype, so a later change to `goblin.health` would show in every goblin that has not set its own `health`. The last line shows that `second` has only one own field.
- A write makes an own property on the new object. A change inside an inherited array is not a write to the new object, so `spawn` gives each object its own copy of each array. The node page shows the bug without that loop.
- `structuredClone` makes a deep copy of plain data, but it does not keep the prototype of the object. It is a copy of data, not a clone of an object with behaviour.
