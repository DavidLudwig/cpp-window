
#include <stdexcept>
#include "ui.hpp"

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
            switch (sdl_event->type) {
                case SDL_KEYDOWN:
                case SDL_KEYUP: {
                    w->_event_callback(key_event(*sdl_event));
                } break;

                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONUP: {
                    w->_event_callback(pointer_event(*sdl_event));
                } break;

                default: {
                    w->_event_callback(event(*sdl_event));
                } break;
            }
        }
        return 0;
    }

    event::event(const SDL_Event &src) : _sdl_event(src) {
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
            case SDL_MOUSEBUTTONDOWN:
                return event_type::pointer_down;
            case SDL_MOUSEMOTION:
                return event_type::pointer_move;
            case SDL_MOUSEBUTTONUP:
                return event_type::pointer_up;
            case SDL_QUIT:
                return event_type::quit;
            default:
                return event_type::unknown;
        }
    }

    std::string to_string(event_type t) {
        switch (t) {
            case ui::event_type::key_down:
                return "key_down";
            case ui::event_type::key_up:
                return "key_up";
            case ui::event_type::pointer_down:
                return "pointer_down";
            case ui::event_type::pointer_move:
                return "pointer_move";
            case ui::event_type::pointer_up:
                return "pointer_up";
            case ui::event_type::unknown:
                return "unknown";
            case ui::event_type::quit:
                return "quit";
        }
        return std::string();
    }

    std::string to_string(pointer_type t) {
        switch (t) {
            case ui::pointer_type::unknown:
                return "unknown";
            case ui::pointer_type::mouse:
                return "mouse";
        }
        return std::string();
    }

    template <>
    const key_event & event::get() const {
        switch (type()) {
            case event_type::key_down:
            case event_type::key_up:
                break;
            default:
                throw std::logic_error("the requested type is not set for this event");
        }
        return static_cast<const key_event &>(*this);
    }

    std::string key_event::key_name() const {
        return SDL_GetKeyName(this->_sdl_event.key.keysym.sym);
    }

    template <>
    const pointer_event & event::get() const {
        switch (type()) {
            case event_type::pointer_down:
            case event_type::pointer_move:
            case event_type::pointer_up:
                break;
            default:
                throw std::logic_error("the requested type is not set for this event");
        }
        return static_cast<const pointer_event &>(*this);
    }

    std::tuple<float,float> pointer_event::position() const {
        switch (this->_sdl_event.type) {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                return std::make_tuple(
                    (float) this->_sdl_event.button.x,
                    (float) this->_sdl_event.button.y
                );
            case SDL_MOUSEMOTION:
                return std::make_tuple(
                    (float) this->_sdl_event.motion.x,
                    (float) this->_sdl_event.motion.y
                );
        }
        return std::make_tuple(0.f,0.f);
    }

    ui::pointer_type pointer_event::pointer_type() const {
        switch (this->_sdl_event.type) {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEMOTION:
                return ui::pointer_type::mouse;
            default:
                break;
        }
        return ui::pointer_type::unknown;
    }

    void run(window & w) {
        while (true) {
            w.pump_events();
        }
    }
};
