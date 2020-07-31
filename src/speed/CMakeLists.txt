#
ADD_EXECUTABLE(speed EXCLUDE_FROM_ALL speed.cpp
    adolc.cpp
    cppad.cpp
    cppadcg.cpp
    subcg.cpp
    subgraph.cpp
    write_csv.cpp
)
SET_TARGET_PROPERTIES(speed
    PROPERTIES COMPILE_FLAGS "${extra_cxx_flags}"
)
#
TARGET_LINK_LIBRARIES(speed
    fun_obj minpack2_c utility -ladolc -lcppad_lib -lColPack -ldl
)
