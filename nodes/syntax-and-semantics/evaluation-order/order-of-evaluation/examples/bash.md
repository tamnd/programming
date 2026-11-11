```bash run id=lists
check() {
  echo "check $1"
  return "$2"
}

check a 0 && check b 1 || echo "fallback"
check c 1 || check d 0 && echo "done"
```

```output for=lists
check a
check b
fallback
check c
check d
done
```

- Verdict: native. In a list, Bash runs the commands from left to right. `&&` runs the next command only if the last status was 0, and `||` runs it only if the status was not 0. The two operators have equal precedence and group from the left.
- Words in a command are expanded from left to right before the command runs.

```bash wrong id=and-or
deploy() {
  echo "deploy"
  return 0
}

notify() {
  echo "notify failed"
  return 1
}

deploy && notify || echo "deploy failed"
```

```output for=and-or
deploy
notify failed
deploy failed
```

`a && b || c` is not an if-then-else. When `b` fails, `c` runs too. Here the deploy succeeded, but the script says that it failed. Use `if deploy; then notify; else echo "deploy failed"; fi`.
