```bash run id=pipeline
yes "line" | head -n 3
seq 1 1000000000 | awk '$1 * $1 > 50 { print; exit }'

config=""
get_config() {
  if [[ -z $config ]]; then
    echo "reading config" >&2
    config="debug"
  fi
}
get_config
get_config
echo "$config"
```

```output for=pipeline
line
line
line
8
debug
```

- Verdict: idiom. The stages of a pipeline run at the same time, and each stage reads only as much input as it needs. When `head` exits, `yes` gets `SIGPIPE` and stops, so an endless producer is safe.
- `&&` and `||` run the right command only when the status of the left command requires it.
