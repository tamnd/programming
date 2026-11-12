// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CountArgsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("files-only", List.of("a.txt", "b.txt"), new CountArgs(false, 10, List.of("a.txt", "b.txt"))),
        Arguments.of("short-options", List.of("-v", "-n", "3", "a.txt"), new CountArgs(true, 3, List.of("a.txt"))),
        Arguments.of("long-options", List.of("--verbose", "--limit", "5", "a.txt", "b.txt"), new CountArgs(true, 5, List.of("a.txt", "b.txt"))),
        Arguments.of("no-args", List.of(), new CountArgs(false, 10, List.of())),
        Arguments.of("long-equals", List.of("--limit=7", "notes.md"), new CountArgs(false, 7, List.of("notes.md"))),
        Arguments.of("repeated", List.of("-n", "3", "--limit", "5", "a.txt"), new CountArgs(false, 5, List.of("a.txt"))),
        Arguments.of("zero", List.of("-n", "0", "a.txt"), new CountArgs(false, 0, List.of("a.txt"))),
        Arguments.of("end-of-options", List.of("-v", "--", "-n", "--limit=4"), new CountArgs(true, 10, List.of("-n", "--limit=4"))),
        Arguments.of("only-terminator", List.of("--"), new CountArgs(false, 10, List.of())),
        Arguments.of("second-terminator", List.of("--", "--"), new CountArgs(false, 10, List.of("--"))),
        Arguments.of("dash", List.of("-"), new CountArgs(false, 10, List.of("-"))),
        Arguments.of("dash-after-option", List.of("-n", "2", "-", "b.txt"), new CountArgs(false, 2, List.of("-", "b.txt"))),
        Arguments.of("empty-arg", List.of(""), new CountArgs(false, 10, List.of(""))),
        Arguments.of("space", List.of("-v", "my file.txt"), new CountArgs(true, 10, List.of("my file.txt"))),
        Arguments.of("non-ascii", List.of("--limit=4", "日本語.txt", "café.md"), new CountArgs(false, 4, List.of("日本語.txt", "café.md"))));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void parses(String name, List<String> args, CountArgs want) throws CountArgs.UsageException {
    assertEquals(want, CountArgs.parse(args.toArray(String[]::new)), name);
  }

  static Stream<Arguments> errors() {
    return Stream.of(
        Arguments.of("unknown-short", List.of("-x", "a.txt")),
        Arguments.of("unknown-long", List.of("--color", "a.txt")),
        Arguments.of("missing-value", List.of("-n")),
        Arguments.of("missing-value-long", List.of("--limit")),
        Arguments.of("not-a-number", List.of("-n", "ten", "a.txt")),
        Arguments.of("not-a-number-long", List.of("--limit=ten")),
        Arguments.of("empty-value", List.of("--limit=", "a.txt")),
        Arguments.of("verbose-with-value", List.of("--verbose=yes")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("errors")
  void rejects(String name, List<String> args) {
    assertThrows(CountArgs.UsageException.class, () -> CountArgs.parse(args.toArray(String[]::new)), name);
  }
}
