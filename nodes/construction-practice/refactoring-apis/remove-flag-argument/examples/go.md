```go run id=explicit-functions
package main

import "fmt"

type Booking struct {
	Customer string
	Seat     string
	Lounge   bool
}

// bookBefore takes a flag. At the call, book("Ana", true) does not say what true means.
func bookBefore(customer string, isPremium bool) Booking {
	b := Booking{Customer: customer, Seat: "standard"}
	if isPremium {
		b.Seat, b.Lounge = "front", true
	}
	return b
}

func regularBooking(customer string) Booking {
	return Booking{Customer: customer, Seat: "standard"}
}

func premiumBooking(customer string) Booking {
	b := regularBooking(customer)
	b.Seat, b.Lounge = "front", true
	return b
}

func main() {
	if regularBooking("Ana") != bookBefore("Ana", false) || premiumBooking("Ana") != bookBefore("Ana", true) {
		panic("mismatch")
	}
	fmt.Printf("%+v\n", premiumBooking("Ana"))
}
```

```output
{Customer:Ana Seat:front Lounge:true}
```
