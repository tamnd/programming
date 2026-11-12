// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;

/** Adds lines to the end of a file. */
public final class AppendLine {
  private AppendLine() {}

  /** Adds line and one "\n" to the end of the file, in one write call. */
  public static void appendLine(Path path, String line) throws IOException {
    if (line.indexOf('\n') >= 0 || line.indexOf('\r') >= 0) {
      throw new IllegalArgumentException("the line must not hold \\n or \\r");
    }
    ByteBuffer data = ByteBuffer.wrap((line + "\n").getBytes(StandardCharsets.UTF_8));
    // APPEND moves the position to the end of the file before each write.
    try (FileChannel channel =
        FileChannel.open(
            path, StandardOpenOption.CREATE, StandardOpenOption.WRITE, StandardOpenOption.APPEND)) {
      channel.write(data);
    }
    if (data.hasRemaining()) {
      throw new IOException("short write: " + data.remaining() + " bytes not written");
    }
  }
}
