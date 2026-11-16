```bash run id=match-guards-bash
describe() {
  local n=$1
  case $n in
    0) echo "zero" ;;
    -*) echo "negative" ;;
    *)
      if ((n < 10 && n % 2 == 0)); then
        echo "small even"
      elif ((n < 10)); then
        echo "small odd"
      else
        echo "large"
      fi
      ;;
  esac
}

result=""
for n in -5 0 4 7 12; do
  result+="${result:+, }$(describe "$n")"
done
echo "$result"
```

```output for=match-guards-bash
negative, zero, small even, small odd, large
```

- Verdict: absent. A `case` pattern is a glob, and a clause has no condition after its pattern. The globs `0` and `-*` take zero and the negative numbers. The last clause tests the other conditions with an `if`.
