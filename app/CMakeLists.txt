# file(GLOB QML_JS_FILES *.qml *.js)

set(QML_SOURCES
    qml/GlowControl.qml
)

QT5_ADD_RESOURCES(glowcontrol-resources ui.qrc)

# Make the files visible on qtcreator
# add_custom_target(glowcontrol_QMlFiles ALL SOURCES ${QML_SOURCES} ${glowcontrol-resources})

add_executable(glowcontrol main.cpp ${glowcontrol-resources})
qt5_use_modules(glowcontrol Widgets Core Gui Quick Qml)
target_link_libraries(glowcontrol Glowcontrolbackend)

install(FILES ${QML_JS_FILES} DESTINATION ${GLOWCONTROL_DIR})
install(TARGETS glowcontrol RUNTIME DESTINATION bin)

