find_package(OpenCV REQUIRED)

file(GLOB SRCS src/*.cpp)
add_executable(demo ${SRCS} "src/demo_threshold.cpp")
target_link_libraries(demo kernels opencv_imgproc opencv_highgui)
