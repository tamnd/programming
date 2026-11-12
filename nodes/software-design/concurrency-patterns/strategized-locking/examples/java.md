```java run id=strategy
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Main {
    interface Lockable {
        void acquire();
        void release();
    }

    // A lock for one thread: both methods do nothing.
    static class NullMutex implements Lockable {
        public void acquire() {}
        public void release() {}
    }

    static class ThreadMutex implements Lockable {
        private final Lock lock = new ReentrantLock();
        public void acquire() { lock.lock(); }
        public void release() { lock.unlock(); }
    }

    static class FileCache {
        private final Lockable lock; // the locking strategy is a parameter
        final Map<String, String> files = new HashMap<>();
        int lookups = 0;

        FileCache(Lockable lock) { this.lock = lock; }

        String find(String path) {
            lock.acquire();
            try {
                lookups++;
                return files.computeIfAbsent(path, p -> "mapped " + p);
            } finally {
                lock.release();
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        FileCache single = new FileCache(new NullMutex());
        for (String path : new String[] {"/a.html", "/b.html", "/a.html"}) single.find(path);
        System.out.println("Null_Mutex cache: " + single.files.size() + " files, " + single.lookups + " lookups");

        FileCache multi = new FileCache(new ThreadMutex());
        String[] paths = {"/a.html", "/b.html", "/c.html"};
        List<Thread> threads = new ArrayList<>();
        for (int w = 0; w < 4; w++) {
            Thread t = new Thread(() -> {
                for (int i = 0; i < 1000; i++) multi.find(paths[i % 3]);
            });
            threads.add(t);
            t.start();
        }
        for (Thread t : threads) t.join();
        System.out.println("Thread_Mutex cache: " + multi.files.size() + " files, " + multi.lookups + " lookups");
    }
}
```

```output
Null_Mutex cache: 2 files, 3 lookups
Thread_Mutex cache: 3 files, 4000 lookups
```

- Verdict: idiom. This is the polymorphic form. `FileCache` sees only the `Lockable` interface, and each strategy is a class that implements it.
