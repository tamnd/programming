```javascript run id=fixpoint
const parent = [["zeus", "ares"], ["ares", "harmonia"], ["harmonia", "semele"], ["semele", "dionysus"]];

// ancestor(X, Y) :- parent(X, Y).
// ancestor(X, Y) :- parent(X, Z), ancestor(Z, Y).
const ancestor = new Set(parent.map(([x, y]) => `${x} ${y}`));
let changed = true;
while (changed) {
  changed = false;
  for (const [x, z] of parent) {
    for (const fact of [...ancestor]) {
      const [z2, y] = fact.split(" ");
      if (z === z2 && !ancestor.has(`${x} ${y}`)) {
        ancestor.add(`${x} ${y}`);
        changed = true;
      }
    }
  }
}
console.log(ancestor.size);
console.log([...ancestor].filter((f) => f.endsWith(" dionysus")).map((f) => f.split(" ")[0]).sort());
```

```output for=fixpoint
10
[ 'ares', 'harmonia', 'semele', 'zeus' ]
```

- Verdict: absent. JavaScript has no logic engine. The loop applies the two rules until no new fact appears, which is how a bottom-up Datalog engine works. Libraries such as Tau Prolog run Prolog in JavaScript.
