include_directories(${PROJECT_SOURCE_DIR}/include /usr/include/eigen3)

add_executable(path_test ${CMAKE_CURRENT_SOURCE_DIR}/path_test.cpp)
target_link_libraries(path_test ${DEPEND_LIBRARIES})

add_executable(copyMakeBorder_test
               ${CMAKE_CURRENT_SOURCE_DIR}/copyMakeBorder_test.cpp)
target_link_libraries(copyMakeBorder_test ${DEPEND_LIBRARIES})

add_executable(
        gaussianPyramid_test
        ${PROJECT_SOURCE_DIR}/src/Config.cpp
        ${PROJECT_SOURCE_DIR}/src/ORBFeature.cpp
        ${PROJECT_SOURCE_DIR}/src/ORBDetector.cpp
        ${PROJECT_SOURCE_DIR}/src/CircleID.cpp
        ${PROJECT_SOURCE_DIR}/test/gaussianPyramid_test.cpp)
target_link_libraries(gaussianPyramid_test ${DEPEND_LIBRARIES})

add_executable(
        detectFAST_test
        ${PROJECT_SOURCE_DIR}/src/Config.cpp
        ${PROJECT_SOURCE_DIR}/src/ORBFeature.cpp
        ${PROJECT_SOURCE_DIR}/src/ORBDetector.cpp
        ${PROJECT_SOURCE_DIR}/src/CircleID.cpp
        ${PROJECT_SOURCE_DIR}/test/detectFAST_test.cpp)
target_link_libraries(detectFAST_test ${DEPEND_LIBRARIES})

add_executable(
        SIMD_test
        ${PROJECT_SOURCE_DIR}/test/SIMD_test.cpp
        ${PROJECT_SOURCE_DIR}/src/Config.cpp
        ${PROJECT_SOURCE_DIR}/src/CircleID.cpp
        ${PROJECT_SOURCE_DIR}/src/ORBDetector.cpp
        ${PROJECT_SOURCE_DIR}/src/ORBFeature.cpp)
target_link_libraries(SIMD_test ${DEPEND_LIBRARIES})

# 检查 AVX 支持
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_AVX)

if(COMPILER_SUPPORTS_AVX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
        message(STATUS "AVX supported, enabling AVX flags")
else()
        message(STATUS "AVX not supported, no AVX flags enabled")
endif()
