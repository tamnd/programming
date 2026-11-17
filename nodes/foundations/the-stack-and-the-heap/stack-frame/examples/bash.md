```bash run id=frames
count() {
  local n=$1
  local here=$((n * 10)) # local: one copy for each call
  echo "enter n=$n here=$here"
  if (( n > 1 )); then
    count $((n - 1))
  fi
  echo "leave n=$n here=$here"
}

count 3
```

```output for=frames
enter n=3 here=30
enter n=2 here=20
enter n=1 here=10
leave n=1 here=10
leave n=2 here=20
leave n=3 here=30
```

- Verdict: native. `local` gives each call of `count` its own `n` and `here`. Without `local`, the variables would be global, and the inner calls would change the values that the outer calls print.
