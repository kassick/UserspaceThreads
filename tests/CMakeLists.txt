add_executable(test1 test1.c)
target_link_libraries(test1 thread)

add_executable(test_sema test_sema.c)
target_link_libraries(test_sema thread)


add_executable(test_list test_list.c)
target_link_libraries(test_list thread)

install(TARGETS test1 test_list
    EXPORT ThreadTargets
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}/bin" COMPONENT bin)
