find_package(OpenCV 3 COMPONENTS core highgui videoio imgcodecs)

if(${OpenCV_FOUND})
else()
	find_package(OpenCV 2 REQUIRED core highgui)
endif()

set(VIDSYNC_INCLUDE_DIRS 
	${OpenCV_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB VIDSYNC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB VIDSYNC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h)

include_directories(${VIDSYNC_INCLUDE_DIRS})

add_executable(vidSync ${VIDSYNC_SOURCES} ${VIDSYNC_HEADERS})
target_link_libraries(vidSync ${OpenCV_LIBS})

install(TARGETS vidSync RUNTIME DESTINATION "./bin/" COMPONENT tools)
