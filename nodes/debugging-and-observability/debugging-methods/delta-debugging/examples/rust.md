```rust run id=ddmin
/// Returns a 1-minimal part of `chars` on which `fails` is still true.
fn ddmin(mut chars: Vec<char>, fails: &mut impl FnMut(&[char]) -> bool) -> Vec<char> {
    let mut n = 2;
    while chars.len() >= 2 {
        let size = chars.len();
        let bounds: Vec<(usize, usize)> = (0..n).map(|i| (i * size / n, (i + 1) * size / n)).collect();
        let subset = bounds.iter().map(|&(lo, hi)| chars[lo..hi].to_vec()).find(|s| fails(s));
        if let Some(s) = subset {
            // reduce to subset
            chars = s;
            n = 2;
            println!("subset     {}", chars.len());
            continue;
        }
        let complement = bounds
            .iter()
            .map(|&(lo, hi)| [&chars[..lo], &chars[hi..]].concat())
            .find(|c| fails(c));
        if let Some(c) = complement {
            // reduce to complement
            chars = c;
            n = (n - 1).max(2);
            println!("complement {}", chars.len());
            continue;
        }
        if n == size {
            break; // each single character was removed
        }
        n = size.min(2 * n); // increase granularity
    }
    chars
}

fn main() {
    let mut tests = 0;
    // Stands for one run of the browser: it crashes on a <SELECT ...> tag.
    let mut browser_crashes = |chars: &[char]| {
        tests += 1;
        let text: String = chars.iter().collect();
        match text.find("<SELECT") {
            Some(start) => text[start..].contains('>'),
            None => false,
        }
    };
    let page = r#"<td align=left><SELECT NAME="priority" MULTIPLE SIZE=7>"#;
    let minimal: String = ddmin(page.chars().collect(), &mut browser_crashes).into_iter().collect();
    println!("{} characters reduced to {} in {} tests", page.len(), minimal, tests);
}
```

```output for=ddmin
complement 42
complement 28
complement 21
complement 17
complement 14
complement 13
complement 12
complement 10
complement 9
complement 8
55 characters reduced to <SELECT> in 147 tests
```

- Verdict: idiom. The standard library has no delta debugging.
- The input is a `Vec<char>`, so a part never splits a UTF-8 character.
- `map(...).find(...)` is lazy. It builds a part and tests it, and it stops at the first part that fails.
- The test changes the counter `tests`, so its type is `FnMut`, and `ddmin` takes it by `&mut`.
