```python run id=list
type IntList = tuple[int, IntList] | None


def total(xs: IntList) -> int:
    match xs:
        case None:
            return 0
        case (head, tail):
            return head + total(tail)


def length(xs: IntList) -> int:
    return 0 if xs is None else 1 + length(xs[1])


def show(xs: IntList) -> str:
    parts = []
    while xs is not None:
        parts.append(str(xs[0]))
        xs = xs[1]
    return " -> ".join(parts)


xs: IntList = (1, (2, (3, None)))
print("sum:", total(xs))
print("length:", length(xs))
print("list:", show(xs))
print("alias:", IntList.__value__)
```

```output for=list
sum: 6
length: 3
list: 1 -> 2 -> 3
alias: tuple[int, IntList] | None
```

- Verdict: native. The `type` statement (Python 3.12 and later) makes an alias that can name itself. Here a list is `None` or a pair of a head and a shorter list.
- The value of the alias is evaluated only when the program asks for it, so the name `IntList` can appear in its own definition.
- A class can also refer to itself, as the `Tree` class in the idea block does.
