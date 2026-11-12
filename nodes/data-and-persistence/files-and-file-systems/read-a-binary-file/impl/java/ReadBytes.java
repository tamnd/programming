// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

import java.io.EOFException;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;

public final class ReadBytes {
  private ReadBytes() {}

  /** Returns all of the bytes of a file, with no decoding. */
  public static byte[] readBytes(Path path) throws IOException {
    return Files.readAllBytes(path);
  }

  /** Returns the first n bytes of a file, or throws EOFException if it is shorter. */
  public static byte[] readExact(Path path, int n) throws IOException {
    try (InputStream in = Files.newInputStream(path)) {
      // readNBytes reads again until it has n bytes or the stream ends.
      // read(byte[]) can stop after fewer bytes, so it is not enough.
      byte[] data = in.readNBytes(n);
      if (data.length < n) {
        throw new EOFException(path + ": " + data.length + " of " + n + " bytes before the end of the file");
      }
      return data;
    }
  }
}
