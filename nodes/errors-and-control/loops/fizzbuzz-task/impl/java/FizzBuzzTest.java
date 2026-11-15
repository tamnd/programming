// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class FizzBuzzTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("fifteen", 15, List.of("1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz", "11", "Fizz", "13", "14", "FizzBuzz")),
        Arguments.of("thirty", 30, List.of("1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz", "11", "Fizz", "13", "14", "FizzBuzz", "16", "17", "Fizz", "19", "Buzz", "Fizz", "22", "23", "Fizz", "Buzz", "26", "Fizz", "28", "29", "FizzBuzz")),
        Arguments.of("zero", 0, List.of()),
        Arguments.of("one", 1, List.of("1")),
        Arguments.of("three", 3, List.of("1", "2", "Fizz")),
        Arguments.of("five", 5, List.of("1", "2", "Fizz", "4", "Buzz")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void fizzbuzz(String name, int n, List<String> want) {
    assertEquals(want, FizzBuzz.fizzbuzz(n), name);
  }
}
