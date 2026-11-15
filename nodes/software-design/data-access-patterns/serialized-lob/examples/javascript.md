```javascript run id=graph-in-one-column
// A stand-in for the database: the customer table, with the departments as one text column.
const customer = new Map();

class Department {
  constructor(name, children = []) {
    this.name = name;
    this.children = children;
  }
  static fromJSON(d) {
    return new Department(d.name, d.children.map(Department.fromJSON));
  }
}

function save(id, name, departments) {
  // The whole graph of departments is written as one value in one column.
  const lob = JSON.stringify(departments);
  customer.set(id, { name, departments: lob });
  return new TextEncoder().encode(lob).length;
}

function load(id) {
  const row = customer.get(id);
  return [row.name, JSON.parse(row.departments).map(Department.fromJSON)];
}

function* paths(departments, prefix = "") {
  for (const d of departments) {
    yield prefix + d.name;
    yield* paths(d.children, prefix + d.name + "/");
  }
}

function ancestors(departments, target, above = []) {
  for (const d of departments) {
    if (d.name === target) return [...above].reverse();
    const found = ancestors(d.children, target, [...above, d.name]);
    if (found !== null) return found;
  }
  return null;
}

const tree = [
  new Department("Sales", [new Department("Presales"), new Department("Key accounts")]),
  new Department("Support"),
];
let size = save(1, "Acme", tree);
console.log(`saved Acme: ${size} bytes in one column`);
console.log(`column: ${customer.get(1).departments}`);
const [name, loaded] = load(1);
console.log(`loaded ${name}: ${[...paths(loaded)].join(" | ")}`);
console.log(`ancestors of Presales: ${ancestors(loaded, "Presales").join(", ")}`);
loaded[1].children.push(new Department("Field"));
size = save(1, name, loaded);
console.log(`add Support/Field: ${size} bytes written again`);
console.log(`rows in customer: ${customer.size}`);
```

```output
saved Acme: 136 bytes in one column
column: [{"name":"Sales","children":[{"name":"Presales","children":[]},{"name":"Key accounts","children":[]}]},{"name":"Support","children":[]}]
loaded Acme: Sales | Sales/Presales | Sales/Key accounts | Support
ancestors of Presales: Sales
add Support/Field: 166 bytes written again
rows in customer: 1
```

- Verdict: idiom. `JSON.stringify` writes the tree with its class fields, and `JSON.parse` with `Department.fromJSON` makes the objects again on load.
- `TextEncoder` gives the size of the column in bytes, as a database counts it.
