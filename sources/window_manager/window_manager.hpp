#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

extern "C" {
  #include <X11/Xlib.h>
}

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <util.hpp>

using ::std::string;

class WindowManager {
 public:
  // Factory method for establishing a connection to an X server and creating a WindowManager instance.
  static ::std::unique_ptr<WindowManager> Create();

  // Disconnects from the X server.
  ~WindowManager();

  // The entry point to this class. Enters the main event loop.
  void Run();

 private:
  WindowManager(Display* display);
  void Frame(Window w, bool was_created_before_window_manager);
  void Unframe(Window w);

  void OnCreateNotify(const XCreateWindowEvent& e);
  void OnDestroyNotify(const XDestroyWindowEvent& e);
  void OnReparentNotify(const XReparentEvent& e);
  void OnMapRequest(const XMapRequestEvent& e);

  // Xlib error handler. It must be static as its address is passed to Xlib.
  static int OnXError(Display* display, XErrorEvent* e);

  // Xlib error handler used to determine whether another window manager is
  // running. It is set as the error handler right before selecting substructure
  // redirection mask on the root window, so it is invoked if and only if
  // another window manager is running. It must be static as its address is passed to Xlib.
  static int OnWMDetected(Display* display, XErrorEvent* e);

  Display* display_;
  const Window root_;

  // Whether an existing window manager has been detected. Set by OnWMDetected, and hence must be static.
  static bool wm_detected_;
  static ::std::mutex wm_detected_mutex_;

  // Maps top-level windows to their frame windows.
  ::std::unordered_map<Window, Window> clients_;

  // The cursor position at the start of a window move/resize.
  Position<int> drag_start_pos_;

  // The position of the affected window at the start of a window move/resize.
  Position<int> drag_start_frame_pos_;

  // The size of the affected window at the start of a window move/resize.
  Size<int> drag_start_frame_size_;

  const Atom WM_PROTOCOLS;
  const Atom WM_DELETE_WINDOW;
};

#endif
