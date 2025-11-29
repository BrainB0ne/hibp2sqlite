cmake_minimum_required(VERSION 3.16)

project(hibp2sqlite VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql)

set(PROJECT_SOURCES
    main.cpp
    hibp2sqlite.rc
)

qt_add_executable(hibp2sqlite
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(hibp2sqlite PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql)

# Resources:
set(hibp2sqlite_resource_files
    "LICENSE"
)

qt_add_resources(hibp2sqlite "hibp2sqlite"
    PREFIX
        "/"
    FILES
        ${hibp2sqlite_resource_files}
)

set_target_properties(hibp2sqlite PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS hibp2sqlite
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(hibp2sqlite)
endif()
