
#include <cstdio>
#include <iostream>
#include <ui.hpp>

void on_event(ui::event e, ui::window * w) {
    switch (e.type()) {
        case ui::event_type::unknown:
        case ui::event_type::ui_paint:
            return;
        default:
            break;
    }
    std::cout << ui::to_string(e.type());
    switch (e.type()) {
        case ui::event_type::key_down:
        case ui::event_type::key_up:
            std::cout << ": " << e.get<ui::key_event>().key_name();
            break;
        case ui::event_type::pointer_down:
        case ui::event_type::pointer_move:
        case ui::event_type::pointer_up: {
            const ui::pointer_event & pe = e.get<ui::pointer_event>();
            const auto [x, y] = pe.position();
            std::cout << ": position={" << x << "," << y << "}, pointer_type=" << ui::to_string(pe.pointer_type());
        } break;
        case ui::event_type::ui_paint:
            break;
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

    // Run indefinitely
    ui::run(w);

    return 0;
}
