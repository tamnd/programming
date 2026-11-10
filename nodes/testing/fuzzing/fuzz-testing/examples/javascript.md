```javascript run id=seeds
// Reads records of the form [tag][length][value].
function parse(data) {
  const records = [];
  let i = 0;
  while (i < data.length) {
    if (data.length - i < 2) {
      throw new RangeError(`truncated header at offset ${i}`);
    }
    const tag = data[i];
    const length = data[i + 1];
    if (data.length - i - 2 < length) {
      throw new RangeError(`truncated value at offset ${i}`);
    }
    records.push({ tag, value: data.subarray(i + 2, i + 2 + length) });
    i += 2 + length;
  }
  return records;
}

// The fuzz target: any exception other than RangeError is a bug.
function fuzzOne(data) {
  try {
    return `records=${parse(data).length}`;
  } catch (e) {
    if (!(e instanceof RangeError)) throw e;
    return `rejected: ${e.message}`;
  }
}

const hex = (data) => Array.from(data, (b) => b.toString(16).padStart(2, "0")).join(" ") || "(empty)";

const seeds = [[], [1, 2, 0x68, 0x69], [1, 2, 0x68, 0x69, 2, 0], [0x7a], [0x7e, 0xe1]].map((s) => Uint8Array.from(s));
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

The original parser does not crash in JavaScript. An index past the end of a typed array gives `undefined`, `2 + undefined` is `NaN`, and the loop ends. The bad inputs are accepted as records with empty values.

```javascript wrong id=no-length-check
// Reads records of the form [tag][length][value], with no length checks.
function parse(data) {
  const records = [];
  let i = 0;
  while (i < data.length) {
    const tag = data[i];
    const length = data[i + 1]; // undefined past the end: no exception
    records.push({ tag, value: data.subarray(i + 2, i + 2 + length) });
    i += 2 + length;
  }
  return records;
}

const seeds = [[0x7a], [0x7e, 0xe1]].map((s) => Uint8Array.from(s));
for (const seed of seeds) {
  console.log(JSON.stringify(parse(seed), (key, v) => (v instanceof Uint8Array ? Array.from(v) : v)));
}
```

```output for=no-length-check
[{"tag":122,"value":[]}]
[{"tag":126,"value":[]}]
```

- Verdict: library. Jazzer.js is a coverage-guided fuzzer for Node.js, based on libFuzzer.
- A fuzzer that waits for a crash finds nothing here. Write the checks in the parser and add an oracle to the target, such as a round trip.
- `subarray` clamps its arguments to the length of the array and treats `NaN` as 0, so a bad length gives a short or empty view with no error.
- `RangeError` is the documented rejection. `fuzzOne` throws again any other error.
