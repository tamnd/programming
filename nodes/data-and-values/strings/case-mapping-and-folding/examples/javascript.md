```javascript run id=locale-upper-and-collator
// toUpperCase uses the full mappings, so the length can change.
const s = "straße";
console.log(s.toUpperCase(), s.length, s.toUpperCase().length);

// toLowerCase handles the final sigma.
console.log("ΌΣΟΣ".toLowerCase());

// The locale methods apply the Turkish rules for "tr".
console.log("istanbul".toUpperCase(), "istanbul".toLocaleUpperCase("tr"));
console.log("TITLE".toLowerCase(), "TITLE".toLocaleLowerCase("tr"));

// JavaScript has no case fold. A collator with sensitivity "accent" ignores case but not accents.
const collator = new Intl.Collator("fr", { sensitivity: "accent" });
const r = "R\u00e9sum\u00e9";
console.log(collator.compare(r, r.toUpperCase()) === 0, collator.compare(r, "RESUME") === 0);
console.log("Straße".toLowerCase() === "STRASSE".toLowerCase());

// Upper case and then lower case is a common approximation of a fold.
const fold = (x) => x.toUpperCase().toLowerCase();
console.log(fold("Straße") === fold("STRASSE"), fold("ΌΣΟΣ") === fold("όσος"));
```

```output for=locale-upper-and-collator
STRASSE 6 7
όσος
ISTANBUL İSTANBUL
title tıtle
true false
false
true true
```

- Verdict: native. `toUpperCase` maps `ß` to `SS`, so the length goes from 6 to 7. `toLowerCase` gives the final sigma. `toLocaleUpperCase("tr")` gives `İSTANBUL`, and `toLocaleLowerCase("tr")` gives `tıtle`. JavaScript has no fold function. A collator with the sensitivity `accent` finds `Résumé` and `RÉSUMÉ` equal, but not `Résumé` and `RESUME`. Lower case alone does not match `Straße` and `STRASSE`, but upper case and then lower case does.
