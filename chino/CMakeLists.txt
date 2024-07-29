include_directories(${CHINO_DDK_INCLUDE_DIR})
add_library(c.host OBJECT atomic_wait.cpp
                          iob.cpp
                          file.cpp
                          termios.cpp)
target_link_libraries(c.host PRIVATE c cxx-headers)
