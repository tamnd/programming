```javascript wrong id=number
const PRICES = { pen: 150, book: 1200 }; // in cents

function orderTotal(form) {
  const quantity = Number(form.quantity);
  if (Number.isNaN(quantity)) {
    throw new Error("not a number");
  }
  return PRICES[form.product] * quantity;
}

for (const quantity of ["2", "-3", "", "0x10", "1e3"]) {
  console.log(JSON.stringify(quantity), orderTotal({ product: "book", quantity }));
}
console.log(orderTotal({ product: "constructor", quantity: "1" }));
```

```output for=number
"2" 2400
"-3" -3600
"" 0
"0x10" 19200
"1e3" 1200000
NaN
```

```javascript run id=parse
const PRICES = new Map([["pen", 150], ["book", 1200]]); // in cents

function parseOrder(form) {
  if (typeof form.product !== "string" || !PRICES.has(form.product)) {
    throw new Error("unknown product");
  }
  if (typeof form.quantity !== "string" || !/^[0-9]{1,3}$/.test(form.quantity)) {
    throw new Error("quantity must be 1 to 3 ASCII digits");
  }
  const quantity = Number(form.quantity);
  if (quantity < 1 || quantity > 100) {
    throw new Error("quantity must be from 1 to 100");
  }
  return Object.freeze({ product: form.product, quantity });
}

function orderTotal(order) {
  return PRICES.get(order.product) * order.quantity;
}

const forms = [
  { product: "book", quantity: "2" },
  { product: "book", quantity: "-3" },
  { product: "book", quantity: "" },
  { product: "book", quantity: "0x10" },
  { product: "book", quantity: "1e3" },
  { product: "constructor", quantity: "1" },
];
for (const form of forms) {
  try {
    console.log(orderTotal(parseOrder(form)));
  } catch (error) {
    console.log("rejected:", error.message);
  }
}
```

```output for=parse
2400
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity must be 1 to 3 ASCII digits
rejected: unknown product
```

- Verdict: stdlib. JavaScript has no validation library in the language, but a regular expression, `Number` and `Map` are enough for a parser per field.
- `Number` ignores white space at both ends, reads `0x` as hexadecimal and accepts exponents and `Infinity`. It turns an empty string into `0`. A check for `NaN` alone misses all of these.
- A plain object as an allow list also answers for keys from `Object.prototype`. In the first program, `PRICES["constructor"]` is a function, so the product name passes the lookup and the total is `NaN`. Use a `Map`, an object made with `Object.create(null)`, or `Object.hasOwn`.
- In a JavaScript pattern without the `m` flag, `$` matches only at the end of the input, so `^...$` matches the whole string. A pattern with the `m` flag matches at each line end.
- Types in TypeScript do not exist at run time. A parameter typed as `number` still gets whatever the JSON had. Parse at the boundary, then give the rest of the code the parsed type.
