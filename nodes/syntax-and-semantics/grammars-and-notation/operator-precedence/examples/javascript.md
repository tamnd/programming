```javascript run id=associativity-and-syntax-errors
// + groups to the left, so the order of the operands changes the result.
console.log(1 + 2 + "3", "1" + 2 + 3);

// ** groups to the right, and assignment too.
let a, b;
a = b = 2 ** 3 ** 2;
console.log(a, b);

// Some mixes are syntax errors, so the parser forces parentheses.
for (const src of ["-2 ** 2", "null ?? 0 || 1", "(null ?? 0) || 1", "(-2) ** 2"]) {
  try {
    console.log(src, "=>", new Function(`return ${src}`)());
  } catch (e) {
    console.log(src, "=>", e.name);
  }
}

// typeof binds more tightly than +, and ! more tightly than instanceof.
console.log(typeof 1 + 2, !{} instanceof Object);
// Comparisons do not chain: 3 > 2 > 1 is (3 > 2) > 1, and true converts to 1.
console.log(3 > 2 > 1, 1 < 3 < 2);
```

```output for=associativity-and-syntax-errors
33 123
512 512
-2 ** 2 => SyntaxError
null ?? 0 || 1 => SyntaxError
(null ?? 0) || 1 => 1
(-2) ** 2 => 4
number2 false
false true
```

- Verdict: native. `+` groups to the left, so `1 + 2 + "3"` adds first and `"1" + 2 + 3` joins strings. `**` and `=` group to the right. JavaScript rejects `-2 ** 2` and a mix of `??` with `||` without parentheses, so the program cannot depend on a level that readers do not know. `typeof 1 + 2` is `"number" + 2`, and `3 > 2 > 1` is false because the first comparison gives `true`, which converts to 1.
