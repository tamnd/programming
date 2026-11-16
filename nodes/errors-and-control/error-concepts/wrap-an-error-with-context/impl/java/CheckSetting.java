// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-an-error-with-context/.

/** Reads a port setting and wraps the error of the parser with the place of the setting. */
final class CheckSetting {
  private CheckSetting() {}

  /** A setting in a configuration file is bad. */
  static final class ConfigException extends Exception {
    // Exception is Serializable, and javac -Xlint:serial asks for this field.
    private static final long serialVersionUID = 1L;

    ConfigException(String message, Throwable cause) {
      super(message, cause);
    }
  }

  /** Returns the port number in text, or throws IllegalArgumentException. */
  static int parsePort(String text) {
    if (text.isEmpty() || !text.chars().allMatch(c -> c >= '0' && c <= '9')) {
      throw new IllegalArgumentException("not a number: '" + text + "'");
    }
    int port = 0;
    for (int i = 0; i < text.length(); i++) {
      port = port * 10 + (text.charAt(i) - '0');
      if (port > 65535) {
        throw new IllegalArgumentException("out of range: " + text);
      }
    }
    if (port == 0) {
      throw new IllegalArgumentException("out of range: " + text);
    }
    return port;
  }

  /** Returns the port number, or throws ConfigException with the cause. */
  static int readPort(String where, String text) throws ConfigException {
    try {
      return parsePort(text);
    } catch (IllegalArgumentException e) {
      throw new ConfigException(where + ": read port", e);
    }
  }

  /** Joins the message of e and the messages of its causes. */
  static String chain(Throwable e) {
    StringBuilder sb = new StringBuilder(e.getMessage());
    for (Throwable c = e.getCause(); c != null; c = c.getCause()) {
      sb.append(": ").append(c.getMessage());
    }
    return sb.toString();
  }

  /** Returns "port N", or "error: " and the chain of messages. */
  static String checkSetting(String where, String text) {
    try {
      return "port " + readPort(where, text);
    } catch (ConfigException e) {
      return "error: " + chain(e);
    }
  }
}
