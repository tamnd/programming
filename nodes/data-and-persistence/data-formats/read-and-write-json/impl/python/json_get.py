# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-and-write-json/.

import json
import re

# An array index: ASCII digits, no sign, no leading zero.
INDEX = re.compile(r"0|[1-9][0-9]*")


def reject_constant(name: str) -> None:
    # json.loads accepts NaN, Infinity and -Infinity unless this hook refuses them.
    raise ValueError(f"{name} is not a JSON value")


def json_get(text: str, path: list[str]) -> str:
    """Returns the value at path in the JSON text, as canonical JSON."""
    value = json.loads(text, parse_constant=reject_constant)
    for step in path:
        if isinstance(value, dict) and step in value:
            value = value[step]
        elif isinstance(value, list) and INDEX.fullmatch(step) and int(step) < len(value):
            value = value[int(step)]
        else:
            raise KeyError(step)
    return json.dumps(
        value,
        ensure_ascii=False,
        allow_nan=False,
        separators=(",", ":"),
        sort_keys=True,
    )
