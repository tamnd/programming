// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotSame;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class PlaylistTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of(
            "middle",
            List.of("Intro", "Song", "Outro"),
            1,
            "Hit",
            List.of(List.of("Intro", "Hit", "Outro"), List.of("Intro", "Hit", "Outro"), List.of("Intro", "Song", "Outro"))),
        Arguments.of("two", List.of("A", "B"), 0, "C", List.of(List.of("C", "B"), List.of("C", "B"), List.of("A", "B"))),
        Arguments.of("single", List.of("Solo"), 0, "Duet", List.of(List.of("Duet"), List.of("Duet"), List.of("Solo"))),
        Arguments.of(
            "first",
            List.of("a", "b", "c"),
            0,
            "z",
            List.of(List.of("z", "b", "c"), List.of("z", "b", "c"), List.of("a", "b", "c"))),
        Arguments.of(
            "last",
            List.of("a", "b", "c"),
            2,
            "z",
            List.of(List.of("a", "b", "z"), List.of("a", "b", "z"), List.of("a", "b", "c"))),
        Arguments.of(
            "same-title", List.of("a", "b"), 1, "b", List.of(List.of("a", "b"), List.of("a", "b"), List.of("a", "b"))),
        Arguments.of(
            "duplicates",
            List.of("x", "x", "x"),
            1,
            "y",
            List.of(List.of("x", "y", "x"), List.of("x", "y", "x"), List.of("x", "x", "x"))));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void copyAndChange(String name, List<String> songs, int index, String title, List<List<String>> want) {
    assertEquals(want, Playlist.copyAndChange(songs, index, title), name);
  }

  @Test
  void copiesAreNewObjects() {
    Playlist original = new Playlist("mix", new java.util.ArrayList<>(List.of("a")));
    Playlist shallow = original.clone();
    Playlist deep = new Playlist(original);
    assertNotSame(original, shallow);
    assertSame(original.songs(), shallow.songs());
    assertNotSame(original.songs(), deep.songs());
    shallow.setName("other");
    assertEquals("mix", original.name());
  }

  @Test
  void badIndex() {
    assertThrows(IndexOutOfBoundsException.class, () -> Playlist.copyAndChange(List.of("a"), 1, "b"));
  }
}
