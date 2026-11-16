// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

export class Playlist {
  constructor(name, songs) {
    this.name = name;
    this.songs = songs;
  }

  // A shallow copy: a new Playlist whose songs field refers to the same array.
  shallowCopy() {
    return new Playlist(this.name, this.songs);
  }

  // A deep copy: a new Playlist with its own copy of the array.
  deepCopy() {
    return new Playlist(this.name, structuredClone(this.songs));
  }
}

export function copyAndChange(songs, index, title) {
  if (!Number.isInteger(index) || index < 0 || index >= songs.length) {
    throw new RangeError(`no song at index ${index}`);
  }
  const original = new Playlist("mix", [...songs]);
  const shallow = original.shallowCopy();
  const deep = original.deepCopy();
  original.songs[index] = title;
  return [[...original.songs], [...shallow.songs], [...deep.songs]];
}
