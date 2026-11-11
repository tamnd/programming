```bash run id=dynamic
level="global"

reads_level() {
  echo "$level"
}

caller() {
  local level="caller"
  reads_level
}

reads_level
caller
```

```output for=dynamic
global
caller
```

- Verdict: absent. Bash has dynamic scope. A function sees the variables of the function that called it, not of the place where it is written. `reads_level` prints `caller` when `caller` calls it.
- A variable that is not declared with `local` is shared between the function and its caller. `local` limits the variable to the function and the functions that it calls.

```bash wrong id=shared-counter
print_row() {
  for ((i = 0; i < 3; i++)); do
    printf '%s%d ' "$1" "$i"
  done
  echo
}

for ((i = 0; i < 3; i++)); do
  print_row "r$i:"
done
```

```output for=shared-counter
r0:0 r0:1 r0:2
```

`print_row` uses the global `i`, the same variable as the outer loop. When `print_row` returns, `i` is `3`, so the outer loop stops after one row. Add `local i` as the first line of `print_row`.
