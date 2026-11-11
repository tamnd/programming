```bash run id=points
describe() {
  case "$1,$2" in
    0,0) echo "origin" ;;
    *,0) echo "on the x axis at $1" ;;
    0,*) echo "on the y axis at $2" ;;
    *)
      if [ "$1" = "$2" ]; then
        echo "on the diagonal at $1"
      else
        echo "at $1, $2"
      fi
      ;;
  esac
}

for point in "0 0" "3 0" "0 -2" "4 4" "1 5"; do
  set -- $point
  echo "($1, $2): $(describe "$1" "$2")"
done
```

```output for=points
(0, 0): origin
(3, 0): on the x axis at 3
(0, -2): on the y axis at -2
(4, 4): on the diagonal at 4
(1, 5): at 1, 5
```

- Verdict: absent. Bash has no structured values to take apart. A `case` statement matches one string against glob patterns, such as `*,0`, and the first match wins.
- A pattern has no guard, so the diagonal test is an `if` inside the last branch.
- A glob pattern is not a regular expression. `*` matches any string, and `?` matches one character.
