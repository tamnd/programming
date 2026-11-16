```bash run id=exhaustive-matching-bash
action() {
  case $1 in
    red) echo "stop" ;;
    yellow) echo "slow down" ;;
    green) echo "go" ;;
    *)
      echo "unknown light: $1" >&2
      return 1
      ;;
  esac
}

for light in red yellow green; do
  echo "$light: $(action "$light")"
done
```

```output for=exhaustive-matching-bash
red: stop
yellow: slow down
green: go
```

- Verdict: absent. A `case` command matches strings, and nothing checks that it covers each value. If no pattern matches, no command runs, and the return status is zero. A last `*)` clause that fails finds a missing case at run time.
