// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

/** Checks whether a text is a palindrome, with case, spaces and punctuation ignored. */
public final class Palindrome {
    private Palindrome() {}

    private static boolean isAsciiLetterOrDigit(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
    }

    /**
     * Returns true if the ASCII letters and digits of text, in lower case,
     * read the same backwards. Two indexes move toward the middle, so the
     * method makes no copy of the text.
     */
    public static boolean isPalindrome(String text) {
        int i = 0;
        int j = text.length() - 1;
        while (i < j) {
            if (!isAsciiLetterOrDigit(text.charAt(i))) {
                i++;
            } else if (!isAsciiLetterOrDigit(text.charAt(j))) {
                j--;
            } else if (Character.toLowerCase(text.charAt(i)) != Character.toLowerCase(text.charAt(j))) {
                return false;
            } else {
                i++;
                j--;
            }
        }
        return true;
    }

    public static void main(String[] args) {
        System.out.println(isPalindrome("A man, a plan, a canal: Panama"));
    }
}
