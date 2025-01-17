find_package(OpenCV 4 COMPONENTS core highgui videoio imgcodecs calib3d  REQUIRED)

if(${OpenCV_FOUND})
else()
    find_package(OpenCV 2 REQUIRED core highgui)
endif()

list(APPEND IMAGE_VIEW_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
)

file(GLOB IMAGE_VIEW_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB IMAGE_VIEW_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)


include_directories(${IMAGE_VIEW_INCLUDE_DIRS})

add_executable(ImageView ${IMAGE_VIEW_SOURCES} ${IMAGE_VIEW_HEADERS})
target_link_libraries(ImageView ${OpenCV_LIBS})

install(TARGETS ImageView RUNTIME DESTINATION "./bin/" COMPONENT tools)
