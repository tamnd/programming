// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class RunProcessTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("hello", List.of("printf", "hello"), 0, "hello", ""),
        Arguments.of("two-arguments", List.of("echo", "one", "two"), 0, "one two\n", ""),
        Arguments.of("exit-status", List.of("sh", "-c", "exit 3"), 3, "", ""),
        Arguments.of(
            "both-streams",
            List.of("sh", "-c", "echo out; echo err >&2; exit 2"),
            2,
            "out\n",
            "err\n"),
        Arguments.of(
            "metacharacters",
            List.of("printf", "%s|", "a b", "$HOME", "*", "x;y", "\"q\"", "'s'"),
            0,
            "a b|$HOME|*|x;y|\"q\"|'s'|",
            ""),
        Arguments.of("empty-argument", List.of("printf", "[%s]", ""), 0, "[]", ""),
        Arguments.of(
            "newline-argument",
            List.of("printf", "%s", "line one\nline two"),
            0,
            "line one\nline two",
            ""),
        Arguments.of("trailing-newlines", List.of("printf", "a\\n\\n"), 0, "a\n\n", ""),
        Arguments.of(
            "non-ascii",
            List.of("printf", "%s\\n", "xin chào, 日本語, 🙂"),
            0,
            "xin chào, 日本語, 🙂\n",
            ""),
        Arguments.of("stderr-only", List.of("sh", "-c", "echo warning >&2"), 0, "", "warning\n"),
        Arguments.of("no-output", List.of("true"), 0, "", ""),
        Arguments.of("false", List.of("false"), 1, "", ""),
        Arguments.of("status-255", List.of("sh", "-c", "exit 255"), 255, "", ""),
        Arguments.of(
            "empty-stdin",
            List.of("sh", "-c", "if read -r line; then echo got; else echo eof; fi"),
            0,
            "eof\n",
            ""));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void runsTheProgram(String name, List<String> argv, int status, String stdout, String stderr)
      throws IOException, InterruptedException {
    assertEquals(new RunProcess.Result(status, stdout, stderr), RunProcess.run(argv), name);
  }

  @Test
  void largeOutputOnBothStreams() throws IOException, InterruptedException {
    String script =
        "head -c 1048576 /dev/zero | tr '\\0' x; head -c 1048576 /dev/zero | tr '\\0' y >&2";
    RunProcess.Result result = RunProcess.run(List.of("sh", "-c", script));
    assertEquals(0, result.status());
    assertEquals("x".repeat(1048576), result.stdout());
    assertEquals("y".repeat(1048576), result.stderr());
  }

  @Test
  void missingProgram() {
    assertThrows(IOException.class, () -> RunProcess.run(List.of("no-such-program-7f3a")));
  }
}
