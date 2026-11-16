```python run id=anonymous-function-python
words = ["house", "car", "bike"]
print(*sorted(words, key=lambda w: len(w)))

print(*map(lambda x: x * x, [1, 2, 3]))

n = 5
print((lambda x: x + n)(10))
```

```output for=anonymous-function-python
car bike house
1 4 9
15
```

- Verdict: native. `lambda w: len(w)` and `lambda x: x * x` are anonymous functions. The last lambda uses the variable `n` from the code around it.
