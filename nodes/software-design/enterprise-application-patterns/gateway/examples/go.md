```go run id=shipping-gateway
package main

import (
	"fmt"
	"math"
	"strconv"
)

// foreignQuote is the foreign API of a shipping service. Its names and types do not
// fit our code: the weight is in grams, the flags are a string, and the result is a
// map with a return code and the amount as a string of cents.
func foreignQuote(weightGrams int, dest, svcFlags string) map[string]string {
	fmt.Printf("  connection: quote(weight_grams=%d, dest=%s, svc_flags=%s)\n", weightGrams, dest, svcFlags)
	if weightGrams > 30000 {
		return map[string]string{"rc": "17", "msg": "too heavy"}
	}
	return map[string]string{"rc": "0", "amt": strconv.Itoa(500 + weightGrams/100*10), "cur": "EUR"}
}

// connection is the call to the foreign API; a test can replace it.
type connection func(weightGrams int, dest, svcFlags string) map[string]string

// ShippingGateway is our interface to the shipping service, in the terms of our code.
type ShippingGateway struct {
	conn connection
}

// NoService is the error for a parcel that the service does not take.
type NoService struct{ code, msg string }

func (e NoService) Error() string { return "code " + e.code + ": " + e.msg }

func (g ShippingGateway) StandardCost(kilograms float64, country string) (int, string, error) {
	reply := g.conn(int(math.Round(kilograms*1000)), country, "STD")
	if reply["rc"] != "0" {
		return 0, "", NoService{reply["rc"], reply["msg"]}
	}
	cents, err := strconv.Atoi(reply["amt"])
	if err != nil {
		return 0, "", err
	}
	return cents, reply["cur"], nil
}

func show(g ShippingGateway, kilograms float64, country string) {
	cents, currency, err := g.StandardCost(kilograms, country)
	if err != nil {
		fmt.Printf("parcel of %.1f kg to %s: no service (%v)\n", kilograms, country, err)
		return
	}
	fmt.Printf("parcel of %.1f kg to %s costs %d.%02d %s\n", kilograms, country, cents/100, cents%100, currency)
}

func main() {
	gateway := ShippingGateway{foreignQuote}
	show(gateway, 1.2, "DE")
	show(gateway, 31.0, "DE")

	// A test gives the gateway a connection that returns canned data.
	canned := ShippingGateway{func(int, string, string) map[string]string {
		return map[string]string{"rc": "0", "amt": "999", "cur": "EUR"}
	}}
	show(canned, 2.0, "FR")
}
```

```output
  connection: quote(weight_grams=1200, dest=DE, svc_flags=STD)
parcel of 1.2 kg to DE costs 6.20 EUR
  connection: quote(weight_grams=31000, dest=DE, svc_flags=STD)
parcel of 31.0 kg to DE: no service (code 17: too heavy)
parcel of 2.0 kg to FR costs 9.99 EUR
```

- Verdict: idiom. The connection is a function type, and the gateway returns the cost and an error. `NoService` is an error type, so the caller can test for it with `errors.As`.
- `strconv.Atoi` checks the string of cents, and the gateway returns its error.
