cmake_minimum_required(VERSION 3.5.0)
project(OBJECT-DETECTION VERSION 0.1.0 LANGUAGES C CXX)

# Find OpenCV and Python3 packages
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})

# Find Qt5 Core, Gui, and Widgets packages
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# Add source files for the project
set(SOURCE_FILES
    main.cpp
    face-recognition/Face.cpp
    camera/Camera.cpp
    GUI/MainWindow.cpp
)

# Enable automatic MOC and UIC for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Add the .hpp file to the MOC processing list explicitly
set(MOC_FILES 
    GUI/MainWindow.hpp
)

# Create executable for the project
add_executable(OBJECT-DETECTION ${SOURCE_FILES} ${MOC_FILES})

# Link necessary libraries (OpenCV, Python, and Qt Widgets)
target_link_libraries(OBJECT-DETECTION 
    ${OpenCV_LIBS} 
    ${Python3_LIBRARIES} 
    Qt5::Core 
    Qt5::Gui 
    Qt5::Widgets
)

# Set the Qt platforms plugin directory path
set(Qt_Platforms_Dir "G:/Object-Detection/build/Release/platforms")
#set(CMAKE_PREFIX_PATH "G:/Object-Detection/build/Release/platforms")

# Copy the platforms directory to the output directory after build
add_custom_command(TARGET OBJECT-DETECTION POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${Qt_Platforms_Dir}"
        "$<TARGET_FILE_DIR:OBJECT-DETECTION>/platforms"
)

# Optional: Include additional directories if needed
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DNOMINMAX)

