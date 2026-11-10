```go run id=repository
package main

import "fmt"

type Repository interface {
	Get(key string) string
	Name() string
}

type Store struct{}

func (Store) Get(key string) string {
	fmt.Println("store: load " + key)
	return "value-" + key
}

func (Store) Name() string { return "store" }

// Logged embeds the interface, so Name is promoted from the inner value.
type Logged struct {
	Repository
}

func (l Logged) Get(key string) string {
	fmt.Println("log: get " + key)
	return l.Repository.Get(key)
}

type Cached struct {
	Repository
	cache map[string]string
}

func NewCached(inner Repository) *Cached {
	return &Cached{Repository: inner, cache: map[string]string{}}
}

func (c *Cached) Get(key string) string {
	value, ok := c.cache[key]
	if !ok {
		value = c.Repository.Get(key)
		c.cache[key] = value
	}
	return value
}

func main() {
	var repo Repository = Logged{NewCached(Store{})}
	for _, key := range []string{"a", "a", "b"} {
		fmt.Println(repo.Get(key))
	}
	fmt.Println("name: " + repo.Name())
}
```

```output
log: get a
store: load a
value-a
log: get a
value-a
log: get b
store: load b
value-b
name: store
```

- Verdict: idiom. A struct that embeds the interface gets the methods of the inner value as promoted methods. The decorator defines only `Get`, and `Name` forwards without code.
- `Cached` has a pointer receiver because `Get` writes to the map. The chain holds a `*Cached`, which satisfies `Repository`.
- For one operation, Go code uses a function that takes a handler and returns a handler. `http.StripPrefix` in `net/http` is an example.
- If the embedded field is nil, a call to a promoted method panics. The constructor is the place to check it.
