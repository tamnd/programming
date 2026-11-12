```rust run id=rule-subset-and-reverse
// The standard library has no grapheme API. A real program uses the
// unicode-segmentation crate. This sketch applies a small subset of the
// UAX 29 rules, with hand-written ranges that are enough for this text.
fn is_extend(c: char) -> bool {
    matches!(c, '\u{0300}'..='\u{036F}' | '\u{1F3FB}'..='\u{1F3FF}' | '\u{FE0F}')
}

fn is_ri(c: char) -> bool {
    ('\u{1F1E6}'..='\u{1F1FF}').contains(&c)
}

fn graphemes(s: &str) -> Vec<&str> {
    let mut out: Vec<&str> = Vec::new();
    let mut start = 0;
    let mut prev: Option<char> = None;
    let mut ri_run = 0;
    for (i, c) in s.char_indices() {
        let join = match prev {
            None => false,
            Some(p) => is_extend(c) || c == '\u{200D}' || p == '\u{200D}' || (is_ri(c) && ri_run % 2 == 1),
        };
        if !join && i > 0 {
            out.push(&s[start..i]);
            start = i;
        }
        ri_run = if is_ri(c) { ri_run + 1 } else { 0 };
        prev = Some(c);
    }
    if !s.is_empty() {
        out.push(&s[start..]);
    }
    out
}

fn main() {
    let t = "cafe\u{0301} \u{1F1EB}\u{1F1F7} \u{1F469}\u{1F3FD}\u{200D}\u{1F692}";

    // len counts bytes, and chars().count() counts code points.
    println!("{} {}", t.len(), t.chars().count());

    let g = graphemes(t);
    println!("{}", g.len());

    // Each grapheme is a &str slice that borrows from t.
    let sizes: Vec<String> = g.iter().map(|x| x.len().to_string()).collect();
    println!("{}", sizes.join(" "));

    // chars().rev() reverses code points, so the accent moves onto the "!".
    let bad: String = "cafe\u{0301}!".chars().rev().collect();
    let good: String = graphemes("cafe\u{0301}!").into_iter().rev().collect();
    println!("{:?} {:?}", bad, good);
}
```

```output for=rule-subset-and-reverse
31 13
8
1 1 1 3 1 8 1 15
"!\u{301}efac" "!e\u{301}fac"
```

- Verdict: library. The standard library has no grapheme API, so this sketch uses hand-written ranges. A real program uses the `unicode-segmentation` crate. Each cluster is a `&str` slice of the text. `chars().rev()` puts U+0301 after the `!`, so the accent moves to the wrong character. A reverse by clusters keeps `e` and its accent together.
