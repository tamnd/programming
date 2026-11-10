```javascript run id=export
class CsvWriter {
  header() {
    return "name,count";
  }

  row(name, count) {
    return `${name},${count}`;
  }
}

class TextWriter {
  header() {
    return "Inventory";
  }

  row(name, count) {
    return `${name}: ${count}`;
  }
}

class Exporter {
  export(rows) {
    const writer = this.createWriter(); // the factory method
    return [writer.header(), ...rows.map(([name, count]) => writer.row(name, count))].join("\n");
  }

  createWriter() {
    throw new Error(`${this.constructor.name} must override createWriter`);
  }
}

class CsvExporter extends Exporter {
  createWriter() {
    return new CsvWriter();
  }
}

class TextExporter extends Exporter {
  createWriter() {
    return new TextWriter();
  }
}

const rows = [
  ["pen", 3],
  ["book", 1],
];
for (const exporter of [new CsvExporter(), new TextExporter()]) {
  console.log(exporter.export(rows));
}
```

```output
name,count
pen,3
book,1
Inventory
pen: 3
book: 1
```

- Verdict: idiom. JavaScript has no abstract methods, so the base `createWriter` throws until a subclass overrides it. The product interface is by convention: any object with `header` and `row` works.
- A class is a value, so a creator can also take the class and call `new this.Writer()`, or take a function that returns a writer. This removes the creator subclasses.
