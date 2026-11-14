```rust run id=hashset-btreeset
use std::collections::{BTreeSet, HashSet};

fn main() {
    // insert returns false when the set has the value already.
    let mut s: HashSet<i32> = HashSet::from([3, 1, 3, 2, 1]);
    println!("{} {} {} {}", s.len(), s.insert(2), s.insert(4), s.contains(&4));

    // A BTreeSet keeps its values sorted, and a HashSet has no order.
    let a: BTreeSet<i32> = BTreeSet::from([1, 2, 3]);
    let b: BTreeSet<i32> = BTreeSet::from([2, 3, 4]);
    println!("{:?}", a);

    // The operators work on references and make a new set.
    println!("{:?} {:?} {:?} {:?}", &a | &b, &a & &b, &a - &b, &a ^ &b);
    println!("{} {}", BTreeSet::from([2, 3]).is_subset(&a), a.is_disjoint(&BTreeSet::from([7, 8])));

    // The method forms are lazy iterators.
    let common: Vec<_> = a.intersection(&b).copied().collect();
    println!("{:?}", common);

    // A sorted set can give the values in a range.
    println!("{:?}", a.range(2..).collect::<Vec<_>>());

    // remove returns whether the value was there.
    println!("{} {}", s.remove(&1), s.remove(&9));
}
```

```output
3 false true true
{1, 2, 3}
{1, 2, 3, 4} {2, 3} {1} {1, 4}
true true
[2, 3]
[2, 3]
true false
```

- Verdict: stdlib. HashSet and BTreeSet. insert and remove return whether the set changed. The operators |, &, - and ^ take two references and make a new set. intersection and the other methods give lazy iterators. A BTreeSet keeps the values sorted and gives ranges.
