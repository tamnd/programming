```c run id=editor
#include <stdio.h>
#include <string.h>

enum { TEXT_MAX = 64, HISTORY_MAX = 16 };

struct buffer {
    char text[TEXT_MAX];
};

struct command;

struct command_ops {
    void (*execute)(struct command *c, struct buffer *b);
    void (*undo)(struct command *c, struct buffer *b);
};

struct command {
    const struct command_ops *ops;
    char arg[TEXT_MAX]; /* the text to append, or the text that a delete removed */
    size_t count;
};

static void append_execute(struct command *c, struct buffer *b) { strcat(b->text, c->arg); }
static void append_undo(struct command *c, struct buffer *b) { b->text[strlen(b->text) - strlen(c->arg)] = '\0'; }

static void delete_execute(struct command *c, struct buffer *b) {
    size_t start = strlen(b->text) - c->count;
    strcpy(c->arg, b->text + start);
    b->text[start] = '\0';
}
static void delete_undo(struct command *c, struct buffer *b) { strcat(b->text, c->arg); }

static const struct command_ops append_ops = {append_execute, append_undo};
static const struct command_ops delete_ops = {delete_execute, delete_undo};

static struct command append(const char *text) {
    struct command c = {&append_ops, "", 0};
    snprintf(c.arg, sizeof c.arg, "%s", text);
    return c;
}

static struct command delete(size_t count) { return (struct command){&delete_ops, "", count}; }

struct history {
    struct command undo[HISTORY_MAX], redo[HISTORY_MAX];
    int undo_count, redo_count;
};

static void run(struct history *h, struct buffer *b, struct command c) {
    c.ops->execute(&c, b);
    h->undo[h->undo_count++] = c;
    h->redo_count = 0;
}

static int undo(struct history *h, struct buffer *b) {
    if (h->undo_count == 0) return 0;
    struct command c = h->undo[--h->undo_count];
    c.ops->undo(&c, b);
    h->redo[h->redo_count++] = c;
    return 1;
}

static int redo(struct history *h, struct buffer *b) {
    if (h->redo_count == 0) return 0;
    struct command c = h->redo[--h->redo_count];
    c.ops->execute(&c, b);
    h->undo[h->undo_count++] = c;
    return 1;
}

static void show(const char *label, const struct buffer *b) { printf("%s: \"%s\"\n", label, b->text); }

int main(void) {
    struct buffer b = {""};
    struct history h = {0};
    run(&h, &b, append("hello")); show("append", &b);
    run(&h, &b, append(" world")); show("append", &b);
    run(&h, &b, delete(5)); show("delete", &b);
    undo(&h, &b); show("undo", &b);
    undo(&h, &b); show("undo", &b);
    redo(&h, &b); show("redo", &b);
    run(&h, &b, append("!")); show("append", &b);
    if (!redo(&h, &b)) printf("redo: nothing to redo\n");
    return 0;
}
```

```output for=editor
append: "hello"
append: "hello world"
delete: "hello "
undo: "hello world"
undo: "hello"
redo: "hello world"
append: "hello world!"
redo: nothing to redo
```

- Verdict: idiom. A command is a struct with a pointer to a table of functions and the data that the functions need. This is how C code writes an object with virtual methods.
- The commands are stored by value in fixed arrays, so the history needs no `malloc` and no `free`. The program does not check the limits of the arrays; real code must.
- `delete_execute` saves the removed text in the command when it runs. The command is then copied to the undo stack with the text inside it.
