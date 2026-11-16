```javascript run id=template-literals
const name = "Ada";
const price = 4.5;
const items = ["tea", "milk"];

console.log(`${name} pays ${(price * 2).toFixed(2)} for ${items.length} items.`);
console.log(`Two lines:
${items.join(", ")}`);
console.log(`{name} and \${name} are text.`);

function tag(parts, ...values) {
  return parts.raw.join("|") + " / " + values.join("|");
}
console.log(tag`a${1}b${2}c`);
```

```output for=template-literals
Ada pays 9.00 for 2 items.
Two lines:
tea, milk
{name} and ${name} are text.
a|b|c / 1|2
```

- Verdict: native. A template literal in backticks accepts any expression in `${...}`, and it can span lines. There is no format part, so the code calls a method such as `toFixed` for the decimals. A brace alone is text, and `\${` gives the text `${`. A tag function receives the text parts and the values separately and returns any value.
