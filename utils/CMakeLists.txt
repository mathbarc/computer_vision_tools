find_package(OpenCV 4 COMPONENTS core highgui videoio imgcodecs)
find_package(Qt5 COMPONENTS Core Gui REQUIRED)

if(${OpenCV_FOUND})
else()
        find_package(OpenCV 2 REQUIRED core highgui)
endif()

set(UTILS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "Utils Includes")

list(APPEND UTILS_INCLUDE_DIRS
        ${UTILS_INCLUDE_DIR}
        ${Qt5_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
)

file(GLOB UTILS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB UTILS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

qt5_wrap_cpp(UTILS_WRAPPED_HEADERS ${UTILS_HEADERS})


include_directories(${UTILS_INCLUDE_DIRS})

add_library(util STATIC ${UTILS_SOURCES} ${UTILS_HEADERS} ${UTILS_WRAPPED_HEADERS})
target_link_libraries(util ${OpenCV_LIBS} Qt5::Gui Qt5::Core)
