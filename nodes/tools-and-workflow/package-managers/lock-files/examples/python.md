```python run id=lock-file-with-hashes
# A lock file records the exact version and the SHA-256 hash of each package, and an install checks both.
import hashlib

REGISTRY = {  # name -> version -> the bytes of the package
    "log": {"1.1.0": b"log 1.1.0", "1.2.0": b"log 1.2.0"},
    "web": {"2.0.0": b"web 2.0.0", "2.0.1": b"web 2.0.1"},
}
MANIFEST = {"log": 1, "web": 2}  # name -> the major version that the project accepts


def key(version):
    return tuple(int(part) for part in version.split("."))


def sha256(data):
    return hashlib.sha256(data).hexdigest()[:16]


def resolve():
    chosen = {}
    for name, major in sorted(MANIFEST.items()):
        fits = [v for v in REGISTRY[name] if key(v)[0] == major]
        chosen[name] = max(fits, key=key)
    print("resolve:", ", ".join(f"{n} {v}" for n, v in chosen.items()))
    return chosen


def lock(chosen):
    lockfile = {name: (version, sha256(REGISTRY[name][version])) for name, version in chosen.items()}
    print("lock file:")
    for name, (version, digest) in lockfile.items():
        print(f"  {name} {version} sha256 {digest}")
    return lockfile


def install(lockfile):
    print("install from the lock file:")
    for name, (version, digest) in lockfile.items():
        actual = sha256(REGISTRY[name][version])
        if actual == digest:
            print(f"  {name} {version} ok")
        else:
            print(f"  {name} {version} error: expected sha256 {digest}, got {actual}")


lockfile = lock(resolve())
REGISTRY["log"]["1.3.0"] = b"log 1.3.0"
print("log 1.3.0 is published")
resolve()
install(lockfile)
REGISTRY["web"]["2.0.1"] = b"web 2.0.1 with other code"
print("web 2.0.1 is changed on the registry")
install(lockfile)
```

```output
resolve: log 1.2.0, web 2.0.1
lock file:
  log 1.2.0 sha256 bc5900c6e46c5b3f
  web 2.0.1 sha256 a72ce45f235c38b5
log 1.3.0 is published
resolve: log 1.3.0, web 2.0.1
install from the lock file:
  log 1.2.0 ok
  web 2.0.1 ok
web 2.0.1 is changed on the registry
install from the lock file:
  log 1.2.0 ok
  web 2.0.1 error: expected sha256 a72ce45f235c38b5, got 6745f93439ad7399
```

- Verdict: idiom. `hashlib.sha256(data).hexdigest()` gives the hash as hexadecimal text.
- The lock file is a dict from each name to a tuple of the version and the hash. A real tool writes it to a file in a format such as TOML or JSON.
- `install` reads only the lock file, so the new version 1.3.0 does not change what it installs.
