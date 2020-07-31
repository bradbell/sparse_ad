SET(source_list
    example.cpp
    f2cpp_ok.cpp
    adolc_ok.cpp
    cppad_ok.cpp
    subgraph_ok.cpp
    cppadcg_ok.cpp
    dficfj_ok.cpp
    dierfj_ok.cpp
    deptfg_ok.cpp
    dgl1fg_ok.cpp
    dficfj_extra.cpp
)
# include hpp version of f2c as system library (no warnings)
INCLUDE_DIRECTORIES( SYSTEM "${PROJECT_SOURCE_DIR}/minpack2/tprobs.92/hpp")
#
ADD_EXECUTABLE(example EXCLUDE_FROM_ALL ${source_list})
SET_TARGET_PROPERTIES(example
    PROPERTIES COMPILE_FLAGS "${extra_cxx_flags}"
)
TARGET_LINK_LIBRARIES(example
    fun_obj minpack2_c utility -ladolc -lcppad_lib -lColPack -ldl
)
ADD_CUSTOM_TARGET(check_example example)
MESSAGE(STATUS "available: make check_example")
