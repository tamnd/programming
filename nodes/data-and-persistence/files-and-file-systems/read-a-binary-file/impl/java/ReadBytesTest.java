// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.EOFException;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.util.HexFormat;
import java.util.Map;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ReadBytesTest {
  @TempDir Path dir;

  private static byte[] hex(String s) {
    return HexFormat.of().parseHex(s);
  }

  private static byte[] allBytes() {
    byte[] b = new byte[256];
    for (int i = 0; i < 256; i++) b[i] = (byte) i;
    return b;
  }

  private static byte[] large() {
    byte[] b = new byte[1 << 20];
    for (int i = 0; i < b.length; i++) b[i] = (byte) i;
    return b;
  }

  private static final Map<String, byte[]> DATA =
      Map.of(
          "signature", hex("89504e470d0a1a0a0000000d49484452"),
          "bytes", hex("0001027f80feff"),
          "empty", new byte[0],
          "all-bytes", allBytes(),
          "zeros", new byte[8],
          "crlf", "a\r\nb\r\n".getBytes(StandardCharsets.US_ASCII),
          "ctrl-z", hex("611a62"),
          "not-utf8", hex("c328fffe80"),
          "large", large());

  private Path write(String name) throws IOException {
    return Files.write(dir.resolve("input.bin"), DATA.get(name));
  }

  static Stream<String> cases() {
    return DATA.keySet().stream().sorted();
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void readsBytes(String name) throws IOException {
    assertArrayEquals(DATA.get(name), ReadBytes.readBytes(write(name)));
  }

  static Stream<Arguments> exact() {
    return Stream.of(
        Arguments.of("signature-only", "signature", 8, hex("89504e470d0a1a0a")),
        Arguments.of("first-four", "all-bytes", 4, hex("00010203")),
        Arguments.of("whole-file", "zeros", 8, new byte[8]),
        Arguments.of("zero-count", "empty", 0, new byte[0]),
        Arguments.of("across-ctrl-z", "ctrl-z", 3, hex("611a62")),
        Arguments.of("large", "large", 1 << 20, large()));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("exact")
  void readsExact(String name, String data, int n, byte[] want) throws IOException {
    assertArrayEquals(want, ReadBytes.readExact(write(data), n));
  }

  static Stream<Arguments> shortFiles() {
    return Stream.of(Arguments.of("one-past-end", "signature", 17), Arguments.of("empty-file", "empty", 1));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("shortFiles")
  void shortFile(String name, String data, int n) throws IOException {
    Path path = write(data);
    assertThrows(EOFException.class, () -> ReadBytes.readExact(path, n));
  }

  @Test
  void missingFile() {
    Path path = dir.resolve("missing.bin");
    assertThrows(NoSuchFileException.class, () -> ReadBytes.readBytes(path));
    assertThrows(NoSuchFileException.class, () -> ReadBytes.readExact(path, 1));
  }

  @Test
  void directory() {
    assertThrows(IOException.class, () -> ReadBytes.readBytes(dir));
    assertThrows(IOException.class, () -> ReadBytes.readExact(dir, 1));
  }
}
