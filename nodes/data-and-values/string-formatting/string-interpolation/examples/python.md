```python run id=f-strings
name = "Ada"
price = 4.5
items = ["tea", "milk"]

print(f"{name} pays {price * 2:.2f} for {len(items)} items.")
print(f"{name!r} {items[0].upper()} {price = }")
print(f"{{name}} is not a placeholder, {{{name}}} is.")
width = 8
print(f"[{name:>{width}}]")
```

```output for=f-strings
Ada pays 9.00 for 2 items.
'Ada' TEA price = 4.5
{name} is not a placeholder, {Ada} is.
[     Ada]
```

- Verdict: native. An f-string accepts any expression in braces, a conversion such as `!r` after it, and a format after `:`. `{price = }` prints the expression, the sign `=` and the value, which helps in debug output. Two braces give one literal brace, and a format can contain a nested placeholder, such as the width `{width}`.
