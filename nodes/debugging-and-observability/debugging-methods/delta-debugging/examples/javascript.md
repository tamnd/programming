```javascript run id=ddmin
// Returns a 1-minimal part of chars on which fails() is still true.
function ddmin(chars, fails) {
  let n = 2;
  while (chars.length >= 2) {
    const size = chars.length;
    const bounds = [];
    for (let i = 0; i < n; i++) {
      bounds.push([Math.floor((i * size) / n), Math.floor(((i + 1) * size) / n)]);
    }
    const subsets = bounds.map(([lo, hi]) => chars.slice(lo, hi));
    const complements = bounds.map(([lo, hi]) => chars.slice(0, lo) + chars.slice(hi));
    let smaller = subsets.find((s) => fails(s));
    if (smaller !== undefined) { // reduce to subset
      chars = smaller;
      n = 2;
      console.log("subset    ", chars.length);
      continue;
    }
    smaller = complements.find((c) => fails(c));
    if (smaller !== undefined) { // reduce to complement
      chars = smaller;
      n = Math.max(n - 1, 2);
      console.log("complement", chars.length);
      continue;
    }
    if (n === size) { // each single character was removed
      break;
    }
    n = Math.min(size, 2 * n); // increase granularity
  }
  return chars;
}

let tests = 0;

// Stands for one run of the browser: it crashes on a <SELECT ...> tag.
function browserCrashes(text) {
  tests++;
  const start = text.indexOf("<SELECT");
  return start >= 0 && text.indexOf(">", start) >= 0;
}

const page = '<td align=left><SELECT NAME="priority" MULTIPLE SIZE=7>';
const minimal = ddmin(page, browserCrashes);
console.log(`${page.length} characters reduced to ${minimal} in ${tests} tests`);
```

```output for=ddmin
complement 42
complement 28
complement 21
complement 17
complement 14
complement 13
complement 12
complement 10
complement 9
complement 8
55 characters reduced to <SELECT> in 147 tests
```

The loop must stop when each part is one character and no part fails. Without the `n === size` check, `n` stays at `size` and the loop runs the same tests forever.

```javascript fails expect=timeout timeout=5 id=no-stop
function ddmin(chars, fails) {
  let n = 2;
  while (chars.length >= 2) {
    const size = chars.length;
    const bounds = [];
    for (let i = 0; i < n; i++) {
      bounds.push([Math.floor((i * size) / n), Math.floor(((i + 1) * size) / n)]);
    }
    const smaller =
      bounds.map(([lo, hi]) => chars.slice(lo, hi)).find(fails) ??
      bounds.map(([lo, hi]) => chars.slice(0, lo) + chars.slice(hi)).find(fails);
    if (smaller !== undefined) {
      chars = smaller;
      n = 2;
      continue;
    }
    n = Math.min(size, 2 * n); // no check for n === size
  }
  return chars;
}

const crashes = (text) => text.includes("<SELECT") && text.includes(">");
console.log(ddmin('<SELECT NAME="priority">', crashes));
```

- Verdict: idiom. No standard library has delta debugging.
- `Array.prototype.find` stops at the first part that fails, but `map` builds all the parts first.
- `slice` counts UTF-16 code units. The input here is ASCII, so each unit is one character.
- The test function is a plain function, so any function that returns a boolean can serve as the test.
