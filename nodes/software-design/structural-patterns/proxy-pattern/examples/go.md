```go run id=report
package main

import (
	"fmt"
	"sync"
)

type Report interface{ Render() string }

type diskReport struct{ orders int }

func loadReport() *diskReport {
	fmt.Println("load: read the report from disk")
	return &diskReport{orders: 42}
}

func (r *diskReport) Render() string { return fmt.Sprintf("report: %d orders", r.orders) }

// lazyReport is a virtual proxy. sync.OnceValue runs loadReport on the first call only.
type lazyReport struct{ real func() *diskReport }

func newLazyReport() lazyReport { return lazyReport{real: sync.OnceValue(loadReport)} }

func (l lazyReport) Render() string { return l.real().Render() }

// guardedReport is a protection proxy.
type guardedReport struct {
	inner Report
	role  string
}

func (g guardedReport) Render() string {
	if g.role != "admin" {
		return "access denied"
	}
	return g.inner.Render()
}

func main() {
	var report Report = newLazyReport()
	fmt.Println("proxy made")
	fmt.Println(report.Render())
	fmt.Println(report.Render())
	for _, role := range []string{"guest", "admin"} {
		fmt.Printf("%s: %s\n", role, guardedReport{report, role}.Render())
	}
}
```

```output
proxy made
load: read the report from disk
report: 42 orders
report: 42 orders
guest: access denied
admin: report: 42 orders
```

- Verdict: stdlib. `sync.OnceValue` wraps a function so that it runs once, and each later call returns the first result. It is safe to call from many goroutines.
- Go interfaces are satisfied without a declaration, so a proxy is any type with the same methods. The client holds a `Report` and does not know which type it has.
- Go has no dynamic proxy. A proxy for an interface with many methods must forward each of them by hand, or embed the interface and override only some methods.
- `httputil.ReverseProxy` in the standard library is a remote proxy for HTTP. It is an `http.Handler` that sends each request to another server.
