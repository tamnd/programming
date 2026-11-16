```python run id=nested-class-is-an-attribute
class Playlist:
    LIMIT = 3

    class Cursor:  # a class attribute of Playlist; no outer instance
        def __init__(self, owner):
            self.owner = owner
            self.i = 0

        def __iter__(self):
            return self

        def __next__(self):
            if self.i >= len(self.owner.songs):
                raise StopIteration
            self.i += 1
            return f"{self.owner.name}: {self.owner.songs[self.i - 1]}"

        def limit(self):
            return Playlist.LIMIT  # LIMIT alone would be a NameError here

    def __init__(self, name):
        self.name = name
        self.songs = []

    def __iter__(self):
        return Playlist.Cursor(self)


p = Playlist("road")
p.songs += ["one", "two"]
print(list(p), Playlist.Cursor(p).limit())
```

```output for=nested-class-is-an-attribute
['road: one', 'road: two'] 3
```

- Verdict: idiom. A class statement inside a class body makes a class attribute, `Playlist.Cursor`. It has no link to an instance of `Playlist`, so the code passes `owner` by hand, as in C# and C++.
- The body of a nested class or a method does not see the names of the outer class body. `limit` must write `Playlist.LIMIT`; the name `LIMIT` alone is not found.
