// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/** Starts one thread per name; each thread builds the greeting for its name. */
public final class Greetings {
  private Greetings() {}

  public static List<String> threadGreetings(List<String> names) throws InterruptedException {
    String[] results = new String[names.size()];
    List<Thread> threads = new ArrayList<>();
    for (int i = 0; i < names.size(); i++) {
      // The lambda captures copies of index and name; it writes only its own slot.
      int index = i;
      String name = names.get(i);
      threads.add(Thread.ofPlatform().name("greeter-" + i).start(() -> results[index] = "hello from " + name));
    }
    // Start all threads first, then wait for each one. join() also makes the writes visible here.
    for (Thread thread : threads) {
      thread.join();
    }
    return Arrays.asList(results);
  }
}
