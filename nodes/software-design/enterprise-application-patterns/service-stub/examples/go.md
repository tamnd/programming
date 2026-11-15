```go run id=tax-service-stub
package main

import (
	"errors"
	"fmt"
)

// TaxService gives the rate in basis points: 650 is 6.5%.
type TaxService interface {
	RateBp(region string) (int64, error)
}

// RemoteTaxService is the real service. Here it stands for a remote call that is not reachable.
type RemoteTaxService struct{}

func (RemoteTaxService) RateBp(string) (int64, error) {
	return 0, errors.New("the tax service is not reachable")
}

// TaxServiceStub runs locally, fast and in memory, with canned rates.
type TaxServiceStub struct{ calls int }

var stubRates = map[string]int64{"north": 650, "south": 0}

func (s *TaxServiceStub) RateBp(region string) (int64, error) {
	s.calls++
	if r, ok := stubRates[region]; ok {
		return r, nil
	}
	return 500, nil // a default rate for the other regions
}

type BillingService struct{ taxes TaxService }

func (b BillingService) TotalCents(amount int64, region string) (int64, error) {
	r, err := b.taxes.RateBp(region)
	if err != nil {
		return 0, err
	}
	return amount + amount*r/10000, nil
}

func money(c int64) string { return fmt.Sprintf("%d.%02d", c/100, c%100) }

func main() {
	if _, err := (BillingService{RemoteTaxService{}}).TotalCents(10000, "north"); err != nil {
		fmt.Println("real service: error:", err)
	}

	stub := &TaxServiceStub{}
	billing := BillingService{stub}
	for _, region := range []string{"north", "south", "east"} {
		t, _ := billing.TotalCents(10000, region)
		fmt.Printf("%s: 100.00 with tax is %s\n", region, money(t))
	}
	fmt.Println("the stub got", stub.calls, "calls")
}
```

```output
real service: error: the tax service is not reachable
north: 100.00 with tax is 106.50
south: 100.00 with tax is 100.00
east: 100.00 with tax is 105.00
the stub got 3 calls
```

- Verdict: idiom. `TaxService` is an interface that the stub satisfies by its method set. The stub has a pointer receiver, so the program can read `calls` after the calls.
- The `net/http/httptest` package can run a stub HTTP server in the test process, for code that calls a service over HTTP.
