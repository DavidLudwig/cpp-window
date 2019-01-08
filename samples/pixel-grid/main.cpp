
#include <cstdio>
#include <iostream>
#include <ui.hpp>

static ui::content::pixel_grid_rgbx8888 content;

void on_event(ui::event e, ui::window * w) {
    switch (e.type()) {
        case ui::event_type::ui_paint: {
            std::tuple<int,int> dims = content.dimensions();
            uint8_t * pixels = content.data();
            int pitch = content.pitch();
            for (int y = 0; y < std::get<1>(dims); ++y) {
                std::fill_n(pixels, pitch, (y % 256));
                pixels += pitch;
            }
            content.update();
            break;
        }
        case ui::event_type::quit:
            exit(0);
            break;
        default:
            break;
    }
}

int main() {
    // Create and display a window
    ui::window w("test", 512, 512);

    // Setup an event handler
    w.set_event_handler(on_event);

    // Add content
    content.attach(w);

    // Run indefinitely.  This function might not return until the app
    // has finished running, or it might return right-away (Emscripten
    // should, perhaps, do this.)
    ui::run(w);

    return 0;
}
