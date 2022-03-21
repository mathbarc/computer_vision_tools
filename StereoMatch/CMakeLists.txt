find_package(OpenCV 4 COMPONENTS core highgui videoio imgcodecs calib3d  REQUIRED)

if(${OpenCV_FOUND})
else()
        find_package(OpenCV 2 REQUIRED core highgui)
endif()

list(APPEND STERIO_MATCH_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${UTILS_INCLUDE_DIR}
        ${OpenCV_INCLUDE_DIRS}
)

file(GLOB STERIO_MATCH_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB STERIO_MATCH_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)


include_directories(${STERIO_MATCH_INCLUDE_DIRS})

add_executable(StereoMatch ${STERIO_MATCH_SOURCES} ${STERIO_MATCH_HEADERS})
target_link_libraries(StereoMatch ${OpenCV_LIBS})

install(TARGETS StereoMatch RUNTIME DESTINATION "./bin/" COMPONENT tools)
