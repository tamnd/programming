// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

import java.util.List;

final class Birthday {
  private Birthday() {}

  /**
   * A record gets a constructor, accessors, equals, hashCode and toString from its
   * components. Its fields are final.
   */
  record Person(String name, int age) {
    Person withAge(int newAge) {
      return new Person(name, newAge);
    }
  }

  /** Returns a new list where each person called name is one year older. */
  static List<Person> birthday(List<Person> people, String name) {
    return people.stream().map(p -> p.name().equals(name) ? p.withAge(p.age() + 1) : p).toList();
  }
}
