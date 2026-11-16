// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class TemplateTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("greeting", "Hello, $name!", new String[]{"name"}, new String[]{"Ada"}, "Hello, Ada!"),
                Arguments.of("braces", "${item}s cost $$${price}", new String[]{"item", "price"}, new String[]{"Apple", "3"}, "Apples cost $3"),
                Arguments.of("two-uses", "$a and $a again", new String[]{"a"}, new String[]{"x"}, "x and x again"),
                Arguments.of("no-placeholders", "plain text", new String[]{}, new String[]{}, "plain text"),
                Arguments.of("empty-template", "", new String[]{"x"}, new String[]{"1"}, ""),
                Arguments.of("dollar-escape", "$$name", new String[]{"name"}, new String[]{"Ada"}, "$name"),
                Arguments.of("adjacent-text", "${n}th", new String[]{"n"}, new String[]{"4"}, "4th"),
                Arguments.of("value-with-dollar", "$v", new String[]{"v"}, new String[]{"$x and ${y}"}, "$x and ${y}"),
                Arguments.of("empty-value", "[$v]", new String[]{"v"}, new String[]{""}, "[]"),
                Arguments.of("underscore-name", "$first_name $last_name", new String[]{"first_name", "last_name"}, new String[]{"Grace", "Hopper"}, "Grace Hopper"),
                Arguments.of("name-ends-at-punct", "$a.$b", new String[]{"a", "b"}, new String[]{"1", "2"}, "1.2"),
                Arguments.of("unused-value", "$a", new String[]{"a", "b"}, new String[]{"1", "2"}, "1")
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void substitute(String name, String template, String[] names, String[] values, String want) {
        assertEquals(want, Template.substitute(template, names, values));
    }

    @Test
    void unknownName() {
        assertThrows(IllegalArgumentException.class,
                () -> Template.substitute("Hi $who", new String[] {"name"}, new String[] {"Ada"}));
    }
}
