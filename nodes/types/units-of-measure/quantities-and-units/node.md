---
tag: A4Z1BN
kind: concept
slug: quantities-and-units
name: Units of measure
aliases: [units of measure, units of measurement, physical units, dimensional analysis, dimension types, quantity types, unit types, units in types]
area: types/units-of-measure
level: core
summary: >-
  A unit type attaches a unit, such as metres or seconds, to a number. The compiler or the library then rejects sums of different units and works out the unit of each product and quotient.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1994
  people: [Andrew Kennedy, Michael Karr, David Loveman, John Barton, Lee Nackman, Howard Hinnant]
  note: Andrew Kennedy described dimension types with inference in 1994 and later added units of measure to F#. Barton and Nackman checked dimensions with C++ templates in the same year.
see_also: [type-conversion, static-and-dynamic-typing, dependent-types, parametric-polymorphism, floating-point-arithmetic, measure-elapsed-time, constant-evaluation]
same_as:
  wikidata: Q217113
  wikipedia: Dimensional_analysis
sources:
- url: https://learn.microsoft.com/en-us/dotnet/fsharp/language-reference/units-of-measure
  title: Units of Measure, F# language reference
  locator: Introduction and Units at Runtime
- url: https://en.cppreference.com/w/cpp/chrono/duration.html
  title: std::chrono::duration, cppreference.com
  locator: Description
- url: https://pkg.go.dev/time#Duration
  title: Package time, Go standard library documentation
  locator: type Duration
- url: https://doc.rust-lang.org/std/time/struct.Duration.html
  title: Struct Duration, Rust standard library documentation
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/Duration.html
  title: Class Duration, Java SE 25 API documentation
  locator: Class description
- url: https://docs.python.org/3/library/datetime.html#timedelta-objects
  title: datetime, Basic date and time types, Python documentation
  locator: timedelta objects
- url: https://drafts.csswg.org/css-values-4/
  title: CSS Values and Units Module Level 4, W3C Editor's Draft
  locator: Type checking of math functions
- url: https://en.wikipedia.org/wiki/Mars_Climate_Orbiter
  title: Mars Climate Orbiter, Wikipedia
  locator: Cause of failure
---

## Idea

A number in a program usually has a unit, such as metres, seconds or bytes, but the language sees only the number. A unit type attaches the unit to the value, so that the compiler or a library can check the arithmetic. Two rules come from physics. Values can be added, subtracted and compared only when they have the same dimension, such as length or time. Multiplication and division combine dimensions, so a length divided by a time is a speed. A unit system also converts between units of one dimension, such as feet and metres, at one place in the code. Without these checks, a value in one unit can reach code that expects another, and the program gives a wrong answer with no error.

```python run id=idea
from fractions import Fraction

BASE = ("m", "kg", "s")  # length, mass, time: the exponents form the dimension


class Q:  # a quantity: a value in SI base units, with a dimension
    def __init__(self, value, dim):
        self.value, self.dim = value, tuple(dim)

    def __add__(self, other):  # only quantities of one dimension can be added
        if self.dim != other.dim:
            raise TypeError(f"cannot add {unit(self.dim)} and {unit(other.dim)}")
        return Q(self.value + other.value, self.dim)

    def __mul__(self, other):  # multiplying adds the exponents
        return Q(self.value * other.value, map(sum, zip(self.dim, other.dim)))

    def __truediv__(self, other):  # dividing subtracts them
        return Q(self.value / other.value,
                 (a - b for a, b in zip(self.dim, other.dim)))

    def __repr__(self):
        return f"{float(self.value):g} {unit(self.dim)}"


def unit(dim):
    return " ".join(b if e == 1 else f"{b}^{e}" for b, e in zip(BASE, dim) if e) or "1"


metre, kilogram, second = Q(1, (1, 0, 0)), Q(1, (0, 1, 0)), Q(1, (0, 0, 1))
newton = kilogram * metre / (second * second)
pound_force = Q(Fraction(44482216152605, 10**13), newton.dim)  # 4.4482... N

distance, time = Q(100, metre.dim), Q(Fraction(958, 100), second.dim)
print("speed:", distance / time)
accel = Q(2, (metre / (second * second)).dim)
print("force:", Q(3, kilogram.dim) * accel, "=", (Q(3, kilogram.dim) * accel).value, "N")
print("10 lbf s in N s:", Q(10, (0, 0, 0)) * pound_force * second)
try:
    distance + time
except TypeError as err:
    print("TypeError:", err)

impulse = 10  # a bare number: lbf s for one team, N s for the other
print(f"read as N s: {impulse} N s, error factor {float(pound_force.value):.2f}")
```

