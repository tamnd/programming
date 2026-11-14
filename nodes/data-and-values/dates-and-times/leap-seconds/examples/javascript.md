```javascript run id=date-normalize
// Date counts milliseconds with leap seconds ignored, so it has no value for 23:59:60.
console.log(Date.parse("2016-12-31T23:59:60Z"));
console.log(new Date("2016-12-31T23:59:60Z").getTime());

// Date.UTC normalizes the second 60 to the next minute.
const leap = Date.UTC(2016, 11, 31, 23, 59, 60);
console.log(new Date(leap).toISOString());

// One second of Unix time crosses the leap second, which lasted two SI seconds.
const a = Date.parse("2016-12-31T23:59:59Z");
const b = Date.parse("2017-01-01T00:00:00Z");
console.log((b - a) / 1000, leap === b);
```

```output
NaN
NaN
2017-01-01T00:00:00.000Z
1 true
```

- Verdict: native. A `Date` counts milliseconds since 1970 "with leap seconds ignored". `Date.parse` rejects the second 60, and `Date.UTC` moves it to the next minute. The language has no table of leap seconds.
