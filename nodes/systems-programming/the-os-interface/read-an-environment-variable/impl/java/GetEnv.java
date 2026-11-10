// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

import java.util.Objects;

public final class GetEnv {
  private GetEnv() {}

  /**
   * Returns the value of the environment variable name, or defaultValue when it is not set. An
   * empty value is a value, so it does not give defaultValue.
   */
  public static String getEnv(String name, String defaultValue) {
    // System.getenv(name) returns null when the variable is not set.
    return Objects.requireNonNullElse(System.getenv(name), defaultValue);
  }
}
