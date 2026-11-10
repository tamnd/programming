// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

import static java.nio.file.StandardOpenOption.APPEND;
import static java.nio.file.StandardOpenOption.CREATE;
import static java.nio.file.StandardOpenOption.WRITE;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

public final class WriteText {
  private WriteText() {}

  /** The modes of {@link #writeText}. */
  public enum Mode {
    TRUNCATE,
    APPEND,
    ATOMIC
  }

  /** Writes text to path as UTF-8, with no change to the line ends. */
  public static void writeText(Path path, String text, Mode mode) throws IOException {
    switch (mode) {
      // With no options, writeString creates or truncates the file.
      case TRUNCATE -> Files.writeString(path, text, StandardCharsets.UTF_8);
      case APPEND -> Files.writeString(path, text, StandardCharsets.UTF_8, CREATE, APPEND);
      case ATOMIC -> replace(path, text);
    }
  }

  private static void replace(Path path, String text) throws IOException {
    // The temporary file is in the same directory, so the move does not
    // cross a file system.
    Path tmp = Files.createTempFile(path.toAbsolutePath().getParent(), ".tmp-", ".part");
    try {
      try (FileChannel channel = FileChannel.open(tmp, WRITE)) {
        ByteBuffer bytes = ByteBuffer.wrap(text.getBytes(StandardCharsets.UTF_8));
        while (bytes.hasRemaining()) {
          channel.write(bytes);
        }
        channel.force(true);
      }
      Files.move(tmp, path, StandardCopyOption.ATOMIC_MOVE);
    } catch (IOException | RuntimeException e) {
      Files.deleteIfExists(tmp);
      throw e;
    }
  }
}
