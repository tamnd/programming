```javascript run id=longest-operator-first
let i = 1, j = 10;
const k = i+++j; // lexed as i ++ + j, the longest operator first
console.log(k, i);

console.log(1_000_000, 0b1010, 0o17, 0xff, 1e3);
const name = "world";
console.log(`hello ${name}`, "a\tb".length, String.raw`a\tb`.length);
const café = "identifiers can use Unicode letters";
console.log(café.length);
```

```output for=longest-operator-first
11 2
1000000 10 15 255 1000
hello world 3 4
35
```

```javascript wrong id=semicolon-after-return
function settings() {
  return
  {
    debug: true;
  }
}

console.log(settings());
```

```output for=semicolon-after-return
undefined
```

- Verdict: library. JavaScript has no tokenizer in its standard library, but parsers such as Acorn, Esprima and the TypeScript compiler expose one. The lexer reads the longest operator it can, so `i+++j` is `i++ + j`. Numeric separators, binary and octal literals and template literals are tokens of the language, and identifiers can use Unicode letters.
- The wrong block puts a line break after `return`. Automatic semicolon insertion ends the statement there, so the function returns `undefined`, and the object below becomes a block with a label that never runs. Put the value on the same line as `return`.
