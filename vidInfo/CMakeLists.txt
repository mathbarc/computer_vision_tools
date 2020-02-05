find_package(OpenCV 4 COMPONENTS core highgui videoio imgcodecs)

if(${OpenCV_FOUND})
else()
	find_package(OpenCV 2 REQUIRED core highgui)
endif()

set(VIDINFO_INCLUDE_DIRS 
	${OpenCV_INCLUDE_DIRS}
)

file(GLOB VIDINFO_SOURCES main.cpp)
include_directories(${VIDINFO_INCLUDE_DIRS})

add_executable(vidInfo ${VIDINFO_SOURCES})
target_link_libraries(vidInfo ${OpenCV_LIBS})

install(TARGETS vidInfo RUNTIME DESTINATION "./bin/" COMPONENT tools)
