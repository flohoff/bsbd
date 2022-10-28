cmake_minimum_required (VERSION 2.6)
project (bsbd)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

find_package(Boost 1.55.0 REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(PkgConfig)
find_package(PahoMqttCpp REQUIRED)

set(THREADS_HAVE_PTHREAD_ARG 1)
find_package(Threads REQUIRED)

add_executable(bsbd bsbd.cpp bus.cpp serial.cpp crc.cpp AddressMap.cpp vtype.cpp mqtt.cpp)
target_include_directories(bsbd PRIVATE ${SERIAL_INCLUDE_DIRS})
target_link_libraries(bsbd PRIVATE ${Boost_LIBRARIES} ${SERIAL_LDFLAGS} ${CMAKE_THREAD_LIBS_INIT} nlohmann_json::nlohmann_json PahoMqttCpp::paho-mqttpp3)

