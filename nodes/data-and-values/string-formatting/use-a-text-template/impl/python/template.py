# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-text-template/.

from string import Template


def substitute(template: str, names: list[str], values: list[str]) -> str:
    """Return template with $$ changed to $, and $name and ${name}
    changed to the value of name. An unknown name raises KeyError."""
    return Template(template).substitute(dict(zip(names, values)))


if __name__ == "__main__":
    print(substitute("Hello, $name!", ["name"], ["Ada"]))
