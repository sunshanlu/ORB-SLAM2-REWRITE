include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SRC_FILE ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SRC_FILE ${PROJECT_SOURCE_DIR}/src/main.cpp)
add_executable(vocabulary_create vocabulary.cpp ${SRC_FILE})
target_link_libraries(vocabulary_create PRIVATE ${DEPEND_LIBRARIES})
