set(TSAICALIBRATION_INCLUDE_DIRS 
	./include
)

file(GLOB TSAICALIBRATION_SOURCES ./source/*.c)
file(GLOB TSAICALIBRATION_HEADERS ./include/*.h)
include_directories(${TSAICALIBRATION_INCLUDE_DIRS})

add_executable(Tsai3D ${TSAICALIBRATION_SOURCES} ${TSAICALIBRATION_HEADERS})

if(WIN32)
else()
target_link_libraries(Tsai3D m)
endif()


install(TARGETS Tsai3D RUNTIME DESTINATION "./bin/" COMPONENT tools)
