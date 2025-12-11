cmake_minimum_required(VERSION 3.22)
project(path_find_examples)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_compile_options(-std=c++2a -Wpedantic -Wall)

option(DEBUG "More output while parsing yaml" OFF)

if(DEBUG)
  add_definitions(-DDEBUG)
endif()

include_directories(include)

set(vectorMap_include
  include/utils/vector_map_app/main_window.hpp
  include/utils/vector_map_app/map.hpp
  include/utils/vector_map_app/primitives.hpp
  include/utils/vector_map_app/settings_dock.hpp
  include/utils/vector_map_app/tools_dock.hpp
)

set(vectorMap_src
  src/utils/vector_map_app/main_window.cpp
  src/utils/vector_map_app/map.cpp
  src/utils/vector_map_app/settings_dock.cpp
  src/utils/vector_map_app/tools_dock.cpp
  src/utils/vector_map_app/primitives.cpp
)

qt5_wrap_cpp(vectorMap_moc_src ${vectorMap_include})

add_library(vectorMap_lib
  ${vectorMap_moc_src}
  ${vectorMap_src}
)

set(dependencies
  rclcpp
  std_msgs
  std_srvs
  geometry_msgs
  ament_index_cpp
)

ament_target_dependencies(vectorMap_lib PUBLIC ${dependencies})
target_link_libraries(vectorMap_lib PRIVATE Qt5::Widgets)

add_executable(vectorMap_app src/utils/vector_map_app/main.cpp)
target_link_libraries(vectorMap_app PRIVATE Qt5::Widgets
                                    PUBLIC vectorMap_lib)

install(
  DIRECTORY resources
  DESTINATION share/${PROJECT_NAME}
)

install(TARGETS vectorMap_app vectorMap_lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib/${PROJECT_NAME}
)

ament_package()