// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Path;

public final class CopyBuffered {
  private CopyBuffered() {}

  /**
   * Copies src to dst through buffers of size bytes and returns the number of bytes copied.
   *
   * <p>The try statement closes the output, and the close flushes the buffer. An error from that
   * last write comes out of the try statement as an IOException.
   */
  public static long copy(Path src, Path dst, int size) throws IOException {
    if (size < 1) {
      throw new IllegalArgumentException("size must be at least 1, not " + size);
    }
    long total = 0;
    try (InputStream in = new BufferedInputStream(Files.newInputStream(src), size);
        OutputStream out = new BufferedOutputStream(Files.newOutputStream(dst), size)) {
      byte[] chunk = new byte[size];
      int n;
      while ((n = in.read(chunk)) != -1) {
        out.write(chunk, 0, n);
        total += n;
      }
    }
    return total;
  }
}
