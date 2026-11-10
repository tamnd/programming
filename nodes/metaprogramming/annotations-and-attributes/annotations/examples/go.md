```go run id=tags
package main

import (
	"encoding/json"
	"fmt"
	"reflect"
)

type User struct {
	Name     string `json:"name"`
	Email    string `json:"email,omitempty"`
	Password string `json:"-"`
}

func main() {
	u := User{Name: "Ada", Password: "secret"}
	out, err := json.Marshal(u)
	if err != nil {
		panic(err)
	}
	fmt.Println(string(out))
	t := reflect.TypeOf(u)
	for i := 0; i < t.NumField(); i++ {
		f := t.Field(i)
		fmt.Printf("%s: %q\n", f.Name, f.Tag.Get("json"))
	}
}
```

```output for=tags
{"name":"Ada"}
Name: "name"
Email: "email,omitempty"
Password: "-"
```

A struct tag is a string after a field. The language ignores it, and a package reads it through `reflect`. `encoding/json` uses the `json` key: `name` renames the field, `omitempty` leaves out an empty value, and `-` leaves out the field.

```go wrong id=bad-tag
package main

import (
	"encoding/json"
	"fmt"
)

type User struct {
	Name string `json: "name"`
}

func main() {
	out, err := json.Marshal(User{Name: "Ada"})
	if err != nil {
		panic(err)
	}
	fmt.Println(string(out))
}
```

```output for=bad-tag
{"Name":"Ada"}
```

The tag has a space after the colon, so it does not have the conventional `key:"value"` format. The compiler accepts any string, and the result of `Tag.Get` for such a tag is unspecified. Here `encoding/json` does not find the key and uses the field name `Name`. `go vet` reports tags that do not follow the format.

- Verdict: idiom. Go has no annotations. Struct tags attach strings to fields, and a package gives them meaning through `reflect`.
- A comment `//go:generate` tells the tool `go generate` which command to run. The compiler does not run it.
