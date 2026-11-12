```rust run id=comparable-mixin
use std::fmt;

trait Comparable {
    fn compare(&self, other: &Self) -> i32; // required

    fn less_than(&self, other: &Self) -> bool { // provided
        self.compare(other) < 0
    }

    fn between(&self, lo: &Self, hi: &Self) -> bool {
        lo.compare(self) <= 0 && self.compare(hi) <= 0
    }
}

struct Version {
    major: i32,
    minor: i32,
}

impl Comparable for Version {
    fn compare(&self, o: &Self) -> i32 {
        if self.major != o.major { self.major - o.major } else { self.minor - o.minor }
    }
}

impl fmt::Display for Version {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}.{}", self.major, self.minor)
    }
}

fn yes(flag: bool) -> &'static str {
    if flag { "yes" } else { "no" }
}

fn main() {
    let v12 = Version { major: 1, minor: 2 };
    let v110 = Version { major: 1, minor: 10 };
    let v20 = Version { major: 2, minor: 0 };
    println!("{v12} < {v110}: {}", yes(v12.less_than(&v110)));
    println!("{v110} between {v12} and {v20}: {}", yes(v110.between(&v12, &v20)));
    println!("{v20} between {v12} and {v110}: {}", yes(v20.between(&v12, &v110)));
}
```

```output
1.2 < 1.10: yes
1.10 between 1.2 and 2.0: yes
2.0 between 1.2 and 1.10: no
```

- Verdict: idiom. A trait with default methods works as a mixin: the type implements the required method and gets the others. The standard `PartialOrd` and `Ord` traits work the same way, and provide `<`, `max` and `clamp` from one comparison method.