```output for=idea
speed: 10.4384 m s^-1
force: 6 m kg s^-2 = 6 N
10 lbf s in N s: 44.4822 m kg s^-1
TypeError: cannot add m and s
read as N s: 10 N s, error factor 4.45
```

Each quantity stores its value in base units and its dimension as a tuple of exponents of metres, kilograms and seconds. Division subtracts the exponents, so 100 metres in 9.58 seconds gives metres per second, and a newton is the derived unit kg m s^-2. Adding a length to a time raises an error. The last line shows the error that a bare number permits: an impulse in pound-force seconds read as newton-seconds is wrong by a factor of 4.45, with no error from the program.

## How it works

**Dimensions and units.** A dimension is a kind of quantity, such as length, mass or time, and a unit is one scale for it, such as the metre or the foot. The SI defines seven base dimensions, and every other dimension is a product of powers of them: speed is length to the power 1 and time to the power -1. A unit system stores the exponents for each value, as the example does at run time, or in the type, where the compiler checks them. Addition needs equal exponents, multiplication adds them, and division subtracts them. A quantity with all exponents zero, such as a ratio of two lengths, is a plain number.

**Units in the type system.** F# has units of measure in the language: "By using quantities with units, you enable the compiler to verify that arithmetic relationships have the correct units, which helps prevent programming errors."[^1] The compiler infers units like it infers types, so a function that divides its arguments gets a type such as `float<'a> -> float<'b> -> float<'a/'b>`, generic over units. The check costs nothing when the program runs: "the units of measure are eliminated, so the units are lost at run time".[^1] Languages without this feature encode the exponents in generic types, as C++ templates and Rust traits do, or write one type per unit.

**One type per unit.** The simplest form gives each unit its own type, such as a `Metres` struct or a named type in Go, and defines only the operations that make sense. The compiler then rejects a `Feet` where a `Metres` is expected, and each conversion is a named function. This form is easy to read, but each product and quotient needs its own type and operator, and generic code over all units is hard to write.

**Time in standard libraries.** Most standard libraries have a unit type for durations, the most common unit in programs. C++'s `std::chrono::duration` puts the unit in the type: "the tick period is a compile-time rational fraction representing the time in seconds from one tick to the next".[^2] Go's time package says that "A Duration represents the elapsed time between two instants as an int64 nanosecond count".[^3] Rust's `Duration` is "A Duration type to represent a span of time, typically used for system timeouts",[^4] Java's `Duration` is "A time-based amount of time, such as '34.5 seconds'",[^5] and in Python "A timedelta object represents a duration, the difference between two datetime or date instances".[^6] An API that takes one of these types, not a bare number of seconds or milliseconds, cannot be called with the wrong unit.

**Units in CSS.** CSS is one of the few languages in wide use with units in the language. Lengths, times and angles are dimensions, such as `10px`, `2s` and `45deg`, and `calc()` checks their types. The specification gives the rule for sums: "At a + or - sub-expression, attempt to add the types of the left and right arguments. If this returns failure, the entire calculation’s type is failure."[^7] So `calc(1px + 2s)` is invalid, while a length divided by a length gives a number.

**Conversions and scale.** A unit system converts at the edges of a program, where values are read and written, and keeps one unit inside. Most libraries store each value in a base unit, as the example and the JavaScript file do, so a conversion is one multiplication. Others, such as `std::chrono`, keep the unit in the type and convert when two units meet, which avoids rounding when all values are integers.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | library | `timedelta` for time; Pint and Astropy for physical units |
| JavaScript | idiom | one base unit per class; `Intl.NumberFormat` units for output only |
| TypeScript | idiom | branded number types |
| Java | library | `java.time.Duration`; JSR 385 and Indriya |
| Kotlin | idiom | `kotlin.time.Duration`; value classes per unit |
| C# | library | `TimeSpan`; record structs with operators; UnitsNet |
| Go | idiom | `time.Duration`; named types per unit |
| Rust | library | `std::time::Duration`; newtypes; the uom crate |
| C | idiom | one struct per unit, or the unit in the name |
| C++ | library | `std::chrono` ratios; dimension templates; mp-units |
| Bash | n/a | values are strings |
| PowerShell | idiom | `KB`, `MB` and `GB` suffixes; `TimeSpan` |
| SQL | idiom | `INTERVAL` types; units in column names |
| Regex | n/a | no values with units |
| HTML | n/a | attributes with fixed units |
| CSS | native | dimensions such as `px`, `s` and `deg`, checked in `calc()` |

No core general-purpose language checks physical dimensions in the language, as F# does, so the verdicts are library or idiom. Each one has a unit type for time in its standard library, and Python, Java, C#, Rust and C++ have widely used libraries for physical quantities. CSS is native: units are part of its values, and its math functions check dimensions.

