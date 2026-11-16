# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/print-formatted-output/.


def format_row(name: str, qty: int, price: float) -> str:
    """Return one line of a table: name in 10 columns on the left, qty in 5
    on the right and price with 2 decimals in 8 on the right.

    A width is a minimum, so a long value is not cut.
    """
    return f"{name:<10}|{qty:5d}|{price:8.2f}"


if __name__ == "__main__":
    for row in [("apple", 3, 1.5), ("widget", 12, 19.99), ("laptop", 1, 1234.5)]:
        print(format_row(*row))
