// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotEquals;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class BirthdayTest {
  static Birthday.Person p(String name, int age) {
    return new Birthday.Person(name, age);
  }

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("first", List.of(p("Ada", 36), p("Alan", 41)), "Ada", List.of(p("Ada", 37), p("Alan", 41))),
        Arguments.of("second", List.of(p("Ada", 36), p("Alan", 41)), "Alan", List.of(p("Ada", 36), p("Alan", 42))),
        Arguments.of("empty", List.of(), "Ada", List.of()),
        Arguments.of("not-found", List.of(p("Ada", 36)), "Grace", List.of(p("Ada", 36))),
        Arguments.of("duplicate-name", List.of(p("Ada", 36), p("Alan", 41), p("Ada", 9)), "Ada", List.of(p("Ada", 37), p("Alan", 41), p("Ada", 10))),
        Arguments.of("zero-age", List.of(p("Baby", 0)), "Baby", List.of(p("Baby", 1))),
        Arguments.of("non-ascii", List.of(p("Zoë", 20), p("Zoe", 30)), "Zoë", List.of(p("Zoë", 21), p("Zoe", 30))),
        Arguments.of("case", List.of(p("ada", 36)), "Ada", List.of(p("ada", 36))));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void birthday(String name, List<Birthday.Person> people, String who, List<Birthday.Person> want) {
    List<Birthday.Person> before = new ArrayList<>(people);
    assertEquals(want, Birthday.birthday(people, who));
    assertEquals(before, people);
  }

  @Test
  void equalityAndPrinting() {
    assertEquals(p("Ada", 36), p("Ada", 36));
    assertNotEquals(p("Ada", 36), p("Ada", 37));
    assertEquals("Person[name=Ada, age=36]", p("Ada", 36).toString());
  }
}
