// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.IOException;
import java.lang.ProcessBuilder.Redirect;
import java.net.URISyntaxException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.util.Map;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

// System.getenv() returns an unmodifiable map. So each case starts a child
// JVM with the environment of the case, and the child calls getEnv.
class GetEnvTest {
  /** The child: prints getEnv(args[0], args[1]) as UTF-8, with no line end. */
  public static void main(String[] args) throws IOException {
    System.out.write(GetEnv.getEnv(args[0], args[1]).getBytes(StandardCharsets.UTF_8));
    System.out.flush();
  }

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("present", Map.of("APP_PORT", "8080"), "APP_PORT", "80", "8080"),
        Arguments.of("path-value", Map.of("APP_HOME", "/srv/app"), "APP_HOME", "/opt/app", "/srv/app"),
        Arguments.of("missing", Map.of(), "APP_PORT", "80", "80"),
        Arguments.of("empty-value", Map.of("APP_MODE", ""), "APP_MODE", "production", ""),
        Arguments.of("empty-default", Map.of(), "APP_MODE", "", ""),
        Arguments.of(
            "spaces", Map.of("APP_TITLE", "  two  spaces  "), "APP_TITLE", "none", "  two  spaces  "),
        Arguments.of(
            "equals", Map.of("APP_QUERY", "key=value=more"), "APP_QUERY", "none", "key=value=more"),
        Arguments.of(
            "newline", Map.of("APP_TEXT", "first\nsecond"), "APP_TEXT", "none", "first\nsecond"),
        Arguments.of(
            "non-ascii",
            Map.of("APP_GREETING", "xin chào, 日本語, 🙂"),
            "APP_GREETING",
            "none",
            "xin chào, 日本語, 🙂"),
        Arguments.of("case", Map.of("APP_PORT", "8080"), "app_port", "80", "80"),
        Arguments.of("inherited-name", Map.of(), "toString", "none", "none"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void returnsTheValueOrTheDefault(
      String label, Map<String, String> env, String name, String defaultValue, String want)
      throws IOException, InterruptedException, URISyntaxException {
    String java = ProcessHandle.current().info().command().orElseThrow();
    String classes =
        Path.of(GetEnvTest.class.getProtectionDomain().getCodeSource().getLocation().toURI())
            .toString();
    ProcessBuilder pb =
        new ProcessBuilder(java, "-cp", classes, "GetEnvTest", name, defaultValue)
            .redirectError(Redirect.INHERIT);
    pb.environment().remove(name);
    pb.environment().putAll(env);
    Process child = pb.start();
    String got = new String(child.getInputStream().readAllBytes(), StandardCharsets.UTF_8);
    assertEquals(0, child.waitFor(), label);
    assertEquals(want, got, label);
  }
}
