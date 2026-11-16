```javascript run id=tostring-and-tostringtag
class Book {
  constructor(title, year) {
    this.title = title;
    this.year = year;
  }

  toString() {
    return `${this.title} (${this.year})`;
  }

  get [Symbol.toStringTag]() {
    return "Book";
  }
}

const b = new Book("Dune", 1965);
console.log(String(b));
console.log(`Read ${b}`);
console.log(Object.prototype.toString.call(b));
console.log(String({ title: "Dune" }));
console.log(JSON.stringify(b));
```

```output for=tostring-and-tostringtag
Dune (1965)
Read Dune (1965)
[object Book]
[object Object]
{"title":"Dune","year":1965}
```

- Verdict: native. `Object.prototype.toString` gives every object a string form, and `String(x)` and template literals call `toString`.
- Without an override, the result is `"[object Object]"`. A `Symbol.toStringTag` getter changes the type name in that default form.
- `JSON.stringify` gives a second form that shows the fields, but it is a data format, not a description: it leaves out methods and fails on cycles.
