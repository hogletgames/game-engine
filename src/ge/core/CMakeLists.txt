set(GE_CORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ge/core)

list(APPEND GE_CORE_SRC
    log.cpp
)

list(APPEND GE_CORE_HEADERS
    ${GE_CORE_INCLUDE_DIR}/asserts.h
    ${GE_CORE_INCLUDE_DIR}/begin.h
    ${GE_CORE_INCLUDE_DIR}/core.h
    ${GE_CORE_INCLUDE_DIR}/interface.h
    ${GE_CORE_INCLUDE_DIR}/log.h
    ${GE_CORE_INCLUDE_DIR}/non_copyable.h
    ${GE_CORE_INCLUDE_DIR}/timestamp.h
    ${GE_CORE_INCLUDE_DIR}/utils.h
)

add_library(ge-core STATIC ${GE_CORE_SRC} ${GE_CORE_HEADERS})
target_link_libraries(ge-core PUBLIC spdlog)
target_include_directories(ge-core PRIVATE
    ${GE_CORE_INCLUDE_DIR}
)
