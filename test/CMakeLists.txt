include_directories(${PROJECT_SOURCE_DIR}/include /usr/include/eigen3)

add_executable(path_test ${CMAKE_CURRENT_SOURCE_DIR}/path_test.cpp)
target_link_libraries(path_test ${DEPEND_LIBRARIES})
message(STATUS ${DEPEND_LIBRARIES})
