add_executable(${NAME} main.cpp)

set(CMAKE_CXX_FLAGS "-Wall -lstdc++fs -lpthread -ldl")
set(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g -lstdc++fs -lpthread -ldl")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -lstdc++fs -lpthread -ldl")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -lstdc++fs -lpthread -ldl")

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})
link_directories(${X11_LIBRARIES})
target_link_libraries(lucien_wm PUBLIC ${X11_LIBRARIES})

add_subdirectory(util)
add_subdirectory(logger)
add_subdirectory(window_manager)

target_link_libraries(${NAME} PUBLIC util)
target_link_libraries(${NAME} PUBLIC logger)
target_link_libraries(${NAME} PUBLIC window_manager)
