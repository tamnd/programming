```python run id=remove-the-middle
songs = [
    {"title": "Intro", "seconds": 90},
    {"title": "Theme", "seconds": 200},
    {"title": "Outro", "seconds": 60},
]


# Before: four methods forward, and one has a rule.
class PlaylistBefore:
    def __init__(self):
        self._songs = []

    def add(self, song):
        self._songs.append(song)

    def size(self):
        return len(self._songs)

    def at(self, i):
        return self._songs[i]

    def any_longer_than(self, seconds):
        return any(s["seconds"] > seconds for s in self._songs)

    def total_seconds(self):
        return sum(s["seconds"] for s in self._songs)


# After: the clients use the list; the rule is a function.
def total_seconds(playlist):
    return sum(s["seconds"] for s in playlist)


before = PlaylistBefore()
for s in songs:
    before.add(s)
after = list(songs)

assert before.size() == len(after)
assert before.at(-1)["title"] == after[-1]["title"]
assert before.any_longer_than(180) == any(s["seconds"] > 180 for s in after)
assert before.total_seconds() == total_seconds(after) == 350
print("playlist: tests pass")
```

```output
playlist: tests pass
```

- A Python list already supports `len`, indexing, slicing and iteration, so the forwarding methods are a second, smaller interface to the same thing.
- `any_longer_than` is a forwarding method with a name. If many clients ask the same question, it can stay as a function next to `total_seconds`.
