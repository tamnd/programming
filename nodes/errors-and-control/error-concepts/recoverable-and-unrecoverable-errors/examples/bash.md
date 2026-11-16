```bash run id=recoverable-bash
port_or_default() {
  # A bad fallback is a bug in the caller: stop the script.
  if (( $2 < 1 || $2 > 65535 )); then
    echo "fallback out of range" >&2
    exit 70
  fi
  # Bad text is an expected error: recover.
  if [[ $1 =~ ^[0-9]+$ ]]; then
    echo "$1"
  else
    echo "$2 (not a number)"
  fi
}

for text in 8080 x; do
  echo "$text -> $(port_or_default "$text" 80)"
done
```

```output for=recoverable-bash
8080 -> 8080
x -> 80 (not a number)
```

- Verdict: idiom. The shell has only exit statuses. A function returns a status for an expected error, and `exit` stops the script for a bug. Inside a command substitution, as here, `exit` stops only the subshell, so a careful script also tests the status of the substitution.
