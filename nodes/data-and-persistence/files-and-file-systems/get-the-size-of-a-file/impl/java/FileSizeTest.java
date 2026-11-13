// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.io.OutputStream;
import java.io.RandomAccessFile;
import java.nio.charset.StandardCharsets;
import java.nio.file.FileSystemException;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

class FileSizeTest {
  @TempDir Path dir;

  private static byte[] bytes(String s) {
    return s.replace("EMPTY", "").replace("\\n", "\n").replace("\\r", "\r").getBytes(StandardCharsets.UTF_8);
  }

  @ParameterizedTest(name = "{0}")
  @CsvSource({
    "text, a.txt, alpha\\n, 6",
    "non-ascii-text, a.txt, café\\n, 6",
    "empty-file, a.txt, EMPTY, 0",
    "crlf-line-ends, a.txt, a\\r\\nb\\r\\n, 6",
    "name-with-spaces, my notes.txt, alpha\\n, 6",
    "non-ascii-name, café 日本.txt, alpha\\n, 6",
  })
  void cases(String name, String rel, String data, long size) throws IOException {
    Path path = dir.resolve(rel);
    Files.write(path, bytes(data));
    assertEquals(size, FileSize.size(path), name);
  }

  @Test
  void binaryData() throws IOException {
    Path path = dir.resolve("a.bin");
    Files.write(path, new byte[] {0x00, 0x0d, 0x0a, (byte) 0xc3, (byte) 0xbf, 0x00});
    assertEquals(6, FileSize.size(path));
  }

  @Test
  void errors() throws IOException {
    assertThrows(NoSuchFileException.class, () -> FileSize.size(dir.resolve("a.txt")));
    Files.createDirectory(dir.resolve("dir"));
    assertThrows(FileSystemException.class, () -> FileSize.size(dir.resolve("dir")));
  }

  @Test
  void symbolicLink() throws IOException {
    Files.writeString(dir.resolve("a.txt"), "alpha beta\n");
    Files.createSymbolicLink(dir.resolve("link"), Path.of("a.txt"));
    assertEquals(11, FileSize.size(dir.resolve("link")));
  }

  @Test
  void sparseFileOf5GiB() throws IOException {
    Path path = dir.resolve("sparse.bin");
    try (RandomAccessFile f = new RandomAccessFile(path.toFile(), "rw")) {
      f.setLength(5L * 1024 * 1024 * 1024);
    }
    assertEquals(5368709120L, FileSize.size(path));
  }

  @Test
  void openFileAfterFlush() throws IOException {
    Path path = dir.resolve("app.log");
    try (OutputStream out = Files.newOutputStream(path)) {
      out.write(bytes("alpha\\n"));
      out.flush();
      assertEquals(6, FileSize.size(path));
    }
  }
}
