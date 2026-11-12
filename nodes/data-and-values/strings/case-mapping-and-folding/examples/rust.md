```rust run id=char-iterator-and-final-sigma
fn main() {
    // to_uppercase returns a new String, because "ß" becomes "SS".
    let s = "straße";
    let up = s.to_uppercase();
    println!("{} {} {}", up, s.chars().count(), up.chars().count());

    // char::to_uppercase returns an iterator, because one char can map to several.
    let sharp: Vec<char> = 'ß'.to_uppercase().collect();
    println!("{:?}", sharp);

    // str::to_lowercase handles the final sigma. char::to_lowercase cannot see the context.
    let w = "ΌΣΟΣ";
    let by_char: String = w.chars().flat_map(|c| c.to_lowercase()).collect();
    println!("{} {}", w.to_lowercase(), by_char);

    // The default mappings have no Turkish rules.
    let dotted: String = 'İ'.to_lowercase().collect();
    println!("{} {} {}", "i".to_uppercase(), dotted, dotted.chars().count());

    // eq_ignore_ascii_case ignores only ASCII case.
    println!("{} {}", "HELLO".eq_ignore_ascii_case("hello"), "ΌΣΟΣ".eq_ignore_ascii_case("όσος"));

    // A simple caseless compare: compare the lower case forms of the upper case forms.
    let fold = |x: &str| x.to_uppercase().to_lowercase();
    println!("{}", fold("Straße") == fold("STRASSE"));
}
```

```output for=char-iterator-and-final-sigma
STRASSE 6 7
['S', 'S']
όσος όσοσ
I i̇ 2
true false
true
```

- Verdict: native. `to_uppercase` returns a new `String`, and the length goes from 6 to 7 `char` values. `'ß'.to_uppercase()` yields two `char` values. `str::to_lowercase` gives the final sigma, and a loop over `char::to_lowercase` does not. `'İ'.to_lowercase()` gives two code points, because the default mappings have no Turkish rules. `eq_ignore_ascii_case` ignores only ASCII case, so the Greek words are not equal. Upper case and then lower case matches `Straße` and `STRASSE`.
