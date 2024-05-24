add_executable (detoursdemo_simple)

target_sources(
  detoursdemo_simple
  PRIVATE
  src/main.cpp
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET detoursdemo_simple PROPERTY CXX_STANDARD 20)
endif()


