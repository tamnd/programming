// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/validate-data-against-a-schema/.

// RFC 6901: "~" becomes "~0" first, then "/" becomes "~1".
const pointer = (name) => "/" + name.replaceAll("~", "~0").replaceAll("/", "~1");

function typeOf(value) {
  // typeof null is "object", so null is tested first.
  if (value === null) return ["null"];
  if (Array.isArray(value)) return ["array"];
  if (typeof value === "number") return Number.isInteger(value) ? ["integer", "number"] : ["number"];
  return [typeof value];
}

function check(schema, value, here, at, errors) {
  if (schema === true) return;
  if (schema === false) {
    errors.push([here, at]);
    return;
  }
  const has = (name) => Object.hasOwn(schema, name);
  const kinds = typeOf(value);
  if (has("type")) {
    const allowed = [schema.type].flat();
    if (!kinds.some((kind) => allowed.includes(kind))) errors.push([here, at + "/type"]);
  }
  if (kinds.includes("number")) {
    if (has("minimum") && value < schema.minimum) errors.push([here, at + "/minimum"]);
    if (has("maximum") && value > schema.maximum) errors.push([here, at + "/maximum"]);
  }
  if (kinds.includes("string")) {
    // value.length counts UTF-16 code units. The spread counts code points.
    const length = [...value].length;
    if (has("minLength") && length < schema.minLength) errors.push([here, at + "/minLength"]);
    if (has("maxLength") && length > schema.maxLength) errors.push([here, at + "/maxLength"]);
  }
  if (kinds.includes("object")) {
    // Object.hasOwn ignores the names that every object inherits, such as toString.
    if (has("required") && schema.required.some((name) => !Object.hasOwn(value, name))) {
      errors.push([here, at + "/required"]);
    }
    const named = has("properties") ? schema.properties : {};
    for (const [name, member] of Object.entries(value)) {
      if (Object.hasOwn(named, name)) {
        check(named[name], member, here + pointer(name), at + "/properties" + pointer(name), errors);
      } else if (has("additionalProperties")) {
        check(schema.additionalProperties, member, here + pointer(name), at + "/additionalProperties", errors);
      }
    }
  }
  if (kinds.includes("array") && has("items")) {
    value.forEach((item, index) => check(schema.items, item, `${here}/${index}`, at + "/items", errors));
  }
}

const order = (a, b) => (a < b ? -1 : a > b ? 1 : 0);

// Returns the sorted [instance location, keyword location] pairs of the failed assertions.
export function validate(schema, instance) {
  const errors = [];
  check(JSON.parse(schema), JSON.parse(instance), "", "", errors);
  return errors.sort((a, b) => order(a[0], b[0]) || order(a[1], b[1]));
}
