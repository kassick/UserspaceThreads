configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)


add_library(thread SHARED thread.c thread.h config.h.in)
set_target_properties( thread PROPERTIES
    PUBLIC_HEADER "thread.h;${CMAKE_CURRENT_BINARY_DIR}/config.h")

install(TARGETS thread
    EXPORT ThreadTargets
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
    PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/thread"
    COMPONENT dev)
