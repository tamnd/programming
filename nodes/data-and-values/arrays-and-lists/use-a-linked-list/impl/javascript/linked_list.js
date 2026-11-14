// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

class Node {
  constructor(value) {
    this.value = value;
    this.prev = null;
    this.next = null;
  }
}

/** A doubly linked list with a head and a tail. JavaScript has no linked list type. */
export class LinkedList {
  head = null;
  tail = null;

  pushFront(value) {
    const node = new Node(value);
    node.next = this.head;
    if (this.head === null) {
      this.tail = node;
    } else {
      this.head.prev = node;
    }
    this.head = node;
  }

  pushBack(value) {
    const node = new Node(value);
    node.prev = this.tail;
    if (this.tail === null) {
      this.head = node;
    } else {
      this.tail.next = node;
    }
    this.tail = node;
  }

  unlink(node) {
    // Update the head and the tail when the node is at an end.
    if (node.prev === null) {
      this.head = node.next;
    } else {
      node.prev.next = node.next;
    }
    if (node.next === null) {
      this.tail = node.prev;
    } else {
      node.next.prev = node.prev;
    }
    node.prev = null;
    node.next = null;
  }

  remove(value) {
    for (let node = this.head; node !== null; node = node.next) {
      if (node.value === value) {
        this.unlink(node);
        return true;
      }
    }
    return false;
  }

  *[Symbol.iterator]() {
    for (let node = this.head; node !== null; node = node.next) {
      yield node.value;
    }
  }
}

/**
 * Runs the operations on a new linked list and returns its items.
 * @param {string[]} ops
 * @returns {number[]}
 */
export function runOps(ops) {
  const list = new LinkedList();
  for (const op of ops) {
    const [name, arg] = op.split(" ");
    if (name === "push_front") {
      list.pushFront(Number(arg));
    } else if (name === "push_back") {
      list.pushBack(Number(arg));
    } else if (name === "pop_front" && list.head !== null) {
      list.unlink(list.head);
    } else if (name === "pop_back" && list.tail !== null) {
      list.unlink(list.tail);
    } else if (name === "remove") {
      list.remove(Number(arg));
    } else {
      throw new Error(`bad operation: ${op}`);
    }
  }
  return [...list];
}
