// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

/**
 * Java cannot add a method to String: the class is final, and the language has no extension
 * methods. A static method in a utility class takes the string as its first parameter instead.
 */
public final class Words {
  private Words() {}

  /** Returns the number of runs of characters that are not space, tab, newline or return. */
  public static int wordCount(String text) {
    int count = 0;
    boolean inWord = false;
    for (int i = 0; i < text.length(); i++) {
      char c = text.charAt(i);
      boolean space = c == ' ' || c == '\t' || c == '\n' || c == '\r';
      if (!space && !inWord) {
        count++;
      }
      inWord = !space;
    }
    return count;
  }
}
