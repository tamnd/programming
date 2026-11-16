// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-an-error-with-context/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertInstanceOf;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CheckSettingTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("valid", "app.conf:3", "8080", "port 8080"),
        Arguments.of("not-a-number", "app.conf:3", "x", "error: app.conf:3: read port: not a number: 'x'"),
        Arguments.of("too-large", "app.conf:7", "70000", "error: app.conf:7: read port: out of range: 70000"),
        Arguments.of("empty", "app.conf:1", "", "error: app.conf:1: read port: not a number: ''"),
        Arguments.of("zero", "app.conf:2", "0", "error: app.conf:2: read port: out of range: 0"),
        Arguments.of("largest", "app.conf:2", "65535", "port 65535"),
        Arguments.of("leading-space", "app.conf:4", " 80", "error: app.conf:4: read port: not a number: ' 80'"),
        Arguments.of("overflow", "app.conf:5", "99999999999999999999", "error: app.conf:5: read port: out of range: 99999999999999999999"),
        Arguments.of("negative", "app.conf:6", "-1", "error: app.conf:6: read port: not a number: '-1'"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void checkSetting(String name, String where, String text, String want) {
    assertEquals(want, CheckSetting.checkSetting(where, text), name);
  }

  @Test
  void cause() {
    CheckSetting.ConfigException e =
        assertThrows(CheckSetting.ConfigException.class, () -> CheckSetting.readPort("app.conf:3", "x"));
    assertInstanceOf(IllegalArgumentException.class, e.getCause());
  }
}
