// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

import java.io.IOException;
import java.io.Serial;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/** Reads a CSV file of RFC 4180 with a header line. */
public final class ReadCsv {
    private ReadCsv() {}

    /** The error for a file that is not valid CSV. kind is "field-count" or "bad-quote". */
    public static final class CsvException extends IOException {
        @Serial private static final long serialVersionUID = 1L;
        private final String kind;

        CsvException(String kind, String message) {
            super(message);
            this.kind = kind;
        }

        public String kind() {
            return kind;
        }
    }

    /** Returns one map for each record after the header, from each name to its field. */
    public static List<Map<String, String>> read(Path path) throws IOException {
        // readString decodes UTF-8 and throws on bytes that are not UTF-8.
        String text = Files.readString(path);
        if (text.startsWith("\uFEFF")) {
            text = text.substring(1);
        }
        List<List<String>> rows = parse(text);
        List<Map<String, String>> records = new ArrayList<>();
        if (rows.isEmpty()) {
            return records;
        }
        List<String> header = rows.getFirst();
        for (int n = 1; n < rows.size(); n++) {
            List<String> row = rows.get(n);
            if (row.size() != header.size()) {
                throw new CsvException("field-count",
                        "record " + n + " has " + row.size() + " fields, want " + header.size());
            }
            Map<String, String> record = new LinkedHashMap<>();
            for (int k = 0; k < row.size(); k++) {
                record.put(header.get(k), row.get(k));
            }
            records.add(record);
        }
        return records;
    }

    /** Returns the length of the line break at i: 1 for LF, 2 for CRLF, 0 for none. */
    private static int lineBreak(String text, int i) {
        if (i < text.length() && text.charAt(i) == '\n') {
            return 1;
        }
        if (i + 1 < text.length() && text.charAt(i) == '\r' && text.charAt(i + 1) == '\n') {
            return 2;
        }
        return 0;
    }

    /** Splits RFC 4180 text into records of fields. An empty line gives no record. */
    static List<List<String>> parse(String text) throws CsvException {
        List<List<String>> records = new ArrayList<>();
        int n = text.length();
        int i = 0;
        while (i < n) {
            int empty = lineBreak(text, i);
            if (empty > 0) {
                i += empty;
                continue;
            }
            List<String> record = new ArrayList<>();
            while (true) {
                StringBuilder field = new StringBuilder();
                if (i < n && text.charAt(i) == '"') {
                    i++;
                    while (true) {
                        if (i >= n) {
                            throw new CsvException("bad-quote", "a quoted field has no closing quote");
                        }
                        char c = text.charAt(i);
                        if (c == '"') {
                            if (i + 1 >= n || text.charAt(i + 1) != '"') {
                                break;
                            }
                            field.append('"');
                            i += 2;
                        } else if (lineBreak(text, i) == 2) {
                            field.append('\n'); // CRLF inside quotes becomes LF.
                            i += 2;
                        } else {
                            field.append(c);
                            i++;
                        }
                    }
                    i++; // the closing quote
                    if (i < n && text.charAt(i) != ',' && lineBreak(text, i) == 0) {
                        throw new CsvException("bad-quote", "text after the closing quote of a field");
                    }
                } else {
                    while (i < n && text.charAt(i) != ',' && lineBreak(text, i) == 0) {
                        field.append(text.charAt(i++));
                    }
                }
                record.add(field.toString());
                if (i >= n || text.charAt(i) != ',') {
                    break;
                }
                i++;
            }
            i += lineBreak(text, i);
            records.add(record);
        }
        return records;
    }
}
