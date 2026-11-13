```javascript run id=json-against-v8
const v8 = require("node:v8");

const record = {
  id: 42,
  name: "Ada",
  joined: new Date(Date.UTC(2024, 0, 15)),
  roles: new Map([["admin", true]]),
  big: 2n ** 64n,
  note: undefined,
};

// JSON is text, and every JSON parser can read it, but it has fewer types.
let text;
try {
  text = JSON.stringify(record);
} catch (e) {
  console.log("JSON:", e.constructor.name);
}
const plain = { ...record, big: record.big.toString() };
text = JSON.stringify(plain);
console.log(text.length, text);
const back = JSON.parse(text);
console.log(typeof back.joined, back.roles, "note" in back);

// v8.serialize is binary and keeps Date, Map and BigInt, but only V8 can read it.
const bin = v8.serialize(record);
// The first byte is the version tag 0xff of the V8 format.
console.log(Buffer.isBuffer(bin), bin[0].toString(16));
const again = v8.deserialize(bin);
console.log(again.joined instanceof Date, again.roles.get("admin"), again.big, "note" in again);
```

```output for=json-against-v8
JSON: TypeError
98 {"id":42,"name":"Ada","joined":"2024-01-15T00:00:00.000Z","roles":{},"big":"18446744073709551616"}
string {} false
true ff
true true 18446744073709551616n true
```

- Verdict: native. `JSON.parse` and `JSON.stringify` are part of the language. `JSON.stringify` throws a `TypeError` for a `BigInt`, writes a `Date` as an ISO string, writes a `Map` as `{}` and drops a property whose value is `undefined`. The parsed value has a string where the `Date` was. The Node.js module `node:v8` writes the binary format of the V8 engine, which keeps `Date`, `Map`, `BigInt` and `undefined`, but only V8 can read it, and its documentation says that equal values can give different bytes.
