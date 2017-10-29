cmake_minimum_required(VERSION 3.5)
project(LTI_Phosphor)

#set(OpenCV_DIR ${PROJECT_SOURCE_DIR})

#find_package( OpenCV REQUIRED )

include_directories(
    ${PROJECT_SOURCE_DIR}/3rd_party)

add_executable(${PROJECT_NAME}
    main.cpp)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

target_link_libraries(
    ${PROJECT_NAME}
    opencv_core
    opencv_highgui
    opencv_imgproc)
