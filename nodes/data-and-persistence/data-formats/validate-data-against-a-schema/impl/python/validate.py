# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/validate-data-against-a-schema/.

import json


def _reject(name):
    raise ValueError(f"{name} is not valid JSON")


def _pointer(name):
    # RFC 6901: "~" becomes "~0" first, then "/" becomes "~1".
    return "/" + name.replace("~", "~0").replace("/", "~1")


def _type_of(value):
    # bool is a subclass of int, so it is tested first.
    if value is None:
        return {"null"}
    if isinstance(value, bool):
        return {"boolean"}
    if isinstance(value, int):
        return {"integer", "number"}
    if isinstance(value, float):
        return {"integer", "number"} if value.is_integer() else {"number"}
    if isinstance(value, str):
        return {"string"}
    if isinstance(value, list):
        return {"array"}
    return {"object"}


def _check(schema, value, here, at, errors):
    if schema is True:
        return
    if schema is False:
        errors.append((here, at))
        return
    kinds = _type_of(value)
    if "type" in schema:
        allowed = schema["type"]
        if isinstance(allowed, str):
            allowed = [allowed]
        if not kinds.intersection(allowed):
            errors.append((here, at + "/type"))
    if "number" in kinds:
        if "minimum" in schema and value < schema["minimum"]:
            errors.append((here, at + "/minimum"))
        if "maximum" in schema and value > schema["maximum"]:
            errors.append((here, at + "/maximum"))
    if "string" in kinds:
        # len counts code points, as RFC 8259 counts characters.
        if "minLength" in schema and len(value) < schema["minLength"]:
            errors.append((here, at + "/minLength"))
        if "maxLength" in schema and len(value) > schema["maxLength"]:
            errors.append((here, at + "/maxLength"))
    if "object" in kinds:
        if any(name not in value for name in schema.get("required", [])):
            errors.append((here, at + "/required"))
        named = schema.get("properties", {})
        for name, member in value.items():
            if name in named:
                _check(named[name], member, here + _pointer(name), at + "/properties" + _pointer(name), errors)
            elif "additionalProperties" in schema:
                _check(schema["additionalProperties"], member, here + _pointer(name), at + "/additionalProperties", errors)
    if "array" in kinds and "items" in schema:
        for index, item in enumerate(value):
            _check(schema["items"], item, f"{here}/{index}", at + "/items", errors)


def validate(schema, instance):
    """Return the sorted (instance location, keyword location) pairs of the failed assertions."""
    errors = []
    _check(json.loads(schema, parse_constant=_reject), json.loads(instance, parse_constant=_reject), "", "", errors)
    return sorted(errors)
