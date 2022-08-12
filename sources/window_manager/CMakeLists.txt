add_library(
  window_manager
  window_manager.hpp
  window_manager.cpp)

target_include_directories(window_manager PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
  logger
  util
)

target_link_libraries(window_manager PUBLIC
  logger
  util
)
