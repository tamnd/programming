```python run id=dict-items-and-a-changed-size
stock = {"apple": 3, "pear": 0, "plum": 5}
for name, count in stock.items():
    print(name, count)

# Deleting from a dictionary during the loop is an error.
try:
    for name in stock:
        if stock[name] == 0:
            del stock[name]
except RuntimeError as e:
    print("error:", e)

# Loop over a copy of the keys, or build a new dictionary.
stock = {"apple": 3, "pear": 0, "plum": 5}
for name in list(stock):
    if stock[name] == 0:
        del stock[name]
print(sorted(stock))
in_stock = {k: v for k, v in stock.items() if v > 0}
print(in_stock)
```

```output for=dict-items-and-a-changed-size
apple 3
pear 0
plum 5
error: dictionary changed size during iteration
['apple', 'plum']
{'apple': 3, 'plum': 5}
```

- Verdict: native. `for name, count in stock.items()` takes each pair apart in the header. The loop that deletes `pear` fails at the next step with a `RuntimeError`, because the dictionary changed size. `list(stock)` makes a copy of the keys, so the second loop can delete safely, and a comprehension builds a new dictionary with no deletes at all.
