```typescript run id=loader
type Item = readonly [name: string, count: number];

abstract class Loader {
  load(lines: readonly string[]): Item[] {
    const records: Item[] = [];
    for (const raw of lines) {
      const line = raw.trim();
      if (line === "" || line.startsWith("#")) continue;
      const record = this.parse(line);
      if (this.accept(record)) records.push(record);
    }
    return records;
  }

  protected abstract parse(line: string): Item;

  protected accept(_record: Item): boolean {
    return true;
  }
}

class CsvLoader extends Loader {
  protected parse(line: string): Item {
    const [name, count] = line.split(",");
    return [name, Number(count)];
  }
}

class KeyValueLoader extends Loader {
  protected parse(line: string): Item {
    const [name, count] = line.split("=");
    return [name, Number(count)];
  }

  protected override accept([, count]: Item): boolean {
    return count > 0;
  }
}

const show = (label: string, records: Item[]): void =>
  console.log(`${label}: ${records.map(([n, c]) => `${n}:${c}`).join(" ")}`);
show("csv", new CsvLoader().load(["# stock", "pen,3", "", "book,1"]));
show("kv", new KeyValueLoader().load(["pen=3", "# sold out", "cup=0"]));
```

```output for=loader
csv: pen:3 book:1
kv: pen:3
```

- Verdict: idiom. An `abstract` class with an `abstract` method. The compiler rejects `new Loader()` and a subclass that does not implement `parse`.
- `protected` keeps `parse` and `accept` out of the public interface. Clients call `load` only.
- TypeScript has no `final` modifier for methods, so a subclass can override `load`. The `override` keyword, with `noImplicitOverride` on, makes each override explicit, which helps a reader see one.
