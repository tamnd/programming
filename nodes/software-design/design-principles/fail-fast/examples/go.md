```go run id=missing-setting
package main

import (
	"errors"
	"fmt"
	"strconv"
)

var config = map[string]string{"maxConections": "50"} // a typo in the file

func maxConnectionsSlow() int {
	n, err := strconv.Atoi(config["maxConnections"]) // "" for a missing key
	if err != nil {
		return 10
	}
	return n
}

func maxConnectionsFast() (int, error) {
	value, ok := config["maxConnections"]
	if !ok {
		return 0, errors.New("maxConnections not found in config")
	}
	return strconv.Atoi(value)
}

func main() {
	fmt.Printf("slow: %d\n", maxConnectionsSlow())
	if n, err := maxConnectionsFast(); err != nil {
		fmt.Printf("fast: %v\n", err)
	} else {
		fmt.Printf("fast: %d\n", n)
	}
}
```

```output
slow: 10
fast: maxConnections not found in config
```
