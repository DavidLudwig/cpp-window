
#include <cstdio>
#include <iostream>
#include <ui.hpp>

void on_event(ui::event e) {
    if (e.type() == ui::event_type::unknown) {
        return;
    }
    std::cout << e.name();
    switch (e.type()) {
        case ui::event_type::key_down:
        case ui::event_type::key_up:
            std::cout << ": " << e.get<ui::key_event>().key_name();
            break;
        case ui::event_type::pointer_down:
        case ui::event_type::pointer_move: {
            const auto [x, y] = e.get<ui::pointer_event>().position();
            std::cout << ": position={" << x << "," << y << "}, pointer_type=";
            switch (e.get<ui::pointer_event>().pointer_type()) {
                case ui::pointer_type::unknown:
                    std::cout << "unknown";
                    break;
                case ui::pointer_type::mouse:
                    std::cout << "mouse";
                    break;
            }
            // std::cout << ": position={" << e.get<ui::pointer_event>().position().get<0>()
        } break;
        case ui::event_type::pointer_up:
        case ui::event_type::unknown:
            break;
        case ui::event_type::quit:
            exit(0);
            break;
    }
    std::cout << "\n";
}

int main() {
    // Create and display a window
    ui::window w("test", 512, 512);

    // Setup an event handler
    w.set_event_handler(on_event);

    // Run indefinitely.  This function might not return until the app
    // has finished running, or it might return right-away (Emscripten
    // should, perhaps, do this.)
    ui::run(w);

    return 0;
}
