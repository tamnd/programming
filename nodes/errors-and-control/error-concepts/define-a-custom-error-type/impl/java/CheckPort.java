// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

/** Reads a TCP port number and reports a bad one with its own exception type. */
final class CheckPort {
  private CheckPort() {}

  /** Reports that a text is not a valid TCP port number. */
  static final class PortException extends Exception {
    // Exception is Serializable, and javac -Xlint:serial asks for this field.
    private static final long serialVersionUID = 1L;

    private final String text;
    private final String reason;

    PortException(String text, String reason) {
      super("invalid port '" + text + "': " + reason);
      this.text = text;
      this.reason = reason;
    }

    String text() {
      return text;
    }

    String reason() {
      return reason;
    }
  }

  /** Returns the port number in text. PortException is checked, so it is in the throws clause. */
  static int parsePort(String text) throws PortException {
    if (text.isEmpty()) {
      throw new PortException(text, "empty");
    }
    if (!text.chars().allMatch(c -> c >= '0' && c <= '9')) {
      throw new PortException(text, "not a number");
    }
    int port = 0;
    for (int i = 0; i < text.length(); i++) {
      port = port * 10 + (text.charAt(i) - '0');
      if (port > 65535) {
        throw new PortException(text, "out of range");
      }
    }
    if (port == 0) {
      throw new PortException(text, "out of range");
    }
    return port;
  }

  /** Returns "port N", or the message of the PortException. */
  static String checkPort(String text) {
    try {
      return "port " + parsePort(text);
    } catch (PortException e) {
      return e.getMessage();
    }
  }
}
