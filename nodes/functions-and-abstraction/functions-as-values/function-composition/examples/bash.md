```bash run id=function-composition-bash
inc() {
  local x
  read -r x
  echo $((x + 1))
}

twice() {
  local x
  read -r x
  echo $((x * 2))
}

echo 5 | inc | twice
echo 5 | twice | inc
echo 5 | inc | twice | inc
```

```output for=function-composition-bash
12
11
13
```

- Verdict: idiom. Bash functions are not values. Each function reads its input and writes its result, and a pipeline connects them from left to right.
