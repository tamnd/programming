```javascript run id=value-in-owner-columns
// A stand-in for the database: one table whose rows are flat lists of columns.
// Columns: person, start_date, end_date, salary_amount, salary_currency.
const employmentTable = new Map();
let lastId = 0;

class DateRange {
  constructor(start, end) {
    this.start = start;
    this.end = end; // null: the employment has not ended.
    Object.freeze(this);
  }
  toString() {
    return `${this.start} to ${this.end ?? "open"}`;
  }
}

class Money {
  constructor(amount, currency) {
    this.amount = amount;
    this.currency = currency;
    Object.freeze(this);
  }
  equals(other) {
    return this.amount === other.amount && this.currency === other.currency;
  }
  toString() {
    return `${this.amount} ${this.currency}`;
  }
}

class Employment {
  constructor(person, period, salary, id = null) {
    this.id = id;
    this.person = person;
    this.period = period;
    this.salary = salary;
  }
}

function toRow(e) {
  return [e.person, e.period.start, e.period.end, e.salary.amount, e.salary.currency];
}

function insert(e) {
  lastId += 1;
  employmentTable.set(lastId, toRow(e));
  e.id = lastId;
}

function update(e) {
  employmentTable.set(e.id, toRow(e));
}

function find(id) {
  const [person, start, end, amount, currency] = employmentTable.get(id);
  return new Employment(person, new DateRange(start, end), new Money(amount, currency), id);
}

function showRows() {
  for (const [id, row] of employmentTable) {
    console.log(`row: ${[id, ...row].map((v) => (v === null ? "null" : String(v))).join(" ")}`);
  }
}

insert(new Employment("Ann", new DateRange("2024-01-01", "2024-12-31"), new Money(5000, "EUR")));
insert(new Employment("Bo", new DateRange("2025-03-01", null), new Money(4000, "EUR")));
showRows();
const ann = find(1);
const bo = find(2);
for (const e of [ann, bo]) console.log(`loaded: ${e.person} | ${e.period} | ${e.salary}`);
const same = ann.salary.equals(new Money(5000, "EUR")) ? "yes" : "no";
console.log(`salary of Ann equals 5000 EUR: ${same}`);

ann.salary = new Money(5500, "EUR"); // A new value replaces the old one.
update(ann);
console.log(`after raise: Ann ${find(1).salary} | Bo ${find(2).salary}`);

const above = [...employmentTable.values()]
  .filter(([, , , amount, currency]) => currency === "EUR" && amount > 4500)
  .map(([person]) => person);
console.log(`EUR salaries above 4500: ${above.join(", ")}`);
```

```output
row: 1 Ann 2024-01-01 2024-12-31 5000 EUR
row: 2 Bo 2025-03-01 null 4000 EUR
loaded: Ann | 2024-01-01 to 2024-12-31 | 5000 EUR
loaded: Bo | 2025-03-01 to open | 4000 EUR
salary of Ann equals 5000 EUR: yes
after raise: Ann 5500 EUR | Bo 4000 EUR
EUR salaries above 4500: Ann
```

- Verdict: idiom. The value classes call `Object.freeze(this)` in the constructor, so their fields cannot change. JavaScript compares objects by reference, so `Money` has an `equals` method that compares the fields.
- `toRow` flattens an employment into a list of columns, and `find` builds new `DateRange` and `Money` objects from the list.
