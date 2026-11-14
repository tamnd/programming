// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.nio.file.attribute.PosixFileAttributeView;
import java.util.List;

/** Replaces a text with another text in each file of a list. */
public final class ReplaceInFiles {
  private ReplaceInFiles() {}

  /**
   * Replaces each instance of oldText with newText in each file, and returns the number of
   * instances in each file. oldText and newText are plain text, not patterns. The code compares
   * the UTF-8 bytes of oldText with the bytes of each file, so an instance can contain LF bytes.
   * Instances do not overlap. A file with no instance is not written. A changed file goes to a new
   * file in the same directory, and an atomic move puts it in the place of the old one. The files
   * are done in order, and the first error stops the method: the files before have the new text,
   * and the files after do not change.
   *
   * @throws IllegalArgumentException if oldText is empty
   * @throws java.nio.file.NoSuchFileException if a file does not exist
   */
  public static int[] replace(List<Path> paths, String oldText, String newText) throws IOException {
    if (oldText.isEmpty()) {
      throw new IllegalArgumentException("oldText is empty");
    }
    byte[] oldBytes = oldText.getBytes(StandardCharsets.UTF_8);
    byte[] newBytes = newText.getBytes(StandardCharsets.UTF_8);
    int[] counts = new int[paths.size()];
    for (int i = 0; i < counts.length; i++) {
      Path path = paths.get(i);
      byte[] data = Files.readAllBytes(path);
      ByteArrayOutputStream out = new ByteArrayOutputStream(data.length);
      int from = 0;
      // The search goes on after the end of each instance.
      for (int at; (at = indexOf(data, oldBytes, from)) != -1; from = at + oldBytes.length) {
        out.write(data, from, at - from);
        out.write(newBytes, 0, newBytes.length);
        counts[i]++;
      }
      if (counts[i] > 0) {
        out.write(data, from, data.length - from);
        writeAtomically(path, out.toByteArray());
      }
    }
    return counts;
  }

  /** Returns the index of the first instance of needle in data at or after from, or -1. */
  private static int indexOf(byte[] data, byte[] needle, int from) {
    outer:
    for (int i = from; i <= data.length - needle.length; i++) {
      for (int j = 0; j < needle.length; j++) {
        if (data[i + j] != needle[j]) {
          continue outer;
        }
      }
      return i;
    }
    return -1;
  }

  /** Writes data to a new file next to path, and moves it over path. */
  private static void writeAtomically(Path path, byte[] data) throws IOException {
    Path dir = path.toAbsolutePath().getParent();
    // createTempFile makes a file with a name that no file has.
    Path tmp = Files.createTempFile(dir, "." + path.getFileName() + ".", ".tmp");
    try {
      try (FileOutputStream out = new FileOutputStream(tmp.toFile())) {
        out.write(data);
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
  }
}
