cmake_minimum_required (VERSION 3.8)
project(demo1)

execute_process(COMMAND bash date2ver  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
message("CMAKE_CXX_COMPILER:${CMAKE_CXX_COMPILER}")

set(CMAKE_CXX_STANDARD 11)

include_directories(
    ./
    ./3rdparty

    ./src/common
    ./src/data
    ./src/function
    ./src/net
    ./src/protocol
    ./src/uart
    ./src/viewlibs
    ./src/windows

    ${CDROID_INCLUDE_DIRS}
    ${CDROID_DEPINCLUDES}
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/include/gui
    ${CMAKE_BINARY_DIR}/include/porting
)

find_package(jsoncpp CONFIG REQUIRED)
find_package(CURL REQUIRED)

add_definitions(-DCDROID_RUNNING)
if (CDROID_CHIPSET STREQUAL sigma)
    add_definitions(-DCDROID_SIGMA)
elseif(CDROID_CHIPSET STREQUAL x64)
    add_definitions(-DCDROID_X64)
endif()

aux_source_directory(./src/common PRJ_SRC_COMM)
aux_source_directory(./src/data PRJ_SRC_DATA)
aux_source_directory(./src/function PRJ_SRC_FUN)
aux_source_directory(./src/net PRJ_SRC_NET)
aux_source_directory(./src/protocol PRJ_SRC_PROTOCOL)
aux_source_directory(./src/uart PRJ_SRC_UART)
aux_source_directory(./src/viewlibs PRJ_SRC_VIEWLIB)
aux_source_directory(./src/windows PRJ_SRC_WINDOWS)
file(GLOB_RECURSE PRJ_SRCS ${CMAKE_CURRENT_SOURCE_DIR} *.cc)


# add_definitions(--include cdtypes.h)
# add_definitions(--include cdlog.h)

message("PROJECT_NAME=${PROJECT_NAME}")

add_executable(${PROJECT_NAME} ${PRJ_SRCS} ${PRJ_SRC_COMM} ${PRJ_SRC_DATA} ${PRJ_SRC_FUN} ${PRJ_SRC_NET} ${PRJ_SRC_VIEWLIB} ${PRJ_SRC_PROTOCOL} ${PRJ_SRC_UART} ${PRJ_SRC_WINDOWS})


CreatePAK(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pak ${PROJECT_SOURCE_DIR}/R.h)

target_link_libraries(${PROJECT_NAME} PRIVATE cdroid PRIVATE JsonCpp::JsonCpp ${CURL_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

