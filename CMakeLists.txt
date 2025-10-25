cmake_minimum_required(VERSION 3.15)

project(nidaros VERSION 0.1.0 LANGUAGES CXX)

## Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include(FetchContent)

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-36-0
)

FetchContent_MakeAvailable(asio)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/config.cpp
    src/common.cpp
    src/bot.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -O3
    -Wpedantic
    -pthread
)

target_include_directories(${PROJECT_NAME} PRIVATE ${asio_SOURCE_DIR}/asio/include)

configure_file(
    ${CMAKE_SOURCE_DIR}/nidaros.conf
    ${CMAKE_BINARY_DIR}/nidaros.conf
    COPYONLY
)
