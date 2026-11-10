// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

/**
 * Returns the value of the environment variable name, or defaultValue when it is not set.
 * An empty value is a value, so it does not give the default.
 * @param {string} name
 * @param {string} defaultValue
 * @returns {string}
 */
export function getEnv(name, defaultValue) {
  // process.env inherits from an object, so process.env.toString is a function.
  // Object.hasOwn sees only the variables.
  return Object.hasOwn(process.env, name) ? process.env[name] : defaultValue;
}
