# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/pretty-print-json/.

import json


def reject_constant(name: str) -> None:
    # json.loads accepts NaN, Infinity and -Infinity unless this hook refuses them.
    raise ValueError(f"{name} is not a JSON value")


def pretty_json(text: str) -> str:
    """Returns the JSON text indented by two spaces, with the names of each object sorted."""
    value = json.loads(text, parse_constant=reject_constant)
    # With indent, the separators are "," at the end of a line and ": " after a name.
    return json.dumps(value, indent=2, sort_keys=True, ensure_ascii=False, allow_nan=False)
