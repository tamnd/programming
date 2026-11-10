```c run id=shop
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* The subsystem. In a real program each part has its own .c file, and these
   functions are static or declared only in a private header. */
struct item {
    const char *name;
    int stock;
};

static struct item inventory[] = {{"book", 2}, {"lamp", 0}};
static int parcels = 0;

static struct item *find(const char *name) {
    for (size_t i = 0; i < sizeof inventory / sizeof inventory[0]; i++) {
        if (strcmp(inventory[i].name, name) == 0) {
            return &inventory[i];
        }
    }
    return NULL;
}

static bool reserve(const char *name) {
    struct item *it = find(name);
    if (it == NULL || it->stock == 0) {
        return false;
    }
    it->stock--;
    return true;
}

static void release(const char *name) { find(name)->stock++; }

static bool charge(const char *card, double amount) {
    (void)amount;
    return strcmp(card, "expired") != 0;
}

static int schedule(void) { return ++parcels; }

/* The facade: the functions that a header shop.h would declare. */
void place_order(const char *name, const char *card, double price) {
    if (!reserve(name)) {
        printf("failed: %s is out of stock\n", name);
        return;
    }
    if (!charge(card, price)) {
        release(name);
        printf("failed: card declined, %s back in stock\n", name);
        return;
    }
    printf("ok: %s, paid %.2f, parcel %d\n", name, price, schedule());
}

int stock(const char *name) {
    struct item *it = find(name);
    return it == NULL ? 0 : it->stock;
}

int main(void) {
    place_order("book", "visa", 12.50);
    place_order("book", "expired", 12.50);
    place_order("lamp", "visa", 30.00);
    printf("stock of book: %d\n", stock("book"));
    return 0;
}
```

```output
ok: book, paid 12.50, parcel 1
failed: card declined, book back in stock
failed: lamp is out of stock
stock of book: 1
```

- Verdict: idiom. In C, a header file is the facade. It declares `place_order` and `stock`, and the subsystem functions are `static`. A `static` function has internal linkage, so other files cannot call it by name.
- The state of the subsystem is in `static` variables of one file. A facade with more than one shop would take a pointer to an opaque `struct shop` that only the `.c` file defines.
