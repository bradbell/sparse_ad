# vim: set expandtab:
SET(source_list
    s_copy.c
    s_cmp.c
    dficfj.c
    depths.c
    dgl1hs.c
)
ADD_LIBRARY(minpack2_c STATIC ${source_list} )
SET_TARGET_PROPERTIES(minpack2_c
    PROPERTIES COMPILE_FLAGS "-Wno-shift-op-parentheses"
)
