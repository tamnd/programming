```go wrong id=wrong-folder
package main

import "fmt"

type doc struct {
	folder int
	title  string
}

// Maps stand for the database.
var (
	readers = map[int][]string{1: {"chi"}, 2: {"ana"}} // folder -> users who may read it
	docs    = map[int]doc{7: {1, "team notes"}, 99: {2, "salaries"}}
)

func canRead(user string, folder int) bool {
	for _, u := range readers[folder] {
		if u == user {
			return true
		}
	}
	return false
}

// GET /folders/{folder}/docs/{id}
// The check is on the folder in the URL. The document comes from its own id.
func getDoc(user string, folder, id int) string {
	if !canRead(user, folder) {
		return "403 forbidden"
	}
	d, ok := docs[id]
	if !ok {
		return "404 not found"
	}
	return "200 " + d.title
}

func main() {
	fmt.Println(getDoc("chi", 1, 7))
	fmt.Println(getDoc("chi", 2, 99))
	fmt.Println(getDoc("chi", 1, 99)) // folder 1 in the URL, a document of folder 2
}
```

```output for=wrong-folder
200 team notes
403 forbidden
200 salaries
```

The handler checks that `chi` may read folder 1, which is the folder in the URL. It then loads document 99 by its own id. Document 99 is in folder 2, so `chi` reads a document of a folder that the check never looked at.

```go run id=load-then-check
package main

import (
	"errors"
	"fmt"
	"slices"
)

type doc struct {
	folder int
	title  string
}

// Maps stand for the database.
var (
	readers = map[int][]string{1: {"chi"}, 2: {"ana"}} // folder -> users who may read it
	docs    = map[int]doc{7: {1, "team notes"}, 99: {2, "salaries"}}
)

var errNotFound = errors.New("404 not found")

// loadDoc returns the document only when it is in the folder of the URL and the
// user may read that folder. The check uses the folder of the loaded document.
func loadDoc(user string, folder, id int) (doc, error) {
	d, ok := docs[id]
	if !ok || d.folder != folder || !slices.Contains(readers[d.folder], user) {
		return doc{}, errNotFound
	}
	return d, nil
}

// GET /folders/{folder}/docs/{id}
func getDoc(user string, folder, id int) string {
	d, err := loadDoc(user, folder, id)
	if err != nil {
		return err.Error()
	}
	return "200 " + d.title
}

func main() {
	fmt.Println(getDoc("chi", 1, 7))
	fmt.Println(getDoc("chi", 2, 99))
	fmt.Println(getDoc("chi", 1, 99))
	fmt.Println(getDoc("ana", 2, 99))
}
```

```output for=load-then-check
200 team notes
404 not found
404 not found
200 salaries
```

`loadDoc` loads the document and then checks the folder that the document is in. A handler can only get a document through `loadDoc`, so it cannot skip the check or check a different object.

- Verdict: idiom. Go has no authorization package in its standard library. One loader function per type of object keeps the check and the lookup together.
- The fixed loader returns the same error for a document that does not exist and for one that the user may not read, so the response does not tell which ids exist.
- With SQL, put both conditions in the query: `SELECT ... FROM docs WHERE id = $1 AND folder = $2`, and check the user against that folder.
