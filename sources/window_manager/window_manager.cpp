#include "window_manager.hpp"

//extern "C" {
//#include <X11/Xutil.h>
//}

#include <cstring>
#include <algorithm>

#include <logger.hpp>
#include <util.hpp>

using ::std::max;
using ::std::mutex;
using ::std::string;
using ::std::unique_ptr;

bool WindowManager::wm_detected_;
mutex WindowManager::wm_detected_mutex_;

unique_ptr<WindowManager> WindowManager::Create()
{
  // 1. Open X display.
  //const char* display_c_str = display_str.empty() ? nullptr : display_str.c_str();
  Display* display = XOpenDisplay(nullptr);
  if (nullptr == display) {
    //LOG(ERROR) << "Failed to open X display " << XDisplayName(nullptr);
    return nullptr;
  }
  // 2. Construct WindowManager instance.
  return unique_ptr<WindowManager>(new WindowManager(display));
}

WindowManager::WindowManager(Display* display)
    : display_(display),
      root_(DefaultRootWindow(display_)),
      WM_PROTOCOLS(XInternAtom(display_, "WM_PROTOCOLS", false)),
      WM_DELETE_WINDOW(XInternAtom(display_, "WM_DELETE_WINDOW", false))
{
}

WindowManager::~WindowManager()
{
  XCloseDisplay(display_);
}

void WindowManager::Run()
{
  // 1. Initialization.
  //   a. Select events on root window. Use a special error handler so we can
  //   exit gracefully if another window manager is already running.
  wm_detected_ = false;
  XSetErrorHandler(&WindowManager::OnWMDetected);
  XSelectInput(
      display_,
      root_,
      SubstructureRedirectMask | SubstructureNotifyMask);

  XSync(display_, false);
  if (wm_detected_)
  {
    //LOG(ERROR) << "Detected another window manager on display "
     //          << XDisplayString(display_);
    return;
  }

  //   b. Set error handler.
  XSetErrorHandler(&WindowManager::OnXError);

  // 2. Main event loop.
  for (;;) {
    // 1. Get next event.
    XEvent e;
    XNextEvent(display_, &e);
    //LOG(INFO) << "Received event: " << ToString(e);

    // 2. Dispatch event.
    switch (e.type) {
      case CreateNotify:
        OnCreateNotify(e.xcreatewindow);
        break;
      case DestroyNotify:
        OnDestroyNotify(e.xdestroywindow);
        break;
      case ReparentNotify:
        OnReparentNotify(e.xreparent);
        break;
     case MapRequest:
        OnMapRequest(e.xmaprequest);
        break;
      //default:

        //LOG_F(INFO);
        //LOG(WARNING) << "Ignored event";
    }
  }
}

void WindowManager::OnCreateNotify(const XCreateWindowEvent& e)
{

}

void WindowManager::OnDestroyNotify(const XDestroyWindowEvent& e)
{

}

void WindowManager::OnReparentNotify(const XReparentEvent& e)
{

}

void WindowManager::OnMapRequest(const XMapRequestEvent& e)
{
  // 1. Frame or re-frame window.
  Frame(e.window, false);

  // 2. Actually map window.
  XMapWindow(display_, e.window);
}

void WindowManager::Frame(Window w, bool was_created_before_window_manager) {
  // Visual properties of the frame to create.
  const unsigned int BORDER_WIDTH = 3;
  const unsigned long BORDER_COLOR = 0xff0000;
  const unsigned long BG_COLOR = 0x0000ff;

  // 1. Retrieve attributes of window to frame.
  XWindowAttributes x_window_attrs;
  //CHECK(XGetWindowAttributes(display_, w, &x_window_attrs));

  // 2. TODO - see Framing Existing Top-Level Windows section below.

  // 3. Create frame.
  const Window frame = XCreateSimpleWindow(
      display_,
      root_,
      x_window_attrs.x,
      x_window_attrs.y,
      x_window_attrs.width,
      x_window_attrs.height,
      BORDER_WIDTH,
      BORDER_COLOR,
      BG_COLOR);
  // 3. Select events on frame.
  XSelectInput(
      display_,
      frame,
      SubstructureRedirectMask | SubstructureNotifyMask);
  // 4. Add client to save set, so that it will be restored and kept alive if we
  // crash.
  XAddToSaveSet(display_, w);
  // 5. Reparent client window.
  XReparentWindow(
      display_,
      w,
      frame,
      0, 0);  // Offset of client window within frame.
  // 6. Map frame.
  XMapWindow(display_, frame);

  // 7. Save frame handle.

  //clients_[w] = frame;
  //// 8. Grab events for window management actions on client window.
  ////   a. Move windows with alt + left button.
  //XGrabButton(...);
  ////   b. Resize windows with alt + right button.
  //XGrabButton(...);
  ////   c. Kill windows with alt + f4.
  //XGrabKey(...);
  ////   d. Switch windows with alt + tab.
  //XGrabKey(...);

  //LOG(INFO) << "Framed window " << w << " [" << frame << "]";
}

int WindowManager::OnWMDetected(Display* display, XErrorEvent* e)
{
  // In the case of an already running window manager, the error code from
  // XSelectInput is BadAccess. We don't expect this handler to receive any
  // other errors.
  //CHECK_EQ(static_cast<int>(e->error_code), BadAccess);
  // Set flag.
  wm_detected_ = true;
  // The return value is ignored.
  return 0;
}

int WindowManager::OnXError(Display* display, XErrorEvent* e)
{
  return 0;
}
