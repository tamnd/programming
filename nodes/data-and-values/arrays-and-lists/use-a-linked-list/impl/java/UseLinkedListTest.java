// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class UseLinkedListTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("deque", new String[] {"push_back 1", "push_back 2", "push_front 0", "pop_back", "push_back 3"}, new int[] {0, 1, 3}),
        Arguments.of("remove-middle", new String[] {"push_back 1", "push_back 2", "push_back 3", "remove 2"}, new int[] {1, 3}),
        Arguments.of("empty", new String[] {}, new int[] {}),
        Arguments.of("push-pop", new String[] {"push_back 1", "pop_front"}, new int[] {}),
        Arguments.of("single", new String[] {"push_front 7"}, new int[] {7}),
        Arguments.of("remove-only", new String[] {"push_back 5", "remove 5", "push_back 6"}, new int[] {6}),
        Arguments.of("remove-head", new String[] {"push_back 1", "push_back 2", "remove 1", "push_front 0"}, new int[] {0, 2}),
        Arguments.of("remove-tail", new String[] {"push_back 1", "push_back 2", "remove 2", "push_back 3"}, new int[] {1, 3}),
        Arguments.of("pop-both-ends", new String[] {"push_back 1", "push_back 2", "pop_front", "pop_back", "push_front 3"}, new int[] {3}),
        Arguments.of("remove-absent", new String[] {"push_back 1", "remove 9"}, new int[] {1}),
        Arguments.of("duplicates", new String[] {"push_back 4", "push_back 5", "push_back 4", "remove 4"}, new int[] {5, 4}),
        Arguments.of("push-front-order", new String[] {"push_front 1", "push_front 2", "push_front 3"}, new int[] {3, 2, 1}),
        Arguments.of("negative", new String[] {"push_front -1", "push_back -2", "remove -1"}, new int[] {-2}),
        Arguments.of("limit", new String[] {"push_back 2147483647", "push_front -2147483648", "remove 2147483647", "push_back 2147483647"}, new int[] {Integer.MIN_VALUE, 2147483647}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void runOps(String name, String[] ops, int[] want) {
    assertArrayEquals(want, UseLinkedList.runOps(ops), name);
  }
}
