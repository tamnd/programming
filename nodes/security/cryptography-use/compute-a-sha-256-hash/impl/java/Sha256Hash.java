// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.HexFormat;

public final class Sha256Hash {
  private Sha256Hash() {}

  // Every Java platform must support SHA-256, so this cannot fail.
  private static MessageDigest sha256() {
    try {
      return MessageDigest.getInstance("SHA-256");
    } catch (NoSuchAlgorithmException e) {
      throw new AssertionError(e);
    }
  }

  /** Returns the SHA-256 of the UTF-8 bytes of text, as lowercase hex. */
  public static String sha256Text(String text) {
    byte[] digest = sha256().digest(text.getBytes(StandardCharsets.UTF_8));
    // HexFormat keeps the leading zeros: always two digits per byte.
    return HexFormat.of().formatHex(digest);
  }

  /**
   * Returns the SHA-256 of the bytes of the file at path, as lowercase hex. The file is read in
   * chunks, so a large file does not fill the memory.
   */
  public static String sha256File(Path path) throws IOException {
    MessageDigest md = sha256();
    byte[] buffer = new byte[8192];
    try (InputStream in = Files.newInputStream(path)) {
      int n;
      while ((n = in.read(buffer)) != -1) {
        md.update(buffer, 0, n);
      }
    }
    return HexFormat.of().formatHex(md.digest());
  }
}
