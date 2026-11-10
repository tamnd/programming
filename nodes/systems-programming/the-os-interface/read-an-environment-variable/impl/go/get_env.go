// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

// Package getenv reads an environment variable with a default.
package getenv

import "os"

// GetEnv returns the value of the environment variable name, or def when it
// is not set. An empty value is a value, so it does not give def.
func GetEnv(name, def string) string {
	// os.Getenv returns "" for both a missing and an empty variable.
	// LookupEnv tells the two apart.
	if value, ok := os.LookupEnv(name); ok {
		return value
	}
	return def
}
