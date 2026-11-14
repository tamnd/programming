```go run id=map-of-slices
package main

import (
	"fmt"
	"net/url"
	"slices"
)

func main() {
	// A map of slices. append works on the nil slice of a missing key.
	m := map[string][]int{}
	m["yellow"] = append(m["yellow"], 1)
	m["blue"] = append(m["blue"], 2)
	m["yellow"] = append(m["yellow"], 3)
	fmt.Println(m["yellow"], m["red"], len(m["red"]))

	// Remove one value. Delete the key when its slice is empty.
	m["blue"] = slices.DeleteFunc(m["blue"], func(x int) bool { return x == 2 })
	if len(m["blue"]) == 0 {
		delete(m, "blue")
	}
	_, ok := m["blue"]
	fmt.Println(len(m), ok)

	// url.Values is a map[string][]string. Get returns the first value.
	q, err := url.ParseQuery("color=red&size=9&color=blue")
	if err != nil {
		panic(err)
	}
	fmt.Println(q.Get("color"), q["color"], q.Get("shape") == "")
	q.Add("color", "green")
	fmt.Println(q.Encode())
}
```

```output
[1 3] [] 0
1 false
red [red blue] true
color=red&color=blue&color=green&size=9
```

- Verdict: idiom. A map[K][]V. append works on the nil slice of a missing key, so no check is needed. url.Values and http.Header in the standard library are maps of string slices with Get for the first value.
