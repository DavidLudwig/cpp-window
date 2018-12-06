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
* window-state retrieval
* window-state storage
* graphic-subsystem co-initialization
* whither?:
  * API(s) to get the current state of input-device(s)
    * EXAMPLES: get_keyboard_state()
    * are there ways to reduce, or eliminate, runtime-size additions, with such (especially on low-end platform(s)!) ?
    * would device-enumeration thus be necessary?
    * would device-identification thus be necessary?
