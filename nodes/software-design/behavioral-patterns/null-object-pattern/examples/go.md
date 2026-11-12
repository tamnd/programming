```go run id=null-logger
package main

import "fmt"

type Logger interface {
	Log(message string)
}

type ConsoleLogger struct{}

func (ConsoleLogger) Log(message string) { fmt.Println("log: " + message) }

type NullLogger struct{}

func (NullLogger) Log(string) {} // do nothing

type Checkout struct {
	logger Logger
}

func (c Checkout) Pay(amount int) string {
	c.logger.Log(fmt.Sprintf("paid %d", amount)) // no check for nil
	return fmt.Sprintf("receipt: %d", amount)
}

func main() {
	fmt.Println(Checkout{ConsoleLogger{}}.Pay(10))
	fmt.Println(Checkout{NullLogger{}}.Pay(20))
}
```

```output
log: paid 10
receipt: 10
receipt: 20
```

- Verdict: idiom. `NullLogger` is an empty struct whose `Log` method does nothing. The standard library has the same idea in `io.Discard`, a `Writer` on which all `Write` calls succeed without doing anything.
