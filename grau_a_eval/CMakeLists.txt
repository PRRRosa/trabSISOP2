
add_executable(test1 test1.c)
target_link_libraries(test1 thread)

add_executable(phil_1 phil_1.c)
target_link_libraries(phil_1 thread)

add_executable(phil_2 phil_2.c)
target_link_libraries(phil_2 thread)

add_executable(phil_3 phil_3.c)
target_link_libraries(phil_3 thread)

add_executable(phil_4 phil_4.c)
target_link_libraries(phil_4 thread)

add_executable(phil_5 phil_5.c)
target_link_libraries(phil_5 thread)

add_executable(prod_cons prod_cons.c)
target_link_libraries(prod_cons thread)

add_executable(prod_cons_condvar prod_cons_condvar.c)
target_link_libraries(prod_cons_condvar thread)

add_executable(prod_cons_yielding prod_cons_yielding.c)
target_link_libraries(prod_cons_yielding thread)

add_executable(rw_locks rw_locks.c)
target_link_libraries(rw_locks thread)

add_executable(sanity_stack sanity_stack.c)
target_link_libraries(sanity_stack thread)

add_executable(sanity_recursion sanity_recursion.c)
target_link_libraries(sanity_recursion thread)

add_executable(sanity_joiner sanity_joiner.c)
target_link_libraries(sanity_joiner thread)


install(TARGETS test1
    EXPORT ThreadTargets
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}/bin" COMPONENT bin)
