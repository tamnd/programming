```javascript run id=intl-time-zone
// Intl.DateTimeFormat shows an instant in a named zone.
function inZone(date, timeZone) {
  const parts = new Intl.DateTimeFormat("en-CA", {
    timeZone, year: "numeric", month: "2-digit", day: "2-digit",
    hour: "2-digit", minute: "2-digit", hourCycle: "h23", timeZoneName: "shortOffset",
  }).formatToParts(date);
  const p = Object.fromEntries(parts.map((x) => [x.type, x.value]));
  return `${p.year}-${p.month}-${p.day} ${p.hour}:${p.minute} ${p.timeZoneName}`;
}

// One zone, two offsets: winter and summer.
console.log(inZone(new Date("2024-01-09T12:00:00Z"), "America/New_York"));
console.log(inZone(new Date("2024-07-09T12:00:00Z"), "America/New_York"));

// An overlap: two instants one hour apart show the same local time.
console.log(inZone(new Date("2024-11-03T05:30:00Z"), "America/New_York"));
console.log(inZone(new Date("2024-11-03T06:30:00Z"), "America/New_York"));

// Some offsets are not whole hours.
console.log(inZone(new Date("2024-01-09T12:00:00Z"), "Asia/Kolkata"));
```

```output
2024-01-09 07:00 GMT-5
2024-07-09 08:00 GMT-4
2024-11-03 01:30 GMT-4
2024-11-03 01:30 GMT-5
2024-01-09 17:30 GMT+5:30
```

- Verdict: native. A Date is an instant, and Intl.DateTimeFormat with the option timeZone shows it in any zone of the IANA database. Temporal.ZonedDateTime holds an instant with a zone, and its option disambiguation resolves gaps and overlaps.
