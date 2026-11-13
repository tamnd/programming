// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;

public final class TruncateFile {
  private TruncateFile() {}

  /**
   * Sets the size of the existing file at path to length bytes.
   *
   * <p>The channel is opened with WRITE and no CREATE, so a missing file gives
   * NoSuchFileException. FileChannel.truncate only makes a file shorter: for a greater size, the
   * file is not modified. So to extend the file, the method writes one zero byte at length - 1, and
   * the bytes before it read as zero. RandomAccessFile.setLength extends too, but its mode "rw"
   * makes a missing file.
   */
  public static void truncate(Path path, long length) throws IOException {
    if (length < 0) {
      throw new IllegalArgumentException("length must be 0 or more: " + length);
    }
    try (FileChannel ch = FileChannel.open(path, StandardOpenOption.WRITE)) {
      if (length < ch.size()) {
        ch.truncate(length);
      } else if (length > ch.size()) {
        ch.write(ByteBuffer.wrap(new byte[1]), length - 1);
      }
    }
  }
}
