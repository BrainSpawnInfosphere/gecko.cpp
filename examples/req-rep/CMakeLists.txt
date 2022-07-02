LIST(APPEND apps
    main.cpp
    imu.cpp
)

message(STATUS ">> ${CMAKE_BINARY_DIR}/_deps/marko-src/include")

foreach(MAIN ${apps})
    add_executable(${MAIN} main.cpp)
    add_dependencies(${MAIN} gecko)
    target_include_directories(${MAIN}
        PRIVATE
            ${CMAKE_BINARY_DIR}/_deps/marko-src/include
            ${CMAKE_BINARY_DIR}/_deps/serialcomm-src/include
    )
    # target_link_directories(${MAIN}
    #     PRIVATE
    #         ${CMAKE_BINARY_DIR}/_deps/marko-build
    #         ${CMAKE_BINARY_DIR}/_deps/serialcomm-build
    # )
    target_link_libraries(${MAIN}
        PRIVATE
            gecko
            marko
            serialcomm
            ${CMAKE_THREAD_LIBS_INIT}
    )
endforeach()
