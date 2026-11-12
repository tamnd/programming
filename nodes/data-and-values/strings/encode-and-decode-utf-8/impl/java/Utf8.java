// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

import java.nio.ByteBuffer;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CodingErrorAction;
import java.nio.charset.StandardCharsets;

final class Utf8 {
  private Utf8() {}

  // encodeUtf8 returns the UTF-8 bytes of the code point cp, from 0 to 255.
  static int[] encodeUtf8(int cp) {
    byte[] b = Character.toString(cp).getBytes(StandardCharsets.UTF_8);
    int[] out = new int[b.length];
    for (int i = 0; i < b.length; i++) {
      out[i] = b[i] & 0xff;
    }
    return out;
  }

  // decodeUtf8 returns the code point of the one UTF-8 sequence in b, or -1.
  // new String(bytes, UTF_8) would replace bad input with U+FFFD, so the
  // decoder reports each error instead.
  static int decodeUtf8(int[] b) {
    byte[] raw = new byte[b.length];
    for (int i = 0; i < b.length; i++) {
      raw[i] = (byte) b[i];
    }
    CharsetDecoder decoder =
        StandardCharsets.UTF_8
            .newDecoder()
            .onMalformedInput(CodingErrorAction.REPORT)
            .onUnmappableCharacter(CodingErrorAction.REPORT);
    String s;
    try {
      s = decoder.decode(ByteBuffer.wrap(raw)).toString();
    } catch (CharacterCodingException e) {
      return -1;
    }
    return s.codePointCount(0, s.length()) == 1 ? s.codePointAt(0) : -1;
  }
}
