```go fails expect=panic id=index
package main

import "fmt"

func setScore(scores []int, player, score int) {
	scores[player] = score
}

func main() {
	scores := make([]int, 4)
	setScore(scores, 2, 10)
	fmt.Println("player 2:", scores[2])
	setScore(scores, 4, 99)
}
```

The language specification says that an index out of range at run time causes a run-time panic. The panic happens before the write.

```go run id=check
package main

import "fmt"

func setScore(scores []int, player, score int) error {
	if player < 0 || player >= len(scores) {
		return fmt.Errorf("player %d is out of range, the size is %d", player, len(scores))
	}
	scores[player] = score
	return nil
}

func main() {
	scores := make([]int, 4)
	setScore(scores, 2, 10)
	fmt.Println("player 2:", scores[2])
	if err := setScore(scores, 4, 99); err != nil {
		fmt.Println("refused:", err)
	}
}
```

```output for=check
player 2: 10
refused: player 4 is out of range, the size is 4
```

- Verdict: native. Index and slice expressions are checked at run time.
- A panic stops the goroutine and, if nothing recovers it, the program. Check an index that comes from input and return an error.
- The packages `unsafe` and `cgo` can skip the checks. Code that uses them needs the same review as C.
