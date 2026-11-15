```python run id=tiny-registry
# A registry keeps each published version for ever. A yank hides a version from new resolutions only.


def key(version):
    return tuple(int(part) for part in version.split("."))


class Registry:
    def __init__(self, name):
        self.name = name
        self.packages = {}  # package -> version -> yanked

    def publish(self, package, version):
        versions = self.packages.setdefault(package, {})
        if version in versions:
            print(f"publish {self.name}: {package} {version} error: the version exists")
            return
        versions[version] = False
        print(f"publish {self.name}: {package} {version} ok")

    def yank(self, package, version):
        self.packages[package][version] = True
        print(f"yank {self.name}: {package} {version}")

    def index(self, package):
        versions = self.packages.get(package, {})
        parts = [v + (" (yanked)" if versions[v] else "") for v in sorted(versions, key=key)]
        print(f"index of {package}: {', '.join(parts)}")

    def find(self, package, pin=None):
        versions = self.packages.get(package, {})
        if pin is not None:  # an exact pin can still get a yanked version
            return pin if pin in versions else None
        live = [v for v, yanked in versions.items() if not yanked]
        return max(live, key=key, default=None)


def best_match(registries, package):
    found = [(r.find(package), r.name) for r in registries if r.find(package)]
    version, name = max(found, key=lambda pair: key(pair[0]))
    print(f"best match over all indexes: {package} {version} from {name}")


def first_index(registries, package):
    for r in registries:
        version = r.find(package)
        if version is not None:
            print(f"first index with the package: {package} {version} from {r.name}")
            return


public = Registry("public")
public.publish("log", "1.0.0")
public.publish("log", "1.1.0")
public.publish("log", "1.1.0")
public.yank("log", "1.1.0")
public.index("log")
print(f"install log: {public.find('log')}")
print(f"install log ==1.1.0: {public.find('log', '1.1.0')}")

internal = Registry("internal")
internal.publish("billing", "1.0.0")
public.publish("billing", "99.0.0")  # someone takes the same name on the public registry
best_match([internal, public], "billing")
first_index([internal, public], "billing")
```

```output
publish public: log 1.0.0 ok
publish public: log 1.1.0 ok
publish public: log 1.1.0 error: the version exists
yank public: log 1.1.0
index of log: 1.0.0, 1.1.0 (yanked)
install log: 1.0.0
install log ==1.1.0: 1.1.0
publish internal: billing 1.0.0 ok
publish public: billing 99.0.0 ok
best match over all indexes: billing 99.0.0 from public
first index with the package: billing 1.0.0 from internal
```

- Verdict: idiom. A dict of dicts holds the registry, and `max` with `key=key` compares versions as tuples of integers, so 1.10.0 sorts after 1.9.0.
- `publish` checks for the version first and refuses to overwrite it, as crates.io and Maven Central do.
- `find` with a pin returns the pinned version even when it is yanked. Without a pin, it skips yanked versions.
