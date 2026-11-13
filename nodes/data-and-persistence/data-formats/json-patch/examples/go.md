```go run id=merge
package main

import (
	"encoding/json"
	"fmt"
)

// mergePatch applies an RFC 7396 merge patch to a decoded JSON value.
func mergePatch(target, patch any) any {
	p, ok := patch.(map[string]any)
	if !ok {
		return patch
	}
	result := map[string]any{}
	if t, ok := target.(map[string]any); ok {
		for k, v := range t {
			result[k] = v
		}
	}
	for name, value := range p {
		if value == nil {
			delete(result, name)
		} else {
			result[name] = mergePatch(result[name], value)
		}
	}
	return result
}

func main() {
	var target, patch any
	json.Unmarshal([]byte(`{"name": "lamp", "note": "fragile", "size": {"w": 20, "h": 40}}`), &target)
	json.Unmarshal([]byte(`{"note": null, "size": {"h": 45}, "color": "red"}`), &patch)
	out, _ := json.Marshal(mergePatch(target, patch)) // Marshal sorts the keys of a map.
	fmt.Println(string(out))

	// A struct field cannot tell a member with the value null from a member that is not there.
	type Update struct {
		Note *string `json:"note"`
	}
	for _, body := range []string{`{"note": null}`, `{}`} {
		var u Update
		json.Unmarshal([]byte(body), &u)
		var m map[string]json.RawMessage
		json.Unmarshal([]byte(body), &m)
		raw, present := m["note"]
		fmt.Printf("%-16s struct field nil: %v, map member present: %v %q\n", body, u.Note == nil, present, string(raw))
	}
}
```

```output for=merge
{"color":"red","name":"lamp","size":{"h":45,"w":20}}
{"note": null}   struct field nil: true, map member present: true "null"
{}               struct field nil: true, map member present: false ""
```

- Verdict: stdlib. `json.Unmarshal` into `any` gives `map[string]any` for objects and `nil` for `null`, so `mergePatch` follows RFC 7396 directly. The second part shows a trap for a PATCH handler in Go. A pointer field in a struct is `nil` both for `{"note": null}` and for `{}`, so the handler cannot tell "remove the note" from "keep the note". A `map[string]json.RawMessage` keeps the difference: the member is present, and its raw text is `null`.
