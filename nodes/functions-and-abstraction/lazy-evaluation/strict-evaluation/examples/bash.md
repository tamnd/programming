```bash run id=strict
show() {
  echo "inside show: n=$n, arguments: $*"
}

n=0
show "$((n += 1))" "$((n += 10))"
false && echo "never printed"
echo "status after the && list: $?"
```

```output for=strict
inside show: n=11, arguments: 1 11
status after the && list: 1
```

- Verdict: native. Before the shell runs a simple command, it expands the words from left to right. Both arithmetic expansions change `n` before `show` starts, so `show` sees `n=11`.
- `&&` and `||` run the right command only when the status of the left command requires it. `false && echo` never runs `echo`.
