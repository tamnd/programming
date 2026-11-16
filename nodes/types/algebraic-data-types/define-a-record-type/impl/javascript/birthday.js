// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

// makePerson builds a frozen object, so its fields cannot change.
export function makePerson(name, age) {
  return Object.freeze({ name, age });
}

// birthday returns a new array where each person called name is one year
// older. The spread copies the other fields into a new object.
export function birthday(people, name) {
  return people.map((p) => (p.name === name ? Object.freeze({ ...p, age: p.age + 1 }) : p));
}
