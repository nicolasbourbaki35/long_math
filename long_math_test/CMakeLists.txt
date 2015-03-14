
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/long_math_lib)

AUX_SOURCE_DIRECTORY(. sources)

ADD_EXECUTABLE(long_math_test ${sources})

TARGET_LINK_LIBRARIES(long_math_test lm ${GTEST_BOTH_LIBRARIES} pthread)

