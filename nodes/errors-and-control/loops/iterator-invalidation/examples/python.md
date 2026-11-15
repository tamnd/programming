```python run id=set-error-backward-index-and-two-passes
# A set also checks its size during a loop.
tags = {"a", "b", "c"}
try:
    for t in tags:
        tags.add(t.upper())
except RuntimeError as e:
    print("RuntimeError:", e)

# Removing by index from the end moves only elements already passed.
nums = [1, 2, 2, 3, 4]
for i in range(len(nums) - 1, -1, -1):
    if nums[i] % 2 == 0:
        del nums[i]
print(nums)

# Two passes: collect the keys, then delete them.
stock = {"pen": 0, "ink": 5, "pad": 0}
empty = [k for k, v in stock.items() if v == 0]
for k in empty:
    del stock[k]
print(stock)

# Changing values, not keys, is allowed.
prices = {"pen": 2, "ink": 5}
for k in prices:
    prices[k] *= 10
print(prices)
```

```output for=set-error-backward-index-and-two-passes
RuntimeError: Set changed size during iteration
[1, 3]
{'ink': 5}
{'pen': 20, 'ink': 50}
```

- Verdict: native. A set raises the same `RuntimeError` as a dictionary when its size changes in a loop. A list has no check, so the code deletes by index from the end, where a deletion moves only the elements that the loop has already seen. The dictionary loop collects the keys first and deletes them in a second loop. An assignment to an existing key does not change the size, so the last loop is safe.
