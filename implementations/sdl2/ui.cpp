
#include <stdexcept>
#include "ui"

namespace ui {
    window::window(const char * title, int width, int height) {
        const int x = SDL_WINDOWPOS_CENTERED;
        const int y = SDL_WINDOWPOS_CENTERED;
        const Uint32 flags = 0;
        _sdl_window = SDL_CreateWindow(title, x, y, width, height, flags);
        if (_sdl_window == nullptr) {
            throw std::runtime_error("SDL_CreateWindow failed");
        }
    }

    window::~window() {
        if (this->_event_callback) {
            SDL_DelEventWatch(window::_on_sdl_event, static_cast<void *>(this));
        }
    }

    void window::pump_events() {
        SDL_PumpEvents();
    }

    void window::set_event_handler(std::function<void (event)> user_callback) {
        if (_event_callback && !user_callback) {
            SDL_DelEventWatch(window::_on_sdl_event, static_cast<void *>(this));
        }
        if (user_callback) {
            SDL_AddEventWatch(window::_on_sdl_event, static_cast<void *>(this));
        }
        this->_event_callback = user_callback;
    }

    int window::_on_sdl_event(void * the_window, SDL_Event * sdl_event) {
        window * w = static_cast<window *>(the_window);
        if (w->_event_callback) {
            event e;
            e._sdl_event = *sdl_event;
            w->_event_callback(std::move(e));
        }
        return 0;
    }

    key_event::key_event(const event & e) :
        _inner_event(e)
    {
    }

    std::string key_event::key_name() const {
        return SDL_GetKeyName(_inner_event._sdl_event.key.keysym.sym);
    }

    event::operator bool() const {
        return (_sdl_event.type != 0);
    }

    event_type event::type() const {
        switch (_sdl_event.type) {
            case SDL_KEYDOWN:
                return event_type::key_down;
            case SDL_KEYUP:
                return event_type::key_up;
            case SDL_QUIT:
                return event_type::quit;
            default:
                return event_type::no_event;
        }
    }

    template <>
    key_event event::get() const {
        switch (type()) {
            case event_type::key_down:
            case event_type::key_up:
                break;
            default:
                throw std::logic_error("the requested type is not set for this event");
        }
        return key_event(*this);
    }

    std::string event::name() const {
        const auto t = type();
        switch (t) {
            case ui::event_type::key_down:
                return "key_down";
            case ui::event_type::key_up:
                return "key_up";
            case ui::event_type::no_event:
                return "no_event";
            case ui::event_type::quit:
                return "quit";
        }
        return std::string();
    }

    void run(window & w) {
        while (true) {
            w.pump_events();
        }
    }
};
