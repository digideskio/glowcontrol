include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../lib/lib-lifx/include
)

set(
Liblifx_SRCS
../lib/lib-lifx/src/lib/lib-lifx.cpp
../lib/lib-lifx/src/lib/lifx_messages.cpp
)

set(
    Glowcontrolbackend_SRCS
    modules/Glowcontrol/lightbulb.cpp
    modules/Glowcontrol/backend.cpp
    modules/Glowcontrol/glowcontrol.cpp
)

add_library(Glowcontrolbackend MODULE
    ${Glowcontrolbackend_SRCS}
    ${Liblifx_SRCS}
)

set_target_properties(Glowcontrolbackend PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY Glowcontrol)

qt5_use_modules(Glowcontrolbackend Gui Qml Quick)

# Copy qmldir file to build dir for running in QtCreator
add_custom_target(Glowcontrolbackend-qmldir ALL
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/modules/Glowcontrol/qmldir ${CMAKE_CURRENT_BINARY_DIR}/Glowcontrol
    DEPENDS ${QMLFILES}
)

# Install plugin file
install(TARGETS Glowcontrolbackend DESTINATION ${QT_IMPORTS_DIR}/Glowcontrol/)
install(FILES   modules/Glowcontrol/qmldir DESTINATION ${QT_IMPORTS_DIR}/Glowcontrol/)
