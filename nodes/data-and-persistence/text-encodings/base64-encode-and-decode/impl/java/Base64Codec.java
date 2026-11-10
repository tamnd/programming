// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/base64-encode-and-decode/.

import java.util.Base64;

/** Encodes bytes as Base64 text and decodes the text strictly. */
public final class Base64Codec {
  /** STANDARD is RFC 4648 section 4 with padding; URL is section 5 with no padding. */
  public enum Alphabet {
    STANDARD,
    URL
  }

  private Base64Codec() {}

  public static String encode(byte[] data, Alphabet alphabet) {
    return switch (alphabet) {
      case STANDARD -> Base64.getEncoder().encodeToString(data);
      case URL -> Base64.getUrlEncoder().withoutPadding().encodeToString(data);
    };
  }

  /** Decodes text, and throws IllegalArgumentException when it is not valid. */
  public static byte[] decode(String text, Alphabet alphabet) {
    byte[] data;
    try {
      data =
          switch (alphabet) {
            case STANDARD -> Base64.getDecoder().decode(text);
            case URL -> Base64.getUrlDecoder().decode(text);
          };
    } catch (IllegalArgumentException e) {
      throw new IllegalArgumentException("invalid base64 text: " + text, e);
    }
    // The decoders accept a missing padding and non-zero pad bits. Only the
    // canonical text encodes back to itself, so this check rejects them.
    if (!encode(data, alphabet).equals(text)) {
      throw new IllegalArgumentException("invalid base64 text: " + text);
    }
    return data;
  }
}
