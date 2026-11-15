```go run id=shop-on-two-servers
package main

import (
	"fmt"
	"sort"
	"strings"
)

// The "database" is a set of tables that every server shares.
// A real system keeps these tables in a database server and uses database/sql.

const timeout = 30 // minutes with no request before a session ends

type session struct {
	user     string
	lastUsed int
}

type line struct {
	key  string // the session ID for a cart line, the order ID for an order line
	item string
	qty  int
}

var (
	sessions   = map[string]*session{}
	cartLines  []*line // the pending data
	orders     []string
	orderLines []*line // the committed business data
)

func hhmm(t int) string { return fmt.Sprintf("%d:%02d", t/60, t%60) }

func start(sid, user string, now int) { sessions[sid] = &session{user, now} }

// touch returns the user of the session, or "" if it is unknown or idle.
func touch(sid string, now int) string {
	s, ok := sessions[sid]
	if !ok || now-s.lastUsed > timeout {
		return ""
	}
	s.lastUsed = now
	return s.user
}

func cart(sid string) string {
	var parts []string
	for _, l := range cartLines {
		if l.key == sid {
			parts = append(parts, fmt.Sprintf("%s %d", l.item, l.qty))
		}
	}
	return strings.Join(parts, ", ")
}

func add(server, sid, item string, now int) {
	user := touch(sid, now)
	var found *line
	for _, l := range cartLines {
		if l.key == sid && l.item == item {
			found = l
		}
	}
	if found != nil {
		found.qty++
	} else {
		cartLines = append(cartLines, &line{sid, item, 1})
	}
	fmt.Printf("%s %s adds %s on server %s: cart %s\n", hhmm(now), user, item, server, cart(sid))
}

// checkout turns the pending cart lines into an order.
// A database does this in one transaction.
func checkout(server, sid string, now int) {
	user := touch(sid, now)
	lines := cart(sid)
	orders = append(orders, user)
	oid := fmt.Sprint(len(orders))
	var rest []*line
	for _, l := range cartLines {
		if l.key == sid {
			orderLines = append(orderLines, &line{oid, l.item, l.qty})
		} else {
			rest = append(rest, l)
		}
	}
	cartLines = rest
	fmt.Printf("%s %s checks out on server %s: order %s with %s\n", hhmm(now), user, server, oid, lines)
}

func report() {
	sums := map[string]int{}
	var items []string
	for _, l := range orderLines {
		if _, ok := sums[l.item]; !ok {
			items = append(items, l.item)
		}
		sums[l.item] += l.qty
	}
	sort.Strings(items)
	var parts []string
	for _, i := range items {
		parts = append(parts, fmt.Sprintf("%s %d", i, sums[i]))
	}
	fmt.Println("sales:", strings.Join(parts, ", "))
}

func purge(now int) {
	var ids []string
	for id, s := range sessions {
		if now-s.lastUsed > timeout {
			ids = append(ids, id)
		}
	}
	sort.Strings(ids)
	idle := map[string]bool{}
	for _, id := range ids {
		idle[id] = true
		delete(sessions, id)
	}
	var rest []*line
	for _, l := range cartLines {
		if !idle[l.key] {
			rest = append(rest, l)
		}
	}
	cartLines = rest
	fmt.Printf("%s purge sessions idle for more than %d minutes: %s\n", hhmm(now), timeout, strings.Join(ids, ", "))
}

func main() {
	start("s1", "ann", 600)
	start("s2", "bob", 601)
	add("a", "s1", "pen", 600)
	add("a", "s2", "ink", 601)
	add("b", "s1", "pen", 603) // server b reads the rows that server a wrote
	add("a", "s1", "ink", 604)
	checkout("b", "s1", 625)
	report()
	purge(640)
	fmt.Printf("rows left: sessions %d, cart lines %d, orders %d, order lines %d\n",
		len(sessions), len(cartLines), len(orders), len(orderLines))
}
```

```output
10:00 ann adds pen on server a: cart pen 1
10:01 bob adds ink on server a: cart ink 1
10:03 ann adds pen on server b: cart pen 2
10:04 ann adds ink on server a: cart pen 2, ink 1
10:25 ann checks out on server b: order 1 with pen 2, ink 1
sales: ink 1, pen 2
10:40 purge sessions idle for more than 30 minutes: s2
rows left: sessions 1, cart lines 0, orders 1, order lines 2
```

- Verdict: stdlib. The `database/sql` package is part of the standard library, but a real server also needs the driver of its database. The example keeps the tables in package variables, so it runs with no driver.
- `touch` returns an empty string for a session that is unknown or idle. The example sorts the idle IDs, because the order of a Go map is not fixed.
