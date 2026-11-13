// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pretty-print-json/.

// A JSON number becomes a double, which loses digits above 2**53.
// The reviver reads the source text of each number and makes a BigInt.
function exactIntegers(key: string, value: unknown, context?: { source: string }): unknown {
  return typeof value === "number" && context ? BigInt(context.source) : value;
}

// UTF-8 byte order is code point order. The default sort uses UTF-16 code units.
function byCodePoint(a: string, b: string): number {
  return Buffer.compare(Buffer.from(a), Buffer.from(b));
}

function isObject(value: unknown): value is Record<string, unknown> {
  return value !== null && typeof value === "object" && !Array.isArray(value);
}

// JSON.stringify(value, null, 2) cannot write a BigInt, and it keeps the
// key order of the object, where integer-like keys such as "10" come first.
// So this function writes the arrays and objects, and JSON.stringify writes the strings.
function write(value: unknown, indent: string): string {
  const inner = indent + "  ";
  if (typeof value === "bigint") return value.toString();
  if (Array.isArray(value)) {
    if (value.length === 0) return "[]";
    const items = value.map((item) => inner + write(item, inner));
    return `[\n${items.join(",\n")}\n${indent}]`;
  }
  if (isObject(value)) {
    const keys = Object.keys(value).sort(byCodePoint);
    if (keys.length === 0) return "{}";
    const members = keys.map((key) => `${inner}${JSON.stringify(key)}: ${write(value[key], inner)}`);
    return `{\n${members.join(",\n")}\n${indent}}`;
  }
  return JSON.stringify(value);
}

// Returns the JSON text indented by two spaces, with the names of each object sorted.
// Throws a SyntaxError when the text is not JSON.
export function prettyJson(text: string): string {
  return write(JSON.parse(text, exactIntegers), "");
}
