```java run id=linkedlist-and-listiterator
import java.util.LinkedList;
import java.util.ListIterator;

public class Main {
    public static void main(String[] args) {
        // LinkedList is a doubly linked list. It is both a List and a Deque.
        LinkedList<String> list = new LinkedList<>();
        list.add("b");
        list.addFirst("a");
        list.addLast("c");
        System.out.println(list + " " + list.getFirst() + " " + list.getLast());

        // A ListIterator can insert and remove at its position while it walks the list.
        ListIterator<String> it = list.listIterator();
        while (it.hasNext()) {
            String s = it.next();
            if (s.equals("b")) {
                it.remove();
                it.add("B1");
                it.add("B2");
            }
        }
        System.out.println(list);

        // As a Deque, the list works as a stack and as a queue.
        LinkedList<Integer> q = new LinkedList<>();
        q.offer(1);
        q.offer(2);
        q.push(0);
        System.out.println(q + " " + q.poll() + " " + q.pollLast() + " " + q);

        // A change to the list outside the iterator makes the iterator fail.
        try {
            for (String s : list) {
                if (s.equals("a")) {
                    list.remove(s);
                }
            }
        } catch (java.util.ConcurrentModificationException e) {
            System.out.println("ConcurrentModificationException");
        }
    }
}
```

```output
[a, b, c] a c
[a, B1, B2, c]
[0, 1, 2] 0 2 [1]
ConcurrentModificationException
```

- Verdict: stdlib. java.util.LinkedList is a doubly linked List and Deque. A ListIterator removes and inserts at its position while it walks. push, offer, poll and pollLast use both ends. A remove through the list, not the iterator, makes the for loop throw ConcurrentModificationException.
