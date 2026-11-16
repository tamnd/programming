// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

import java.util.ArrayList;
import java.util.List;
import java.util.logging.Formatter;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.LogRecord;
import java.util.logging.Logger;

/** Reads port settings and logs the bad ones with java.util.logging. */
final class ReadPorts {
  private ReadPorts() {}

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

  /** Puts value in double quotes and escapes the characters that could end the line or the value. */
  static String quote(String value) {
    StringBuilder sb = new StringBuilder("\"");
    for (char c : value.toCharArray()) {
      switch (c) {
        case '\\' -> sb.append("\\\\");
        case '"' -> sb.append("\\\"");
        case '\n' -> sb.append("\\n");
        case '\r' -> sb.append("\\r");
        case '\t' -> sb.append("\\t");
        default -> sb.append(c);
      }
    }
    return sb.append('"').toString();
  }

  /**
   * Writes level=LEVEL and the message. SEVERE is written as ERROR. This formatter does not write
   * the Throwable of the record, so that the test output is short; a real formatter writes its stack
   * trace.
   */
  static final class LogfmtFormatter extends Formatter {
    @Override
    public String format(LogRecord r) {
      String level = r.getLevel() == Level.SEVERE ? "ERROR" : r.getLevel().getName();
      return "level=" + level + " " + r.getMessage() + "\n";
    }
  }

  /** A handler that keeps the formatted records in a string. */
  static final class StringHandler extends Handler {
    private final StringBuilder text = new StringBuilder();
    private final Formatter formatter = new LogfmtFormatter();

    @Override
    public void publish(LogRecord r) {
      if (isLoggable(r)) {
        text.append(formatter.format(r));
      }
    }

    @Override
    public void flush() {}

    @Override
    public void close() {}

    String text() {
      return text.toString();
    }
  }

  /** Returns the good ports. Logs each bad text at the SEVERE level with its exception, then a summary. */
  static List<Integer> readPorts(List<String> texts, Logger log) {
    List<Integer> ports = new ArrayList<>();
    for (int i = 0; i < texts.size(); i++) {
      try {
        ports.add(parsePort(texts.get(i)));
      } catch (IllegalArgumentException e) {
        // The error is handled here: it is logged once and not thrown again.
        log.log(Level.SEVERE, "msg=\"read port failed\" line=" + (i + 1) + " err=" + quote(e.getMessage()), e);
      }
    }
    log.info("msg=\"ports read\" ok=" + ports.size() + " total=" + texts.size());
    return ports;
  }
}
