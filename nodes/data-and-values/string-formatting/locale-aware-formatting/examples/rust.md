```rust run id=no-locale-in-std-fmt
struct Locale {
    name: &'static str,
    group: &'static str,
    decimal: &'static str,
}

fn format(x: f64, digits: usize, loc: &Locale) -> String {
    let text = format!("{:.*}", digits, x.abs());
    let (whole, frac) = text.split_once('.').unwrap_or((text.as_str(), ""));
    let mut out = String::new();
    for (i, c) in whole.chars().enumerate() {
        if i > 0 && (whole.len() - i) % 3 == 0 {
            out.push_str(loc.group);
        }
        out.push(c);
    }
    if !frac.is_empty() {
        out.push_str(loc.decimal);
        out.push_str(frac);
    }
    if x < 0.0 { format!("-{out}") } else { out }
}

fn main() {
    let x = 1234567.891;
    let locales = [
        Locale { name: "en-US", group: ",", decimal: "." },
        Locale { name: "de-DE", group: ".", decimal: "," },
        Locale { name: "it-CH", group: "'", decimal: "." },
    ];
    for loc in &locales {
        println!("{} {} {}", loc.name, format(x, 2, loc), format(-1234.5, 1, loc));
    }
    println!("{x:.2} {x}");
}
```

```output for=no-locale-in-std-fmt
en-US 1,234,567.89 -1,234.5
de-DE 1.234.567,89 -1.234,5
it-CH 1'234'567.89 -1'234.5
1234567.89 1234567.891
```

- Verdict: library. `std::fmt` has no concept of a locale, and the same program gives the same text on each system. The crate `num-format` adds the group separators of many locales, and the `icu` crates of the ICU4X project add the full CLDR data. Without them, a program puts in the symbols itself, as here.
