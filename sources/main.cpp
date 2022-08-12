#include <cstdlib>
#include <logger.hpp>
#include <window_manager.hpp>

using ::std::unique_ptr;

int main(int argc, char** argv) {
  loguru::init(argc, argv);

  unique_ptr<WindowManager> window_manager = WindowManager::Create();
  if (!window_manager) {
    LOG_F(ERROR, "Failed to initialize window manager.");
    return EXIT_FAILURE;
  }

  window_manager->Run();

  return EXIT_SUCCESS;
}
