```bash run id=error-bash
parse() {
  if [[ $1 =~ ^-?[0-9]+$ ]]; then
    echo "$1"
  else
    return 1
  fi
}

for text in 42 x42; do
  if n=$(parse "$text"); then
    echo "$text -> $n"
  else
    echo "$text -> error (exit status $?)"
  fi
done
```

```output for=error-bash
42 -> 42
x42 -> error (exit status 1)
```

- Verdict: native. A shell command reports an error with its exit status: 0 means success, and a value from 1 to 255 means failure. `if` runs the `else` branch when the status is not 0. The shell does not stop on a failed command unless `set -e` is on.
