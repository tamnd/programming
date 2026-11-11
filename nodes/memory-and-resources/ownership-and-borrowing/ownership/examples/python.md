```python run id=weak
import weakref


class Frame:
    def __init__(self, name):
        self.name = name


owner = Frame("frame 1")
view = weakref.ref(owner)
print(view().name)
del owner
print(view())
```

```output for=weak
frame 1
None
```

- Verdict: absent. All references to an object are equal, and the garbage collector frees the object when no reference remains. No reference owns the object.
- A weak reference does not keep its object alive, so it acts as a non-owning view. In CPython, reference counting frees the object at once when the last strong reference goes.
- A `with` block owns a resource, such as a file, for the duration of the block, and closes it at the end.
