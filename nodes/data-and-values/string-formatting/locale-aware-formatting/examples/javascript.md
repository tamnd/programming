```javascript run id=intl-numberformat
const n = 1234567.891;
for (const loc of ["en-US", "de-DE", "fr-FR", "en-IN", "ar-EG"]) {
  const s = new Intl.NumberFormat(loc).format(n);
  const odd = [...s].filter((c) => c > "~").map((c) => "U+" + c.codePointAt(0).toString(16).toUpperCase().padStart(4, "0"));
  console.log(loc, odd.length ? [...new Set(odd)].join(" ") : s);
}

const usd = new Intl.NumberFormat("en-US", { style: "currency", currency: "USD" });
const jpy = new Intl.NumberFormat("ja-JP", { style: "currency", currency: "JPY" });
console.log(usd.format(n), jpy.format(n).replace("￥", "Yen "));

const d = new Date(Date.UTC(2026, 0, 2));
for (const loc of ["en-US", "en-GB", "de-DE"]) {
  console.log(loc, d.toLocaleDateString(loc, { timeZone: "UTC" }));
}
console.log((0.5).toLocaleString("en-US", { style: "percent" }), String(n));
```

```output for=intl-numberformat
en-US 1,234,567.891
de-DE 1.234.567,891
fr-FR U+202F
en-IN 12,34,567.891
ar-EG U+0661 U+066C U+0662 U+0663 U+0664 U+0665 U+0666 U+0667 U+066B U+0668 U+0669
$1,234,567.89 Yen 1,234,568
en-US 1/2/2026
en-GB 02/01/2026
de-DE 2.1.2026
50% 1234567.891
```

- Verdict: stdlib. `Intl.NumberFormat` and `Intl.DateTimeFormat` use the CLDR data of the runtime, and `toLocaleString` is a short form. The French group separator is U+202F, a narrow no-break space, and Arabic in Egypt uses Arabic-Indic digits. `String(n)` never uses a locale, so it is safe for machine text.
