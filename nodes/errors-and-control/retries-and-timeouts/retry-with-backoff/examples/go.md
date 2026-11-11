```go run id=backoff
package main

import (
	"errors"
	"fmt"
	"math/rand/v2"
)

var ErrTransient = errors.New("503 Service Unavailable")
var ErrPermanent = errors.New("400 Bad Request")

type fakeClock struct{ now int }

func (c *fakeClock) Sleep(ms int) { c.now += ms }

func retry(op func() (string, error), clock *fakeClock, rng *rand.Rand) (string, error) {
	const attempts, base, limit = 5, 100, 1000
	var err error
	for attempt := 0; attempt < attempts; attempt++ {
		var result string
		result, err = op()
		if err == nil {
			return result, nil
		}
		if !errors.Is(err, ErrTransient) {
			return "", fmt.Errorf("attempt %d: %w", attempt+1, err)
		}
		if attempt == attempts-1 {
			break
		}
		delay := rng.IntN(min(limit, base<<attempt) + 1)
		fmt.Printf("  t=%4d ms: attempt %d failed (%v); wait %d ms\n", clock.now, attempt+1, err, delay)
		clock.Sleep(delay)
	}
	return "", fmt.Errorf("gave up after %d attempts: %w", attempts, err)
}

func service(failures int, failure error) func() (string, error) {
	calls := 0
	return func() (string, error) {
		calls++
		if calls <= failures {
			return "", failure
		}
		return fmt.Sprintf("200 OK after %d calls", calls), nil
	}
}

func main() {
	rng := rand.New(rand.NewPCG(1, 2))
	cases := []struct {
		name string
		op   func() (string, error)
	}{
		{"flaky", service(3, ErrTransient)},
		{"bad request", service(1, ErrPermanent)},
		{"down", service(100, ErrTransient)},
	}
	for _, c := range cases {
		fmt.Println(c.name)
		result, err := retry(c.op, &fakeClock{}, rng)
		if err != nil {
			fmt.Println("  error:", err)
			continue
		}
		fmt.Println("  result:", result)
	}
}
```

```output
flaky
  t=   0 ms: attempt 1 failed (503 Service Unavailable); wait 77 ms
  t=  77 ms: attempt 2 failed (503 Service Unavailable); wait 123 ms
  t= 200 ms: attempt 3 failed (503 Service Unavailable); wait 314 ms
  result: 200 OK after 4 calls
bad request
  error: attempt 1: 400 Bad Request
down
  t=   0 ms: attempt 1 failed (503 Service Unavailable); wait 80 ms
  t=  80 ms: attempt 2 failed (503 Service Unavailable); wait 47 ms
  t= 127 ms: attempt 3 failed (503 Service Unavailable); wait 16 ms
  t= 143 ms: attempt 4 failed (503 Service Unavailable); wait 400 ms
  error: gave up after 5 attempts: 503 Service Unavailable
```

```go run id=deadline
package main

import (
	"context"
	"errors"
	"fmt"
	"time"
)

func fetch(ctx context.Context) error {
	select {
	case <-time.After(time.Second):
		return nil
	case <-ctx.Done():
		return ctx.Err()
	}
}

func main() {
	request, cancel := context.WithTimeout(context.Background(), 20*time.Millisecond)
	defer cancel()

	call, cancelCall := context.WithTimeout(request, time.Hour)
	defer cancelCall()

	rd, _ := request.Deadline()
	cd, _ := call.Deadline()
	fmt.Println("the call keeps the request deadline:", cd.Equal(rd))

	err := fetch(call)
	fmt.Println("fetch:", err)
	fmt.Println("deadline exceeded:", errors.Is(err, context.DeadlineExceeded))
}
```

```output
the call keeps the request deadline: true
fetch: context deadline exceeded
deadline exceeded: true
```

- Verdict: stdlib. Go has no retry function in the standard library. The package `context` carries deadlines and cancellation from a request to the calls that it makes.
- `errors.Is` separates the error that is worth a retry from the one that is not.
- A child context cannot extend the deadline of its parent. The call asks for an hour, but it keeps the 20 ms deadline of the request, so `fetch` stops after 20 ms with `context.DeadlineExceeded`.

A retry repeats the request. When the server applied the first request and only the response was lost, the retry applies it a second time. The customer pays twice.

```go wrong id=double-charge
package main

import (
	"errors"
	"fmt"
)

type bank struct {
	balance int
	calls   int
}

func (b *bank) Charge(amount int) error {
	b.calls++
	b.balance -= amount
	if b.calls == 1 {
		return errors.New("timeout: no response")
	}
	return nil
}

func main() {
	b := &bank{balance: 500}
	for attempt := 1; attempt <= 3; attempt++ {
		err := b.Charge(100)
		if err == nil {
			break
		}
		fmt.Println("attempt", attempt, "failed:", err)
	}
	fmt.Println("balance after one charge of 100:", b.balance)
}
```

```output
attempt 1 failed: timeout: no response
balance after one charge of 100: 300
```
