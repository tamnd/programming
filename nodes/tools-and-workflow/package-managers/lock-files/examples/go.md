```go run id=lock-file-with-hashes
// A lock file records the exact version and the SHA-256 hash of each package, and an install checks both.
package main

import (
	"crypto/sha256"
	"encoding/hex"
	"fmt"
	"maps"
	"slices"
	"strconv"
	"strings"
)

// registry maps a name to a version to the bytes of the package.
var registry = map[string]map[string][]byte{
	"log": {"1.1.0": []byte("log 1.1.0"), "1.2.0": []byte("log 1.2.0")},
	"web": {"2.0.0": []byte("web 2.0.0"), "2.0.1": []byte("web 2.0.1")},
}

// manifest maps a name to the major version that the project accepts.
var manifest = map[string]int{"log": 1, "web": 2}

type entry struct{ name, version, digest string }

func key(v string) []int {
	var k []int
	for _, part := range strings.Split(v, ".") {
		n, _ := strconv.Atoi(part)
		k = append(k, n)
	}
	return k
}

func hash(data []byte) string {
	sum := sha256.Sum256(data)
	return hex.EncodeToString(sum[:])[:16]
}

func resolve() []entry {
	var chosen []entry
	var parts []string
	for _, name := range slices.Sorted(maps.Keys(manifest)) {
		best := ""
		for v := range registry[name] {
			if key(v)[0] == manifest[name] && (best == "" || slices.Compare(key(v), key(best)) > 0) {
				best = v
			}
		}
		chosen = append(chosen, entry{name, best, ""})
		parts = append(parts, name+" "+best)
	}
	fmt.Println("resolve:", strings.Join(parts, ", "))
	return chosen
}

func lock(chosen []entry) []entry {
	fmt.Println("lock file:")
	var lockfile []entry
	for _, e := range chosen {
		e.digest = hash(registry[e.name][e.version])
		fmt.Printf("  %s %s sha256 %s\n", e.name, e.version, e.digest)
		lockfile = append(lockfile, e)
	}
	return lockfile
}

func install(lockfile []entry) {
	fmt.Println("install from the lock file:")
	for _, e := range lockfile {
		if actual := hash(registry[e.name][e.version]); actual == e.digest {
			fmt.Printf("  %s %s ok\n", e.name, e.version)
		} else {
			fmt.Printf("  %s %s error: expected sha256 %s, got %s\n", e.name, e.version, e.digest, actual)
		}
	}
}

func main() {
	lockfile := lock(resolve())
	registry["log"]["1.3.0"] = []byte("log 1.3.0")
	fmt.Println("log 1.3.0 is published")
	resolve()
	install(lockfile)
	registry["web"]["2.0.1"] = []byte("web 2.0.1 with other code")
	fmt.Println("web 2.0.1 is changed on the registry")
	install(lockfile)
}
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

- Verdict: idiom. `sha256.Sum256` returns an array of 32 bytes, and `hex.EncodeToString` turns a slice of it into text.
- The lock file is a slice of structs, so its order is fixed; Go map order is random, so `resolve` sorts the names first.
- `lock` sets the field `digest` on a copy of each entry, because the loop variable is a copy.
