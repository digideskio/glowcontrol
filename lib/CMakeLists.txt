include_directories(lib-lifx/include)

set(
Lifx_SRC
lib-lifx/include/lib-lifx/lifx.h
lib-lifx/include/lib-lifx/lifx_messages.h
lib-lifx/src/lib/lib-lifx.cpp
lib-lifx/src/lib/lifx_messages.cpp
)

add_library(Lifx SHARED ${Lifx_SRC})

qt5_use_modules(Lifx Core Qml)

set_target_properties(Lifx PROPERTIES VERSION 0.0 SOVERSION 0.0)
set_target_properties(Lifx PROPERTIES LIBRARY_OUTPUT_DIRECTORY Glowcontrol)

install(TARGETS Lifx LIBRARY DESTINATION ${QT_IMPORTS_DIR} NAMELINK_SKIP)

