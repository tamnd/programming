```bash run id=strategy-bash
divide() {
  if (( $2 == 0 )); then
    echo "division by zero" >&2
    return 1
  fi
  echo $(( $1 / $2 ))
}

for pair in "10 2" "10 0"; do
  read -r a b <<< "$pair"
  if out=$(divide "$a" "$b" 2>&1); then
    echo "$a / $b = $out"
  else
    echo "$a / $b: error: $out"
  fi
done
```

```output for=strategy-bash
10 / 2 = 5
10 / 0: error: division by zero
```

- Verdict: native. Strategy: exit status. A function returns a status from 0 to 255, where 0 means success, and writes its message to standard error. The caller tests the status with `if`. `set -e` makes the shell stop on a failed command, with exceptions for commands in a test.
