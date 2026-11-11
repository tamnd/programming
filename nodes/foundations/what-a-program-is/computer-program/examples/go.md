```go run id=gcd-in-package-main
package main

import "fmt"

func gcd(a, b int) int {
	for b != 0 {
		a, b = b, a%b
		fmt.Println(a, b)
	}
	return a
}

func main() {
	fmt.Println(1071, 462)
	fmt.Println("gcd", gcd(1071, 462))
}
```

```output for=gcd-in-package-main
1071 462
462 147
147 21
21 0
gcd 21
```

```go fails expect=compile id=no-main-function
package main

import "fmt"

func gcd(a, b int) int {
	for b != 0 {
		a, b = b, a%b
	}
	return a
}

func Main() { // Go looks for main, not Main
	fmt.Println("gcd", gcd(1071, 462))
}
```

- Verdict: native. A Go program is the package `main` linked with every package that it imports, and it starts by calling the function `main` of that package. When `main` returns, the program exits with status 0. Go assigns both values of `a, b = b, a%b` at once.
- The fails block declares `Main` with a capital letter. Go names are case-sensitive, so the package has no function `main`, and the compiler rejects it, because a package `main` without one is not a program.
