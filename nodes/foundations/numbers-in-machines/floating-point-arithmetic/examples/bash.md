Bash arithmetic works on integers only. A decimal point in `$(( ))` is a syntax error.

```bash fails expect=exit:1 id=no-floats
#!/usr/bin/env bash
echo $((0.1 + 0.2))
```

```output
```

Keep amounts as integers in the smallest unit, such as cents, and put the decimal point in only for output.

```bash run id=cents
#!/usr/bin/env bash
total=0
for price in 199 250 1005; do
  total=$((total + price))
done
printf 'total: %d.%02d\n' $((total / 100)) $((total % 100))
```

```output
total: 14.54
```

- Verdict: absent. Bash has no floating-point type. Tools such as `awk` and `bc` do the arithmetic when a script needs fractions.
