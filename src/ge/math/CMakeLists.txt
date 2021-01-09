set(GE_MATH_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ge/math)

list(APPEND GE_MATH_SCR
    math.cpp
)

list(APPEND GE_MATH_HEADERS
    ${GE_MATH_INCLUDE_DIR}/math.h
)

add_library(ge-math STATIC ${GE_MATH_SCR} ${GE_MATH_HEADERS})
