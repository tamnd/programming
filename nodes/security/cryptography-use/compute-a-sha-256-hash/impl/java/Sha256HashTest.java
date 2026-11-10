// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class Sha256HashTest {
  @TempDir Path dir;

  static Stream<Arguments> textCases() {
    return Stream.of(
        Arguments.of(
            "abc", "abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"),
        Arguments.of(
            "two-block-56",
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"),
        Arguments.of(
            "fox",
            "The quick brown fox jumps over the lazy dog",
            "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592"),
        Arguments.of(
            "empty", "", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"),
        Arguments.of(
            "a-55",
            "a".repeat(55),
            "9f4390f8d30c2dd92ec9f095b65e2b9ae9b0a925a5258e241c9f1e910f734318"),
        Arguments.of(
            "a-56",
            "a".repeat(56),
            "b35439a4ac6f0948b6d6f9e3c6af0f5f590ce20f1bde7090ef7970686ec6738a"),
        Arguments.of(
            "a-63",
            "a".repeat(63),
            "7d3e74a05d7db15bce4ad9ec0658ea98e3f06eeecf16b4c6fff2da457ddc2f34"),
        Arguments.of(
            "a-64",
            "a".repeat(64),
            "ffe054fe7ae0cb6dc65c3af9b61d5209f439851db43d0ba5997337df154668eb"),
        Arguments.of(
            "multi-block-112",
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
                + "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1"),
        Arguments.of(
            "non-ascii",
            "naïve café 日本語 🙂",
            "9c53e090acd67abd65c64ee431a36bdc36c3fd86341f5814a94d1b901ca9ed8d"),
        Arguments.of(
            "nfc-cafe",
            "caf\u00e9",
            "850f7dc43910ff890f8879c0ed26fe697c93a067ad93a7d50f466a7028a9bf4e"),
        Arguments.of(
            "nfd-cafe",
            "cafe\u0301",
            "81ef060bcd98adc7824eb5c1ada83c32491b16018e11e79f00ab9d09e04b015a"),
        Arguments.of(
            "trailing-newline",
            "abc\n",
            "edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb"));
  }

  static Stream<Arguments> fileCases() {
    byte[] millionA = new byte[1_000_000];
    Arrays.fill(millionA, (byte) 'a');
    return Stream.of(
        Arguments.of(
            "abc-file",
            "abc".getBytes(StandardCharsets.US_ASCII),
            "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"),
        Arguments.of(
            "abc-newline-file",
            "abc\n".getBytes(StandardCharsets.US_ASCII),
            "edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb"),
        Arguments.of(
            "abc-crlf-file",
            "abc\r\n".getBytes(StandardCharsets.US_ASCII),
            "552bab6864c7a7b69a502ed1854b9245c0e1a30f008aaa0b281da62585fdb025"),
        Arguments.of(
            "empty-file",
            new byte[0],
            "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"),
        Arguments.of(
            "binary-file",
            new byte[] {0x00, 0x01, (byte) 0xff},
            "26a66b061e8f48f39927c312f25293959729eee95978e2892d49d3512a5cc092"),
        // One million repetitions of "a": more than one chunk.
        Arguments.of(
            "million-a-file",
            millionA,
            "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("textCases")
  void hashesText(String name, String text, String want) {
    assertEquals(want, Sha256Hash.sha256Text(text), name);
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("fileCases")
  void hashesFile(String name, byte[] data, String want) throws IOException {
    Path path = dir.resolve("in.bin");
    Files.write(path, data);
    assertEquals(want, Sha256Hash.sha256File(path), name);
  }

  @Test
  void missingFile() {
    assertThrows(NoSuchFileException.class, () -> Sha256Hash.sha256File(dir.resolve("missing.txt")));
  }
}
