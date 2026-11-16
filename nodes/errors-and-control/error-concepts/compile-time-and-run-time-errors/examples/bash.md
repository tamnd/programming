```bash run id=when-bash
items=(10 20 30)
for i in 1 5; do
  echo "items[$i] = '${items[$i]}'"
done
```

```output for=when-bash
items[1] = '20'
items[5] = ''
```

- Verdict: absent. Bash reads a line, or a full compound command, before it runs it, so a syntax error later in the script is found only when Bash reaches it. An index past the end of an array is not an error: the result is an empty string.
