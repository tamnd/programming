// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.FileSystemException;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

class TruncateFileTest {
  @TempDir Path dir;

  private static byte[] bytes(String s) {
    return s.replace("EMPTY", "").replace("\\n", "\n").replace("\\0", "\0").getBytes(StandardCharsets.UTF_8);
  }

  @ParameterizedTest(name = "{0}")
  @CsvSource(
      delimiter = '|',
      value = {
        "shrink | a.txt | 5 | alpha beta\\n | alpha",
        "extend | a.txt | 8 | alpha | alpha\\0\\0\\0",
        "same-length | a.txt | 6 | alpha\\n | alpha\\n",
        "to-zero | a.txt | 0 | alpha\\n | EMPTY",
        "empty-file | a.txt | 3 | EMPTY | \\0\\0\\0",
        "name-with-spaces | my notes.txt | 2 | alpha\\n | al",
      })
  void cases(String name, String rel, long length, String before, String after) throws IOException {
    Path path = Files.write(dir.resolve(rel), bytes(before));
    TruncateFile.truncate(path, length);
    assertArrayEquals(bytes(after), Files.readAllBytes(path), name);
  }

  @Test
  void errors() throws IOException {
    assertThrows(NoSuchFileException.class, () -> TruncateFile.truncate(dir.resolve("a.txt"), 0));
    assertFalse(Files.exists(dir.resolve("a.txt")));
    Path sub = Files.createDirectory(dir.resolve("dir"));
    assertThrows(FileSystemException.class, () -> TruncateFile.truncate(sub, 0));
    Path path = Files.writeString(dir.resolve("a.txt"), "alpha\n");
    assertThrows(IllegalArgumentException.class, () -> TruncateFile.truncate(path, -1));
    assertEquals("alpha\n", Files.readString(path));
  }

  @Test
  void openInAppendMode() throws IOException {
    Path path = Files.writeString(dir.resolve("app.log"), "alpha beta\n");
    try (OutputStream log = Files.newOutputStream(path, StandardOpenOption.APPEND)) {
      TruncateFile.truncate(path, 0);
      log.write("new\n".getBytes(StandardCharsets.UTF_8));
    }
    assertEquals("new\n", Files.readString(path));
  }

  @Test
  void openAfterTheNewEnd() throws IOException {
    Path path = Files.writeString(dir.resolve("data.bin"), "alpha beta\n");
    try (FileChannel f = FileChannel.open(path, StandardOpenOption.WRITE)) {
      f.position(f.size());
      TruncateFile.truncate(path, 5);
      f.write(ByteBuffer.wrap(new byte[] {'X'}));
    }
    assertArrayEquals(bytes("alpha\\0\\0\\0\\0\\0\\0X"), Files.readAllBytes(path));
  }
}
