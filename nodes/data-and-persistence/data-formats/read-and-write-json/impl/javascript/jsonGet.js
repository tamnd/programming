// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-json/.

// An array index: ASCII digits, no sign, no leading zero.
const INDEX = /^(?:0|[1-9][0-9]*)$/;

// A JSON number becomes a double, which loses digits above 2**53.
// The reviver reads the source text of each number and makes a BigInt.
function exactIntegers(key, value, context) {
  return typeof value === "number" ? BigInt(context.source) : value;
}

// UTF-8 byte order is code point order. The default sort uses UTF-16 code units.
function byCodePoint(a, b) {
  return Buffer.compare(Buffer.from(a), Buffer.from(b));
}

// Writes the value as canonical JSON. JSON.stringify writes the strings,
// but it cannot write a BigInt, and it keeps the key order of the object,
// where integer-like keys such as "10" come first.
function write(value) {
  if (typeof value === "bigint") return value.toString();
  if (Array.isArray(value)) return `[${value.map(write).join(",")}]`;
  if (value !== null && typeof value === "object") {
    const members = Object.keys(value)
      .sort(byCodePoint)
      .map((key) => `${JSON.stringify(key)}:${write(value[key])}`);
    return `{${members.join(",")}}`;
  }
  return JSON.stringify(value);
}

// Returns the value at path in the JSON text, as canonical JSON.
// Throws a SyntaxError when the text is not JSON.
export function jsonGet(text, path) {
  let value = JSON.parse(text, exactIntegers);
  for (const step of path) {
    if (Array.isArray(value) && INDEX.test(step) && Number(step) < value.length) {
      value = value[Number(step)];
    } else if (value !== null && typeof value === "object" && !Array.isArray(value) && Object.hasOwn(value, step)) {
      value = value[step];
    } else {
      throw new RangeError(`no value at ${JSON.stringify(step)}`);
    }
  }
  return write(value);
}
