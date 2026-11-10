// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

// Returns the value of the environment variable name, or defaultValue when it is not set.
// An empty value is a value, so it does not give the default.
export function getEnv(name: string, defaultValue: string): string {
  // process.env inherits from an object, so process.env.toString is a function.
  // Object.hasOwn sees only the variables.
  const value = Object.hasOwn(process.env, name) ? process.env[name] : undefined;
  return value ?? defaultValue;
}
