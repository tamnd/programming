```bash run id=thunks
calls=0
total=""

compute_total() {
  calls=$((calls + 1))
  total=42
}

force() {
  "$1"
}

force compute_total
force compute_total
echo "plain: calls=$calls"

calls=0
total=""
force_once() {
  if [[ -z $total ]]; then
    "$1"
  fi
}

force_once compute_total
force_once compute_total
echo "memoized: calls=$calls total=$total"
```

```output for=thunks
plain: calls=2
memoized: calls=1 total=42
```

- Verdict: idiom. Bash has no closures. The name of a function is the thunk, and `"$1"` forces it. A global variable that the function fills one time makes it memoized.
- A force inside a command substitution, such as `$(force compute_total)`, runs in a subshell, so the stored value is lost when the subshell ends.
