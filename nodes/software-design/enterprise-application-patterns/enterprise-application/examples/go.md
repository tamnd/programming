```go run id=booking-system
package main

import (
	"fmt"
	"strings"
	"sync"
)

type booking struct {
	flightID int
	customer string
	sent     bool
}

// Store stands in for the database. The mutex makes each method one transaction,
// so two users cannot both take the last seat.
type Store struct {
	mu        sync.Mutex
	city      map[int]string
	seatsLeft map[int]int
	bookings  []*booking
}

func (s *Store) Book(flightID int, customer string) string {
	s.mu.Lock()
	defer s.mu.Unlock()
	if s.seatsLeft[flightID] == 0 {
		return "no seat left"
	}
	s.seatsLeft[flightID]--
	s.bookings = append(s.bookings, &booking{flightID: flightID, customer: customer})
	return "ok"
}

// Many screens: each screen shows the same data in a different way.
func (s *Store) AgentScreen(flightID int) string {
	s.mu.Lock()
	defer s.mu.Unlock()
	var names []string
	for _, b := range s.bookings {
		if b.flightID == flightID {
			names = append(names, b.customer)
		}
	}
	return fmt.Sprintf("agent screen: flight %d to %s, seats left %d, booked by %s",
		flightID, s.city[flightID], s.seatsLeft[flightID], strings.Join(names, ", "))
}

func (s *Store) CustomerScreen(customer string) string {
	s.mu.Lock()
	defer s.mu.Unlock()
	var trips []string
	for _, b := range s.bookings {
		if b.customer == customer {
			trips = append(trips, fmt.Sprintf("flight %d to %s", b.flightID, s.city[b.flightID]))
		}
	}
	return "customer screen for " + customer + ": " + strings.Join(trips, "; ")
}

// NightBatch is batch processing and integration: it sends new bookings to another system as a file.
func (s *Store) NightBatch() []string {
	s.mu.Lock()
	defer s.mu.Unlock()
	var lines []string
	for _, b := range s.bookings {
		if !b.sent {
			lines = append(lines, fmt.Sprintf("%d;%s", b.flightID, b.customer))
			b.sent = true
		}
	}
	return lines
}

func main() {
	store := &Store{city: map[int]string{7: "Oslo"}, seatsLeft: map[int]int{7: 1}}
	for _, customer := range []string{"Ann", "Bob"} {
		fmt.Printf("%s books flight 7: %s\n", customer, store.Book(7, customer))
	}
	fmt.Println(store.AgentScreen(7))
	fmt.Println(store.CustomerScreen("Ann"))
	lines := store.NightBatch()
	fmt.Printf("night batch: %d booking sent to the airline system\n", len(lines))
	fmt.Println("airline file:", strings.Join(lines, "\n"))
	fmt.Println("second night batch:", len(store.NightBatch()), "bookings")
}
```

```output
Ann books flight 7: ok
Bob books flight 7: no seat left
agent screen: flight 7 to Oslo, seats left 0, booked by Ann
customer screen for Ann: flight 7 to Oslo
night batch: 1 booking sent to the airline system
airline file: 7;Ann
second night batch: 0 bookings
```

A `Store` value stands in for the database, and its `sync.Mutex` makes each method run alone, as a database transaction would. A real Go application would use `database/sql` and the transactions of the database.
