// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/** A text template with $name, ${name} and $$, as in Python string.Template. */
public final class Template {
    private static final Pattern PLACEHOLDER =
            Pattern.compile("\\$(?:(\\$)|\\{([A-Za-z_][A-Za-z0-9_]*)\\}|([A-Za-z_][A-Za-z0-9_]*))?");

    private Template() {}

    /**
     * Returns template with $$ changed to $, and $name and ${name} changed to
     * the value of name. replaceAll reads $ and \ in the returned text, so each
     * value goes through Matcher.quoteReplacement. An unknown name or a $
     * without a name throws IllegalArgumentException.
     */
    public static String substitute(String template, String[] names, String[] values) {
        Map<String, String> table = new HashMap<>();
        for (int i = 0; i < names.length; i++) {
            table.put(names[i], values[i]);
        }
        return PLACEHOLDER.matcher(template).replaceAll(m -> {
            if (m.group(1) != null) {
                return Matcher.quoteReplacement("$");
            }
            String name = m.group(2) != null ? m.group(2) : m.group(3);
            if (name == null) {
                throw new IllegalArgumentException("a $ without a name at " + m.start());
            }
            String value = table.get(name);
            if (value == null) {
                throw new IllegalArgumentException("unknown name: " + name);
            }
            return Matcher.quoteReplacement(value);
        });
    }

    public static void main(String[] args) {
        System.out.println(substitute("Hello, $name!", new String[] {"name"}, new String[] {"Ada"}));
    }
}
