```javascript run id=collisions
class Ship {
  get name() { return "ship"; }
  collide(other) { return other.collideWithShip(this); }
  collideWithShip(ship) { return "ships bounce"; }
  collideWithAsteroid(asteroid) { return "ship takes damage"; }
}

class Asteroid {
  get name() { return "asteroid"; }
  collide(other) { return other.collideWithAsteroid(this); }
  collideWithShip(ship) { return "ship takes damage"; }
  collideWithAsteroid(asteroid) { return "asteroids break apart"; }
}

const pairs = [
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

- Verdict: idiom. A method call dispatches on the receiver, so `collide` calls back the other object with a method named for its own class.
