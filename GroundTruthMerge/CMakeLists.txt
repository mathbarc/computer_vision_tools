find_package(OpenCV 3 COMPONENTS core highgui videoio imgcodecs)

if(${OpenCV_FOUND})
else()
        find_package(OpenCV 2 REQUIRED core highgui)
endif()

set(GROUNDTRUTHMERGE_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
)

file(GLOB GROUNDTRUTHMERGE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB GROUNDTRUTHMERGE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)


include_directories(${GROUNDTRUTHMERGE_INCLUDE_DIRS})

add_executable(GroundTruthMerge ${GROUNDTRUTHMERGE_SOURCES} ${GROUNDTRUTHMERGE_HEADERS})
target_link_libraries(GroundTruthMerge ${OpenCV_LIBS})

install(TARGETS GroundTruthMerge RUNTIME DESTINATION "./bin/" COMPONENT tools)


