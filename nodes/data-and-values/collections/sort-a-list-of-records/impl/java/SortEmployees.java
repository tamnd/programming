// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

import java.text.Collator;
import java.util.Comparator;
import java.util.List;
import java.util.Locale;

/** Sorts records of employees. */
public final class SortEmployees {

  /** An employee record. */
  public record Employee(String name, String dept, int salary) {}

  /** By dept, then by salary from high to low. comparingInt does not subtract, so it cannot overflow. */
  static final Comparator<Employee> BY_DEPT_THEN_SALARY_DESC =
      Comparator.comparing(Employee::dept)
          .thenComparing(Comparator.comparingInt(Employee::salary).reversed());

  private SortEmployees() {}

  /**
   * Returns a new list of the records by dept, then by salary from high to low. The sort is
   * stable, so records with equal keys keep their input order.
   */
  public static List<Employee> sortEmployees(List<Employee> employees) {
    // String.compareTo compares UTF-16 code units, the same for every locale.
    return employees.stream().sorted(BY_DEPT_THEN_SALARY_DESC).toList();
  }

  /** Returns a new list of the names in the order of the collation rules of a locale, such as "sv". */
  public static List<String> sortNames(List<String> names, String locale) {
    Collator collator = Collator.getInstance(Locale.forLanguageTag(locale));
    return names.stream().sorted(collator).toList();
  }
}
