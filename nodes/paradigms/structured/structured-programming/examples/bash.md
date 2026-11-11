```bash run id=break
for r in 0 1 2; do
  for c in 0 1 2; do
    if (( r + c == 3 )); then
      echo "found $r $c"
      break 2
    fi
  done
done

n=0
while true; do
  (( n++ ))
  (( n % 2 == 0 )) && continue
  (( n > 5 )) && break
  echo "odd $n"
done
```

```output for=break
found 1 2
odd 1
odd 3
odd 5
```

- Verdict: native. Bash has `if`, `case`, `for`, `while`, `until` and functions, and no `goto`. `break n` and `continue n` act on the nth enclosing loop.
