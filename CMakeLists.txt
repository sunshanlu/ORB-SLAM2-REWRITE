cmake_minimum_required(VERSION 3.18)
project(ORB-SLAM2-REWRITE)

find_package(Eigen3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(g2o CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(Pangolin CONFIG REQUIRED)
find_package(Sophus CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(DEPEND_LIBRARY depend_library)
set(G2O_LIBRARIES g2o::core g2o::stuff g2o::types_icp g2o::types_sba)
set(PLATFORM x64-linux)

include_directories(${PROJECT_BINARY_DIR}/vcpkg_installed/${PLATFORM}/include)
add_library(${DEPEND_LIBRARY} INTERFACE)
target_link_libraries(
        ${DEPEND_LIBRARY}
        INTERFACE Eigen3::Eigen
        INTERFACE fmt::fmt-header-only
        INTERFACE ${G2O_LIBRARIES}
        INTERFACE JsonCpp::JsonCpp
        INTERFACE ${OpenCV_LIBS}
        INTERFACE ${Pangolin_LIBRARIES}
        INTERFACE Sophus::Sophus
        INTERFACE spdlog::spdlog_header_only)

message(STATUS ${OpenCV_INCLUDE_DIRS})
