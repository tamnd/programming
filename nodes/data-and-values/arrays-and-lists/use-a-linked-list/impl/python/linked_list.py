# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-linked-list/.

from collections.abc import Iterator


class Node:
    """One node of a doubly linked list."""

    __slots__ = ("value", "prev", "next")

    def __init__(self, value: int) -> None:
        self.value = value
        self.prev: Node | None = None
        self.next: Node | None = None


class LinkedList:
    """A doubly linked list with a head and a tail.

    Python has no linked list type. collections.deque is the standard
    choice for push and pop at both ends. This class shows the links.
    """

    def __init__(self) -> None:
        self.head: Node | None = None
        self.tail: Node | None = None

    def push_front(self, value: int) -> None:
        node = Node(value)
        node.next = self.head
        if self.head is None:
            self.tail = node
        else:
            self.head.prev = node
        self.head = node

    def push_back(self, value: int) -> None:
        node = Node(value)
        node.prev = self.tail
        if self.tail is None:
            self.head = node
        else:
            self.tail.next = node
        self.tail = node

    def unlink(self, node: Node) -> None:
        # Update the head and the tail when the node is at an end.
        if node.prev is None:
            self.head = node.next
        else:
            node.prev.next = node.next
        if node.next is None:
            self.tail = node.prev
        else:
            node.next.prev = node.prev
        node.prev = node.next = None

    def remove(self, value: int) -> bool:
        node = self.head
        while node is not None:
            if node.value == value:
                self.unlink(node)
                return True
            node = node.next
        return False

    def __iter__(self) -> Iterator[int]:
        node = self.head
        while node is not None:
            yield node.value
            node = node.next


def run_ops(ops: list[str]) -> list[int]:
    """Run the operations on a new linked list and return its items."""
    items = LinkedList()
    for op in ops:
        name, _, arg = op.partition(" ")
        if name == "push_front":
            items.push_front(int(arg))
        elif name == "push_back":
            items.push_back(int(arg))
        elif name == "pop_front" and items.head is not None:
            items.unlink(items.head)
        elif name == "pop_back" and items.tail is not None:
            items.unlink(items.tail)
        elif name == "remove":
            items.remove(int(arg))
        else:
            raise ValueError(f"bad operation: {op}")
    return list(items)
