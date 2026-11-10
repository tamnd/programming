```python run id=defined
x = 2**31 - 1
print(x + 1)
print(2**100)

try:
    [1, 2, 3][3]
except IndexError as e:
    print("IndexError:", e)
```

```output for=defined
2147483648
1267650600228229401496703205376
IndexError: list index out of range
```

- Verdict: absent. Python integers have an unlimited range, so integer arithmetic never overflows.
- An index out of range raises `IndexError`. Each error that C leaves undefined becomes an exception in Python.
- The `ctypes` module runs outside these rules. Its documentation warns that incorrect use can corrupt data or crash the process.
