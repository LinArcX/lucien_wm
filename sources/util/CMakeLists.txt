add_library(
  util
  util.hpp
  util.cpp)

target_include_directories(util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
#target_link_libraries(util PUBLIC)
