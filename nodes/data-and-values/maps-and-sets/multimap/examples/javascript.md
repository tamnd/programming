```javascript run id=map-of-arrays
// A Map whose values are arrays.
const m = new Map();
function add(k, v) {
  if (!m.has(k)) m.set(k, []);
  m.get(k).push(v);
}
add("yellow", 1);
add("blue", 2);
add("yellow", 3);
console.log(m.get("yellow"), m.get("red") ?? []);

// Map.groupBy (ES2024) builds a multimap from a list in one call.
const words = ["ant", "bee", "cat", "ape", "bat"];
const byFirst = Map.groupBy(words, (w) => w[0]);
console.log([...byFirst].map(([k, v]) => `${k}:${v.join(",")}`).join(" "));

// URLSearchParams is a multimap of strings. get returns the first value.
const q = new URLSearchParams("color=red&size=9&color=blue");
console.log(q.get("color"), q.getAll("color"), q.getAll("shape"));
q.append("color", "green");
console.log(q.toString());

// Object.fromEntries keeps only the last value for a repeated key.
console.log(Object.fromEntries(q));
```

```output
[ 1, 3 ] []
a:ant,ape b:bee,bat c:cat
red [ 'red', 'blue' ] []
color=red&size=9&color=blue&color=green
{ color: 'green', size: '9' }
```

- Verdict: idiom. A Map whose values are arrays. Map.groupBy from ES2024 builds one from a list. URLSearchParams is a multimap of strings, with get for the first value and getAll for all values. Object.fromEntries keeps only the last value of a repeated key.
