```bash run id=prefix
search=(grep -c -i)
printf 'Error\nok\nERROR\n' > log.txt
"${search[@]}" error log.txt

log_with() {
  local level=$1
  shift
  echo "[$level] $*"
}

warn() { log_with WARN "$@"; }
warn disk is full
```

```output for=prefix
2
[WARN] disk is full
```

- Verdict: idiom. A command and its first arguments can be stored in an array and run with more arguments. A small function that calls another function with a fixed first argument is a partial application.
- Shell functions are not values, so there is no closure and no curried function.
