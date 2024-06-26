cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#The three settings used by Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Protobuf REQUIRED)
find_package(Boost COMPONENTS REQUIRED)   #I don't specify components, I need to revise this statement
find_package(OpenCV COMPONENTS highgui REQUIRED)
list(APPEND CMAKE_PREFIX_PATH "/home/chihyuan/Qt/6.7.2/gcc_64/")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROTO_FILE AnalyzedResults.proto)
protobuf_generate_cpp(PROTO_SRC PROTO_HDR ${PROTO_FILE})
file (GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
set(SOURCES SocketToServer.cpp
            ProcessImage.cpp
            ProcessAudio.cpp
            JPEG.cpp
            utility_directory.cpp
            utility_TimeRecorder.cpp
            utility_compute.cpp
            utility_string.cpp
            utility_csv.cpp
            Main.cpp
            human_pose_estimator.cpp
            Pose.cpp
            ${PROTO_SRC}
            mainwindow.cpp
            mainwindow.h
            mainwindow.ui
    )

add_demo(NAME 9_NurseHelper
         SOURCES ${SOURCES}
         HEADERS ${HEADERS}
         DEPENDENCIES monitors models pipelines portaudio
         INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
         )

#enable debug information
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")   


include_directories(${PROTOBUF_INCLUDE_DIRS})
get_filename_component(PROTO_HDR_DIRECTORY ${PROTO_HDR} DIRECTORY)
include_directories(${PROTO_HDR_DIRECTORY})
include_directories(OpenCV_INCLUDE_DIR)

link_directories(${LIB_FOLDER} /usr/local/lib)

set_target_properties(${TARGET_NAME} PROPERTIES "CMAKE_CXX_FLAGS" "${CMAKE_CXX_FLAGS}")

target_link_libraries(${OMZ_DEMO_NAME} PRIVATE ${PROTOBUF_LIBRARIES} Qt${QT_VERSION_MAJOR}::Widgets)
