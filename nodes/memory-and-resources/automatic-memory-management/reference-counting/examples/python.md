```python run id=cascade
import sys


class Node:
    def __init__(self, name, child=None):
        self.name = name
        self.child = child

    def __del__(self):
        print("freed", self.name)


head = Node("first", Node("second", Node("third")))
second = head.child
base = sys.getrefcount(second)
alias = second
print("one more reference:", sys.getrefcount(second) - base)
del alias, second
print("drop head")
del head
print("done")
```

```output
one more reference: 1
drop head
freed first
freed second
freed third
done
```

- Verdict: native. CPython stores a reference count in the header of each object, and it frees the object when the count reaches zero. A cycle collector frees the groups of objects that refer to each other.
- `del head` removes the last reference to the first node. Its deallocation drops the reference to the second node, which then has a count of zero too, so the whole chain goes in one step.
- `sys.getrefcount` counts its own argument, so compare two calls in the same place instead of reading one value.
- The free-threaded build splits the count into a part for the thread that owns the object and a shared part for other threads.
