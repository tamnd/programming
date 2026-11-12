// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

import java.nio.charset.StandardCharsets;

final class ToBytes {
  private ToBytes() {}

  // toBytes returns the UTF-8 bytes of s. The charset is given each time, so
  // the result does not depend on the default charset.
  static byte[] toBytes(String s) {
    return s.getBytes(StandardCharsets.UTF_8);
  }
}
