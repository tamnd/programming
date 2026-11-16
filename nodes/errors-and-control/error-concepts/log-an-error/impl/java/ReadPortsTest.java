// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ReadPortsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("one-bad", List.of("8080", "x", "443"), List.of(8080, 443), "level=ERROR msg=\"read port failed\" line=2 err=\"not a number: 'x'\"\nlevel=INFO msg=\"ports read\" ok=2 total=3\n"),
        Arguments.of("all-good", List.of("80", "443"), List.of(80, 443), "level=INFO msg=\"ports read\" ok=2 total=2\n"),
        Arguments.of("out-of-range", List.of("70000"), List.of(), "level=ERROR msg=\"read port failed\" line=1 err=\"out of range: 70000\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"),
        Arguments.of("empty-list", List.of(), List.of(), "level=INFO msg=\"ports read\" ok=0 total=0\n"),
        Arguments.of("empty-text", List.of(""), List.of(), "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: ''\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"),
        Arguments.of("boundary", List.of("1", "65535", "0"), List.of(1, 65535), "level=ERROR msg=\"read port failed\" line=3 err=\"out of range: 0\"\nlevel=INFO msg=\"ports read\" ok=2 total=3\n"),
        Arguments.of("newline", List.of("80\nlevel=INFO msg=\"all good\""), List.of(), "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '80\\nlevel=INFO msg=\\\"all good\\\"'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"),
        Arguments.of("backslash", List.of("C:\\port"), List.of(), "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: 'C:\\\\port'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"),
        Arguments.of("carriage-return", List.of("80\r"), List.of(), "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '80\\r'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"),
        Arguments.of("tab", List.of("8\t0"), List.of(), "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '8\\t0'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void readPorts(String name, List<String> texts, List<Integer> wantPorts, String wantLog) {
    Logger log = Logger.getAnonymousLogger();
    log.setUseParentHandlers(false);
    log.setLevel(Level.INFO);
    ReadPorts.StringHandler handler = new ReadPorts.StringHandler();
    log.addHandler(handler);
    assertEquals(wantPorts, ReadPorts.readPorts(texts, log), name);
    assertEquals(wantLog, handler.text(), name);
  }
}
