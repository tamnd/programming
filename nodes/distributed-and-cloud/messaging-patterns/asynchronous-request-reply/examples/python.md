```python run id=status-polling
JOB_TIME = 5  # seconds that the worker needs for one report


class Api:
    def __init__(self):
        self.jobs = {}  # request id -> time the job started
        self.keys = {}  # idempotency key -> request id
        self.queued = 0

    def post(self, key, month, now):
        if not month:
            return 400, {}, "month is missing"
        if key in self.keys:  # a retry: give back the first status URL
            rid = self.keys[key]
        else:
            rid = "r%d" % (len(self.jobs) + 1)
            self.keys[key], self.jobs[rid] = rid, now
            self.queued += 1
        return 202, {"Location": "/status/" + rid, "Retry-After": "2"}, ""

    def get(self, path, now):
        kind, rid = path.strip("/").split("/")
        if rid not in self.jobs:
            return 404, {}, ""
        elapsed = now - self.jobs[rid]
        if kind == "status":
            if elapsed < JOB_TIME:
                return 200, {}, "status=Running percentComplete=%d" % (elapsed * 100 // JOB_TIME)
            return 303, {"Location": "/reports/" + rid}, ""
        if elapsed < JOB_TIME:
            return 404, {}, ""
        return 200, {}, "report %s: 3 rows" % rid


def show(t, req, resp):
    code, headers, body = resp
    parts = [str(code)] + ["%s: %s" % kv for kv in headers.items()] + ([body] if body else [])
    print("t=%d %s -> %s" % (t, req, " ".join(parts)))
    return resp


api = Api()
code, h, _ = show(0, "POST /reports key=k1", api.post("k1", "2026-10", 0))
show(0, "POST /reports key=k1 (retry)", api.post("k1", "2026-10", 0))
show(0, "POST /reports key=k2, no month", api.post("k2", "", 0))
t, url = 0, h["Location"]
while True:
    t += int(h.get("Retry-After", "2"))
    code, h2, _ = show(t, "GET " + url, api.get(url, t))
    if code == 303:
        show(t, "GET " + h2["Location"], api.get(h2["Location"], t))
        break
show(t, "GET /status/r9", api.get("/status/r9", t))
print("jobs queued: %d" % api.queued)
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

- Verdict: idiom. Each handler returns a tuple of the status code, a dict of headers and a body, so the client code reads like a client of a real HTTP API.
- The dict `keys` maps an idempotency key to a request id, so a second POST with the same key gives back the first status URL.
- The time is a parameter, not the clock, so the output is the same at each run; a real server keeps the start time of each job in a store.
