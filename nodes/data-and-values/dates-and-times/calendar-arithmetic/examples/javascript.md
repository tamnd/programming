```javascript run id=date-overflow
process.env.TZ = "America/New_York";

// Date has no month arithmetic: setMonth moves the month and lets the day overflow.
const d = new Date(Date.UTC(2024, 0, 31));
d.setUTCMonth(d.getUTCMonth() + 1);
console.log(d.toISOString().slice(0, 10));

// Clamp by hand: day 0 of the next month is the last day of this month.
function addMonths(date, n) {
  const y = date.getUTCFullYear(), m = date.getUTCMonth() + n;
  const last = new Date(Date.UTC(y, m + 1, 0)).getUTCDate();
  return new Date(Date.UTC(y, m, Math.min(date.getUTCDate(), last)));
}
console.log(addMonths(new Date(Date.UTC(2024, 0, 31)), 1).toISOString().slice(0, 10));

// setDate adds a calendar day in local time; adding milliseconds adds 24 hours.
const t = new Date(2024, 2, 9, 18, 0);
const day = new Date(t); day.setDate(day.getDate() + 1);
const hours = new Date(t.getTime() + 24 * 3600 * 1000);
console.log(day.toString().slice(0, 33));
console.log(hours.toString().slice(0, 33));
console.log((day - t) / 3600000);
```

```output
2024-03-02
2024-02-29
Sun Mar 10 2024 18:00:00 GMT-0400
Sun Mar 10 2024 19:00:00 GMT-0400
23
```

- Verdict: native. `Date` has no month arithmetic. `setMonth` and `setDate` let the day overflow into the next month, so January 31 plus 1 month is March 2. `setDate` works in local time and keeps the wall clock, and adding milliseconds adds exact hours. `Temporal.PlainDate.prototype.add` clamps the day by default, with the option `overflow: "constrain"`, or throws with `"reject"`.
