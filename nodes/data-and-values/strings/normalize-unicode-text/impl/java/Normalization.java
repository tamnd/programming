// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/normalize-unicode-text/.

import java.text.Normalizer;

final class Normalization {
  private Normalization() {}

  // normalize returns s in the form NFC or NFD. The 2 forms of the same text
  // are different strings, so equals is true only after this.
  static String normalize(String s, String form) {
    return Normalizer.normalize(s, Normalizer.Form.valueOf(form));
  }
}
