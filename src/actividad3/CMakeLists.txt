cmake_minimum_required(VERSION 3.8)
project(actividad3)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(actividad3_interfaces REQUIRED)

add_executable(emulator src/Node1.cpp)
ament_target_dependencies(emulator rclcpp actividad3_interfaces)

add_executable(monitor src/Node2.cpp)
ament_target_dependencies(monitor rclcpp actividad3_interfaces)

add_executable(operator src/Node3.cpp)
ament_target_dependencies(operator rclcpp actividad3_interfaces)

install(TARGETS
  emulator
  monitor
  operator
  DESTINATION lib/${PROJECT_NAME})

ament_package()