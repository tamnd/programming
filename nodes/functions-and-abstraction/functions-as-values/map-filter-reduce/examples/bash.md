```bash run id=map-filter-reduce-bash
is_even() { (($1 % 2 == 0)); }
square() { echo $(($1 * $1)); }
add() { echo $(($1 + $2)); }

squares=()
for x in 1 2 3 4 5 6; do
  if is_even "$x"; then
    squares+=("$(square "$x")")
  fi
done
echo "${squares[*]}"

total=0
for x in "${squares[@]}"; do
  total=$(add "$total" "$x")
done
echo "$total"
```

```output for=map-filter-reduce-bash
4 16 36
56
```

- Verdict: idiom. Bash has no map, filter or reduce, and functions are not values. A `for` loop calls a function for each word, an `if` keeps the words that pass the test, and a variable holds the running total.
