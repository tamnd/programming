```bash run id=callback-bash
repeat() {
  local n=$1 callback=$2
  for ((i = 1; i <= n; i++)); do
    "$callback" "$i"
  done
}

tick() {
  echo "tick $1"
}

printf '%s\n' 3 1 2 | sort -rn | paste -s -d ' ' -
repeat 2 tick
```

```output for=callback-bash
3 2 1
tick 1
tick 2
```

- Verdict: idiom. A Bash function is not a value, so `repeat` takes the name of a function and runs it as a command. The `sort` command takes options and no comparison function.
