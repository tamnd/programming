```python run id=type-var-tuple
def to_tuple[*Ts](*args: *Ts) -> tuple[*Ts]:
    return args


class Row[*Ts]:
    def __init__(self, *cells: *Ts) -> None:
        self.cells = cells


t = to_tuple(1, "a", 2.5)  # a checker infers tuple[int, str, float]
print(t)
r: Row[int, str] = Row(7, "x")
print(r.cells, to_tuple.__type_params__)
```

```output for=type-var-tuple
(1, 'a', 2.5)
(7, 'x') (Ts,)
```

- Verdict: native. `*Ts` declares a type variable tuple, so `to_tuple` and `Row` take any number of type arguments. A type checker infers `tuple[int, str, float]` for `t`. Python does not check these types when the program runs.
