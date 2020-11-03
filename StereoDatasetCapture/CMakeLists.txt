find_package(OpenCV 4 COMPONENTS core highgui videoio imgcodecs)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

if(${OpenCV_FOUND})
else()
        find_package(OpenCV 2 REQUIRED core highgui)
endif()

list(APPEND STEREO_DATASET_CAPTURE_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${UTILS_INCLUDE_DIR}
        ${OpenCV_INCLUDE_DIRS}
)

file(GLOB STEREO_DATASET_CAPTURE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB STEREO_DATASET_CAPTURE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)


include_directories(${STEREO_DATASET_CAPTURE_INCLUDE_DIRS})

add_executable(StereoDatasetCapture ${STEREO_DATASET_CAPTURE_SOURCES} ${STEREO_DATASET_CAPTURE_HEADERS})
target_link_libraries(StereoDatasetCapture Qt5::Widgets Qt5::Core ${OpenCV_LIBS} util)

install(TARGETS StereoDatasetCapture RUNTIME DESTINATION "./bin/" COMPONENT tools)
