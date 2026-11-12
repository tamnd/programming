```go run id=move-the-rules-in
package main

import (
	"errors"
	"fmt"
	"time"
)

func d(s string) time.Time {
	t, err := time.Parse(time.DateOnly, s)
	if err != nil {
		panic(err)
	}
	return t
}

const day = 24 * time.Hour

// Before: a data struct with exported fields, and the rules in the code that uses it.
type SubscriptionBefore struct{ Start, End time.Time }

func billingActive(s SubscriptionBefore, t time.Time) bool {
	return !t.Before(s.Start) && t.Before(s.End)
}
func reminderDaysLeft(s SubscriptionBefore, t time.Time) int {
	return max(0, int(s.End.Sub(t)/day))
}

// After: unexported fields, a constructor that checks the rule, and methods.
type Subscription struct{ start, end time.Time }

var errEnd = errors.New("end must be after start")

func NewSubscription(start, end time.Time) (Subscription, error) {
	if !end.After(start) {
		return Subscription{}, errEnd
	}
	return Subscription{start, end}, nil
}

func (s Subscription) IsActive(t time.Time) bool { return !t.Before(s.start) && t.Before(s.end) }
func (s Subscription) DaysLeft(t time.Time) int  { return max(0, int(s.end.Sub(t)/day)) }
func (s Subscription) Renewed(days int) Subscription {
	return Subscription{s.start, s.end.AddDate(0, 0, days)}
}

func main() {
	old := SubscriptionBefore{d("2026-01-01"), d("2026-02-01")}
	sub, err := NewSubscription(d("2026-01-01"), d("2026-02-01"))
	if err != nil {
		panic(err)
	}
	for _, t := range []time.Time{d("2025-12-31"), d("2026-01-20"), d("2026-02-01")} {
		if sub.IsActive(t) != billingActive(old, t) || sub.DaysLeft(t) != reminderDaysLeft(old, t) {
			panic("rules differ at " + t.Format(time.DateOnly))
		}
	}
	old.End = old.End.AddDate(0, 0, 30)
	next := sub.Renewed(30)
	if next.DaysLeft(d("2026-02-01")) != reminderDaysLeft(old, d("2026-02-01")) || sub.DaysLeft(d("2026-02-01")) != 0 {
		panic("renewal")
	}
	if _, err := NewSubscription(d("2026-02-01"), d("2026-01-01")); !errors.Is(err, errEnd) {
		panic("the rule must be checked")
	}
	fmt.Println("subscription: tests pass")
}
```

```output
subscription: tests pass
```

- Go has no constructors. A function such as `NewSubscription` that returns a value and an error is the usual way to check a rule at creation. In this one-file example `main` could still write `Subscription{}` directly; the protection is real when the type is in its own package.
- The methods have value receivers, and `Renewed` returns a value, so a `Subscription` behaves as an immutable value.
- `max` is a built-in function since Go 1.21.
