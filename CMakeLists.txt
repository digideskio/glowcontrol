project(GlowControl C CXX)
cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Do not remove this line, its required for the correct functionality of the Ubuntu-SDK
set(UBUNTU_MANIFEST_PATH "manifest.json.in" CACHE INTERNAL "Tells QtCreator location and name of the manifest file")
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Qt5Core)
find_package(Qt5Qml)
find_package(Qt5Quick)
# find_package(ubuntu-sdk-libs)

# Standard install paths
include(GNUInstallDirs)

# Automatically create moc files
set(CMAKE_AUTOMOC ON)
set(QT_IMPORTS_DIR "lib/${ARCH_TRIPLET}")

set(APP_NAME          GlowControl)
set(APP_ID            "com.ubuntu.developer.jonas-drange")
#set(GLOWCONTROL_DIR   "share/qml/GlowControl")
set(MAIN_QML          "GlowControl.qml")
set(ICON              "GlowControl.png")
set(DESKTOP_FILE_NAME "glowcontrol.desktop")

set(SHARE_DIR         "share/GlowControl")
set(LOCALE_DIR        "${LOCALEDIR}")


# Components PATH
execute_process(
    COMMAND dpkg-architecture -qDEB_HOST_MULTIARCH
    OUTPUT_VARIABLE ARCH_TRIPLET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(CLICK_MODE)
    set(CMAKE_INSTALL_BINDIR bin)
    set(CMAKE_INSTALL_PREFIX /)
    STRING(REPLACE "/usr/" "/" QT_IMPORTS_DIR ${QT_IMPORTS_DIR})
else(CLICK_MODE)
    set(SHARE_DIR /${SHARE_DIR})
endif(CLICK_MODE)


if(CLICK_MODE)
    # set(GLOWCONTROL_APP_DIR "${CMAKE_INSTALL_DATADIR}/qml")
    # set(DATA_DIR /)
    add_definitions(-DLIB_DIR="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
    set(EXEC "${CMAKE_INSTALL_BINDIR}/glowcontrol")
    #set(DESKTOP_DIR ${DATA_DIR})

    # This command figures out the target architecture for use in the manifest file
    execute_process(
        COMMAND dpkg-architecture -qDEB_HOST_ARCH
        OUTPUT_VARIABLE CLICK_ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    configure_file(manifest.json.in ${CMAKE_CURRENT_BINARY_DIR}/manifest.json)
else(CLICK_MODE)
    #set(GLOWCONTROL_APP_DIR "${CMAKE_INSTALL_DATADIR}/glowcontrol")
    #set(DATA_DIR ${CMAKE_INSTALL_DATADIR}/${APP_NAME})
    add_definitions(-DLIB_DIR="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
    set(EXEC "${CMAKE_INSTALL_BINDIR}/glowcontrol")
        #set(ICON "${CMAKE_INSTALL_PREFIX}/${DATA_DIR}/${ICON}")
        #set(DESKTOP_DIR ${CMAKE_INSTALL_DATADIR}/applications)
endif(CLICK_MODE)

configure_file(${DESKTOP_FILE_NAME}.in ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE_NAME}.in)

if(CLICK_MODE)
    #install(FILES manifest.json DESTINATION ${CMAKE_INSTALL_PREFIX})
    #install(DIRECTORY "app/graphics" DESTINATION ${DATA_DIR})
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.json DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${ICON} DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/glowcontrol.apparmor DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest.json DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
else(CLICK_MODE)
    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE_NAME}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/applications
    )
endif(CLICK_MODE)

option(INSTALL_TESTS "Install the tests on make install" on)

#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE_NAME}.in DESTINATION ${DESKTOP_DIR})

install(DIRECTORY "app/graphics" DESTINATION ${CMAKE_INSTALL_PREFIX}${SHARE_DIR})
install(DIRECTORY "app/qml" DESTINATION ${CMAKE_INSTALL_PREFIX}${SHARE_DIR})
# install(FILES "glowcontrol.apparmor" DESTINATION ${DATA_DIR})
# install(FILES ${ICON} DESTINATION ${DATA_DIR})

add_subdirectory(app)
add_subdirectory(backend)
#add_subdirectory(lib)
add_subdirectory(po)

add_custom_target("autopilot" chmod +x ${CMAKE_SOURCE_DIR}/app/tests/autopilot/run
                    COMMAND ${CMAKE_SOURCE_DIR}/app/tests/autopilot/run
                    DEPENDS GlowControlbackend GlowControlbackend-qmldir
                    WORKING_DIRECTORY ./app)

add_custom_target("check" /usr/bin/qmltestrunner -input ${CMAKE_SOURCE_DIR}/app/tests/unit -import ${CMAKE_BINARY_DIR}/backend
                    DEPENDS GlowControlbackend GlowControlbackend-qmldir
                    WORKING_DIRECTORY ./app)

add_custom_target("run" /usr/bin/qmlscene -I ${CMAKE_BINARY_DIR}/backend  ${CMAKE_SOURCE_DIR}/app/GlowControl.qml
                    DEPENDS GlowControlbackend GlowControlbackend-qmldir
                    WORKING_DIRECTORY ./app)

# No op custom target for all not compiled files, so they show up in the QtCreator project tree
add_custom_target("glowcontrol_ClickFiles" ALL SOURCES "glowcontrol.apparmor" "manifest.json.in")

