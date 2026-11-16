// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CheckPortTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("valid", "8080", "port 8080"),
        Arguments.of("not-a-number", "x80", "invalid port 'x80': not a number"),
        Arguments.of("too-large", "65536", "invalid port '65536': out of range"),
        Arguments.of("empty", "", "invalid port '': empty"),
        Arguments.of("zero", "0", "invalid port '0': out of range"),
        Arguments.of("one", "1", "port 1"),
        Arguments.of("largest", "65535", "port 65535"),
        Arguments.of("leading-zeros", "0080", "port 80"),
        Arguments.of("negative", "-1", "invalid port '-1': not a number"),
        Arguments.of("leading-space", " 80", "invalid port ' 80': not a number"),
        Arguments.of("overflow", "99999999999999999999", "invalid port '99999999999999999999': out of range"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void checkPort(String name, String text, String want) {
    assertEquals(want, CheckPort.checkPort(text), name);
  }

  @Test
  void fields() {
    CheckPort.PortException e =
        assertThrows(CheckPort.PortException.class, () -> CheckPort.parsePort("x80"));
    assertEquals("x80", e.text());
    assertEquals("not a number", e.reason());
  }
}
