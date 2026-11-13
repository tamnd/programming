```javascript run id=slice
// The array slice selector of RFC 9535, section 2.3.4.2.2.
function normalize(i, len) {
  return i >= 0 ? i : len + i;
}

function slice(a, start, end, step = 1) {
  const len = a.length;
  if (step === 0) return [];
  start ??= step >= 0 ? 0 : len - 1;
  end ??= step >= 0 ? len : -len - 1;
  const [nStart, nEnd] = [normalize(start, len), normalize(end, len)];
  const out = [];
  if (step > 0) {
    const lower = Math.min(Math.max(nStart, 0), len);
    const upper = Math.min(Math.max(nEnd, 0), len);
    for (let i = lower; i < upper; i += step) out.push(a[i]);
  } else {
    const upper = Math.min(Math.max(nStart, -1), len - 1);
    const lower = Math.min(Math.max(nEnd, -1), len - 1);
    for (let i = upper; lower < i; i += step) out.push(a[i]);
  }
  return out;
}

const a = ["a", "b", "c", "d", "e", "f", "g"];
console.log("$[1:3]    ", slice(a, 1, 3).join(""));
console.log("$[5:]     ", slice(a, 5).join(""));
console.log("$[1:5:2]  ", slice(a, 1, 5, 2).join(""));
console.log("$[5:1:-2] ", slice(a, 5, 1, -2).join(""));
console.log("$[::-1]   ", slice(a, undefined, undefined, -1).join(""));
console.log("$[-2:]    ", slice(a, -2).join(""), "=", a.slice(-2).join(""));
console.log("$[::0]    ", JSON.stringify(slice(a, undefined, undefined, 0)));
```

```output for=slice
$[1:3]     bc
$[5:]      fg
$[1:5:2]   bd
$[5:1:-2]  fd
$[::-1]    gfedcba
$[-2:]     fg = fg
$[::0]     []
```

- Verdict: native. JavaScript has no JSONPath, but `JSON.parse` gives arrays, and the slice selector is a few lines over them. The function follows the normative steps of RFC 9535 for the array slice selector: the default start and end depend on the sign of the step, negative bounds count from the end, and the bounds are clamped to the array. The first five queries and their results are the examples of the RFC. With a positive step, the result is the same as `Array.prototype.slice`, which inspired the selector. A step of 0 selects nothing.
