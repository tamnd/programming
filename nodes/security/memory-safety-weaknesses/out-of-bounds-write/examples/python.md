```python fails expect=exception:IndexError id=index
def set_score(scores, player, score):
    scores[player] = score


scores = [0] * 4
set_score(scores, 2, 10)
print("player 2:", scores[2])
set_score(scores, 4, 99)
```

```python wrong id=negative
def set_score(scores, player, score):
    scores[player] = score


scores = [0] * 4
set_score(scores, -1, 99)
print(scores)
```

```output for=negative
[0, 0, 0, 99]
```

A negative index is not an error in Python. It counts from the end, so player `-1` changes the last score. Memory is safe, but the program wrote to the wrong element.

```python run id=check
def set_score(scores, player, score):
    if not 0 <= player < len(scores):
        raise ValueError(f"player {player} is out of range, the size is {len(scores)}")
    scores[player] = score


scores = [0] * 4
set_score(scores, 2, 10)
print("player 2:", scores[2])
for player in [4, -1]:
    try:
        set_score(scores, player, 99)
    except ValueError as error:
        print("refused:", error)
print(scores)
```

```output for=check
player 2: 10
refused: player 4 is out of range, the size is 4
refused: player -1 is out of range, the size is 4
[0, 0, 10, 0]
```

- Verdict: native. A list, `bytearray` or `array` raises `IndexError` for an index out of range, so a write cannot leave the object.
- Check the range yourself when the index comes from input, because a negative index is valid.
- The weakness comes back in native code. A C extension and `ctypes` write to memory that Python does not check.
