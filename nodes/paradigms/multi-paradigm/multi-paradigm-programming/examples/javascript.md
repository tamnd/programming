```javascript run id=orders
class Order {
  constructor(customer, amount) {
    this.customer = customer;
    this.amount = amount;
    Object.freeze(this); // a value: no field changes after construction
  }
}

const parse = (line) => {
  const [name, amount] = line.split(",");
  return new Order(name.trim(), Number(amount));
};

function totals(orders) {
  const out = new Map();
  for (const o of orders) out.set(o.customer, (out.get(o.customer) ?? 0) + o.amount);
  return out;
}

// Any object with a line method is a report: no class is needed.
const plain = { line: (name, total) => `${name} ${total}` };
const csv = { line: (name, total) => `${name},${total}` };

const text = "ana, 30\nbo, 70\nana, 25\ncy, 10\nbo, 5";
const big = [...totals(text.split("\n").map(parse))]
  .filter(([, total]) => total > 50)
  .sort((a, b) => b[1] - a[1]);

for (const report of [plain, csv]) {
  for (const [name, total] of big) console.log(report.line(name, total));
}
```

```output for=orders
bo 75
ana 55
bo,75
ana,55
```

```javascript wrong id=var-capture
// Build three functions that should return 0, 1 and 2.
const fns = [];
for (var i = 0; i < 3; i++) {
  fns.push(() => i);
}
console.log(fns.map((f) => f()));
```

```output for=var-capture
[ 3, 3, 3 ]
```

- Verdict: native. JavaScript has first-class functions and closures from its start, objects with prototypes, classes as a syntax over prototypes, and an event loop. The block uses a frozen class instance as a value, a pure `parse`, a loop over a `Map`, an array pipeline, and two plain objects that share a method name in place of an interface.
- The wrong block declares the loop variable with `var`, so the three closures share one variable, which is 3 after the loop. `let` makes a new binding for each iteration and gives `[ 0, 1, 2 ]`.
