```bash run id=combinator-bash
identity() { echo "$1"; }
constant() { echo "$1"; }
flip() { "$1" "$3" "$2"; }
both() { "$1" "$3" && "$2" "$3"; }

sub() { echo $(($1 - $2)); }
is_even() { (($1 % 2 == 0)); }
is_positive() { (($1 > 0)); }

identity 5
constant 7 99
flip sub 2 10
kept=()
for x in -2 -1 0 1 2 3 4; do
  if both is_even is_positive "$x"; then
    kept+=("$x")
  fi
done
echo "${kept[*]}"
```

```output for=combinator-bash
5
7
8
2 4
```

- Verdict: idiom. Bash functions are not values. A function gets the names of other functions as arguments and calls them, and it runs them at once with the arguments.
