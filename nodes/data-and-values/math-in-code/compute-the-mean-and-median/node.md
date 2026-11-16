---
tag: T42T3A
kind: task
slug: compute-the-mean-and-median
name: Compute the mean and median
aliases: [average of a list, arithmetic mean, median of a list, mode of a list, statistics.mean, statistics.median, average(), Enumerable.Average, middle value]
area: data-and-values/math-in-code
level: intro
summary: >-
  Compute the arithmetic mean and the median of a list of integers. The mean needs a floating-point division, and the median of an even-length list is the mean of the two middle values.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Averages-Arithmetic-mean/00-TASK.txt
  title: Rosetta Code, Averages/Arithmetic mean (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Averages-Median/00-TASK.txt
  title: Rosetta Code, Averages/Median (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://en.wikipedia.org/wiki/Median
  title: Wikipedia, Median
  locator: Finite set of numbers
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Averages-Mode/00-TASK.txt
  title: Rosetta Code, Averages/Mode (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://docs.python.org/3/library/statistics.html
  title: The Python Standard Library, statistics
  locator: mean; fmean; median; median_low; multimode
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/LongStream.html
  title: Java SE 25 API, Interface LongStream
  locator: average()
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.average?view=net-10.0
  title: .NET API reference, Enumerable.Average method
  locator: Exceptions
- url: https://pkg.go.dev/slices
  title: Go standard library, slices
  locator: func Sorted; func Sort
- url: https://doc.rust-lang.org/std/primitive.slice.html
  title: Rust standard library, primitive type slice
  locator: select_nth_unstable
- url: https://en.cppreference.com/w/cpp/algorithm/nth_element.html
  title: cppreference.com, std::nth_element
  locator: the description of the function
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/DoubleStream.html
  title: Java SE 25 API, Interface DoubleStream
  locator: sum()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
  title: MDN Web Docs, Array.prototype.sort()
  locator: Description
---

## Task

Compute the arithmetic mean and the median of a non-empty list of integers.

- The mean is the sum of the items divided by the count. It is also called the arithmetic average[^1].
- The median is the middle value of the sorted list. The Rosetta Code median task says that a solution "must handle the case where there are an even number of elements", and for that case it says: "return the average of the two middle values"[^2]. Wikipedia gives the same rule: "If the data set has an even number of observations, there is no distinct middle value and the median is usually defined to be the arithmetic mean of the two middle values"[^3].

The function `mean_and_median(items)` returns the pair `(mean, median)` as floating-point numbers. For example, `mean_and_median([1, 2, 3, 4, 100])` is `(22.0, 3.0)`, and `mean_and_median([4, 1, 3, 2])` is `(2.5, 2.5)`. The example shows why the median is useful: one large value moves the mean far, but not the median.

The items are 64-bit signed integers, and the list is not empty. The mean of an empty list has no value, and the Rosetta Code task says that "if the programming language has an established convention for conveying math errors or undefined values, it's preferable to follow it"[^1]. The results are floating-point numbers, so the checker compares them with an absolute tolerance of 1e-9 or a relative tolerance of 1e-12.

The mode is the most frequent value. A list can have more than one mode, and Rosetta Code says: "Care must be taken to handle the case where the mode is non-unique"[^4]. The Python example below shows it.

## Solutions

| Language | Mean | Median |
|---|---|---|
| Python | `statistics.mean(items)` | `statistics.median(items)` |
| JavaScript, TypeScript | sum with `reduce`, then divide by `length` | sort a copy with a compare function |
| Java | `LongStream.of(items).asDoubleStream().average()` | sort a copy with `Arrays.sort` |
| C# | `items.Average(x => (double)x)` | sort a copy with `Order()` |
| Go | sum as `float64` in a loop | `slices.Sorted(slices.Values(items))` |
| Rust | sum as `f64` with `map` and `sum` | `to_vec()`, then `sort_unstable()` |
| C | sum as `double` in a loop | `qsort` of a copy |
| C++ | `std::accumulate` with the start value `0.0` | `std::nth_element` on a copy |
| Bash | awk | `sort -n`, then awk |

- **Python.** `statistics.mean` "Return the sample arithmetic mean of data"[^5]. It is exact for integers. `statistics.fmean` is faster, and "it always returns a float"[^5]. `statistics.median` uses "the common “mean of middle two” method"[^5]. All of them raise `StatisticsError` for empty data.
- **JavaScript.** There is no mean or median function. The solution sums the items, and sorts a copy with `(a, b) => a - b` for the median. A 64-bit integer above 2 to the power 53 is not exact in a `number`, but the results are floating-point numbers, so the error is small.
- **TypeScript.** The same code as JavaScript, with the type `number[]`.
- **Java.** `average()` of a `LongStream` "Returns an OptionalDouble describing the arithmetic mean of elements of this stream, or an empty optional if this stream is empty"[^6]. The solution converts the items to `double` first, because the `long` sum can overflow, as a pitfall shows.
- **C#.** `Enumerable.Average` throws `InvalidOperationException` when the "source contains no elements"[^7]. The selector `x => (double)x` makes the sum a `double`.
- **Go.** `slices.Sorted` collects the values of an iterator into a new sorted slice[^8], so the caller's slice does not change.
- **Rust.** The solution sorts a copy. `select_nth_unstable` "Reorders the slice such that the element at index is at a sort-order position"[^9], so it finds a middle value without a full sort.
- **C.** The solution copies the items, sorts the copy with `qsort`, and sums them in a `double`.
- **C++.** `std::nth_element` puts the element of a position in its sorted place, with the smaller elements before it[^10]. It finds the median in linear time on average, without a full sort.
- **Bash.** Bash arithmetic has only integers, so `sort -n` sorts the items, and awk computes the mean and the median. The function prints them with the awk format `%.17g`, which shows each digit of a double.

The Python functions, with the mode:

```python run id=python-statistics
import statistics

data = [1, 2, 3, 4, 100]
print(statistics.mean(data), statistics.median(data))
print(statistics.median([4, 1, 3, 2]))
print(statistics.median_low([4, 1, 3, 2]), statistics.median_high([4, 1, 3, 2]))
print(statistics.mode([1, 2, 2, 3, 3]), statistics.multimode([1, 2, 2, 3, 3]))
```

```output for=python-statistics
22 3
2.5
2 3
2 [2, 3]
```

`statistics.mean` of integers gives the integer 22 here, because the mean is exact. `median_low` and `median_high` return a member of the data. `mode` "returns the first one encountered in the data"[^5] when there are many modes, and `multimode` returns all of them.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `odd`, `even` and `skewed`: an odd count has one middle value, and an even count has two.
- `single`: the mean and the median of one item are the item.
- `two-items`: the median is the mean of both items.
- `negative`: negative items.
- `unsorted-even`: the items must be sorted first. The median of `[10, 2, 8, 4]` is 6.
- `duplicates`: the mean is 2 and the median is 1.
- `not-an-integer`: the mean of `[1, 2, 2]` is 5/3. An integer division gives 1.
- `large`: the sum of two items near the 64-bit limit does not fit in 64 bits. An integer sum overflows.

## Pitfalls

- **An integer division drops the fraction.** In Go, Java, C, C++ and C#, the division of two integers gives an integer. Convert to a floating-point type before the division. See [[integer-division-and-modulo]]:

```go wrong id=go-integer-mean
package main

import "fmt"

func main() {
	items := []int{1, 2, 2}
	sum := 0
	for _, x := range items {
		sum += x
	}
	fmt.Println(sum / len(items))
	fmt.Println(float64(sum) / float64(len(items)))
}
```

```output for=go-integer-mean
1
1.6666666666666667
```

- **The sum overflows.** A mean of large integers needs a sum that does not fit. In Java, the `long` sum inside `LongStream.average()` wraps around, so the mean of two copies of `Long.MAX_VALUE` is -1.0. A `DoubleStream` sum does not overflow, and it "may be implemented using compensated summation or other technique to reduce the error bound"[^11]:

```java wrong id=java-average-overflow
import java.util.stream.LongStream;

public class Main {
  public static void main(String[] args) {
    long[] items = {Long.MAX_VALUE, Long.MAX_VALUE};
    System.out.println(LongStream.of(items).average().getAsDouble());
    System.out.println(LongStream.of(items).asDoubleStream().average().getAsDouble());
  }
}
```

```output for=java-average-overflow
-1.0
9.223372036854776E18
```

  The same is true for the mean of the two middle values: `(a + b) / 2` can overflow when `a` and `b` are large. Convert them to `double` first, or compute `a + (b - a) / 2`.

- **The default JavaScript sort compares strings.** "The default sort order is ascending, built upon converting the elements into strings"[^12], so 10 comes before 2, and the median is wrong. Give a compare function:

```javascript wrong id=javascript-sort-strings
const items = [10, 2, 8, 4];
console.log([...items].sort());
console.log([...items].sort((a, b) => a - b));
```

```output for=javascript-sort-strings
[ 10, 2, 4, 8 ]
[ 2, 4, 8, 10 ]
```

- **An in-place sort changes the caller's list.** `list.sort()` in Python, `Arrays.sort` in Java and `sort` in JavaScript change the list. A median function that sorts its argument surprises the caller. Sort a copy, or use a function that makes one, such as `sorted` in Python or `toSorted` in JavaScript.

- **An even-length list has two middle values.** Taking only `items[n / 2]` gives the upper middle value. For `[1, 2, 3, 4]` it gives 3, not 2.5.

- **The mean of an empty list.** Python raises `StatisticsError`. In JavaScript, `0 / 0` is `NaN` with no error. Check for an empty list first:

```python run id=python-mean-empty
import statistics

try:
    statistics.mean([])
except statistics.StatisticsError as e:
    print(type(e).__name__)
```

```output for=python-mean-empty
StatisticsError
```

## Related tasks

- [[sum-and-multiply-a-list]]: the sum is the first step of the mean.
- [[sort-a-list]]: the median needs the sorted order.
- [[count-the-items-of-each-kind]]: the counts give the mode.
- [[find-the-largest-element]]: the largest and the smallest items are other summaries of a list.

The selection algorithm that finds the median in linear time is on the algorithm site: [quickselect](https://algorithm.tamnd.com/quickselect/).

## References
