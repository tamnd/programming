```go run id=key-generator-plugin
package main

import (
	"fmt"
	"strings"
)

type KeyGenerator interface {
	NextKey() int64
}

// CounterKeyGenerator is for unit tests: a counter in memory.
type CounterKeyGenerator struct{ last int64 }

func (g *CounterKeyGenerator) NextKey() int64 { g.last++; return g.last }

// SequenceKeyGenerator is for production: it stands for a database sequence.
type SequenceKeyGenerator struct{ last int64 }

func (g *SequenceKeyGenerator) NextKey() int64 { g.last++; return g.last }

// Go cannot create a type from its name, so each plugin registers a constructor,
// as database drivers do with sql.Register.
var plugins = map[string]func() KeyGenerator{}

func Register(name string, make func() KeyGenerator) { plugins[name] = make }

func init() {
	Register("CounterKeyGenerator", func() KeyGenerator { return &CounterKeyGenerator{} })
	Register("SequenceKeyGenerator", func() KeyGenerator { return &SequenceKeyGenerator{last: 1000} })
}

type PluginFactory struct{ config map[string]string }

func (f PluginFactory) KeyGenerator() (KeyGenerator, string, error) {
	name := f.config["KeyGenerator"]
	make, ok := plugins[name]
	if !ok {
		return nil, "", fmt.Errorf("no plugin named %s", name)
	}
	return make(), name, nil
}

func main() {
	// One configuration for each deployment. Only the configuration names the types.
	names := []string{"test", "production", "broken"}
	configs := map[string]map[string]string{
		"test":       {"KeyGenerator": "CounterKeyGenerator"},
		"production": {"KeyGenerator": "SequenceKeyGenerator"},
		"broken":     {"KeyGenerator": "FileKeyGenerator"},
	}
	for _, name := range names {
		gen, plugin, err := PluginFactory{configs[name]}.KeyGenerator()
		if err != nil {
			fmt.Printf("%s: error: %v\n", name, err)
			continue
		}
		keys := make([]string, 3)
		for i := range keys {
			keys[i] = fmt.Sprint(gen.NextKey())
		}
		fmt.Printf("%s: keys %s from %s\n", name, strings.Join(keys, ", "), plugin)
	}
}
```

```output
test: keys 1, 2, 3 from CounterKeyGenerator
production: keys 1001, 1002, 1003 from SequenceKeyGenerator
broken: error: no plugin named FileKeyGenerator
```

- Verdict: idiom. Go cannot create a type from its name, so each plugin registers a constructor in `init`, in the same way as database drivers call `sql.Register`. A program imports the package of a plugin only for this side effect.
- The standard `plugin` package loads shared objects at run time, but its documentation warns that it works only on some platforms.
