// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.time.Duration;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class FollowFileTest {
  @TempDir Path dir;

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("append-lines", "old\n", List.of("append:a\nb\n", "poll", "append:c\n", "poll"), List.of(List.of("a", "b"), List.of("c"))),
        Arguments.of("partial-line", "", List.of("append:ab", "poll", "append:c\nd", "poll", "append:\n", "poll"), List.of(List.of(), List.of("abc"), List.of("d"))),
        Arguments.of("rotate", "1\n", List.of("append:2\n", "poll", "rotate", "append:3\n", "poll"), List.of(List.of("2"), List.of("3"))),
        Arguments.of("truncate", "", List.of("append:a\nb\n", "poll", "truncate:", "poll", "append:c\n", "poll"), List.of(List.of("a", "b"), List.of(), List.of("c"))),
        Arguments.of("no-change", "x\n", List.of("poll", "poll"), List.of(List.of(), List.of())),
        Arguments.of("empty-line", "", List.of("append:\n\na\n", "poll"), List.of(List.of("", "", "a"))),
        Arguments.of("crlf", "", List.of("append:a\r\nb\r\n", "poll"), List.of(List.of("a\r", "b\r"))),
        Arguments.of("truncate-and-write", "", List.of("append:aaaa\n", "poll", "truncate:b\n", "poll"), List.of(List.of("aaaa"), List.of("b"))),
        Arguments.of("truncate-partial", "", List.of("append:abc", "poll", "truncate:d\n", "poll"), List.of(List.of(), List.of("abc", "d"))),
        Arguments.of("rotate-late-writes", "", List.of("append:1\n", "poll", "rotate", "append-old:2\n", "append:3\n", "poll"), List.of(List.of("1"), List.of("2", "3"))),
        Arguments.of("rotate-partial", "", List.of("append:a", "poll", "rotate", "append:b\n", "poll"), List.of(List.of(), List.of("a", "b"))),
        Arguments.of("rotate-twice", "", List.of("append:1\n", "poll", "rotate", "append:2\n", "poll", "rotate", "append:3\n", "poll"), List.of(List.of("1"), List.of("2"), List.of("3"))),
        Arguments.of("delete", "", List.of("append:1\n", "poll", "delete", "poll", "poll"), List.of(List.of("1"), List.of(), List.of())),
        Arguments.of("delete-partial", "", List.of("append:x", "poll", "delete", "poll"), List.of(List.of(), List.of("x"))),
        Arguments.of("delete-and-create", "", List.of("append:1\n", "poll", "delete", "poll", "create:2\n", "poll"), List.of(List.of("1"), List.of(), List.of("2"))),
        Arguments.of("delete-and-create-between-polls", "", List.of("append:1\n", "poll", "delete", "create:2\n", "poll"), List.of(List.of("1"), List.of("2"))),
        Arguments.of("start-mid-line", "abc", List.of("append:d\ne\n", "poll"), List.of(List.of("d", "e"))),
        Arguments.of("non-ascii", "", List.of("append:caf\u00e9\nna\u00efve\n", "poll"), List.of(List.of("caf\u00e9", "na\u00efve"))));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void followFile(String name, String initial, List<String> steps, List<List<String>> want)
      throws IOException {
    assertEquals(want, FollowFile.followFile(dir.resolve("followed.txt"), initial, steps));
  }

  @Test
  void notFound() {
    assertThrows(NoSuchFileException.class,
        () -> FollowFile.followFile(dir.resolve("missing/followed.txt"), "", List.of("poll")));
    assertThrows(NoSuchFileException.class, () -> new FollowFile.Follower(dir.resolve("missing.txt")));
  }

  @Test
  void manyLines() throws IOException {
    StringBuilder text = new StringBuilder("append:");
    for (int i = 0; i < 100000; i++) {
      text.append(i).append('\n');
    }
    List<List<String>> polls =
        FollowFile.followFile(dir.resolve("followed.txt"), "", List.of(text.toString(), "poll"));
    assertEquals(100000, polls.get(0).size());
    assertEquals("0", polls.get(0).get(0));
    assertEquals("99999", polls.get(0).get(99999));
  }

  @Test
  void characterAcrossPolls() throws IOException {
    Path path = dir.resolve("followed.txt");
    Files.writeString(path, "");
    try (FollowFile.Follower follower = new FollowFile.Follower(path)) {
      Files.write(path, new byte[] {'c', 'a', 'f', (byte) 0xc3}, StandardOpenOption.APPEND);
      assertEquals(List.of(), follower.poll());
      Files.write(path, new byte[] {(byte) 0xa9, '\n'}, StandardOpenOption.APPEND);
      assertEquals(List.of("caf\u00e9"), follower.poll());
    }
  }

  @Test
  void follow() throws IOException, InterruptedException {
    Path path = dir.resolve("followed.txt");
    Files.writeString(path, "old\n");
    List<String> lines = new ArrayList<>();
    int[] polls = {0};
    try (FollowFile.Follower follower = new FollowFile.Follower(path)) {
      Files.writeString(path, "a\nb\n", StandardCharsets.UTF_8, StandardOpenOption.APPEND);
      follower.follow(Duration.ofMillis(10), () -> lines.size() >= 2 || ++polls[0] > 500, lines::add);
    }
    assertEquals(List.of("a", "b"), lines);
  }
}
