```javascript run id=to-iso-string
process.env.TZ = "America/New_York";

// toISOString always writes UTC, with milliseconds and the suffix Z.
const t = new Date(Date.UTC(2024, 6, 9, 12, 30, 5, 120));
console.log(t.toISOString());
console.log(JSON.stringify({ t }));
console.log(new Date(Date.UTC(-1, 0, 1)).toISOString()); // a year before 0 has six digits and a sign

// With no offset, a date-only text is UTC, but a date-time text is local time.
console.log(new Date("2024-07-09").toISOString());
console.log(new Date("2024-07-09T00:00").toISOString());
console.log(new Date("2024-07-09T00:00Z").toISOString());

// Week dates and ordinal dates are not in the format of Date.
console.log(Number.isNaN(Date.parse("2024-W28-2")), Number.isNaN(Date.parse("2024-191")));
```

```output
2024-07-09T12:30:05.120Z
{"t":"2024-07-09T12:30:05.120Z"}
-000001-01-01T00:00:00.000Z
2024-07-09T00:00:00.000Z
2024-07-09T04:00:00.000Z
2024-07-09T00:00:00.000Z
true true
```

- Verdict: native. `Date` has its own format, a simplified form of ISO 8601. `toISOString()` and `JSON.stringify` always write UTC with milliseconds. A text with no offset is UTC if it has only a date and local time if it has a time, so the same day gives two instants here. Temporal adds ISO 8601 durations and the zone suffix of RFC 9557.
