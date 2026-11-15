```go run id=status-polling
package main

import (
	"fmt"
	"strings"
)

// jobTime is the number of seconds that the worker needs for one report.
const jobTime = 5

type header struct{ name, value string }

type response struct {
	code    int
	headers []header
	body    string
}

// jobs maps a request id to the time its job started, and keys maps an
// idempotency key to a request id.
type api struct {
	jobs   map[string]int
	keys   map[string]string
	queued int
}

func (a *api) post(key, month string, now int) response {
	if month == "" {
		return response{code: 400, body: "month is missing"}
	}
	rid, ok := a.keys[key]
	if !ok {
		rid = fmt.Sprintf("r%d", len(a.jobs)+1)
		a.keys[key] = rid
		a.jobs[rid] = now
		a.queued++
	}
	return response{code: 202, headers: []header{{"Location", "/status/" + rid}, {"Retry-After", "2"}}}
}

func (a *api) get(path string, now int) response {
	parts := strings.Split(strings.Trim(path, "/"), "/")
	kind, rid := parts[0], parts[1]
	started, ok := a.jobs[rid]
	if !ok {
		return response{code: 404}
	}
	elapsed := now - started
	if kind == "status" {
		if elapsed < jobTime {
			return response{code: 200, body: fmt.Sprintf("status=Running percentComplete=%d", elapsed*100/jobTime)}
		}
		return response{code: 303, headers: []header{{"Location", "/reports/" + rid}}}
	}
	if elapsed < jobTime {
		return response{code: 404}
	}
	return response{code: 200, body: fmt.Sprintf("report %s: 3 rows", rid)}
}

func show(t int, req string, r response) response {
	parts := []string{fmt.Sprint(r.code)}
	for _, h := range r.headers {
		parts = append(parts, h.name+": "+h.value)
	}
	if r.body != "" {
		parts = append(parts, r.body)
	}
	fmt.Printf("t=%d %s -> %s\n", t, req, strings.Join(parts, " "))
	return r
}

func headerOf(r response, name string) string {
	for _, h := range r.headers {
		if h.name == name {
			return h.value
		}
	}
	return ""
}

func main() {
	a := &api{jobs: map[string]int{}, keys: map[string]string{}}
	first := show(0, "POST /reports key=k1", a.post("k1", "2026-10", 0))
	show(0, "POST /reports key=k1 (retry)", a.post("k1", "2026-10", 0))
	show(0, "POST /reports key=k2, no month", a.post("k2", "", 0))
	var wait int
	fmt.Sscan(headerOf(first, "Retry-After"), &wait)
	t, url := 0, headerOf(first, "Location")
	for {
		t += wait
		r := show(t, "GET "+url, a.get(url, t))
		if r.code == 303 {
			loc := headerOf(r, "Location")
			show(t, "GET "+loc, a.get(loc, t))
			break
		}
	}
	show(t, "GET /status/r9", a.get("/status/r9", t))
	fmt.Printf("jobs queued: %d\n", a.queued)
}
```

```output
t=0 POST /reports key=k1 -> 202 Location: /status/r1 Retry-After: 2
t=0 POST /reports key=k1 (retry) -> 202 Location: /status/r1 Retry-After: 2
t=0 POST /reports key=k2, no month -> 400 month is missing
t=2 GET /status/r1 -> 200 status=Running percentComplete=40
t=4 GET /status/r1 -> 200 status=Running percentComplete=80
t=6 GET /status/r1 -> 303 Location: /reports/r1
t=6 GET /reports/r1 -> 200 report r1: 3 rows
t=6 GET /status/r9 -> 404
jobs queued: 1
```

- Verdict: idiom. A struct `response` holds the status code, a slice of headers and a body; a slice keeps the order of the headers, which a map does not.
- The map `keys` maps an idempotency key to a request id, so a second POST with the same key starts no second job.
- A real server would use `net/http`, with `w.Header().Set` for the `Location` header and `w.WriteHeader(http.StatusAccepted)`; the handlers here are plain methods, so the program needs no network.
