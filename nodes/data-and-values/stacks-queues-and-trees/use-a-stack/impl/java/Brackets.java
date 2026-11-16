// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

import java.util.ArrayDeque;
import java.util.Deque;

final class Brackets {
  private Brackets() {}

  /** Returns true if the brackets of text match and nest correctly. */
  static boolean balanced(String text) {
    // ArrayDeque is the stack. The class Stack is older; its documentation
    // tells you to use a Deque.
    Deque<Character> stack = new ArrayDeque<>();
    for (int i = 0; i < text.length(); i++) {
      char c = text.charAt(i);
      switch (c) {
        case '(', '[', '{' -> stack.push(c);
        case ')', ']', '}' -> {
          Character top = stack.poll(); // null if the stack is empty
          if (top == null || top != open(c)) {
            return false;
          }
        }
        default -> {}
      }
    }
    return stack.isEmpty();
  }

  private static char open(char close) {
    return switch (close) {
      case ')' -> '(';
      case ']' -> '[';
      default -> '{';
    };
  }
}
