```javascript wrong id=typed-array
const scores = new Int32Array(4);
scores[2] = 10;
scores[4] = 99;
console.log(scores.length, scores[4], Array.from(scores));

const list = [0, 0, 0, 0];
list[6] = 99;
console.log(list.length, list);
```

```output for=typed-array
4 undefined [ 0, 0, 10, 0 ]
7 [ 0, 0, 0, 0, <2 empty items>, 99 ]
```

Neither write corrupts memory. The typed array keeps its length and drops the write past the end without an error. The plain array grows to fit the index and fills the gap with empty slots. Both are silent, so a bug in the index stays hidden.

```javascript run id=check
function setScore(scores, player, score) {
  if (!Number.isInteger(player) || player < 0 || player >= scores.length) {
    throw new RangeError(`player ${player} is out of range, the size is ${scores.length}`);
  }
  scores[player] = score;
}

const scores = new Int32Array(4);
setScore(scores, 2, 10);
console.log("player 2:", scores[2]);
try {
  setScore(scores, 4, 99);
} catch (error) {
  console.log("refused:", error.message);
}
```

```output for=check
player 2: 10
refused: player 4 is out of range, the size is 4
```

- Verdict: native. The engine checks each index into a typed array against its `ArrayBuffer`, so script cannot write outside the buffer.
- The language does not report a bad index. Check it yourself, as the second example does.
- Out-of-bounds writes still happen below the script, in the engine and in native add-ons, which are written in C and C++. CVE-2023-4863, in the libwebp image library, let a crafted HTML page write out of bounds in Google Chrome.
