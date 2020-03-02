cmake_minimum_required(VERSION 2.8.3)

project(dbc_converter)

set (CMAKE_CXX_FLAGS "-fPIC --std=c++11")

find_package(catkin REQUIRED COMPONENTS
)

catkin_package(
    CATKIN_DEPENDS)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)
add_executable(converter src/main.c src/2c.c src/can.c src/mpc.c src/parse.c src/util.c src/getopt.c)

# target_link_libraries(aeva_node ${catkin_LIBRARIES} AevaAPI ${PCL_LIBRARIES} func draco libstdc++.so.6)


install(TARGETS converter
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# install(DIRECTORY launch DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})