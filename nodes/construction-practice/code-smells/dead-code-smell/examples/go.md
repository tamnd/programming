```go run id=delete-what-does-not-run
package main

import (
	"fmt"
	"math"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
)

// Before: a helper that nothing calls. The compiler allows an unused function.
func legacyRoundUSD(cents int) int { return (cents + 2) / 5 * 5 }

func priceBefore(cents int) int {
	withTax := int(math.Round(float64(cents) * 1.2))
	display := fmt.Sprintf("%.2f", float64(withTax)/100)
	_ = display // the blank assignment hides a value that is never read
	return withTax
}

// After: only the code that runs.
func price(cents int) int { return int(math.Round(float64(cents) * 1.2)) }

// vet compiles a small program with the go tool and returns its errors.
func vet(src string) string {
	dir, err := os.MkdirTemp("", "prog-test-dead")
	if err != nil {
		panic(err)
	}
	defer os.RemoveAll(dir)
	file := filepath.Join(dir, "probe.go")
	if err := os.WriteFile(file, []byte(src), 0o600); err != nil {
		panic(err)
	}
	out, _ := exec.Command("go", "vet", file).CombinedOutput()
	return string(out)
}

func main() {
	for _, cents := range []int{0, 1, 999, 1250} {
		if price(cents) != priceBefore(cents) {
			panic("price")
		}
	}

	// An unused local variable and an unused import do not compile.
	local := vet("package main\nfunc main() { x := 1 }\n")
	imp := vet("package main\nimport \"strings\"\nfunc main() {}\n")
	if !strings.Contains(local, "declared and not used") || !strings.Contains(imp, "imported and not used") {
		panic("expected compile errors:\n" + local + imp)
	}
	fmt.Println("price: tests pass")
}
```

```output
price: tests pass
```

The Go compiler refuses an unused local variable and an unused import, as the probe shows; the specification allows the first rule and requires the second. It does not refuse an unused function such as `legacyRoundUSD`, and the blank assignment `_ = display` silences the rule for `display`. Those need a linter or a review; Staticcheck is one such linter, and its checks include values that are assigned and never read.
