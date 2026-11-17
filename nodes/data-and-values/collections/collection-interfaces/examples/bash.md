```bash run id=to-the-interface
# Bash has no interfaces. The convention: a function takes the name of an array.
describe() {
  local -n c=$1
  local sum=0 x
  for x in "${c[@]}"; do sum=$(( sum + x )); done
  echo "size ${#c[@]}, total $sum"
}

list=(1 2 3)
declare -A prices=([tea]=3 [cake]=4)
values=("${prices[@]}")

echo "list: $(describe list)"
echo "map values: $(describe values)"
```

```output for=to-the-interface
list: size 3, total 6
map values: size 2, total 7
```

- Verdict: absent. Bash has indexed and associative arrays, and no way to name an interface. A function that takes the name of an array works with any array, by convention only.
