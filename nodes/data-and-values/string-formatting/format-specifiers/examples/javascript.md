```javascript run id=number-methods
const x = 3.14159;
console.log(`[${x.toFixed(2).padStart(8)}] [${x.toFixed(2).padEnd(8)}] [${x.toFixed(2).padStart(8, "0")}]`);
console.log(`[${(255).toString(16)}] [${(5).toString(2)}] [${(1234.5).toExponential(2)}] [${(1234.5).toPrecision(2)}]`);
console.log(new Intl.NumberFormat("en-US", { minimumFractionDigits: 2, maximumFractionDigits: 2 }).format(1234567.891));
```

```output for=number-methods
[    3.14] [3.14    ] [00003.14]
[ff] [101] [1.23e+3] [1.2e+3]
1,234,567.89
```

- Verdict: absent. JavaScript has no format strings. Each part of a format is a method: `toFixed` for the decimals, `toExponential` and `toPrecision`, `toString(16)` for a base, `padStart` and `padEnd` for the width, and `Intl.NumberFormat` for digit groups.
