```go run id=struct-state-and-snapshot
package main

import (
	"errors"
	"fmt"
)

// A Turnstile is locked until a coin arrives, and a push locks it again.
type Turnstile struct {
	locked bool
	coins  int
	passes int
}

func (t *Turnstile) Coin() {
	t.coins++
	t.locked = false
}

func (t *Turnstile) Push() error {
	if t.locked {
		return errors.New("locked: insert a coin")
	}
	t.passes++
	t.locked = true
	return nil
}

func main() {
	t := &Turnstile{locked: true}
	fmt.Println("push:", t.Push())
	t.Coin()
	fmt.Println("push:", t.Push())
	fmt.Println("push:", t.Push())
	fmt.Printf("state: %+v\n", *t)

	snapshot := *t // copying the struct copies the state
	t.Coin()
	fmt.Printf("now %+v, snapshot %+v\n", *t, snapshot)
}
```

```output for=struct-state-and-snapshot
push: locked: insert a coin
push: <nil>
push: locked: insert a coin
state: {locked:true coins:1 passes:1}
now {locked:false coins:2 passes:1}, snapshot {locked:true coins:1 passes:1}
```

- Verdict: native. The fields of a struct hold its state, and methods with a pointer receiver change it. The turnstile refuses a push while it is locked, so the state stays valid. Fields that start with a lower-case letter cannot be reached from other packages. Assigning a struct copies all its fields, so `snapshot` keeps the old state, but fields that are slices, maps or pointers would still share data with the original.
