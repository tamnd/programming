```c run id=invoices
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* A role is a function pointer and a context pointer. */
struct clock { int (*today)(void *ctx); void *ctx; };
struct mailer { void (*send)(void *ctx, const char *to, const char *text); void *ctx; };

struct invoice { int number; const char *customer; int due; };

struct reminder_service { struct clock clock; struct mailer mailer; };

static bool remind(const struct reminder_service *s, const struct invoice *inv) {
    int late = s->clock.today(s->clock.ctx) - inv->due;
    if (late <= 0) return false;
    char text[80];
    snprintf(text, sizeof text, "invoice %d is %d days overdue", inv->number, late);
    s->mailer.send(s->mailer.ctx, inv->customer, text);
    return true;
}

/* Implementations. */
static int fixed_today(void *ctx) { return *(const int *)ctx; }

static void console_send(void *ctx, const char *to, const char *text) {
    (void)ctx;
    printf("to %s: %s\n", to, text);
}

struct recording { char sent[4][96]; int count; };

static void recording_send(void *ctx, const char *to, const char *text) {
    struct recording *r = ctx;
    if (r->count < 4) snprintf(r->sent[r->count++], sizeof r->sent[0], "to %s: %s", to, text);
}

int main(void) {
    /* The composition root. */
    int day = 40;
    struct reminder_service service = {{fixed_today, &day}, {console_send, NULL}};
    const struct invoice invoices[] = {{17, "ana@example.com", 31}, {18, "bo@example.com", 45}};
    int sent = 0;
    for (size_t i = 0; i < 2; i++) sent += remind(&service, &invoices[i]);
    printf("reminders sent: %d\n", sent);

    /* A test wires the same function to a recording double. */
    int later = 50;
    struct recording recorder = {0};
    struct reminder_service test = {{fixed_today, &later}, {recording_send, &recorder}};
    remind(&test, &invoices[1]);
    printf("recorded: %s\n", recorder.sent[0]);
    return 0;
}
```

```output
to ana@example.com: invoice 17 is 9 days overdue
reminders sent: 1
recorded: to bo@example.com: invoice 18 is 5 days overdue
```

- Verdict: idiom. A struct with a function pointer and a `void *` context plays the role of an interface. The caller fills it, so `remind` never names an implementation.
- The context pointer carries the state of each implementation, such as the day of the fixed clock or the list of the recorder. The compiler cannot check that a context has the type that the function expects.
- Link-time substitution is the other C technique: the test links a different object file that defines the same functions. It needs no pointers, but it fixes one implementation for each executable.
