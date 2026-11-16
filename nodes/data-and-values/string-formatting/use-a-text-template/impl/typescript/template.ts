// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

const PLACEHOLDER = /\$(?:(\$)|\{([A-Za-z_][A-Za-z0-9_]*)\}|([A-Za-z_][A-Za-z0-9_]*))?/g;

/**
 * Returns template with $$ changed to $, and $name and ${name} changed to
 * the value of name. It reads the template once, so a value is not read as a
 * template. An unknown name or a $ without a name throws an Error.
 */
export function substitute(template: string, names: string[], values: string[]): string {
  const table = new Map(names.map((n, i) => [n, values[i]]));
  return template.replace(PLACEHOLDER, (_match: string, dollar?: string, braced?: string, bare?: string, offset?: number) => {
    if (dollar !== undefined) {
      return "$";
    }
    const name = braced ?? bare;
    if (name === undefined) {
      throw new Error(`a $ without a name at ${offset}`);
    }
    const value = table.get(name);
    if (value === undefined) {
      throw new Error(`unknown name: ${name}`);
    }
    return value;
  });
}
