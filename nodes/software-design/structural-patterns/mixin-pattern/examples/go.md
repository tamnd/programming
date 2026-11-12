```go run id=comparable-mixin
package main

import "fmt"

// Go has no mixins. The shared behaviour is a set of generic functions
// over an interface that the type must satisfy.
type Ordered[T any] interface {
	Compare(other T) int
}

func LessThan[T Ordered[T]](a, b T) bool { return a.Compare(b) < 0 }

func Between[T Ordered[T]](x, lo, hi T) bool {
	return lo.Compare(x) <= 0 && x.Compare(hi) <= 0
}

type Version struct{ Major, Minor int }

func (v Version) Compare(o Version) int {
	if v.Major != o.Major {
		return v.Major - o.Major
	}
	return v.Minor - o.Minor
}

func (v Version) String() string { return fmt.Sprintf("%d.%d", v.Major, v.Minor) }

func yes(flag bool) string {
	if flag {
		return "yes"
	}
	return "no"
}

func main() {
	v12, v110, v20 := Version{1, 2}, Version{1, 10}, Version{2, 0}
	fmt.Printf("%v < %v: %s\n", v12, v110, yes(LessThan(v12, v110)))
	fmt.Printf("%v between %v and %v: %s\n", v110, v12, v20, yes(Between(v110, v12, v20)))
	fmt.Printf("%v between %v and %v: %s\n", v20, v12, v110, yes(Between(v20, v12, v110)))
}
```

```output
1.2 < 1.10: yes
1.10 between 1.2 and 2.0: yes
2.0 between 1.2 and 1.10: no
```

- Verdict: absent. An embedded struct promotes its methods, but its methods cannot call a method of the outer struct, so embedding cannot supply behaviour that depends on `Compare`. Generic functions over an interface take the place of the mixin.
