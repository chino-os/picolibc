include_directories(${CHINO_DDK_INCLUDE_DIR})
set(SRCS atomic_wait.cpp
         iob.cpp
         file.cpp
         termios.cpp
         time.cpp)

add_library(c.host OBJECT ${SRCS})
target_link_libraries(c.host PRIVATE c ${CHINO_LIBCXX_STATIC})

add_library(c.kernel_host OBJECT ${SRCS})
target_link_libraries(c.kernel_host PRIVATE c ${CHINO_LIBCXX_HEADERS})
target_compile_definitions(c.kernel_host PUBLIC CHINO_CRT_KERNEL=1)
