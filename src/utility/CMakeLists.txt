# vim: set expandtab:
SET(source_list
    adolc2sparse_rcv.cpp
    check_sparse.cpp
    cppadcg_library.cpp
    prt_sparse.cpp
    upper_triangle.cpp
)
#
ADD_LIBRARY(utility STATIC ${source_list})
SET_TARGET_PROPERTIES(utility
    PROPERTIES COMPILE_FLAGS "${extra_cxx_flags}"
)
