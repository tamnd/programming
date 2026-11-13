// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

/** Writes records as a CSV file of RFC 4180. The JDK has no CSV writer, so this class has a small one. */
public final class WriteCsv {
    private WriteCsv() {}

    /** A record has more or fewer fields than the first record (the header). */
    public static final class CsvException extends IllegalArgumentException {
        private static final long serialVersionUID = 1L;

        CsvException(int record, int got, int want) {
            super("record " + record + " has " + got + " fields, the header has " + want);
        }

        public String kind() {
            return "field-count";
        }
    }

    private static boolean needsQuotes(String field, boolean alone) {
        // A record with one empty field needs quotes, or the line is empty and a reader skips it.
        if (alone && field.isEmpty()) {
            return true;
        }
        for (int i = 0; i < field.length(); i++) {
            char c = field.charAt(i);
            if (c == ',' || c == '"' || c == '\r' || c == '\n') {
                return true;
            }
        }
        return false;
    }

    /** Returns the CSV text of the records. The first record is the header. */
    public static String formatCsv(List<List<String>> records) {
        StringBuilder out = new StringBuilder();
        for (int n = 0; n < records.size(); n++) {
            List<String> record = records.get(n);
            if (record.size() != records.get(0).size()) {
                throw new CsvException(n, record.size(), records.get(0).size());
            }
            for (int i = 0; i < record.size(); i++) {
                if (i > 0) {
                    out.append(',');
                }
                String field = record.get(i);
                if (needsQuotes(field, record.size() == 1)) {
                    out.append('"').append(field.replace("\"", "\"\"")).append('"');
                } else {
                    out.append(field);
                }
            }
            out.append("\r\n");
        }
        return out.toString();
    }

    /** Writes the records to the file at path as UTF-8, with no byte order mark. */
    public static void writeCsv(Path path, List<List<String>> records) throws IOException {
        Files.writeString(path, formatCsv(records));
    }
}
