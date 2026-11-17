```javascript run id=frames
function count(n) {
  const here = n * 10; // a local: one copy in each call
  console.log(`enter n=${n} here=${here}`);
  if (n > 1) {
    count(n - 1);
  }
  console.log(`leave n=${n} here=${here}`);
}

count(3);
```

```output for=frames
enter n=3 here=30
enter n=2 here=20
enter n=1 here=10
leave n=1 here=10
leave n=2 here=20
leave n=3 here=30
```

- Verdict: native. Each call has its own execution context with its own `n` and `here`, so the inner calls do not change the values of the outer ones.
- The program cannot read a frame. `new Error().stack` gives only the names and places of the calls, as text.
