```javascript run id=milliseconds-since-the-epoch
const t = 1709985600;

// A Date holds milliseconds since the epoch, so multiply seconds by 1000.
const d = new Date(t * 1000);
console.log(d.toISOString());
console.log(d.getTime(), Math.floor(d.getTime() / 1000));

// Seconds where milliseconds are expected give a date in January 1970.
console.log(new Date(t).toISOString());

// The epoch is 0, and a negative value is before 1970.
console.log(Date.UTC(1970, 0, 1));
console.log(new Date(-1000).toISOString());

// The current time in milliseconds is an exact integer in a Number.
console.log(Number.isSafeInteger(Date.now()));
```

```output
2024-03-09T12:00:00.000Z
1709985600000 1709985600
1970-01-20T18:59:45.600Z
0
1969-12-31T23:59:59.000Z
true
```

- Verdict: native. Date.now and Date.prototype.getTime give milliseconds since the epoch, and new Date(ms) converts back. Temporal.Instant gives the same count as epochMilliseconds and as a BigInt of nanoseconds in epochNanoseconds.
