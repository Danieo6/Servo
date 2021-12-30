add_library(Servo INTERFACE)

target_sources(Servo INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/Servo.cpp
)

target_include_directories(Servo INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(Servo INTERFACE pico_stdlib hardware_pwm)