The example files show three forms. Python, Java, C#, Go, Rust and C++ use the standard type for durations, and the Go file shows a limit: Go's `Duration` does not track dimensions, so a duration divided by a duration prints as `4ns`. C#, Go, Rust and C use one type per unit, and their fails blocks show the compiler rejecting a sum of different units. The C++ file encodes the exponents in template parameters, as the idea block does at run time. The C file also shows the common unchecked form, a plain number with the unit in its name.

## Variations and related ideas

- **Unit inference.** F# infers units for functions with no annotations and makes functions generic over units. Andrew Kennedy's work showed that this inference works like ordinary [type inference](/hindley-milner/).
- **Run-time quantities.** Pint, Astropy and UnitsNet store the unit with each value and check it during arithmetic. They are simpler to use with data from files, at a cost in speed and memory.
- **Affine units.** Temperatures in Celsius and Fahrenheit, positions in time and dates have an offset: two temperatures cannot be added, but a temperature and a difference can. `std::chrono` separates `time_point` from `duration` for this reason. See [measure elapsed time](/measure-elapsed-time/).
- **Information units.** Bytes, bits and pixels are not SI units, and systems such as mp-units define them as separate dimensions.
- **Angles and dimensionless quantities.** Radians, ratios and percentages have no dimension, so a dimension check cannot tell them apart. Some libraries treat angles as a separate kind.
- **Money.** A currency acts like a unit: amounts in two currencies cannot be added without a conversion rate, and the rate changes over time.
- **Refinement types.** A unit is one kind of fact about a number; [dependent and refinement types](/dependent-types/) state others, such as ranges.

## Pitfalls

- **Bare numbers at boundaries.** Most unit errors happen where one component passes a plain number to another. The Mars Climate Orbiter was lost in 1999 because "software that calculated the total impulse produced by thruster firings produced results in pound-force seconds", and the receiving software expected them "to be in newton-seconds (incorrect by a factor of 4.45)".[^8] Put the unit in the type of every interface, file format and API, or at least in the name.
- **Timeouts in the wrong unit.** An API that takes `timeout` as a number can be called with seconds where it expects milliseconds, and the program waits 1000 times too long or too short. Take a duration type, as the Go, Rust and Java standard libraries do.
- **Lossy conversions.** Converting milliseconds to seconds in integers truncates. `std::chrono` makes this explicit with `duration_cast`; in other languages, convert in floating point or keep the smaller unit.
- **Conversion factors written by hand.** A factor such as 0.3048 or 4.448 typed in many places can be wrong in one of them. Define each factor once, in the unit system.
- **Dimensionless confusion.** A check on dimensions cannot tell radians from degrees, or a ratio from a percentage, because all have dimension one. Name the unit, or use a separate type.
- **Floating-point error.** A conversion through a base unit and back can change the last digits of a value. See [floating-point arithmetic](/floating-point-arithmetic/).

## Cost

Units checked at compile time cost nothing when the program runs: F# erases them, and a C++ template quantity or a Rust newtype is a single `double` in memory, as the C++ file shows. A conversion costs one multiplication, which the compiler can often fold into a constant. Run-time unit libraries store a unit with each value, often a pointer or a small table of exponents, and check and combine units on each operation, which can make numeric code many times slower than plain floats; they also stop vectorized arrays unless the unit is stored once for a whole array, as Pint and Astropy do with NumPy. Compile-time unit libraries can increase compile times and give long error messages.

## History

Proposals to add units to programming languages appeared in the 1970s: Narain Gehani described units as a data attribute in 1977, and Michael Karr and David Loveman described the incorporation of units into programming languages in 1978. Andrew Kennedy defined dimension types with inference in 1994, and John Barton and Lee Nackman showed in the same year that C++ templates can check dimensions at compile time. Kennedy later added units of measure to F#, which first shipped them in 2008.[^1]

Standard libraries added types for durations. Python's `timedelta` arrived with the datetime module in 2003,[^6] C++11 added `std::chrono` from Howard Hinnant's design in 2011,[^2] Go 1.0 shipped `time.Duration` in 2012,[^3] Java 8 added `java.time` in 2014,[^5] and Rust 1.3 stabilized `Duration` in 2015.[^4] Libraries for physical units followed: Boost.Units in 2008, Pint in 2012, JSR 385 for Java in 2019, and mp-units, which is proposed for a future C++ standard. CSS added typed arithmetic with units to `calc()`.[^7] Accidents kept the topic visible, from the fuel error of an Air Canada flight in 1983, measured in pounds where kilograms were needed, to the loss of the Mars Climate Orbiter in 1999.[^8]

## References
