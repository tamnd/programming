```go run id=tiny-registry
// A registry keeps each published version for ever. A yank hides a version from new resolutions only.
package main

import (
	"fmt"
	"slices"
	"strconv"
	"strings"
)

func key(version string) []int {
	var parts []int
	for _, p := range strings.Split(version, ".") {
		n, _ := strconv.Atoi(p)
		parts = append(parts, n)
	}
	return parts
}

func compare(a, b string) int { return slices.Compare(key(a), key(b)) }

type Registry struct {
	name     string
	packages map[string]map[string]bool // package -> version -> yanked
}

func NewRegistry(name string) *Registry {
	return &Registry{name: name, packages: map[string]map[string]bool{}}
}

func (r *Registry) Publish(pkg, version string) {
	versions, ok := r.packages[pkg]
	if !ok {
		versions = map[string]bool{}
		r.packages[pkg] = versions
	}
	if _, exists := versions[version]; exists {
		fmt.Printf("publish %s: %s %s error: the version exists\n", r.name, pkg, version)
		return
	}
	versions[version] = false
	fmt.Printf("publish %s: %s %s ok\n", r.name, pkg, version)
}

func (r *Registry) Yank(pkg, version string) {
	r.packages[pkg][version] = true
	fmt.Printf("yank %s: %s %s\n", r.name, pkg, version)
}

func (r *Registry) Index(pkg string) {
	versions := r.packages[pkg]
	var all []string
	for v := range versions {
		all = append(all, v)
	}
	slices.SortFunc(all, compare)
	var parts []string
	for _, v := range all {
		if versions[v] {
			v += " (yanked)"
		}
		parts = append(parts, v)
	}
	fmt.Printf("index of %s: %s\n", pkg, strings.Join(parts, ", "))
}

// Find returns the newest version that is not yanked, or the pinned version if pin is not empty.
func (r *Registry) Find(pkg, pin string) string {
	versions := r.packages[pkg]
	if pin != "" { // an exact pin can still get a yanked version
		if _, ok := versions[pin]; ok {
			return pin
		}
		return ""
	}
	best := ""
	for v, yanked := range versions {
		if !yanked && (best == "" || compare(v, best) > 0) {
			best = v
		}
	}
	return best
}

func bestMatch(registries []*Registry, pkg string) {
	version, name := "", ""
	for _, r := range registries {
		if v := r.Find(pkg, ""); v != "" && (version == "" || compare(v, version) > 0) {
			version, name = v, r.name
		}
	}
	fmt.Printf("best match over all indexes: %s %s from %s\n", pkg, version, name)
}

func firstIndex(registries []*Registry, pkg string) {
	for _, r := range registries {
		if v := r.Find(pkg, ""); v != "" {
			fmt.Printf("first index with the package: %s %s from %s\n", pkg, v, r.name)
			return
		}
	}
}

func main() {
	public := NewRegistry("public")
	public.Publish("log", "1.0.0")
	public.Publish("log", "1.1.0")
	public.Publish("log", "1.1.0")
	public.Yank("log", "1.1.0")
	public.Index("log")
	fmt.Printf("install log: %s\n", public.Find("log", ""))
	fmt.Printf("install log ==1.1.0: %s\n", public.Find("log", "1.1.0"))

	internal := NewRegistry("internal")
	internal.Publish("billing", "1.0.0")
	public.Publish("billing", "99.0.0") // someone takes the same name on the public registry
	registries := []*Registry{internal, public}
	bestMatch(registries, "billing")
	firstIndex(registries, "billing")
}
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

- Verdict: idiom. A map from a name to a map from a version to a bool holds the registry, and `slices.Compare` compares the integer parts of two versions.
- Go map order is random, so `Index` sorts the versions with `slices.SortFunc` before it prints them, and `Find` keeps the largest version in a loop.
- `Find` takes an empty string for no pin, because Go has no optional parameters.
