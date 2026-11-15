---
tag: RJ6CRP
kind: pattern
slug: money
name: Money
aliases: [Money pattern, money type, monetary amount]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  Money is a value object for an amount and its currency. It keeps exact amounts, refuses to add different currencies, and splits an amount with no lost cents.
family: architectural
polarity: positive
intent: "Represent a monetary value as an amount and a currency, with exact arithmetic."
history:
  year: 2002
  people: [Martin Fowler]
  note: Martin Fowler described the pattern in Patterns of Enterprise Application Architecture (2002), in the chapter on base patterns.
see_also: [value-object, floating-point-arithmetic, primitive-obsession, embedded-value, integer-overflow, immutable-object, value-equality-and-identity, domain-model]
sources:
  - url: https://martinfowler.com/eaaCatalog/money.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Money
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 18, Base Patterns, Money
  - url: https://docs.python.org/3/library/decimal.html
    title: Python documentation, decimal, Decimal fixed-point and floating-point arithmetic
    locator: introduction
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/Currency.html
    title: Java SE 21 API, java.util.Currency
    locator: class description and getDefaultFractionDigits
---

## Intent

The Money pattern "Represents a monetary value."[^1] A money object has an amount and a currency. It does exact arithmetic, it refuses to mix currencies, and it splits an amount so that no part of a cent is lost.

## Problem

Many programs calculate with money, but most languages have no money type. A program that keeps money in a plain number has two problems. The catalog names the first: "you want to avoid adding your dollars to your yen without taking the currency differences into account".[^1] A number has no currency, so the language cannot stop the addition.

The second problem is rounding. "Monetary calculations are often rounded to the smallest currency unit. When you do this it's easy to lose pennies (or your local equivalent) because of rounding errors."[^1] Binary floating point makes this worse, because it cannot keep 0.10 exactly.

## Solution

Make a [[value-object]] for money. The parts are:

- **The amount.** Keep it in an exact type: an integer number of the minor unit, such as cents, or a decimal type. Never use a binary floating point number.
- **The currency.** Each amount has its currency, such as an ISO 4217 code. The currency also tells how many digits the minor unit has.
- **Arithmetic that checks the currency.** Addition and subtraction of two amounts with different currencies is an error. Conversion is a separate step with an exchange rate.
- **Allocation.** To split an amount by ratios, give each part its share rounded down, then give the units that are left to the parts one by one. The sum of the parts is always the whole amount. The book shows this with 5 cents that must go 70% to one account and 30% to another.[^2]
- **Equality by value.** Two money objects are equal when the amount and the currency are equal.

```text
  Money(1000, EUR) + Money(50, EUR)   -> Money(1050, EUR)   "10.50 EUR"
  Money(1000, EUR) + Money(500, JPY)  -> error: cannot add EUR and JPY
  Money(5, EUR).allocate([70, 30])    -> [Money(4, EUR), Money(1, EUR)]
```

In the following program, `allocate` splits amounts in minor units. 100.00 in three parts gives 33.34, 33.33 and 33.33, and 0.05 in the ratio 70 to 30 gives 0.04 and 0.01.

```python run id=allocate-minor-units
def allocate(units, ratios):
    total = sum(ratios)
    parts = [units * r // total for r in ratios]
    for i in range(units - sum(parts)):  # give the units that are left, one each
        parts[i] += 1
    return parts


print(allocate(10000, [1, 1, 1]))  # 100.00 in three parts
print(allocate(5, [70, 30]))  # 0.05 in the ratio 70 to 30
```

```output
[3334, 3333, 3333]
[4, 1]
```

## Example

Each example has a `Money` value with an integer amount in the minor unit and a currency code. The program adds 0.50 EUR to 10.00 EUR. It tries to add 500 JPY to an amount in EUR and gets an error. It prints 500 JPY with no decimal point, because the yen has no minor unit. Then it splits 5 cents in the ratio 70 to 30, and 100 EUR in three equal parts. Last, it compares two equal amounts.

## Consequences

**Benefits.**

- **No lost cents.** Integer or decimal amounts are exact, and allocation keeps the whole amount.
- **No mixed currencies.** The money type finds an addition of two currencies when the program runs, or when it compiles in a language with a type for each currency.
- **One place for the rules.** Rounding, formatting and allocation are in one class, and not copied in each calculation.
- **Clearer code.** A parameter of type `Money` tells more than a number. The code smell [[primitive-obsession]] describes the opposite.

**Liabilities.**

**A float amount loses a cent when it is split.** In the following program, 100.00 is split into three parts with `round`. Each part is 33.33, so the sum is 99.99, and one cent is gone.

```python wrong id=float-split-loses-a-cent
total = 100.00  # a float amount
shares = [round(total / 3, 2) for _ in range(3)]
print("shares:", shares)
print("sum of the shares:", round(sum(shares), 2))
```

```output
shares: [33.33, 33.33, 33.33]
sum of the shares: 99.99
```

Keep the amount as 10000 cents, and allocate the cents that are left after the division to the first parts, as `allocate` does.

**More work for each operation.** Multiplication by a rate, such as a tax rate, needs a rounding rule. The rule depends on the business and sometimes on the law, so the class must make it explicit.

**Overflow.** An integer amount of a fixed size can overflow for very large amounts. Use a checked addition or a type with no fixed size. See [[integer-overflow]].

**Storage.** The database needs both the amount and the currency. A money object is usually saved as an [[embedded-value]] in two columns of the row of its owner.

## When not to use it

**When the amount is not money for the program.** A price that the program only shows, and never adds, can stay a string from the source.

**When the platform already has a money type.** If a library or a framework of the program has a tested money type, use it and do not write a second one.

## Variants and related patterns

**Decimal amounts.** A decimal type is an alternative to integer minor units. "Decimal numbers can be represented exactly."[^3] The Python documentation adds that "In decimal floating point, 0.1 + 0.1 + 0.1 - 0.3 is exactly equal to zero", and for this reason "decimal is preferred in accounting applications".[^3] A decimal amount still needs a currency and a rounding rule.

**Currency data.** Java has a class for the currency. It says: "Currencies are identified by their ISO 4217 currency codes."[^4] Its method `getDefaultFractionDigits` "Gets the default number of fraction digits used with this currency."[^4] For example, "the default number of fraction digits for the Euro is 2, while for the Japanese Yen it's 0."[^4]

**Related patterns.** Money is a [[value-object]]: it is immutable and equal by value. A [[domain-model]] uses money objects for prices, balances and totals. The [[floating-point-arithmetic]] page explains why 0.1 has no exact binary form.

## Known uses

- The Java `Currency` class gives ISO 4217 codes and fraction digits, and its documentation recommends `BigDecimal` for monetary values.[^4]
- The Python `decimal` module gives exact decimal arithmetic for amounts.[^3]

## References
