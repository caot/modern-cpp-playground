set(TARGET calculator)
add_executable(${TARGET}
        src/main.cpp
        # src/parser.cpp src/parser.hpp  # add more as you grow
)

# If this project needs libraries later:
# target_link_libraries(${TARGET} PRIVATE some_lib)

# Optional: tests via Catch2 (if BUILD_TESTS=ON)
if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.6.0
    )
    FetchContent_MakeAvailable(catch2)

    add_executable(${TARGET}_tests
            tests/test_basic.cpp
    )
    target_link_libraries(${TARGET}_tests PRIVATE Catch2::Catch2WithMain)

    include(Catch)
    catch_discover_tests(${TARGET}_tests)
endif()
