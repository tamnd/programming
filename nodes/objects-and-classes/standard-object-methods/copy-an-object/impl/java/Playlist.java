// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

import java.util.ArrayList;
import java.util.List;

/** A playlist with a list field, copied in two ways. */
public final class Playlist implements Cloneable {
  private String name;
  private final List<String> songs;

  public Playlist(String name, List<String> songs) {
    this.name = name;
    this.songs = songs;
  }

  /** A deep copy: a copy constructor that makes a new list. */
  public Playlist(Playlist other) {
    this(other.name, new ArrayList<>(other.songs));
  }

  /** A shallow copy: Object.clone copies the fields, so the copy shares the list. */
  @Override
  public Playlist clone() {
    try {
      return (Playlist) super.clone();
    } catch (CloneNotSupportedException e) {
      throw new AssertionError(e);
    }
  }

  public String name() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }

  /** Returns the list itself, not a copy, so that a caller can see the sharing. */
  public List<String> songs() {
    return songs;
  }

  public static List<List<String>> copyAndChange(List<String> songs, int index, String title) {
    Playlist original = new Playlist("mix", new ArrayList<>(songs));
    Playlist shallow = original.clone();
    Playlist deep = new Playlist(original);
    original.songs().set(index, title);
    return List.of(List.copyOf(original.songs()), List.copyOf(shallow.songs()), List.copyOf(deep.songs()));
  }
}
