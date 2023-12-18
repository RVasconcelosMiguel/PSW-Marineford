cmake_minimum_required(VERSION 3.5)

project(Marineford VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia Network)

set(PROJECT_SOURCES
    main.cpp
    Gui.ui
    Gui.h Gui.cpp

    Map.h
    Map.cpp
    Ship.h
    Ship.cpp
    Server.h
    Server.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Marineford
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Resources.qrc
        Client.h
        Client.cpp
        Player.h Player.cpp



    )
else()
    if(ANDROID)
        add_library(Marineford SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Marineford
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Marineford PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Marineford PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Marineford PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)

# Add QMediaPlayer
if(Qt${QT_VERSION_MAJOR}Multimedia_FOUND)
    target_link_libraries(Marineford PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)
    add_definitions(-DHAS_QMEDIAPLAYER)
    message("QMediaPlayer is available.")
else()
    message("QMediaPlayer is not available.")
endif()

# Rest of your configurations...
# ...

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Marineford)
endif()
set_target_properties(Marineford PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Marineford
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Marineford)
endif()
