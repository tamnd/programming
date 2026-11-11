```java run id=flow
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Flow;
import java.util.concurrent.SubmissionPublisher;

public class Main {
    public static void main(String[] args) throws Exception {
        CountDownLatch done = new CountDownLatch(1);
        try (SubmissionPublisher<Integer> readings = new SubmissionPublisher<>()) {
            readings.subscribe(new Flow.Subscriber<>() {
                Flow.Subscription subscription;
                int sum;

                public void onSubscribe(Flow.Subscription s) {
                    subscription = s;
                    s.request(1);
                }

                public void onNext(Integer t) {
                    if (t > 20) {
                        sum += t;
                        System.out.println("hot " + t + ", running sum " + sum);
                    }
                    subscription.request(1); // ask for one more item: back-pressure
                }

                public void onError(Throwable e) {
                    e.printStackTrace();
                }

                public void onComplete() {
                    System.out.println("done");
                    done.countDown();
                }
            });
            for (int t : new int[] {18, 22, 19, 25, 30}) {
                readings.submit(t);
            }
        }
        done.await();
    }
}
```

```output for=flow
hot 22, running sum 22
hot 25, running sum 47
hot 30, running sum 77
done
```

- Verdict: stdlib. `java.util.concurrent.Flow` defines the four interfaces of the Reactive Streams standard: publisher, subscriber, subscription and processor. `SubmissionPublisher` delivers items on a thread pool. The subscriber asks for items with `request`, so a slow subscriber is never sent more than it asked for. Libraries such as Reactor and RxJava give the operators.
