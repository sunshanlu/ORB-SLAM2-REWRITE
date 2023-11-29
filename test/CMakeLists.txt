include_directories(${PROJECT_SOURCE_DIR}/include /usr/include/eigen3)

file(GLOB SRC_FILE ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SRC_FILE ${PROJECT_SOURCE_DIR}/src/main.cpp)

add_executable(path_test ${CMAKE_CURRENT_SOURCE_DIR}/path_test.cpp ${SRC_FILE})
target_link_libraries(path_test ${DEPEND_LIBRARIES})

add_executable(copyMakeBorder_test
               ${CMAKE_CURRENT_SOURCE_DIR}/copyMakeBorder_test.cpp ${SRC_FILE})
target_link_libraries(copyMakeBorder_test ${DEPEND_LIBRARIES})

add_executable(gaussianPyramid_test
               ${PROJECT_SOURCE_DIR}/test/gaussianPyramid_test.cpp ${SRC_FILE})
target_link_libraries(gaussianPyramid_test ${DEPEND_LIBRARIES})

add_executable(detectFAST_test ${PROJECT_SOURCE_DIR}/test/detectFAST_test.cpp
                               ${SRC_FILE})
target_link_libraries(detectFAST_test ${DEPEND_LIBRARIES})

add_executable(SIMD_test ${PROJECT_SOURCE_DIR}/test/SIMD_test.cpp ${SRC_FILE})
target_link_libraries(SIMD_test ${DEPEND_LIBRARIES})

add_executable(drawCircle ${PROJECT_SOURCE_DIR}/test/drawCircle.cpp ${SRC_FILE})
target_link_libraries(drawCircle ${DEPEND_LIBRARIES})

add_executable(keyPointAngle_test
               ${PROJECT_SOURCE_DIR}/test/keyPointAngle_test.cpp ${SRC_FILE})
target_link_libraries(keyPointAngle_test ${DEPEND_LIBRARIES})

add_executable(descriptor_test ${PROJECT_SOURCE_DIR}/test/descriptor_test.cpp
                               ${SRC_FILE})
target_link_libraries(descriptor_test ${DEPEND_LIBRARIES})

add_executable(matConstruct_test
               ${PROJECT_SOURCE_DIR}/test/matConstruct_test.cpp ${SRC_FILE})
target_link_libraries(matConstruct_test ${DEPEND_LIBRARIES})

add_executable(quadTree_test ${PROJECT_SOURCE_DIR}/test/quadTree_test.cpp
                             ${SRC_FILE})

target_link_libraries(quadTree_test ${DEPEND_LIBRARIES})

add_executable(cvRowColRange_test
               ${PROJECT_SOURCE_DIR}/test/cvRowColRange_test.cpp ${SRC_FILE})
target_link_libraries(cvRowColRange_test ${DEPEND_LIBRARIES})

add_executable(image2cells_test ${PROJECT_SOURCE_DIR}/test/image2cells_test.cpp
                                ${SRC_FILE})
target_link_libraries(image2cells_test ${DEPEND_LIBRARIES})

add_executable(cvFast_test ${PROJECT_SOURCE_DIR}/test/cvFast_test.cpp
                           ${SRC_FILE})
target_link_libraries(cvFast_test ${DEPEND_LIBRARIES})

add_executable(detectORB_test ${PROJECT_SOURCE_DIR}/test/detectORB_test.cpp
                              ${SRC_FILE})
target_link_libraries(detectORB_test ${DEPEND_LIBRARIES})

# 检查 AVX 支持
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_AVX)

if(COMPILER_SUPPORTS_AVX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
        message(STATUS "AVX supported, enabling AVX flags")
else()
        message(STATUS "AVX not supported, no AVX flags enabled")
endif()
