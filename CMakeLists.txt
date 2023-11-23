cmake_minimum_required(VERSION 3.18)
project(ORB-SLAME2-REWRITE CXX)

set(DEPEND_LIBRARIES depend_libraries)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include_directories(${PROJECT_SOURCE_DIR}/include /usr/include/eigen3)

find_package(FMT REQUIRED)
find_package(OpenCV 3.4.16 REQUIRED)
find_package(Eigen3 REQUIRED) # 头文件库
find_package(Sophus REQUIRED) # 头文件库
find_package(jsoncpp REQUIRED)
find_package(G2O REQUIRED)
find_package(spdlog REQUIRED)
find_package(Pangolin REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp REQUIRED IMPORTED_TARGET)

add_library(${DEPEND_LIBRARIES} INTERFACE)
target_link_libraries(
        ${DEPEND_LIBRARIES}
        INTERFACE fmt::fmt
        INTERFACE ${OpenCV_LIBS}
        INTERFACE spdlog::spdlog
        INTERFACE ${Pangolin_LIBRARY}
        INTERFACE PkgConfig::JSONCPP
        INTERFACE ${G2O_LIBRARIES})

# 注意add_subdirectory的位置，应该在需要依赖target的下方
add_subdirectory(${PROJECT_SOURCE_DIR}/test)

file(GLOB SRC_FILE ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB INCLUDE_FILE ${PROJECT_SOURCE_DIR}/include/*.h)

add_executable(main ${SRC_FILE} ${INCLUDE_FILE})
target_link_libraries(main ${DEPEND_LIBRARIES})
