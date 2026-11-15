```java run id=status-polling
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

public class Main {
    static final int JOB_TIME = 5; // seconds that the worker needs for one report

    record Response(int code, Map<String, String> headers, String body) {
        static Response of(int code, String body, String... headers) {
            Map<String, String> h = new LinkedHashMap<>();
            for (int i = 0; i < headers.length; i += 2) {
                h.put(headers[i], headers[i + 1]);
            }
            return new Response(code, h, body);
        }
    }

    static class Api {
        final Map<String, Integer> jobs = new HashMap<>(); // request id -> time the job started
        final Map<String, String> keys = new HashMap<>(); // idempotency key -> request id
        int queued = 0;

        Response post(String key, String month, int now) {
            if (month.isEmpty()) {
                return Response.of(400, "month is missing");
            }
            String rid = keys.get(key);
            if (rid == null) {
                rid = "r" + (jobs.size() + 1);
                keys.put(key, rid);
                jobs.put(rid, now);
                queued++;
            }
            return Response.of(202, "", "Location", "/status/" + rid, "Retry-After", "2");
        }

        Response get(String path, int now) {
            String[] parts = path.substring(1).split("/");
            Integer started = jobs.get(parts[1]);
            if (started == null) {
                return Response.of(404, "");
            }
            int elapsed = now - started;
            if (parts[0].equals("status")) {
                if (elapsed < JOB_TIME) {
                    return Response.of(200, "status=Running percentComplete=" + elapsed * 100 / JOB_TIME);
                }
                return Response.of(303, "", "Location", "/reports/" + parts[1]);
            }
            if (elapsed < JOB_TIME) {
                return Response.of(404, "");
            }
            return Response.of(200, "report " + parts[1] + ": 3 rows");
        }
    }

    static Response show(int t, String req, Response r) {
        StringBuilder sb = new StringBuilder(String.valueOf(r.code()));
        r.headers().forEach((k, v) -> sb.append(' ').append(k).append(": ").append(v));
        if (!r.body().isEmpty()) {
            sb.append(' ').append(r.body());
        }
        System.out.println("t=" + t + " " + req + " -> " + sb);
        return r;
    }

    public static void main(String[] args) {
        Api api = new Api();
        Response first = show(0, "POST /reports key=k1", api.post("k1", "2026-10", 0));
        show(0, "POST /reports key=k1 (retry)", api.post("k1", "2026-10", 0));
        show(0, "POST /reports key=k2, no month", api.post("k2", "", 0));
        int wait = Integer.parseInt(first.headers().get("Retry-After"));
        String url = first.headers().get("Location");
        int t = 0;
        while (true) {
            t += wait;
            Response r = show(t, "GET " + url, api.get(url, t));
            if (r.code() == 303) {
                String loc = r.headers().get("Location");
                show(t, "GET " + loc, api.get(loc, t));
                break;
            }
        }
        show(t, "GET /status/r9", api.get("/status/r9", t));
        System.out.println("jobs queued: " + api.queued);
    }
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

- Verdict: idiom. A record `Response` holds the status code, the headers in a `LinkedHashMap`, which keeps their order, and a body.
- The map `keys` maps an idempotency key to a request id, so a second POST with the same key starts no second job.
- The time is a parameter, not the clock, so the output is the same at each run; a real server keeps the start time of each job in a store.
