* provide access to the native object(s) for
  * window
  * event
* add event types for
  * mouse, touch, and pen, perhaps via a "pointer"-styled event (ala. HTML or Win32 "pointer" events)
    * https://developer.mozilla.org/en-US/docs/Web/API/Pointer_events
    * https://docs.microsoft.com/en-us/windows/desktop/api/_inputmsg/
  * window-state changes
    * size
    * minification
    * restoration (from minification)
    * closure
  * painting/display?
  * IME text-input
* window-state
  * reading
  * modifying/requesting
  * make sure fullscreen-edness is supported!
  * what's async vs. what's not?
    * some platforms have make certain state changes be asynchonous
      * EXAMPLE: MacOS and fullscreen changes can be async
    * whither use std::future in async state-changes?
      * compatiblity with Coroutine TS?
    * might some, or all, state-getters be async?
* different numerical types may be used for measurement
  * EXAMPLE: integers on Win32
  * EXAMPLE: floating-point numbers on iOS and MacOS
* graphic-subsystem co-initialization
  * EXAMPLE: SDL offers window-creation flags, "SDL_WINDOW_VULKAN" and "SDL_WINDOW_OPENGL", which may be necessary? 
* systems with more than one 'system level' window system
  * EXAMPLE: *nix with Wayland, X11, perhaps more
  * EXAMPLE: Windows with Win32 (aka. HWND) or WinRT (aka. ICoreWindow, etc.)
* whither?:
  * APIs to get the current state of input-device(s)
    * EXAMPLES: get_keyboard_state()
    * are there ways to reduce, or eliminate, runtime-size additions, with such (especially on low-end platform(s)!) ?
    * would device-enumeration thus be necessary?
    * would device-identification thus be necessary?
  * APIs for users (or UI-toolkit-vendors!) to plug-in support for new/different/non-OS windowing systems?
    * 'struct window_system'?
    * 'concept window_system'?
  * Possiblly-singleton, 'struct application'?
  * ability to enable or disable system, or user-expected keyboard shortcuts?
    * EXAMPLE: Cmd/Ctrl+Q for quit?
