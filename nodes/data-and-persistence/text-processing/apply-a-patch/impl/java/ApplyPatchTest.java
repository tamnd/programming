// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ApplyPatchTest {
  @TempDir Path dir;

  // name, the old text, the patch, the new text
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("change-a-line", "a\nb\nc\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n a\n-b\n+B\n c\n", "a\nB\nc\n"),
        Arguments.of("two-hunks", "apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n apple\n-banana\n+blueberry\n cherry\n@@ -6,2 +6,2 @@\n fig\n-grape\n+guava\n", "apple\nblueberry\ncherry\ndate\nelder\nfig\nguava\n"),
        Arguments.of("git-header", "def main():\n    print(1)\n", "diff --git a/main.py b/main.py\nindex 1234567..89abcde 100644\n--- a/main.py\n+++ b/main.py\n@@ -1,2 +1,2 @@ def main():\n def main():\n-    print(1)\n+    print(2)\n", "def main():\n    print(2)\n"),
        Arguments.of("add-at-end", "a\n", "@@ -1 +1,2 @@\n a\n+b\n", "a\nb\n"),
        Arguments.of("old-empty", "", "--- /dev/null\n+++ b.txt\n@@ -0,0 +1,2 @@\n+a\n+b\n", "a\nb\n"),
        Arguments.of("delete-all", "a\nb\n", "@@ -1,2 +0,0 @@\n-a\n-b\n", ""),
        Arguments.of("no-count", "a\nb\nc\n", "@@ -2 +2 @@\n-b\n+x\n", "a\nx\nc\n"),
        Arguments.of("zero-context", "apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "@@ -3 +3 @@\n-cherry\n+coconut\n@@ -6 +5,0 @@\n-fig\n@@ -7,0 +7 @@\n+honeydew\n", "apple\nbanana\ncoconut\ndate\nelder\ngrape\nhoneydew\n"),
        Arguments.of("insert-at-start", "b\n", "@@ -0,0 +1 @@\n+a\n", "a\nb\n"),
        Arguments.of("old-no-final-newline", "a\nb", "@@ -1,2 +1,2 @@\n a\n-b\n\\ No newline at end of file\n+b\n", "a\nb\n"),
        Arguments.of("new-no-final-newline", "a\nb\n", "@@ -1,2 +1,2 @@\n a\n-b\n+b\n\\ No newline at end of file\n", "a\nb"),
        Arguments.of("empty-context-line", "a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n\n-b\n+c\n", "a\n\nc\n"),
        Arguments.of("space-context-line", "a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n \n-b\n+c\n", "a\n\nc\n"),
        Arguments.of("crlf", "a\r\nb\r\n", "@@ -1,2 +1,2 @@\n a\r\n-b\r\n+c\r\n", "a\r\nc\r\n"),
        Arguments.of("header-like-line", "x\n-- y\n", "--- old.txt\n+++ new.txt\n@@ -1,2 +1 @@\n x\n--- y\n", "x\n"),
        Arguments.of("non-ascii", "caf\u00e9\n", "@@ -1 +1 @@\n-caf\u00e9\n+na\u00efve\n", "na\u00efve\n"));
  }

  // name, the old text, the patch, the kind of the error
  static Stream<Arguments> errors() {
    return Stream.of(
        Arguments.of("mismatch", "a\nb\nc\n", "@@ -1,3 +1,3 @@\n a\n-x\n+y\n c\n", "mismatch"),
        Arguments.of("malformed", "a\nb\nc\n", "@@ -1,3 +1,3 @@\n a\n-b\n", "malformed"),
        Arguments.of("no-hunk", "a\n", "no hunk here\n", "malformed"),
        Arguments.of("bad-mark", "a\n", "@@ -1 +1 @@\n*a\n", "malformed"),
        Arguments.of("past-the-end", "a\n", "@@ -2 +2 @@\n-a\n+b\n", "mismatch"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void apply(String name, String oldText, String patch, String want) {
    assertEquals(want, ApplyPatch.apply(oldText, patch), name);
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("errors")
  void error(String name, String oldText, String patch, String kind) {
    ApplyPatch.PatchException e =
        assertThrows(ApplyPatch.PatchException.class, () -> ApplyPatch.apply(oldText, patch), name);
    assertEquals(kind, e.kind().name().toLowerCase(java.util.Locale.ROOT), name);
  }

  @Test
  void manyHunks() {
    // 1000 lines, and 10 hunks with no context change 10 of them.
    StringBuilder oldText = new StringBuilder();
    StringBuilder patch = new StringBuilder();
    StringBuilder want = new StringBuilder();
    for (int i = 0; i < 1000; i++) {
      oldText.append(i).append('\n');
      if (i % 100 == 50) {
        patch.append("@@ -" + (i + 1) + " +" + (i + 1) + " @@\n-" + i + "\n+x" + i + "\n");
        want.append('x');
      }
      want.append(i).append('\n');
    }
    assertEquals(want.toString(), ApplyPatch.apply(oldText.toString(), patch.toString()));
  }

  @Test
  void applyFile() throws IOException {
    Files.writeString(dir.resolve("old.txt"), "a\r\nb\r\n", StandardCharsets.UTF_8);
    Files.writeString(dir.resolve("a.patch"), "@@ -1,2 +1,2 @@\n a\r\n-b\r\n+c\r\n", StandardCharsets.UTF_8);
    assertEquals("a\r\nc\r\n", ApplyPatch.applyFile(dir.resolve("old.txt"), dir.resolve("a.patch")));
  }

  @Test
  void notFound() throws IOException {
    Files.writeString(dir.resolve("a.patch"), "@@ -1 +1 @@\n-a\n+b\n", StandardCharsets.UTF_8);
    assertThrows(
        NoSuchFileException.class,
        () -> ApplyPatch.applyFile(dir.resolve("missing.txt"), dir.resolve("a.patch")));
  }
}
