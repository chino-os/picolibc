include_directories(${CHINO_DDK_INCLUDE_DIR})
set(SRCS atomic_wait.cpp
         iob.cpp
         file.cpp
         termios.cpp)

add_library(c.host OBJECT ${SRCS})
target_link_libraries(c.host PRIVATE c cxx-headers)

add_library(c.kernel_host OBJECT ${SRCS})
target_link_libraries(c.kernel_host PRIVATE c cxx-headers)
target_compile_definitions(c.kernel_host PUBLIC CHINO_CRT_KERNEL=1)
