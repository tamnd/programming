```javascript run id=regex-exec-loop-and-binary-search
// exec with the g flag returns the next match, then null at the end.
const re = /\d+/g;
const text = "a1 b22 c333";
let m;
while ((m = re.exec(text)) !== null) {
  console.log(m[0], "at", m.index);
}

// A while loop whose condition is false at the start never runs.
let n = 0;
while (n > 0) n--;
console.log("n:", n);

// Binary search: the range shrinks until it is empty.
const a = [2, 3, 5, 7, 11, 13];
let lo = 0;
let hi = a.length;
while (lo < hi) {
  const mid = (lo + hi) >> 1;
  if (a[mid] < 11) lo = mid + 1;
  else hi = mid;
}
console.log("11 is at", lo);
```

```output for=regex-exec-loop-and-binary-search
1 at 1
22 at 4
333 at 8
n: 0
11 is at 4
```

- Verdict: native. The condition assigns the next match and compares it with `null`, so the loop ends after the last match. The second loop never runs its body. The binary search halves the range `[lo, hi)` until it is empty, and the number of passes depends on the data.
