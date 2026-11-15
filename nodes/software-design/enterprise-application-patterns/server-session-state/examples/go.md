```go run id=sessions-on-two-servers
package main

import (
	"fmt"
	"sort"
	"strconv"
	"strings"
)

const timeout = 30 // minutes with no request, then the session ends

type entry struct {
	text    string
	lastUse int
}

// sessionStore holds one server's sessions in its memory as serialized text, with the time of their last use.
type sessionStore struct {
	name     string
	sessions map[string]entry
	count    int
}

func newStore(name string) *sessionStore {
	return &sessionStore{name: name, sessions: map[string]entry{}}
}

func (s *sessionStore) newID() string {
	// A real server makes a random ID with a secure generator, such as crypto/rand.
	s.count++
	return s.name + strconv.Itoa(s.count)
}

func (s *sessionStore) load(sid string, now int) ([]line, bool) {
	e, ok := s.sessions[sid]
	if !ok || now-e.lastUse > timeout {
		return nil, false
	}
	return deserialize(e.text), true
}

func (s *sessionStore) save(sid string, cart []line, now int) {
	s.sessions[sid] = entry{serialize(cart), now}
}

func (s *sessionStore) ids() []string {
	ids := make([]string, 0, len(s.sessions))
	for sid := range s.sessions {
		ids = append(ids, sid)
	}
	sort.Strings(ids)
	return ids
}

func (s *sessionStore) sweep(now int) []string {
	var idle []string
	for _, sid := range s.ids() {
		if now-s.sessions[sid].lastUse > timeout {
			idle = append(idle, sid)
			delete(s.sessions, sid)
		}
	}
	return idle
}

// line is one item of the cart. A slice keeps the order of the items.
type line struct {
	item string
	qty  int
}

func serialize(cart []line) string {
	parts := make([]string, len(cart))
	for i, l := range cart {
		parts[i] = l.item + ":" + strconv.Itoa(l.qty)
	}
	return strings.Join(parts, ",")
}

func deserialize(text string) []line {
	var cart []line
	for _, part := range strings.Split(text, ",") {
		if item, qty, ok := strings.Cut(part, ":"); ok {
			n, _ := strconv.Atoi(qty)
			cart = append(cart, line{item, n})
		}
	}
	return cart
}

func show(cart []line) string {
	parts := make([]string, len(cart))
	for i, l := range cart {
		parts[i] = fmt.Sprintf("%s %d", l.item, l.qty)
	}
	return strings.Join(parts, ", ")
}

// add is one request. The client sends only its session ID; the state is on the server.
func add(server *sessionStore, sid, who, item string, now int) string {
	cart, ok := server.load(sid, now)
	var status string
	if !ok {
		if sid != "" {
			status = "session " + sid + " is unknown here, "
		}
		sid = server.newID()
		status += "new session " + sid
	} else {
		status = "session " + sid
	}
	found := false
	for i := range cart {
		if cart[i].item == item {
			cart[i].qty++
			found = true
		}
	}
	if !found {
		cart = append(cart, line{item, 1})
	}
	server.save(sid, cart, now)
	fmt.Printf("10:%02d %s adds %s on server %s: %s, cart %s\n", now, who, item, server.name, status, show(cart))
	return sid
}

func main() {
	a, b := newStore("a"), newStore("b")
	ann := add(a, "", "ann", "pen", 0)
	add(a, "", "bob", "ink", 1)
	add(a, ann, "ann", "pen", 2)
	add(b, ann, "ann", "ink", 3) // a load balancer sends this request to the other server
	add(a, ann, "ann", "ink", 30)
	fmt.Println("10:40 server a removes idle sessions:", strings.Join(a.sweep(40), ", "))
	for _, sid := range a.ids() {
		e := a.sessions[sid]
		fmt.Printf("server a keeps %s = '%s', last used at 10:%02d\n", sid, e.text, e.lastUse)
	}
}
```

```output
10:00 ann adds pen on server a: new session a1, cart pen 1
10:01 bob adds ink on server a: new session a2, cart ink 1
10:02 ann adds pen on server a: session a1, cart pen 2
10:03 ann adds ink on server b: session a1 is unknown here, new session b1, cart ink 1
10:30 ann adds ink on server a: session a1, cart pen 2, ink 1
10:40 server a removes idle sessions: a2
server a keeps a1 = 'pen:2,ink:1', last used at 10:30
```

- Verdict: idiom. The store is a map from the ID to an `entry`. An HTTP server in Go runs each request in its own goroutine, so a real store needs a `sync.Mutex` around the map.
- `ids` sorts the keys, because the order of a range over a map is not defined.
