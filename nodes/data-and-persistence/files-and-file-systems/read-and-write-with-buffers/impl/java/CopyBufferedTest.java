// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.condition.EnabledIf;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;
import org.junit.jupiter.params.provider.ValueSource;

class CopyBufferedTest {
  @TempDir Path dir;

  private static final byte[] TEXT = "alpha\nbeta\ngamma\n".getBytes(StandardCharsets.US_ASCII);

  private static byte[] pattern(int n) {
    byte[] b = new byte[n];
    for (int i = 0; i < n; i++) {
      b[i] = (byte) i;
    }
    return b;
  }

  private static byte[] ascii(String s) {
    return s.getBytes(StandardCharsets.US_ASCII);
  }

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("text", TEXT, 4096),
        Arguments.of("several-buffers", TEXT, 4),
        Arguments.of("empty", new byte[0], 8),
        Arguments.of("smaller-than-buffer", ascii("12345"), 8),
        Arguments.of("exact-buffer", ascii("12345678"), 8),
        Arguments.of("one-past-buffer", ascii("123456789"), 8),
        Arguments.of("size-one", TEXT, 1),
        Arguments.of("binary", pattern(256), 64),
        Arguments.of("large", pattern(1 << 20), 4096),
        Arguments.of("large-odd-buffer", pattern(1 << 20), 4093));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void copies(String name, byte[] data, int size) throws IOException {
    Path src = Files.write(dir.resolve("in.bin"), data);
    Path dst = dir.resolve("out.bin");
    assertEquals(data.length, CopyBuffered.copy(src, dst, size));
    assertArrayEquals(data, Files.readAllBytes(dst));
  }

  @Test
  void replacesALongerFile() throws IOException {
    Path src = Files.write(dir.resolve("in.bin"), ascii("new"));
    Path dst = Files.write(dir.resolve("out.bin"), ascii("old and longer"));
    assertEquals(3, CopyBuffered.copy(src, dst, 8));
    assertArrayEquals(ascii("new"), Files.readAllBytes(dst));
  }

  @Test
  void missingSrc() {
    assertThrows(
        NoSuchFileException.class,
        () -> CopyBuffered.copy(dir.resolve("missing.bin"), dir.resolve("out.bin"), 8));
    assertFalse(Files.exists(dir.resolve("out.bin")));
  }

  @Test
  void missingDstDir() throws IOException {
    Path src = Files.write(dir.resolve("in.bin"), TEXT);
    assertThrows(
        NoSuchFileException.class,
        () -> CopyBuffered.copy(src, dir.resolve("no-such-dir").resolve("out.bin"), 8));
  }

  @ParameterizedTest
  @ValueSource(ints = {0, -1})
  void invalidSize(int size) throws IOException {
    Path src = Files.write(dir.resolve("in.bin"), TEXT);
    assertThrows(
        IllegalArgumentException.class, () -> CopyBuffered.copy(src, dir.resolve("out.bin"), size));
  }

  static boolean hasFullDevice() {
    return Files.exists(Path.of("/dev/full"));
  }

  @ParameterizedTest
  @ValueSource(ints = {17, 1 << 20})
  @EnabledIf("hasFullDevice")
  void fullDevice(int n) throws IOException {
    // 17 bytes stay in the buffer, so the error comes only at the close.
    Path src = Files.write(dir.resolve("in.bin"), pattern(n));
    IOException err =
        assertThrows(IOException.class, () -> CopyBuffered.copy(src, Path.of("/dev/full"), 4096));
    assertTrue(String.valueOf(err.getMessage()).contains("No space left"), err.getMessage());
  }
}
