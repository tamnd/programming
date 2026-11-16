# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/interpolate-values-into-a-string/.


def describe_order(name: str, count: int, price: float) -> str:
    """Return "Order {name}: count item(s), total count*price".

    The doubled braces are literal braces. The total has 2 decimals.
    """
    return f"Order {{{name}}}: {count} item{'' if count == 1 else 's'}, total {count * price:.2f}"


if __name__ == "__main__":
    print(describe_order("Ada", 3, 1.5))
