
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/long_math_lib)

AUX_SOURCE_DIRECTORY(. sources)

ADD_EXECUTABLE(poly_mult_perf ${sources})

TARGET_LINK_LIBRARIES(poly_mult_perf lm)

