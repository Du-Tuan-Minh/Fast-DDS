cmake_minimum_required(VERSION 3.16)
project(my_fastdds_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fastdds REQUIRED)
find_package(fastcdr REQUIRED)

set(GEN_SOURCES
    TestTypePubSubTypes.cxx
    TestTypeTypeObjectSupport.cxx
)

add_executable(publisher publisher.cpp ${GEN_SOURCES})
target_link_libraries(publisher fastdds fastcdr)
target_include_directories(publisher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(subscriber subscriber.cpp ${GEN_SOURCES})
target_link_libraries(subscriber fastdds fastcdr)
target_include_directories(subscriber PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/profiles.xml")
    configure_file(profiles.xml ${CMAKE_CURRENT_BINARY_DIR}/profiles.xml COPYONLY)
endif()
