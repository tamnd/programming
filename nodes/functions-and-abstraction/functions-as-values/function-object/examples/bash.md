```bash run id=function-object-bash
factor=3
calls=0

triple() {
  calls=$((calls + 1))
  result=$(($1 * factor))
}

triple 5
echo "$result"

out=()
for x in 1 2 3; do
  triple "$x"
  out+=("$result")
done
echo "${out[*]}"
echo "$calls"
```

```output for=function-object-bash
15
3 6 9
4
```

- Verdict: absent. Bash has no objects. The function keeps its data in the global variables `factor` and `calls` and gives its result in `result`. A command substitution such as `$(triple 5)` runs in a subshell, and the change to `calls` would be lost.
