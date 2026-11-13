```javascript run id=merge
const isObject = (v) => v !== null && typeof v === "object" && !Array.isArray(v);

// mergePatch applies an RFC 7396 merge patch and returns a new value.
function mergePatch(target, patch) {
  if (!isObject(patch)) return patch;
  const result = isObject(target) ? { ...target } : {};
  for (const [name, value] of Object.entries(patch)) {
    if (value === null) delete result[name];
    else result[name] = mergePatch(result[name], value);
  }
  return result;
}

// makeMergePatch returns a merge patch that turns before into after.
function makeMergePatch(before, after) {
  if (!isObject(before) || !isObject(after)) return after;
  const patch = {};
  for (const name of Object.keys(before)) {
    if (!Object.hasOwn(after, name)) patch[name] = null;
  }
  for (const [name, value] of Object.entries(after)) {
    if (!Object.hasOwn(before, name)) patch[name] = value;
    else if (JSON.stringify(before[name]) !== JSON.stringify(value)) {
      patch[name] = makeMergePatch(before[name], value);
    }
  }
  return patch;
}

const before = { id: 7, name: "lamp", tags: ["home", "sale"], size: { w: 20, h: 40 } };
const after = { id: 7, name: "desk lamp", tags: ["home"], size: { w: 20, h: 45 } };
const patch = makeMergePatch(before, after);
console.log(JSON.stringify(patch));
console.log(JSON.stringify(mergePatch(before, patch)) === JSON.stringify(after));

// An array is replaced as a whole, and null always removes a member.
console.log(JSON.stringify(mergePatch({ tags: ["a", "b"] }, { tags: ["c"] })));
console.log(JSON.stringify(makeMergePatch({ note: "x" }, { note: null })));
console.log(JSON.stringify(mergePatch({ note: "x" }, makeMergePatch({ note: "x" }, { note: null }))));
```

```output for=merge
{"name":"desk lamp","tags":["home"],"size":{"h":45}}
true
{"tags":["c"]}
{"note":null}
{}
```

- Verdict: native. A merge patch is plain JSON, so `JSON.parse` and object spread are all the code needs. `mergePatch` follows the MergePatch function of RFC 7396 and returns a new object. `makeMergePatch` goes the other way: it compares two documents and writes `null` for each member that is gone. The last three lines show the limits of the format. An array is replaced as a whole, and a member with the value `null` cannot be set, because the patch `{"note":null}` removes the member.
