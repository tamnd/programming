```typescript run id=export
interface Writer {
  header(): string;
  row(name: string, count: number): string;
}

class CsvWriter implements Writer {
  header(): string {
    return "name,count";
  }

  row(name: string, count: number): string {
    return `${name},${count}`;
  }
}

class TextWriter implements Writer {
  header(): string {
    return "Inventory";
  }

  row(name: string, count: number): string {
    return `${name}: ${count}`;
  }
}

abstract class Exporter {
  export(rows: ReadonlyArray<readonly [string, number]>): string {
    const writer = this.createWriter(); // the factory method
    return [writer.header(), ...rows.map(([name, count]) => writer.row(name, count))].join("\n");
  }

  protected abstract createWriter(): Writer;
}

class CsvExporter extends Exporter {
  protected createWriter(): Writer {
    return new CsvWriter();
  }
}

class TextExporter extends Exporter {
  protected createWriter(): Writer {
    return new TextWriter();
  }
}

const rows = [
  ["pen", 3],
  ["book", 1],
] as const;
const exporters: Exporter[] = [new CsvExporter(), new TextExporter()];
for (const exporter of exporters) {
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

- Verdict: idiom. `abstract` classes and methods are part of TypeScript's type system. The compiler rejects `new Exporter()` and a subclass that does not implement `createWriter`.
- `abstract` and `protected` disappear when the types are stripped, so they are compile-time checks only.
- A constructor type such as `new () => Writer` lets a creator take the class of the product as a value, with no creator subclass.
