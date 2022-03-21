find_package(OpenCV 4 COMPONENTS core highgui videoio imgcodecs calib3d  REQUIRED)

if(${OpenCV_FOUND})
else()
        find_package(OpenCV 2 REQUIRED core highgui)
endif()

list(APPEND STEREO_CALIB_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${UTILS_INCLUDE_DIR}
        ${OpenCV_INCLUDE_DIRS}
)

file(GLOB STEREO_CALIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB STEREO_CALIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)


include_directories(${STEREO_CALIB_INCLUDE_DIRS})

add_executable(StereoCalib ${STEREO_CALIB_SOURCES} ${STEREO_CALIB_HEADERS})
target_link_libraries(StereoCalib ${OpenCV_LIBS})

install(TARGETS StereoCalib RUNTIME DESTINATION "./bin/" COMPONENT tools)
