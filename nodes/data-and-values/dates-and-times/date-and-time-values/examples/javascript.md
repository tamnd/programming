```javascript run id=date-is-an-instant
// A Date holds a count of milliseconds since 1970-01-01T00:00:00Z.
// The month index starts at 0, so 2 is March.
const d = new Date(Date.UTC(2024, 2, 9, 12, 0));
console.log(d.toISOString(), d.getTime());

// A Date can change. Copy it before a change.
const copy = new Date(d.getTime());
d.setUTCDate(d.getUTCDate() + 1);
console.log(d.toISOString(), copy.toISOString());

// A field that is out of range rolls over: February 30 is March 1.
console.log(new Date(Date.UTC(2024, 1, 30)).toISOString());

// The difference of two dates is a number of milliseconds.
console.log((d - copy) / 3600000);

// A date that is not valid holds NaN.
const bad = new Date("not a date");
console.log(Number.isNaN(bad.getTime()));
```

```output
2024-03-09T12:00:00.000Z 1709985600000
2024-03-10T12:00:00.000Z 2024-03-09T12:00:00.000Z
2024-03-01T00:00:00.000Z
24
true
```

- Verdict: native. Date is an instant with millisecond precision, and its methods read it in UTC or in the local zone of the host. It is mutable, and its month index starts at 0. Temporal, which has Instant, PlainDate, ZonedDateTime and Duration, replaces it in newer engines.
