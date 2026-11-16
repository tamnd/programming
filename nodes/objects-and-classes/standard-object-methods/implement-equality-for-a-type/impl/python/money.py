# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/implement-equality-for-a-type/.


class Money:
    """An amount in cents and a currency code. Equal amounts in the same currency are equal."""

    __slots__ = ("_cents", "_currency")

    def __init__(self, cents, currency):
        self._cents = cents
        self._currency = currency.upper()  # "eur" and "EUR" are the same currency

    @property
    def cents(self):
        return self._cents

    @property
    def currency(self):
        return self._currency

    def __eq__(self, other):
        if not isinstance(other, Money):
            return NotImplemented  # Python then tries other.__eq__, and at last identity
        return self._cents == other._cents and self._currency == other._currency

    def __hash__(self):
        # The same fields as __eq__, so equal values have equal hashes.
        return hash((self._cents, self._currency))

    def __repr__(self):
        return f"Money({self._cents!r}, {self._currency!r})"


def count_distinct(amounts, currencies):
    return len({Money(a, c) for a, c in zip(amounts, currencies)})
