find_package(OpenCV 3 COMPONENTS core highgui imgproc videoio imgcodecs)

if(${OpenCV_FOUND})
else()
        find_package(OpenCV 2 REQUIRED core highgui imgproc)
endif()

set(GROUNDTRUTH_INCLUDE_DIRS 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
)

file(GLOB GROUNDTRUTH_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB GROUNDTRUTH_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)


include_directories(${GROUNDTRUTH_INCLUDE_DIRS}) 

add_executable(GroundTruth ${GROUNDTRUTH_SOURCES} ${GROUNDTRUTH_HEADERS})
target_link_libraries(GroundTruth ${OpenCV_LIBS})

install(TARGETS GroundTruth RUNTIME DESTINATION "./bin/" COMPONENT tools)
 
