// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/validate-data-against-a-schema/.

type Json = null | boolean | number | string | Json[] | { [name: string]: Json };
type Schema = boolean | { [keyword: string]: Json };
export type Failure = [instanceLocation: string, keywordLocation: string];

// RFC 6901: "~" becomes "~0" first, then "/" becomes "~1".
const pointer = (name: string): string => "/" + name.replaceAll("~", "~0").replaceAll("/", "~1");

function typeOf(value: Json): string[] {
  // typeof null is "object", so null is tested first.
  if (value === null) return ["null"];
  if (Array.isArray(value)) return ["array"];
  if (typeof value === "number") return Number.isInteger(value) ? ["integer", "number"] : ["number"];
  return [typeof value];
}

function check(schema: Schema, value: Json, here: string, at: string, errors: Failure[]): void {
  if (schema === true) return;
  if (schema === false) {
    errors.push([here, at]);
    return;
  }
  const has = (name: string): boolean => Object.hasOwn(schema, name);
  const kinds = typeOf(value);
  if (has("type")) {
    const allowed = [schema.type].flat();
    if (!kinds.some((kind) => allowed.includes(kind))) errors.push([here, at + "/type"]);
  }
  if (typeof value === "number") {
    if (has("minimum") && value < (schema.minimum as number)) errors.push([here, at + "/minimum"]);
    if (has("maximum") && value > (schema.maximum as number)) errors.push([here, at + "/maximum"]);
  }
  if (typeof value === "string") {
    // value.length counts UTF-16 code units. The spread counts code points.
    const length = [...value].length;
    if (has("minLength") && length < (schema.minLength as number)) errors.push([here, at + "/minLength"]);
    if (has("maxLength") && length > (schema.maxLength as number)) errors.push([here, at + "/maxLength"]);
  }
  if (value !== null && typeof value === "object" && !Array.isArray(value)) {
    // Object.hasOwn ignores the names that every object inherits, such as toString.
    const required = has("required") ? (schema.required as string[]) : [];
    if (required.some((name) => !Object.hasOwn(value, name))) errors.push([here, at + "/required"]);
    const named = (has("properties") ? schema.properties : {}) as { [name: string]: Schema };
    for (const [name, member] of Object.entries(value)) {
      if (Object.hasOwn(named, name)) {
        check(named[name], member, here + pointer(name), at + "/properties" + pointer(name), errors);
      } else if (has("additionalProperties")) {
        check(schema.additionalProperties as Schema, member, here + pointer(name), at + "/additionalProperties", errors);
      }
    }
  }
  if (Array.isArray(value) && has("items")) {
    value.forEach((item, index) => check(schema.items as Schema, item, `${here}/${index}`, at + "/items", errors));
  }
}

const order = (a: string, b: string): number => (a < b ? -1 : a > b ? 1 : 0);

// Returns the sorted [instance location, keyword location] pairs of the failed assertions.
export function validate(schema: string, instance: string): Failure[] {
  const errors: Failure[] = [];
  check(JSON.parse(schema) as Schema, JSON.parse(instance) as Json, "", "", errors);
  return errors.sort((a, b) => order(a[0], b[0]) || order(a[1], b[1]));
}
