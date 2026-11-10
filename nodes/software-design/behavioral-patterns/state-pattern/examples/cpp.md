```cpp run id=player
#include <iostream>
#include <string_view>
#include <utility>

struct State;
using Transition = std::pair<const State*, std::string_view>;

// The base class gives the default for each event: stay and do nothing.
struct State {
    virtual ~State() = default;
    virtual std::string_view name() const = 0;
    virtual Transition play() const { return {this, "ignored"}; }
    virtual Transition pause() const { return {this, "ignored"}; }
    virtual Transition stop() const { return {this, "ignored"}; }
};

struct Stopped final : State {
    std::string_view name() const override { return "stopped"; }
    Transition play() const override;
};

struct Playing final : State {
    std::string_view name() const override { return "playing"; }
    Transition pause() const override;
    Transition stop() const override;
};

struct Paused final : State {
    std::string_view name() const override { return "paused"; }
    Transition play() const override;
    Transition stop() const override;
};

// One shared object for each state. They hold no data, so all players can use them.
const Stopped stopped{};
const Playing playing{};
const Paused paused{};

Transition Stopped::play() const { return {&playing, "start at 0:00"}; }
Transition Playing::pause() const { return {&paused, "hold the position"}; }
Transition Playing::stop() const { return {&stopped, "rewind"}; }
Transition Paused::play() const { return {&playing, "resume"}; }
Transition Paused::stop() const { return {&stopped, "rewind"}; }

class Player {
public:
    void play() { change("play", state_->play()); }
    void pause() { change("pause", state_->pause()); }
    void stop() { change("stop", state_->stop()); }

private:
    void change(std::string_view event, Transition t) {
        std::cout << event << ": " << t.second << " (" << state_->name() << " -> "
                  << t.first->name() << ")\n";
        state_ = t.first;
    }

    const State* state_ = &stopped;
};

int main() {
    Player player;
    player.play();
    player.pause();
    player.play();
    player.stop();
    player.pause();
}
```

```output
play: start at 0:00 (stopped -> playing)
pause: hold the position (playing -> paused)
play: resume (paused -> playing)
stop: rewind (playing -> stopped)
pause: ignored (stopped -> stopped)
```

- Verdict: idiom. This is the class structure of Design Patterns: an abstract `State` with a virtual function for each event, and one subclass for each state.
- The state objects are `const` globals, and the player holds a pointer to one of them. No state is made or destroyed at run time.
- The member functions are defined after the three objects, because each state refers to the others. A declaration of each class comes first, and the definitions follow.
- `std::variant` with `std::visit` is the other common form. It keeps the states in one closed type, as the Rust enum does.
