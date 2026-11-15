// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

import java.util.ArrayList;
import java.util.List;

final class FizzBuzz {
  private FizzBuzz() {}

  /** Returns FizzBuzz, Fizz, Buzz or the number for each number from 1 to n. */
  static List<String> fizzbuzz(int n) {
    List<String> out = new ArrayList<>();
    for (int i = 1; i <= n; i++) {
      // Test 15 first: a multiple of 15 is also a multiple of 3 and of 5.
      if (i % 15 == 0) {
        out.add("FizzBuzz");
      } else if (i % 3 == 0) {
        out.add("Fizz");
      } else if (i % 5 == 0) {
        out.add("Buzz");
      } else {
        out.add(Integer.toString(i));
      }
    }
    return out;
  }
}
