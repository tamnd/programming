```bash run id=three-kinds
words=(red blue red green)
declare -A counts
for w in "${words[@]}"; do
  counts[$w]=$(( ${counts[$w]:-0} + 1 ))
done

echo "${#words[@]} ${words[1]}"
echo "${#counts[@]} ${counts[red]}"
for k in "${!counts[@]}"; do
  echo "$k=${counts[$k]}"
done | sort | paste -sd ' '
```

```output for=three-kinds
4 blue
3 2
blue=1 green=1 red=2
```

- Verdict: native. Bash has indexed arrays and associative arrays. The elements are strings.
- `declare -A` makes an associative array. Bash has no set type, so the keys of an associative array act as a set.
- The order of the keys of an associative array is not fixed, so the code sorts them.
