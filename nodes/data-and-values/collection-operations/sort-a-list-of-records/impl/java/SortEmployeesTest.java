// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.IntStream;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SortEmployeesTest {

  private static SortEmployees.Employee e(String name, String dept, int salary) {
    return new SortEmployees.Employee(name, dept, salary);
  }

  /** The 40 records of the many-ties case: depts "b" and "a" in turn, all with salary 1. */
  private static List<SortEmployees.Employee> many(int parity) {
    return IntStream.range(0, 40)
        .filter(i -> parity < 0 || i % 2 == parity)
        .mapToObj(i -> e("n%02d".formatted(i + 1), i % 2 == 1 ? "a" : "b", 1))
        .toList();
  }

  static Stream<Arguments> cases() {
    List<SortEmployees.Employee> manyWant = new ArrayList<>(many(1));
    manyWant.addAll(many(0));
    return Stream.of(
        Arguments.of(
            "mixed",
            List.of(e("Ada", "eng", 120), e("Bob", "sales", 90), e("Cy", "eng", 150), e("Di", "ops", 100)),
            List.of(e("Cy", "eng", 150), e("Ada", "eng", 120), e("Di", "ops", 100), e("Bob", "sales", 90))),
        Arguments.of(
            "same-salary-other-dept",
            List.of(e("Ann", "ops", 100), e("Ben", "eng", 100), e("Cat", "hr", 100)),
            List.of(e("Ben", "eng", 100), e("Cat", "hr", 100), e("Ann", "ops", 100))),
        Arguments.of("empty", List.of(), List.of()),
        Arguments.of("single", List.of(e("Ada", "eng", 120)), List.of(e("Ada", "eng", 120))),
        Arguments.of(
            "sorted",
            List.of(e("Cy", "eng", 150), e("Ada", "eng", 120), e("Bob", "ops", 90)),
            List.of(e("Cy", "eng", 150), e("Ada", "eng", 120), e("Bob", "ops", 90))),
        Arguments.of(
            "reversed",
            List.of(e("Bob", "ops", 90), e("Ada", "eng", 120), e("Cy", "eng", 150)),
            List.of(e("Cy", "eng", 150), e("Ada", "eng", 120), e("Bob", "ops", 90))),
        Arguments.of(
            "ties",
            List.of(e("Zoe", "eng", 100), e("Xi", "hr", 80), e("Al", "eng", 100), e("Yu", "hr", 80), e("Mo", "eng", 100)),
            List.of(e("Zoe", "eng", 100), e("Al", "eng", 100), e("Mo", "eng", 100), e("Xi", "hr", 80), e("Yu", "hr", 80))),
        Arguments.of("many-ties", many(-1), manyWant),
        Arguments.of(
            "case",
            List.of(e("Ann", "eng", 100), e("Bea", "Sales", 90), e("Cal", "ops", 80)),
            List.of(e("Bea", "Sales", 90), e("Ann", "eng", 100), e("Cal", "ops", 80))),
        Arguments.of(
            "non-ascii",
            List.of(e("Eve", "équipe", 100), e("Fay", "zoo", 90), e("Gus", "eng", 80)),
            List.of(e("Gus", "eng", 80), e("Fay", "zoo", 90), e("Eve", "équipe", 100))),
        Arguments.of(
            "negative",
            List.of(e("Hal", "eng", 0), e("Ida", "eng", -5), e("Jo", "eng", 5)),
            List.of(e("Jo", "eng", 5), e("Hal", "eng", 0), e("Ida", "eng", -5))),
        Arguments.of(
            "overflow",
            List.of(e("Kim", "eng", -2000000000), e("Lee", "eng", 2000000000)),
            List.of(e("Lee", "eng", 2000000000), e("Kim", "eng", -2000000000))));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void sortsByDeptThenSalaryDescending(
      String name, List<SortEmployees.Employee> given, List<SortEmployees.Employee> want) {
    assertEquals(want, SortEmployees.sortEmployees(given), name);
  }

  /** The cases of cases/locale.jsonl. */
  static Stream<Arguments> locales() {
    return Stream.of(
        Arguments.of("en-accents", "en", List.of("zebra", "Émile", "apple", "Eve", "eagle"), List.of("apple", "eagle", "Émile", "Eve", "zebra")),
        Arguments.of("en-case", "en", List.of("b", "B", "a", "A"), List.of("a", "A", "b", "B")),
        Arguments.of("en-accent-order", "en", List.of("resume", "résumé", "Resume", "rèsume"), List.of("resume", "Resume", "résumé", "rèsume")),
        Arguments.of("sv-letters", "sv", List.of("ö", "z", "o", "a", "å", "ä"), List.of("a", "o", "z", "å", "ä", "ö")),
        Arguments.of("sv-names", "sv", List.of("Örjan", "Zara", "Olof", "Åsa", "Anna", "Ärla"), List.of("Anna", "Olof", "Zara", "Åsa", "Ärla", "Örjan")),
        Arguments.of("de-umlaut", "de", List.of("Äpfel", "Zebra", "Apfel", "apfel"), List.of("apfel", "Apfel", "Äpfel", "Zebra")),
        Arguments.of("da-aa", "da", List.of("Aase", "Zelda", "Ågot", "Øjvind", "Ærø"), List.of("Zelda", "Ærø", "Øjvind", "Ågot", "Aase")),
        Arguments.of("es-enye", "es", List.of("llama", "luz", "lzx", "ñu", "nube", "oso"), List.of("llama", "luz", "lzx", "nube", "ñu", "oso")));
  }

  @ParameterizedTest(name = "locale {0}")
  @MethodSource("locales")
  void sortsNamesByLocale(String name, String locale, List<String> names, List<String> want) {
    assertEquals(want, SortEmployees.sortNames(names, locale), name);
  }
}
