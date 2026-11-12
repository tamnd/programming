```rust run id=invoice
struct Item {
    name: &'static str,
    price: u32,
    quantity: u32,
}

fn invoice_before(customer: &str, items: &[Item]) -> String {
    let mut lines = vec![format!("Invoice for {customer}")];
    for item in items {
        lines.push(format!(
            "  {}: {} x {}.{:02}",
            item.name,
            item.quantity,
            item.price / 100,
            item.price % 100
        ));
    }
    // compute the total
    let mut total = 0;
    for item in items {
        total += item.price * item.quantity;
    }
    lines.push(format!("Total: {}.{:02}", total / 100, total % 100));
    lines.join("\n")
}

fn format_cents(cents: u32) -> String {
    format!("{}.{:02}", cents / 100, cents % 100)
}

fn total_cents(items: &[Item]) -> u32 {
    items.iter().map(|item| item.price * item.quantity).sum()
}

fn invoice_after(customer: &str, items: &[Item]) -> String {
    let mut lines = vec![format!("Invoice for {customer}")];
    for item in items {
        lines.push(format!("  {}: {} x {}", item.name, item.quantity, format_cents(item.price)));
    }
    lines.push(format!("Total: {}", format_cents(total_cents(items))));
    lines.join("\n")
}

fn main() {
    let items = [
        Item { name: "pen", price: 250, quantity: 2 },
        Item { name: "pad", price: 1205, quantity: 1 },
    ];
    let expected = "Invoice for Ada\n  pen: 2 x 2.50\n  pad: 1 x 12.05\nTotal: 17.05";
    let versions: [(&str, fn(&str, &[Item]) -> String); 2] =
        [("invoice_before", invoice_before), ("invoice_after", invoice_after)];
    for (name, invoice) in versions {
        assert_eq!(invoice("Ada", &items), expected);
        assert_eq!(invoice("Bob", &[]), "Invoice for Bob\nTotal: 0.00");
        println!("{name}: tests pass");
    }
    println!("{}", invoice_after("Ada", &items));
}
```

```output for=invoice
invoice_before: tests pass
invoice_after: tests pass
Invoice for Ada
  pen: 2 x 2.50
  pad: 1 x 12.05
Total: 17.05
```

- Verdict: idiom. The refactoring makes a function. The borrow checker decides how each variable can pass: `&[Item]` for a value that the fragment reads, `&mut` for one that it changes, or by value for one that it consumes.
- A fragment that assigns two variables can return a tuple: `let (count, total) = add_up(&items);`.
- The usual tests are `#[test]` functions in a `#[cfg(test)]` module. This page checks both versions in `main` so that the block runs as one program.
- rust-analyzer has the assist `extract_function`, which finds the parameters and the return value.
