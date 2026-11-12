```go run id=reactor
package main

import "fmt"

type Handler func(handle, data string)

var handlers = map[string]Handler{}

func echo(handle, data string) {
	if data != "" {
		fmt.Printf("%s: %s\n", handle, data)
	} else {
		fmt.Println("close", handle)
		delete(handlers, handle)
	}
}

func main() {
	handlers["listener"] = func(_, newHandle string) {
		fmt.Println("accept", newHandle)
		handlers[newHandle] = echo
	}
	// ready stands in for select: handle and data pairs in arrival order
	ready := [][2]string{
		{"listener", "conn-1"}, {"listener", "conn-2"},
		{"conn-1", "hello"}, {"conn-2", "hi"},
		{"conn-1", ""}, {"conn-2", ""},
	}
	for _, event := range ready {
		handlers[event[0]](event[0], event[1])
	}
}
```

```output
accept conn-1
accept conn-2
conn-1: hello
conn-2: hi
close conn-1
close conn-2
```

- Verdict: idiom. The handler table is a map from handle to function value.
