```typescript run id=invoice
import assert from "node:assert/strict";

type Item = readonly [name: string, price: number, quantity: number];

function invoiceBefore(customer: string, items: readonly Item[]): string {
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

function formatCents(cents: number): string {
  return `${Math.trunc(cents / 100)}.${String(cents % 100).padStart(2, "0")}`;
}

function totalCents(items: readonly Item[]): number {
  return items.reduce((sum, [, price, quantity]) => sum + price * quantity, 0);
}

function invoiceAfter(customer: string, items: readonly Item[]): string {
  const lines = [`Invoice for ${customer}`];
  for (const [name, price, quantity] of items) {
    lines.push(`  ${name}: ${quantity} x ${formatCents(price)}`);
  }
  lines.push(`Total: ${formatCents(totalCents(items))}`);
  return lines.join("\n");
}

const items: Item[] = [["pen", 250, 2], ["pad", 1205, 1]];
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

- Verdict: idiom. The refactoring is the same as in JavaScript, and the extracted function gets parameter and return types.
- The types are a check of step 5. If the extracted function misses a parameter, the name is not in scope and the type checker reports it before any test runs.
- The type alias `Item` names the tuple once, so the source function and the extracted function share it.
- The TypeScript language service, used by Visual Studio Code, offers "Extract to function in module scope" and other targets for the extracted code.
