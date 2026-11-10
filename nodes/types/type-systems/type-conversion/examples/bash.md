```bash run id=arithmetic
count="7"
echo $((count + 1))
name="abc"
echo $((name + 1))
sum="3 + 4"
echo $((sum * 2))
echo $((010 + 1))
if [[ "10" < "9" ]]; then
  echo "as strings, 10 sorts before 9"
fi
if ((10 > 9)); then
  echo "as numbers, 10 is greater than 9"
fi
```

```output for=arithmetic
8
1
14
9
as strings, 10 sorts before 9
as numbers, 10 is greater than 9
```

- Verdict: native. Every value is a string. Arithmetic expansion converts it to an integer when it is used.
- Inside `$(( ))` a variable name is evaluated as an arithmetic expression. `abc` names an unset variable, so `name + 1` is 1 and not an error.
- The value `"3 + 4"` is evaluated as an expression, so `sum * 2` is 14. A leading 0 makes a constant octal, so `010` is 8 and `010 + 1` is 9.
- `[[ a < b ]]` compares strings, and `(( a < b ))` compares numbers.
