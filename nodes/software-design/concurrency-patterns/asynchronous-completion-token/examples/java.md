```java run id=act
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Main {
    record Request(int token, String name) {}
    record Reply(int token, String result) {}

    static List<Reply> service(List<Request> requests) {
        List<Reply> replies = new ArrayList<>();
        for (Request r : requests) replies.add(new Reply(r.token(), r.name() + " loaded"));
        Collections.reverse(replies);  // replies in the reverse order
        return replies;
    }

    public static void main(String[] args) {
        Map<Integer, String> pending = new HashMap<>();
        List<Request> requests = new ArrayList<>();
        String[] names = {"image-7", "image-9"};
        for (int i = 0; i < names.length; i++) {
            int token = i + 1;
            pending.put(token, names[i]);
            requests.add(new Request(token, names[i]));
            System.out.println("send " + token + ": load " + names[i]);
        }
        for (Reply reply : service(requests)) {
            pending.remove(reply.token());  // the token finds the state at once
            System.out.println("done " + reply.token() + ": " + reply.result());
        }
    }
}
```

```output
send 1: load image-7
send 2: load image-9
done 2: image-9 loaded
done 1: image-7 loaded
```

- Verdict: idiom. Records carry the token in the request and in the reply, and a `HashMap` holds the pending state.
