```javascript run id=invoice
import assert from "node:assert/strict";

function invoiceBefore(customer, items) {
  const lines = [`Invoice for ${customer}`];
  for (const [name, price, quantity] of items) {
    lines.push(`  ${name}: ${quantity} x ${Math.trunc(price / 100)}.${String(price % 100).padStart(2, "0")}`);
  }
  // compute the total
  let total = 0;
  for (const [, price, quantity] of items) {
    total += price * quantity;
  }
  lines.push(`Total: ${Math.trunc(total / 100)}.${String(total % 100).padStart(2, "0")}`);
  return lines.join("\n");
}

function formatCents(cents) {
  return `${Math.trunc(cents / 100)}.${String(cents % 100).padStart(2, "0")}`;
}

function totalCents(items) {
  return items.reduce((sum, [, price, quantity]) => sum + price * quantity, 0);
}

function invoiceAfter(customer, items) {
  const lines = [`Invoice for ${customer}`];
  for (const [name, price, quantity] of items) {
    lines.push(`  ${name}: ${quantity} x ${formatCents(price)}`);
  }
  lines.push(`Total: ${formatCents(totalCents(items))}`);
  return lines.join("\n");
}

const items = [["pen", 250, 2], ["pad", 1205, 1]];
const expected = "Invoice for Ada\n  pen: 2 x 2.50\n  pad: 1 x 12.05\nTotal: 17.05";

for (const invoice of [invoiceBefore, invoiceAfter]) {
  assert.equal(invoice("Ada", items), expected);
  assert.equal(invoice("Bob", []), "Invoice for Bob\nTotal: 0.00");
  console.log(`${invoice.name}: tests pass`);
}
console.log(invoiceAfter("Ada", items));
```

```output for=invoice
invoiceBefore: tests pass
invoiceAfter: tests pass
Invoice for Ada
  pen: 2 x 2.50
  pad: 1 x 12.05
Total: 17.05
```

- Verdict: idiom. A function declaration is enough. The same checks run on both versions.
- A fragment that assigns several variables can return an object or an array, and the caller destructures it: `const { count, total } = addUp(items);`.
- A fragment that uses `this` must become a method, or take the object as a parameter. An arrow function keeps the `this` of the place where it is defined.
- Visual Studio Code offers "Extract to function" through the TypeScript language service, for JavaScript files too.
