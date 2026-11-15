```python run id=endless-count-and-a-cycle-check
from itertools import count

# count() never ends, so the loop needs its own exit.
for n in count(1):
    if n * n > 50:
        print("first square over 50:", n * n)
        break


def has_cycle(links, start):
    # Floyd's check: the fast walker meets the slow one only in a cycle.
    slow = fast = start
    while fast in links and links[fast] in links:
        slow = links[slow]
        fast = links[links[fast]]
        if slow == fast:
            return True
    return False


print("cycle:", has_cycle({"a": "b", "b": "c", "c": "d", "d": "b"}, "a"))
print("cycle:", has_cycle({"a": "b", "b": "c"}, "a"))
```

```output for=endless-count-and-a-cycle-check
first square over 50: 64
cycle: True
cycle: False
```

- Verdict: idiom. Python has no statement for an infinite loop, and `while True:` or a loop over the endless `itertools.count()` needs a `break` or a `return`. A naive walk over the first list, where `d` links back to `b`, would never end. The two walkers of the cycle check meet inside the cycle, so the check ends for both lists.
