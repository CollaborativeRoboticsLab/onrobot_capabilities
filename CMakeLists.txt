cmake_minimum_required(VERSION 3.8)
project(onrobot_capabilities)

find_package(ament_cmake REQUIRED)
find_package(capabilities2_events REQUIRED)
find_package(capabilities2_runner REQUIRED)
find_package(onrobot_msgs REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)

add_library(${PROJECT_NAME} SHARED src/vg_command_runner.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(${PROJECT_NAME}
  capabilities2_events
  capabilities2_runner
  onrobot_msgs
  pluginlib
  rclcpp
  rclcpp_action
)
pluginlib_export_plugin_description_file(capabilities2_runner plugins.xml)

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)

install(DIRECTORY docs interfaces plans providers semantic_interfaces
  DESTINATION share/${PROJECT_NAME}
)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})
ament_export_dependencies(capabilities2_events capabilities2_runner onrobot_msgs rclcpp rclcpp_action)

ament_package()