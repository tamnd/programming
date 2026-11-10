```javascript run id=loader
class Loader {
  // The template method. Subclasses are not meant to override it.
  load(lines) {
    const records = [];
    for (const raw of lines) {
      const line = raw.trim();
      if (line === "" || line.startsWith("#")) continue;
      const record = this.parse(line);
      if (this.accept(record)) records.push(record);
    }
    return records;
  }

  parse(line) {
    throw new Error(`${this.constructor.name} must implement parse`);
  }

  accept(record) {
    return true; // the hook: keep every record unless a subclass says otherwise
  }
}

class CsvLoader extends Loader {
  parse(line) {
    const [name, count] = line.split(",");
    return [name, Number(count)];
  }
}

class KeyValueLoader extends Loader {
  parse(line) {
    const [name, count] = line.split("=");
    return [name, Number(count)];
  }

  accept([, count]) {
    return count > 0;
  }
}

const show = (label, records) => console.log(`${label}: ${records.map(([n, c]) => `${n}:${c}`).join(" ")}`);
show("csv", new CsvLoader().load(["# stock", "pen,3", "", "book,1"]));
show("kv", new KeyValueLoader().load(["pen=3", "# sold out", "cup=0"]));
```

```output for=loader
csv: pen:3 book:1
kv: pen:3
```

- Verdict: idiom. A base class with `extends`. JavaScript has no abstract methods, so `parse` throws until a subclass gives it. The error appears on the first call, not when the object is made.
- JavaScript has no `final`. A subclass can override `load`, and nothing reports it.
- The hook `accept` takes a destructured array, so `KeyValueLoader` reads the count by position.
