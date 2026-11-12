```typescript run id=collisions
interface Body {
  name: string;
  collide(other: Body): string;
  collideWithShip(ship: Ship): string;
  collideWithAsteroid(asteroid: Asteroid): string;
}

class Ship implements Body {
  name = "ship";
  collide(other: Body): string { return other.collideWithShip(this); }
  collideWithShip(_ship: Ship): string { return "ships bounce"; }
  collideWithAsteroid(_asteroid: Asteroid): string { return "ship takes damage"; }
}

class Asteroid implements Body {
  name = "asteroid";
  collide(other: Body): string { return other.collideWithAsteroid(this); }
  collideWithShip(_ship: Ship): string { return "ship takes damage"; }
  collideWithAsteroid(_asteroid: Asteroid): string { return "asteroids break apart"; }
}

const pairs: [Body, Body][] = [
  [new Ship(), new Asteroid()],
  [new Asteroid(), new Ship()],
  [new Asteroid(), new Asteroid()],
  [new Ship(), new Ship()],
];
for (const [a, b] of pairs) {
  console.log(`${a.name} + ${b.name}: ${a.collide(b)}`);
}
```

```output
ship + asteroid: ship takes damage
asteroid + ship: ship takes damage
asteroid + asteroid: asteroids break apart
ship + ship: ships bounce
```

- Verdict: idiom. The interface `Body` lists one callback method for each class, and each class implements all of them.
