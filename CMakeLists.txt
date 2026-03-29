cmake_minimum_required(VERSION 3.16)

project(Process VERSION 0.2 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.h
    Process.h Process.cpp
    ScheDuler.h ScheDuler.cpp
    List.h
    ListSorter.h
    SchedulingStrategy.h
    PriorityStrategy.h
    FCFSStrategy.h
    SJFStrategy.h
    RoundRobinStrategy.h
    MLFQStrategy.h
    CyberStyle.h
    ColorGenerator.h
    ControlPanel.h ControlPanel.cpp
    CpuWidget.h CpuWidget.cpp
    ReadyQueueWidget.h ReadyQueueWidget.cpp
    ProcessCard.h ProcessCard.cpp
    GanttWidget.h GanttWidget.cpp
    StatsPanel.h StatsPanel.cpp
    ExplanationBar.h ExplanationBar.cpp
    ComparisonDialog.h ComparisonDialog.cpp
    .qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Process
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Process SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Process
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Process PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Process)
endif()
set_target_properties(Process PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Process
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Process)
endif()
