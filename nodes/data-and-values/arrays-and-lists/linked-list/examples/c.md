```c run id=intrusive-list
#include <stddef.h>
#include <stdio.h>

/* An intrusive, circular, doubly linked list with a sentinel, in the style of the Linux kernel. */
struct list_head {
    struct list_head *next, *prev;
};

static void list_init(struct list_head *head) {
    head->next = head;
    head->prev = head;
}

/* Insert node between prev and next. The sentinel removes all special cases for the ends. */
static void list_insert(struct list_head *node, struct list_head *prev, struct list_head *next) {
    node->prev = prev;
    node->next = next;
    prev->next = node;
    next->prev = node;
}

static void list_add_tail(struct list_head *node, struct list_head *head) {
    list_insert(node, head->prev, head);
}

static void list_del(struct list_head *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = node->prev = NULL;
}

/* The node is a member of the item, so the list never allocates. */
struct task {
    int id;
    struct list_head link;
};

#define task_of(ptr) ((struct task *)((char *)(ptr) - offsetof(struct task, link)))

static void show(const struct list_head *head) {
    for (const struct list_head *p = head->next; p != head; p = p->next) {
        printf("%d ", task_of(p)->id);
    }
    printf("| empty=%d\n", head->next == head);
}

int main(void) {
    struct list_head run_queue;
    list_init(&run_queue);
    show(&run_queue);

    struct task tasks[3] = {{.id = 10}, {.id = 20}, {.id = 30}};
    for (int i = 0; i < 3; i++) {
        list_add_tail(&tasks[i].link, &run_queue);
    }
    show(&run_queue);

    list_del(&tasks[1].link);
    show(&run_queue);

    /* Walking backwards uses prev. The last node is one step from the sentinel. */
    printf("last=%d\n", task_of(run_queue.prev)->id);
    return 0;
}
```

```output
| empty=1
10 20 30 | empty=0
10 30 | empty=0
last=30
```

- Verdict: idiom. C has no list type. This list is intrusive, circular and doubly linked, with a sentinel head, in the style of the Linux kernel. The links are a member of each task, so the list never allocates, and offsetof turns a link back into its task. The empty test is head->next == head.
