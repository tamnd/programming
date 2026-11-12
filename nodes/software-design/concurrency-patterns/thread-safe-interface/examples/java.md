```java run id=interface
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.locks.ReentrantLock;

public class Main {
    static class FileCache {
        private final ReentrantLock lock = new ReentrantLock();
        final Map<String, String> files = new HashMap<>();
        int acquires = 0;

        // Interface methods: take the lock, then call an implementation method.
        public String find(String path) {
            lock.lock();
            try {
                acquires++;
                return findI(path);
            } finally {
                lock.unlock();
            }
        }

        public void bind(String path) {
            lock.lock();
            try {
                acquires++;
                bindI(path);
            } finally {
                lock.unlock();
            }
        }

        // Implementation methods: trust that the lock is held, never take it.
        private String findI(String path) {
            String file = checkCacheI(path);
            if (file == null) {
                bindI(path); // not bind(): that would take the lock again
                file = checkCacheI(path);
            }
            return file;
        }

        private String checkCacheI(String path) {
            return files.get(path);
        }

        private void bindI(String path) {
            files.put(path, "mapped " + path);
        }
    }

    public static void main(String[] args) throws InterruptedException {
        FileCache cache = new FileCache();
        cache.bind("/index.html");
        String[] paths = {"/a.html", "/b.html", "/c.html"};
        List<Thread> threads = new ArrayList<>();
        for (int w = 0; w < 4; w++) {
            Thread t = new Thread(() -> {
                for (int i = 0; i < 1000; i++) cache.find(paths[i % 3]);
            });
            threads.add(t);
            t.start();
        }
        for (Thread t : threads) t.join();
        System.out.println("find /a.html: " + cache.find("/a.html"));
        System.out.println("files cached: " + cache.files.size());
        System.out.println("lock acquisitions: " + cache.acquires);
    }
}
```

```output
find /a.html: mapped /a.html
files cached: 4
lock acquisitions: 4002
```

- Verdict: idiom. Public methods take the lock and private methods do the work, so each interface call takes the lock once.
