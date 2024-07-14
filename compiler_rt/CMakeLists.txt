if (MSVC)
  target_sources(c PRIVATE msvc.cpp)
  target_include_directories(c PRIVATE ${WIN_SDK_INCLUDE_DIR})
endif()
