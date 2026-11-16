// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

/** Hands out consecutive ids. Get the one instance with {@code IdGenerator.instance()}. */
public final class IdGenerator {
  static final AtomicInteger CONSTRUCTIONS = new AtomicInteger();

  private final AtomicLong next = new AtomicLong(1);

  private IdGenerator() { // private: no other class can make one
    CONSTRUCTIONS.incrementAndGet();
  }

  // The holder class is initialized on the first call of instance(), and the
  // JVM runs class initialization once, under a lock.
  private static final class Holder {
    static final IdGenerator INSTANCE = new IdGenerator();
  }

  public static IdGenerator instance() {
    return Holder.INSTANCE;
  }

  public long nextId() {
    return next.getAndIncrement();
  }

  public static List<Long> singletonReport(int calls) {
    List<IdGenerator> seen = new ArrayList<>();
    long first = 0;
    long last = 0;
    for (int i = 0; i < calls; i++) {
      IdGenerator g = instance();
      if (seen.stream().noneMatch(s -> s == g)) {
        seen.add(g);
      }
      last = g.nextId();
      if (i == 0) {
        first = last;
      }
    }
    return List.of(last - first, (long) CONSTRUCTIONS.get(), (long) seen.size());
  }
}
