```typescript run id=ddmin
// Returns a 1-minimal part of chars on which fails() is still true.
function ddmin(chars: string, fails: (text: string) => boolean): string {
  let n = 2;
  while (chars.length >= 2) {
    const size = chars.length;
    const bounds: [number, number][] = [];
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
function browserCrashes(text: string): boolean {
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

- Verdict: idiom. The program is the JavaScript program with types.
- The type `(text: string) => boolean` documents the contract of the test function.
- `find` returns `string | undefined`, and the compiler makes the program check for `undefined` before it uses the result.
