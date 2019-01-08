
#include <limits>
#include <mutex>
#include <stdexcept>
#include "ui.hpp"

namespace ui {
    static Uint32 _get_base_sdl_user_event() {
        static std::once_flag did_init;
        static Uint32 first_event_number;
        std::call_once(did_init, [&]() {
            first_event_number = SDL_RegisterEvents(1);
        });
        return first_event_number;
    }

    constexpr Uint32 _get_sdl_user_event_offset(ui::event_type et) {
        switch (et) {
            case ui::event_type::ui_paint:
                return 0;
            default:
                break;
        }
        return std::numeric_limits<Uint32>::max();
    }

    static inline Uint32 _get_sdl_event_type_from_ui(ui::event_type et) {
        return _get_base_sdl_user_event() + _get_sdl_user_event_offset(et);
    }

    static ui::event_type _get_ui_event_type_from_sdl(Uint32 sdl_event_type) {
        const Uint32 b = _get_base_sdl_user_event();
        switch (sdl_event_type - b) {
            case 0:
                return ui::event_type::ui_paint;
            default:
                break;
        }
        return event_type::unknown;
    }

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

    template <>
    SDL_Window * window::get_native_handle() {
        return this->_sdl_window;
    }

    void window::pump_events() {
        SDL_PumpEvents();
    }

    void window::set_event_handler(std::function<void (event, window *)> user_callback) {
        if (user_callback) {
            if (!this->_event_callback) {
                SDL_AddEventWatch(window::_on_sdl_event, static_cast<void *>(this));
            }
        } else {
            if (this->_event_callback) {
                SDL_DelEventWatch(window::_on_sdl_event, static_cast<void *>(this));
            }
        }
        this->_event_callback = user_callback;
    }

    int window::_on_sdl_event(void * the_window, SDL_Event * sdl_event) {
        window * w = static_cast<window *>(the_window);
        if (w->_event_callback) {
            switch (sdl_event->type) {
                case SDL_KEYDOWN:
                case SDL_KEYUP: {
                    w->_event_callback(key_event(*sdl_event), w);
                } break;

                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONUP: {
                    w->_event_callback(pointer_event(*sdl_event), w);
                } break;

                default: {
                    w->_event_callback(event(*sdl_event), w);
                } break;
            }
        }
        return 0;
    }

    void window::_emit_ui_paint() {
        if (this->_event_callback) {
            this->_event_callback(ui_paint_event(), this);
        }
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
                break;
        }

        return _get_ui_event_type_from_sdl(_sdl_event.type);
    }

    std::string to_string(event_type t) {
        switch (t) {
            case event_type::key_down:
                return "key_down";
            case event_type::key_up:
                return "key_up";
            case event_type::pointer_down:
                return "pointer_down";
            case event_type::pointer_move:
                return "pointer_move";
            case event_type::pointer_up:
                return "pointer_up";
            case event_type::ui_paint:
                return "ui_paint";
            case event_type::unknown:
                return "unknown";
            case event_type::quit:
                return "quit";
        }
        return std::string();
    }

    std::string to_string(pointer_type t) {
        switch (t) {
            case pointer_type::unknown:
                return "unknown";
            case pointer_type::mouse:
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

    pointer_type pointer_event::pointer_type() const {
        switch (this->_sdl_event.type) {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEMOTION:
                return pointer_type::mouse;
            default:
                break;
        }
        return pointer_type::unknown;
    }

    ui_paint_event::ui_paint_event() {
        memset(&this->_sdl_event, 0, sizeof(this->_sdl_event));
        this->_sdl_event.type = _get_sdl_event_type_from_ui(ui::event_type::ui_paint);
    }

    void run(window & w) {
        while (true) {
            w.pump_events();
            w._emit_ui_paint();
        }
    }
};


namespace ui::content {
    SDL_Surface * pixel_grid_rgbx8888::_get_sdl_surface() {
        if (!this->_window) {
            return nullptr;
        }
        SDL_Window * w = this->_window->get_native_handle<SDL_Window *>();
        if (!w) {
            return nullptr;
        }
        return SDL_GetWindowSurface(w);
    }

    void pixel_grid_rgbx8888::attach(ui::window & w) {
        this->_window = &w;
    }

    uint8_t * pixel_grid_rgbx8888::data() {
        SDL_Surface * s = this->_get_sdl_surface();
        if (!s) {
            return nullptr;
        }
        return (uint8_t*)s->pixels;
    }

    std::tuple<int,int> pixel_grid_rgbx8888::dimensions() {
        SDL_Surface * s = this->_get_sdl_surface();
        if (!s) {
            return std::make_tuple(0,0);
        }
        return std::make_tuple(s->w, s->h);
    }

    size_t pixel_grid_rgbx8888::size() {
        SDL_Surface * s = this->_get_sdl_surface();
        if (!s) {
            return 0;
        }
        return s->h * s->pitch;
    }

    int pixel_grid_rgbx8888::pitch() {
        SDL_Surface * s = this->_get_sdl_surface();
        if (!s) {
            return 0;
        }
        return s->pitch;
    }

    void pixel_grid_rgbx8888::update() {
        if (!this->_window) {
            return;
        }
        SDL_Window * w = this->_window->get_native_handle<SDL_Window *>();
        if (!w) {
            return;
        }
        SDL_UpdateWindowSurface(w);
    }
};
