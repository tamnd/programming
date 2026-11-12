```javascript run id=json-and-structured-clone
const person = {
  name: "Ada",
  born: new Date(Date.UTC(1815, 11, 10)),
  tags: new Set(["math"]),
  note: undefined,
  greet() { return "hello"; },
};
const text = JSON.stringify(person);
console.log(text);
const back = JSON.parse(text);
console.log(typeof back.born, back.tags instanceof Set);
const revived = JSON.parse(text, (key, value) => (key === "born" ? new Date(value) : value));
console.log("revived year:", revived.born.getUTCFullYear());
try {
  JSON.stringify({ count: 10n });
} catch (err) {
  console.log("BigInt:", err.name);
}

const loop = { name: "loop" };
loop.self = loop;
const copy = structuredClone({ born: person.born, tags: person.tags, loop });
console.log(copy.born instanceof Date, copy.tags.has("math"), copy.loop.self === copy.loop);
try {
  structuredClone(person);
} catch (err) {
  console.log("function:", err.name);
}
```

```output for=json-and-structured-clone
{"name":"Ada","born":"1815-12-10T00:00:00.000Z","tags":{}}
string false
revived year: 1815
BigInt: TypeError
true true true
function: DataCloneError
```

- Verdict: native. The `JSON` object is part of the language. `JSON.stringify` drops `undefined` and functions, writes a `Date` as a string and a `Set` as an empty object, and throws a `TypeError` for a `BigInt`. A reviver function given to `JSON.parse` rebuilds the types. `structuredClone`, from the web platform and in Node, copies dates, sets, maps and cycles, but it throws a `DataCloneError` for a function.
