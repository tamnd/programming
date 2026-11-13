```python run id=sort-returns-none
import random

# list.sort() sorts in place and returns None. sorted() returns a new list.
nums = [3, 1, 2]
print(sorted(nums), nums)
print(nums.sort(), nums)

# The common bug: the name now holds None, not a list.
scores = [70, 90, 80]
scores = scores.sort()
print(scores)

# random.shuffle and list.reverse also work in place and return None.
deck = [1, 2, 3, 4]
print(random.shuffle(deck), deck.reverse(), len(deck))

# reversed() is neither: it returns an iterator over the same list.
letters = ["a", "b", "c"]
r = reversed(letters)
letters[0] = "z"
print(type(r).__name__, list(r))

# += changes the list in place, but a = a + b makes a new list.
a = [1, 2]
alias = a
a += [3]
print(alias)
a = a + [4]
print(alias, a)

# A copy is shallow: the new list holds the same inner lists.
rows = [[2, "b"], [1, "a"]]
ordered = sorted(rows)
ordered[0][1] = "changed"
print(rows)
```

```output
[1, 2, 3] [3, 1, 2]
None [1, 2, 3]
None
None None 4
list_reverseiterator ['c', 'b', 'z']
[1, 2, 3]
[1, 2, 3] [1, 2, 3, 4]
[[2, 'b'], [1, 'changed']]
```

- Verdict: native. `sort()`, `random.shuffle()` and `reverse()` work in place and return `None`, so `scores = scores.sort()` loses the list. `reversed()` returns an iterator over the same list, so a change after the call shows in it. `+=` changes the list in place and an alias sees it, but `a = a + [4]` makes a new list. `sorted()` copies only the outer list, so a change to an inner list shows in the original.
