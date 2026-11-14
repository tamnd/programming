// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.nio.file.attribute.PosixFileAttributeView;

/** Removes lines from a file and writes the rest back under the same name. */
public final class RemoveLines {
  private RemoveLines() {}

  /**
   * Removes count lines from the file at path, from line start, and returns how many it removed.
   * Line 1 is the first line. A line ends at LF, and the LF goes with the line. The result is less
   * than count if the file ends first. If no line goes, the file is not written. Otherwise the rest
   * of the file goes to a new file in the same directory, and an atomic move puts it in the place
   * of the old one.
   *
   * @throws IllegalArgumentException if start is less than 1 or count is less than 0
   * @throws java.nio.file.NoSuchFileException if the file does not exist
   */
  public static int remove(Path path, int start, int count) throws IOException {
    if (start < 1 || count < 0) {
      throw new IllegalArgumentException("bad range: start " + start + ", count " + count);
    }
    // Bytes, not lines of text: the bytes that stay do not change.
    byte[] data = Files.readAllBytes(path);
    int offset = 0;
    for (int n = 1; n < start && offset < data.length; n++) {
      offset = nextLine(data, offset);
    }
    int from = offset;
    int removed = 0;
    while (removed < count && offset < data.length) {
      offset = nextLine(data, offset);
      removed++;
    }
    if (removed == 0) {
      return 0;
    }
    Path dir = path.toAbsolutePath().getParent();
    // createTempFile makes a file with a name that no file has.
    Path tmp = Files.createTempFile(dir, "." + path.getFileName() + ".", ".tmp");
    try {
      try (FileOutputStream out = new FileOutputStream(tmp.toFile())) {
        out.write(data, 0, from);
        out.write(data, offset, data.length - offset);
        out.getFD().sync();
      }
      if (Files.getFileAttributeView(path, PosixFileAttributeView.class) != null) {
        Files.setPosixFilePermissions(tmp, Files.getPosixFilePermissions(path));
      }
      Files.move(tmp, path, StandardCopyOption.ATOMIC_MOVE);
    } catch (IOException | RuntimeException e) {
      Files.deleteIfExists(tmp);
      throw e;
    }
    return removed;
  }

  /** Returns the offset just after the line that starts at offset. */
  private static int nextLine(byte[] data, int offset) {
    while (offset < data.length && data[offset] != '\n') {
      offset++;
    }
    return offset < data.length ? offset + 1 : offset;
  }
}
