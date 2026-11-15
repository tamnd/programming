```python run id=rebuild
# Two rebuilders on one graph: file modification times (as in Make) and content hashes.
import hashlib

RULES = {
    "main.o": ["main.c", "util.h"],
    "util.o": ["util.c", "util.h"],
    "app": ["main.o", "util.o"],
}
ORDER = ["main.o", "util.o", "app"]
SOURCES = {
    "main.c": "int main(void) { return util(); }",
    "util.c": "int util(void) { return 1; }",
    "util.h": "int util(void);",
}
clock = 0


def tick():
    global clock
    clock += 1
    return clock


def action(target, content):
    # The compiler drops comments, so an object file depends only on the code.
    parts = [content[dep].split("//")[0].rstrip() for dep in RULES[target]]
    return target + "(" + " | ".join(parts) + ")"


def digest(text):
    return hashlib.sha256(text.encode()).hexdigest()


def from_scratch(content):
    values = {name: content[name] for name in SOURCES}
    for target in ORDER:
        values[target] = action(target, values)
    return values["app"]


class Builder:
    def __init__(self, by_hash):
        self.by_hash = by_hash
        self.content = {}
        self.mtime = {}
        self.traces = {}  # target -> {dependency: its hash at the last build}

    def write(self, name, text, mtime=None):
        self.content[name] = text
        self.mtime[name] = tick() if mtime is None else mtime

    def dirty(self, target):
        if target not in self.content:
            return True
        deps = RULES[target]
        if self.by_hash:
            seen = self.traces[target]
            return any(seen[dep] != digest(self.content[dep]) for dep in deps)
        return any(self.mtime[dep] > self.mtime[target] for dep in deps)

    def build(self):
        ran = []
        for target in ORDER:
            if self.dirty(target):
                self.write(target, action(target, self.content))
                self.traces[target] = {d: digest(self.content[d]) for d in RULES[target]}
                ran.append(target)
        return " ".join(ran) if ran else "-"


def clean(b):
    for name, text in SOURCES.items():
        b.write(name, text)


def touch(b):
    b.mtime["util.h"] = tick()


def comment(b):
    b.write("main.c", SOURCES["main.c"] + " // entry point")


def restore(b):
    # A copy from a backup keeps its old time, older than util.o.
    b.write("util.c", "int util(void) { return 2; }", mtime=1)


by_time, by_hash = Builder(False), Builder(True)
print("step".ljust(24) + "mtime".ljust(20) + "hash")
for label, edit in [("clean build", clean), ("touch util.h", touch),
                    ("comment in main.c", comment), ("old util.c, old time", restore)]:
    edit(by_time)
    edit(by_hash)
    print(label.ljust(24) + by_time.build().ljust(20) + by_hash.build())
for name, b in [("mtime", by_time), ("hash", by_hash)]:
    print(name + ": app matches a clean build:", b.content["app"] == from_scratch(b.content))
```

```output
step                    mtime               hash
clean build             main.o util.o app   main.o util.o app
touch util.h            main.o util.o app   -
comment in main.c       main.o app          main.o
old util.c, old time    -                   util.o app
mtime: app matches a clean build: False
hash: app matches a clean build: True
```

- Verdict: idiom. A dict maps each target to its inputs, and `hashlib.sha256` gives the hash of a string.
- `Builder.traces` keeps, for each target, a dict from each input to its hash at the last build. `dirty` compares these hashes, or the times, with the current state.
- `from_scratch` builds all targets again from the sources, so the last two lines compare each incremental result with a clean build.
