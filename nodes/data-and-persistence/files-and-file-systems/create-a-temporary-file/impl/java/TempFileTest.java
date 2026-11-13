// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.LinkOption;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.attribute.PosixFilePermissions;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;
import org.junit.jupiter.params.provider.ValueSource;

class TempFileTest {
  @TempDir Path dir;

  @BeforeEach
  void setUp() throws IOException {
    Files.createDirectory(dir.resolve("sub"));
  }

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("one-line", ".", "tmp-", "alpha\n"),
        Arguments.of("many-lines", ".", "report-", "alpha\nbeta\ngamma\n"),
        Arguments.of("in-subdirectory", "sub", "tmp-", "alpha\n"),
        Arguments.of("empty-prefix", ".", "", "alpha\n"),
        Arguments.of("empty-data", ".", "tmp-", ""),
        Arguments.of("non-ascii-data", ".", "tmp-", "café 日本\n"),
        Arguments.of("prefix-with-spaces", ".", "my temp ", "alpha\n"),
        Arguments.of("non-ascii-prefix", ".", "café-", "alpha\n"));
  }

  private List<String> names(Path d) throws IOException {
    try (Stream<Path> s = Files.list(d)) {
      return s.map(p -> p.getFileName().toString()).sorted().toList();
    }
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void cases(String name, String rel, String prefix, String data) throws IOException {
    assertEquals(data, TempFile.useTempFile(dir.resolve(rel), prefix, data), name);
    assertEquals(List.of("sub"), names(dir));
    assertEquals(List.of(), names(dir.resolve("sub")));
  }

  @ParameterizedTest
  @ValueSource(strings = {"tmp-", "", "my temp ", "café-"})
  void makeTempFile(String prefix) throws IOException {
    Path path = TempFile.makeTempFile(dir, prefix);
    assertEquals(dir, path.getParent());
    String name = path.getFileName().toString();
    assertTrue(name.startsWith(prefix), name);
    assertTrue(name.length() >= prefix.length() + 6, name);
    assertTrue(Files.isRegularFile(path, LinkOption.NOFOLLOW_LINKS));
    assertEquals("rw-------", PosixFilePermissions.toString(Files.getPosixFilePermissions(path)));
    assertEquals(0, Files.size(path));
  }

  @Test
  void twoCallsGiveTwoFiles() throws IOException {
    Path a = TempFile.makeTempFile(dir, "tmp-");
    Path b = TempFile.makeTempFile(dir, "tmp-");
    assertNotEquals(a, b);
    assertEquals(3, names(dir).size());
  }

  @Test
  void missingDirectory() {
    assertThrows(NoSuchFileException.class, () -> TempFile.useTempFile(dir.resolve("nodir"), "tmp-", "alpha\n"));
  }

  @Test
  void prefixWithSlash() throws IOException {
    assertThrows(IllegalArgumentException.class, () -> TempFile.useTempFile(dir, "sub/tmp-", "alpha\n"));
    assertEquals(List.of(), names(dir.resolve("sub")));
  }
}
