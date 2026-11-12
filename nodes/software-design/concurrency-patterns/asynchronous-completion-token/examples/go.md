```go run id=act
package main

import "fmt"

type request struct {
	token int
	name  string
}

func service(requests []request) []request {
	replies := make([]request, 0, len(requests))
	for i := len(requests) - 1; i >= 0; i-- { // reverse order
		replies = append(replies, request{requests[i].token, requests[i].name + " loaded"})
	}
	return replies
}

func main() {
	pending := map[int]string{}
	var requests []request
	for i, name := range []string{"image-7", "image-9"} {
		token := i + 1
		pending[token] = name
		requests = append(requests, request{token, name})
		fmt.Printf("send %d: load %s\n", token, name)
	}
	for _, reply := range service(requests) {
		delete(pending, reply.token) // the token finds the state at once
		fmt.Printf("done %d: %s\n", reply.token, reply.name)
	}
}
```

```output
send 1: load image-7
send 2: load image-9
done 2: image-9 loaded
done 1: image-7 loaded
```

- Verdict: idiom. A struct carries the token, and a map holds the pending state.
