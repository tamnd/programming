```bash run id=local-function-bash
sum_of_scaled() {
  local factor=$1
  shift
  scale() {
    echo $(( $1 * factor ))
  }
  local total=0 v
  for v in "$@"; do
    total=$(( total + $(scale "$v") ))
  done
  echo "$total"
}

sum_of_scaled 10 1 2 3
sum_of_scaled 2 4
```

```output for=local-function-bash
60
8
```

- Verdict: idiom. A function defined in a function body becomes a global function when the outer function runs. It sees the local `factor` because Bash uses dynamic scope.
