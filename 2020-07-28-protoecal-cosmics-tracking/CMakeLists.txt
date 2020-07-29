cmake_minimum_required(VERSION 3.15)
project(proto_ecal_tracking)

set(CMAKE_CXX_STANDARD 14)

# Expose custom cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake")

find_package(JANA REQUIRED)

set (COSMICS_DETECTOR_SOURCES
        main.cpp
        ProtoECalHit.h
        ProtoECalTrack.h
        ProtoECalTrack_Factory.cc
        ProtoECalTrack_Factory.h
        TriggerDecision.h
        TriggerDecision_Factory_Fast.cc
        TriggerDecision_Factory_Fast.h
        TriggerDecision_Factory_Slow.cc
        TriggerDecision_Factory_Slow.h
        )

add_executable(proto_ecal_tracking ${COSMICS_DETECTOR_SOURCES})
target_include_directories(proto_ecal_tracking PUBLIC ${JANA_INCLUDE_DIR})
target_link_libraries(proto_ecal_tracking ${JANA_LIBRARY})


