```javascript run id=resolve
const INDEX = /^(0|[1-9][0-9]*)$/;

// resolve returns the value that an RFC 6901 pointer identifies in doc.
function resolve(doc, pointer) {
  if (pointer === "") return doc;
  if (!pointer.startsWith("/") || /~(?![01])/.test(pointer)) {
    throw new SyntaxError(`bad pointer ${JSON.stringify(pointer)}`);
  }
  let value = doc;
  for (const raw of pointer.slice(1).split("/")) {
    const token = raw.replaceAll("~1", "/").replaceAll("~0", "~");
    if (Array.isArray(value)) {
      if (!INDEX.test(token) || Number(token) >= value.length) {
        throw new RangeError(`no element ${token}`);
      }
      value = value[Number(token)];
    } else if (value !== null && typeof value === "object" && Object.hasOwn(value, token)) {
      value = value[token];
    } else {
      throw new RangeError(`no member ${JSON.stringify(token)}`);
    }
  }
  return value;
}

const doc = JSON.parse('{"users": [{"name": "Ana", "roles": ["admin"]}], "a/b": 1}');
console.log(resolve(doc, "/users/0/name"));
console.log(JSON.stringify(resolve(doc, "/users/0/roles")));
console.log(resolve(doc, "/a~1b"));

// A plain lookup with value[token] also finds inherited properties.
console.log(typeof doc["constructor"]);
for (const p of ["/constructor", "/users/1", "/users/00"]) {
  try {
    resolve(doc, p);
  } catch (error) {
    console.log(`${error.name}: ${error.message}`);
  }
}
```

```output for=resolve
Ana
["admin"]
1
function
RangeError: no member "constructor"
RangeError: no element 1
RangeError: no element 00
```

- Verdict: native. `JSON.parse` gives plain objects and arrays, and the resolver walks them one token at a time. The language has no function that resolves a JSON Pointer. The resolver uses `Object.hasOwn`, because a plain lookup `value[token]` also finds the inherited properties of `Object.prototype`. Without the check, the pointer `/constructor` gives a function, which is not a JSON value. `replaceAll` decodes `~1` before `~0`, in the order that RFC 6901 gives.
