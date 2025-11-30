cmake_minimum_required(VERSION 3.16)

project(hibp2sqlite LANGUAGES CXX)

set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/dist")

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

set(RESOURCE
    "hibp2sqlite.qrc"
)

qt_add_resources(PROJECT_RESOURCES ${RESOURCE})

qt_add_executable(hibp2sqlite
    ${PROJECT_SOURCES}
    ${PROJECT_RESOURCES}
)

target_link_libraries(hibp2sqlite PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql)

include(GNUInstallDirs)
install(TARGETS hibp2sqlite
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# if(WIN32)
#     # Run Qt Deploy Tool
#     set(MY_APP hibp2sqlite)
#     set(executable_path "\${QT_DEPLOY_BIN_DIR}/$<TARGET_FILE_NAME:${MY_APP}>")
#     set(deploy_script "${CMAKE_CURRENT_BINARY_DIR}/deploy_${MY_APP}.cmake")
# 
#     file(GENERATE OUTPUT ${deploy_script} CONTENT "
#     include(\"${QT_DEPLOY_SUPPORT}\")
#     qt_deploy_runtime_dependencies(
#         EXECUTABLE \"${executable_path}\"
#         PLUGINS_DIR bin
#         NO_TRANSLATIONS
#         NO_COMPILER_RUNTIME
#     )"
#     )
# 
#     install(TARGETS ${MY_APP} DESTINATION bin)
#     install(SCRIPT ${deploy_script})
# endif()
