```javascript run id=account-record-set
// Rows and named columns in memory, in the shape of the result of a query.
class RecordSet {
  constructor(columns, rows) {
    this.columns = [...columns];
    this.rows = rows.map((r) => [...r]);
    this.states = rows.map(() => "unchanged");
  }

  get(i, column) {
    return this.rows[i][this.columns.indexOf(column)];
  }

  set(i, column, value) {
    this.rows[i][this.columns.indexOf(column)] = value;
    if (this.states[i] === "unchanged") this.states[i] = "modified";
  }

  add(...values) {
    this.rows.push(values);
    this.states.push("added");
  }

  where(test) {
    return this.rows.map((_, i) => i).filter((i) => test((c) => this.get(i, c)));
  }
}

const money = (c) => `${Math.floor(c / 100)}.${String(c % 100).padStart(2, "0")}`;

// The columns and rows as a database driver returns them for:
// select id, name, balance from account order by id
const rs = new RecordSet(["id", "name", "balance"], [
  [1, "Ann", 12000],
  [2, "Bob", 1550],
]);

console.log("columns:", rs.columns.join(", "));
rs.set(1, "balance", rs.get(1, "balance") + 1000);
rs.add(3, "Cy", 0);
rs.rows.forEach((_, i) => {
  console.log(`${rs.get(i, "id")} | ${rs.get(i, "name")} | ${money(rs.get(i, "balance"))} | ${rs.states[i]}`);
});
const rich = rs.where((col) => col("balance") > 2000);
console.log("balance over 20.00:", rich.map((i) => rs.get(i, "name")).join(", "));
console.log("rows to save:", rs.states.filter((s) => s !== "unchanged").length);
```

```output
columns: id, name, balance
1 | Ann | 120.00 | unchanged
2 | Bob | 25.50 | modified
3 | Cy | 0.00 | added
balance over 20.00: Ann, Bob
rows to save: 2
```

- Verdict: idiom. A database driver for JavaScript usually returns the rows of a query as an array of objects or arrays and a list of fields. The `RecordSet` class keeps the columns and the rows, and the state of each row.
- `where` gives the code a function `col` that reads a column by its name, so the test does not use positions.
