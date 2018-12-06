
#ifndef __UI_WINDOW
#define __UI_WINDOW

#if __has_include(<SDL2/SDL.h>)
    #include <SDL2/SDL.h>
#elif __has_include(<SDL.h>)
    #include <SDL.h>
#else
    #error "Cannot find SDL.h"
#endif

#include <functional>
#include <string>
#include <tuple>

namespace ui {
    // enum class window_unit {
    //     system,     // perhaps in DIPs on Windows or screen coordinates on MacOS 
    // };

    struct event;
    struct key_event;
    struct pointer_event;

    struct window {
      public:
        window(const char * title, int width, int height);
        ~window();
        void set_event_handler(std::function<void (event)>);
        void pump_events();
      private:
        SDL_Window *_sdl_window = nullptr;
        std::function<void(event)> _event_callback;
        static int _on_sdl_event(void *, SDL_Event *);   // is of the function signature, SDL_EventFilter
    };

    enum class event_type {
        unknown = 0,
        key_down,
        key_up,
        pointer_down,
        pointer_move,
        pointer_up,
        quit
    };

    struct event {
      public:
        event() = default;
        operator bool() const;
        event_type type() const;
        template <typename EventSubType> const EventSubType & get() const;
        std::string name() const;
      protected:
        event(const SDL_Event & src);
        SDL_Event _sdl_event = {0};
        friend struct window;
    };

    struct key_event : public event {
      public:
        std::string key_name() const;

      private:
        friend struct window;
        inline key_event(const SDL_Event &src) : event(src) {}
    };

    struct pointer_event : public event {
      public:
        std::tuple<float,float> position() const;

      private:
        friend struct window;
        inline pointer_event(const SDL_Event &src) : event(src) {}
    };

    void run(window & w);
}

#endif // __UI_WINDOW
