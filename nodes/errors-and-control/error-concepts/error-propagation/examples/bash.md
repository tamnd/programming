```bash run id=propagate-bash
parse_port() {
  case $1 in
    '' | *[!0-9]*)
      echo "not a number: $1"
      return 1
      ;;
  esac
  echo "$1"
}

load_config() {
  local line=$1 out
  if ! out=$(parse_port "${line#*=}"); then
    echo "config line '$line': $out"
    return 1
  fi
  echo "$out"
}

for line in "port=8080" "port=x"; do
  if out=$(load_config "$line"); then
    echo "port $out"
  else
    echo "error: $out"
  fi
done
```

```output for=propagate-bash
port 8080
error: config line 'port=x': not a number: x
```

- Verdict: idiom. A function returns a status, and each caller tests it with `if` or `||` and returns a status of its own. `set -e` makes the shell exit when a command fails, but it does not add context.
