```javascript run id=infinite
function* naturalsFrom(n) {
  for (;;) {
    yield n;
    n += 1;
  }
}

function* iterate(f, x) {
  for (;;) {
    yield x;
    x = f(x);
  }
}

function isPrime(n) {
  for (let d = 2; d * d <= n; d += 1) {
    if (n % d === 0) return false;
  }
  return true;
}

const primes = naturalsFrom(2).filter(isPrime);
console.log(primes.take(10).toArray().join(" "));
console.log(iterate((x) => 2 * x, 1).take(8).toArray().join(" "));

for (const n of naturalsFrom(1)) {
  if (n * n > 50) {
    console.log("first square over 50:", n * n);
    break;
  }
}
```

```output for=infinite
2 3 5 7 11 13 17 19 23 29
1 2 4 8 16 32 64 128
first square over 50: 64
```

- Verdict: native. A generator function with an endless loop is an infinite iterator. The iterator helpers `filter` and `take` are lazy, and `toArray` reads only the elements that `take` lets through.
- A `for...of` loop with `break` also stops an infinite generator: the loop calls the `return` method of the generator, so the generator ends.
