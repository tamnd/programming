```go run id=shop
package main

import "fmt"

// The subsystem types are unexported, so code outside this package cannot use them.
type inventory struct{ stock map[string]int }

func (i *inventory) reserve(item string) bool {
	if i.stock[item] == 0 {
		return false
	}
	i.stock[item]--
	return true
}

func (i *inventory) release(item string) { i.stock[item]++ }

type payments struct{}

func (payments) charge(card string, amount float64) bool { return card != "expired" }

type shipping struct{ parcels int }

func (s *shipping) schedule(item string) int {
	s.parcels++
	return s.parcels
}

// Shop is the facade, and it is the only exported type.
type Shop struct {
	inv  *inventory
	pay  payments
	ship *shipping
}

func NewShop(stock map[string]int) *Shop {
	return &Shop{inv: &inventory{stock: stock}, ship: &shipping{}}
}

func (s *Shop) PlaceOrder(item, card string, price float64) string {
	if !s.inv.reserve(item) {
		return fmt.Sprintf("failed: %s is out of stock", item)
	}
	if !s.pay.charge(card, price) {
		s.inv.release(item)
		return fmt.Sprintf("failed: card declined, %s back in stock", item)
	}
	return fmt.Sprintf("ok: %s, paid %.2f, parcel %d", item, price, s.ship.schedule(item))
}

func (s *Shop) Stock(item string) int { return s.inv.stock[item] }

func main() {
	shop := NewShop(map[string]int{"book": 2, "lamp": 0})
	fmt.Println(shop.PlaceOrder("book", "visa", 12.50))
	fmt.Println(shop.PlaceOrder("book", "expired", 12.50))
	fmt.Println(shop.PlaceOrder("lamp", "visa", 30.00))
	fmt.Printf("stock of book: %d\n", shop.Stock("book"))
}
```

```output
ok: book, paid 12.50, parcel 1
failed: card declined, book back in stock
failed: lamp is out of stock
stock of book: 1
```

- Verdict: idiom. A Go package is the facade. A name that starts with an upper-case letter is exported, so `Shop`, `NewShop`, `PlaceOrder` and `Stock` are the interface, and the subsystem types are hidden.
- In a real program the subsystem would be in its own package, or in an `internal` directory, which only the packages near it may import.
- `NewShop` builds the subsystem. A version that takes the three parts as arguments lets a test pass fakes.
