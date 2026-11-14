```javascript run id=intl-zone-history
// Intl.DateTimeFormat reads the tz database that the JavaScript engine ships with (ICU in V8).
function show(instant, timeZone) {
  const f = new Intl.DateTimeFormat("en-CA", {
    timeZone, year: "numeric", month: "2-digit", day: "2-digit",
    hour: "2-digit", minute: "2-digit", second: "2-digit", hourCycle: "h23",
    timeZoneName: "longOffset",
  });
  const p = Object.fromEntries(f.formatToParts(instant).map((x) => [x.type, x.value]));
  return `${p.year}-${p.month}-${p.day} ${p.hour}:${p.minute}:${p.second} ${p.timeZoneName}`;
}

for (const iso of ["1880-01-01T12:00:00Z", "1936-06-01T12:00:00Z", "2024-07-01T12:00:00Z"]) {
  console.log(show(new Date(iso), "America/Chicago"));
}
// A link name gives the same local time as the zone it points to.
const t = new Date("2024-07-01T12:00:00Z");
console.log(show(t, "Europe/Vatican") === show(t, "Europe/Rome"));
console.log(Intl.supportedValuesOf("timeZone").includes("America/Chicago"));
```

```output
1880-01-01 06:09:24 GMT-05:50:36
1936-06-01 07:00:00 GMT-05:00
2024-07-01 07:00:00 GMT-05:00
true
true
```

- Verdict: native. The engine ships its own copy of the tz data, which ICU holds in V8 and SpiderMonkey. `Intl.DateTimeFormat` with the option `timeZone` uses the full history, so the offset in 1880 has seconds. `Intl.supportedValuesOf` lists the zone names that the engine knows.
