```javascript run id=parse-emit
const text = '{"id": 9007199254740993, "total": 0.10, "tags": ["new"], "paid": false, "note": null}';

const order = JSON.parse(text);
console.log(order.id);
console.log(JSON.stringify(order));
console.log(JSON.stringify({ city: "Hà Nội" }, null, 2));

const exact = JSON.parse(text, (key, value, context) =>
  key === "id" ? BigInt(context.source) : value,
);
console.log(exact.id);
console.log(JSON.stringify({ id: JSON.rawJSON(exact.id.toString()) }));
```

```output for=parse-emit
9007199254740992
{"id":9007199254740992,"total":0.1,"tags":["new"],"paid":false,"note":null}
{
  "city": "Hà Nội"
}
9007199254740993n
{"id":9007199254740993}
```

```javascript run id=round-trip
const before = {
  ratio: NaN,
  limit: Infinity,
  missing: undefined,
  day: new Date(Date.UTC(2026, 10, 10)),
  counts: new Map([["a", 1]]),
};
const text = JSON.stringify(before);
console.log(text);
console.log(typeof JSON.parse(text).day);

try {
  JSON.stringify({ id: 1n });
} catch (error) {
  console.log(`${error.name}: ${error.message}`);
}

console.log(JSON.parse('{"total": 1, "total": 2}'));
```

```output for=round-trip
{"ratio":null,"limit":null,"day":"2026-11-10T00:00:00.000Z","counts":{}}
string
TypeError: Do not know how to serialize a BigInt
{ total: 2 }
```

- Verdict: native. `JSON` is a built-in object of the language. `JSON.parse` reads text and `JSON.stringify` writes it.
- Every JSON number becomes a `Number`, an IEEE 754 double. `9007199254740993` is above 2^53, so it reads as `9007199254740992` with no error.
- The reviver of `JSON.parse` gets a third argument with the source text of each primitive value, and `JSON.rawJSON` writes a number from its text. With both, a large integer can travel as a `BigInt`.
- `NaN` and `Infinity` become `null`. A property whose value is `undefined` or a function is left out. A `Date` becomes its ISO string through `toJSON`, and it comes back as a string. A `Map` becomes `{}`.
- A `BigInt` value makes `JSON.stringify` throw a `TypeError`, unless a replacer or a `toJSON` method converts it.
- A repeated name keeps the last value.
