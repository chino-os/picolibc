include_directories(${CHINO_DDK_INCLUDE_DIR})
add_library(c.host OBJECT iob.cpp file.cpp)
target_link_libraries(c.host PRIVATE c cxx-headers)
