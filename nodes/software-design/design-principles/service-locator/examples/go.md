```go run id=report-locator
package main

import "fmt"

// services is the dynamic locator: a map from names to services.
var services = map[string]any{}

func register(name string, service any) { services[name] = service }

func get(name string) (any, error) {
	s, ok := services[name]
	if !ok {
		return nil, fmt.Errorf("no service: %s", name)
	}
	return s, nil
}

type Report struct{}

func (Report) Print(total int) (string, error) {
	s, err := get("format")
	if err != nil {
		return "", err
	}
	return s.(func(int) string)(total), nil
}

type TaxReport struct{}

func (TaxReport) Print(total int) (string, error) {
	t, err := get("tax") // nobody registered this service
	if err != nil {
		return "", err
	}
	f, err := get("format")
	if err != nil {
		return "", err
	}
	return f.(func(int) string)(total + t.(func(int) int)(total)), nil
}

func main() {
	// Configuration: the program puts the services into the locator.
	register("format", func(n int) string { return fmt.Sprintf("total: %d", n) })

	out, _ := Report{}.Print(12)
	fmt.Println(out)
	taxReport := TaxReport{} // building it does not fail
	if out, err := taxReport.Print(12); err != nil {
		fmt.Println("error:", err)
	} else {
		fmt.Println(out)
	}
}
```

```output
total: 12
error: no service: tax
```
