cmake_minimum_required(VERSION 3.16)

project(PJC2025 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON) # Essential for Q_OBJECT in .cpp files
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Only Widgets component is needed for QGraphicsView/Scene based 2D drawing
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets) # Changed here
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets) # Changed here

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

    qt_add_executable(PJC2025
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}









        resources.qrc




        roadnetwork.h roadnetwork.cpp
        renderer.h renderer.cpp
        simulation.h simulation.cpp
        intersection.h intersection.cpp
        roadway.h roadway.cpp

        lane.h lane.cpp


        road.h road.cpp

        intersectionconnection.h intersectionconnection.cpp

        roadgeometrypoint.h roadgeometrypoint.cpp
        roadgeometry.h roadgeometry.cpp
        simulationview.h simulationview.cpp
        graphicsviewpannable.h graphicsviewpannable.cpp
        graphicsviewzoom.h graphicsviewzoom.cpp
        geometryutils.h
        roadwaygeometrycalculator.h roadwaygeometrycalculator.cpp
        lanegeometrycalculator.h lanegeometrycalculator.cpp
        roadwayutils.h roadwayutils.cpp

        geometrymanager.h geometrymanager.cpp









    )
else()
    if(ANDROID)
        add_library(PJC2025 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(PJC2025
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(PJC2025 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(PJC2025 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets # Changed here - only Widgets
)
target_link_libraries(PJC2025 PRIVATE Qt6::Core)
target_link_libraries(PJC2025 PRIVATE Qt6::Widgets)
target_link_libraries(PJC2025 PRIVATE Qt6::Core)
target_link_libraries(PJC2025 PRIVATE Qt6::Core)
target_link_libraries(PJC2025 PRIVATE Qt6::Core)
target_link_libraries(PJC2025 PRIVATE Qt6::Core)
target_link_libraries(PJC2025 PRIVATE Qt6::Core)
target_link_libraries(PJC2025 PRIVATE Qt6::Core)
target_link_libraries(PJC2025 PRIVATE Qt6::Core)
target_link_libraries(PJC2025 PRIVATE Qt6::Core)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PJC2025)
endif()
set_target_properties(PJC2025 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PJC2025
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PJC2025)
endif()

if(MSVC)
    target_compile_options(PJC2025 PRIVATE /W4 /WX)
else()
    target_compile_options(PJC2025 PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
