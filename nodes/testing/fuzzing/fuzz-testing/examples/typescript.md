```typescript run id=seeds
type Entry = { tag: number; value: Uint8Array };

type ParseResult = { ok: true; records: Entry[] } | { ok: false; error: string };

// Reads records of the form [tag][length][value]; a truncated input is an error value.
function parse(data: Uint8Array): ParseResult {
  const records: Entry[] = [];
  let i = 0;
  while (i < data.length) {
    if (data.length - i < 2) {
      return { ok: false, error: `truncated header at offset ${i}` };
    }
    const tag = data[i]!;
    const length = data[i + 1]!;
    if (data.length - i - 2 < length) {
      return { ok: false, error: `truncated value at offset ${i}` };
    }
    records.push({ tag, value: data.subarray(i + 2, i + 2 + length) });
    i += 2 + length;
  }
  return { ok: true, records };
}

// The fuzz target: it must not throw for any input.
function fuzzOne(data: Uint8Array): string {
  const result = parse(data);
  return result.ok ? `records=${result.records.length}` : `rejected: ${result.error}`;
}

const hex = (data: Uint8Array): string =>
  Array.from(data, (b) => b.toString(16).padStart(2, "0")).join(" ") || "(empty)";

const seeds: Uint8Array[] = [[], [1, 2, 0x68, 0x69], [1, 2, 0x68, 0x69, 2, 0], [0x7a], [0x7e, 0xe1]].map((s) =>
  Uint8Array.from(s),
);
for (const seed of seeds) {
  console.log(`${hex(seed)}: ${fuzzOne(seed)}`);
}
console.log(`seed corpus: ${seeds.length} inputs passed`);
```

```output
(empty): records=0
01 02 68 69: records=1
01 02 68 69 02 00: records=2
7a: rejected: truncated header at offset 0
7e e1: rejected: truncated value at offset 0
seed corpus: 5 inputs passed
```

- Verdict: library. Jazzer.js fuzzes TypeScript code as well as JavaScript.
- The parser returns a result union instead of throwing. The target then cannot throw for bad input, so any exception is a bug.
- An index into a `Uint8Array` past the end gives `undefined` at run time, as in JavaScript, but its static type is `number`. The `!` after `data[i]` states that the checks above make the index valid. The checks, not the types, prevent the bug.
- The types are erased before the program runs, so they add no checks at run time.
