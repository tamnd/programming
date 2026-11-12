```c run id=one-struct-per-unit
#include <stdio.h>

/* One struct per unit: the compiler rejects a feet value where metres
   are expected, because the two struct types are different. */
typedef struct { double v; } metres;
typedef struct { double v; } feet;
typedef struct { double v; } seconds;

static metres from_feet(feet f) { return (metres){f.v * 0.3048}; }
static metres add_m(metres a, metres b) { return (metres){a.v + b.v}; }
static double speed_mps(metres d, seconds t) { return d.v / t.v; }

/* The common C idiom instead: plain numbers with the unit in the name */
static void set_timeout_ms(long timeout_ms) { printf("timeout %ld ms\n", timeout_ms); }

int main(void) {
    metres trip = add_m((metres){5000}, from_feet((feet){1000}));
    printf("trip %.1f m, speed %.3f m/s\n", trip.v, speed_mps(trip, (seconds){1800}));
    long timeout_s = 30;
    set_timeout_ms(timeout_s * 1000);
    set_timeout_ms(timeout_s); /* compiles: the name is only a convention */
    return 0;
}
```

```output for=one-struct-per-unit
trip 5304.8 m, speed 2.947 m/s
timeout 30000 ms
timeout 30 ms
```

- Verdict: idiom. C has no units. A struct per unit makes the compiler reject one unit where another is expected, at the cost of a function for each operation, because C has no operator overloading. The common idiom is a plain number with the unit in its name, such as `timeout_ms`, which nothing checks: the second call passes seconds where milliseconds are expected.
