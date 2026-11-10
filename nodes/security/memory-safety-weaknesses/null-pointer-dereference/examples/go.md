```go fails expect=panic id=nil-pointer
package main

import "fmt"

type User struct {
	Quota int
}

var users = map[string]*User{"alice": {Quota: 10}, "bob": {Quota: 20}}

func main() {
	for _, name := range []string{"alice", "nobody"} {
		u := users[name] // nil when the key is missing
		fmt.Printf("%s: quota %d\n", name, u.Quota)
	}
}
```

A map returns the zero value of its element type for a missing key, and the zero value of a pointer is `nil`. The selector `u.Quota` on a nil pointer causes a run-time panic: `invalid memory address or nil pointer dereference`.

```go run id=comma-ok
package main

import "fmt"

type User struct {
	Quota int
}

var users = map[string]*User{"alice": {Quota: 10}, "bob": {Quota: 20}}

func main() {
	for _, name := range []string{"alice", "nobody"} {
		u, ok := users[name]
		if !ok || u == nil {
			fmt.Printf("%s: no such user\n", name)
			continue
		}
		fmt.Printf("%s: quota %d\n", name, u.Quota)
	}
}
```

```output for=comma-ok
alice: quota 10
nobody: no such user
```

- Verdict: absent. Pointers, maps, slices, channels, functions and interfaces can be `nil`, and the compiler does not check that the code tests them.
- Use the two-value form `v, ok := m[key]`, and check `err` before the other results of a function.
- Store values instead of pointers when the zero value is a valid value.
- A nil dereference panics and does not read memory. A server that does not recover the panic stops, so the impact is a denial of service, as in CVE-2020-29652.
