SET(source_list
    fun_base.cpp
    fun_dficfj.cpp
    fun_dierfj.cpp
    fun_deptfg.cpp
    fun_dgl1fg.cpp
)
# include hpp version of f2c as system library (no warnings)
INCLUDE_DIRECTORIES( SYSTEM "${PROJECT_SOURCE_DIR}/minpack2/tprobs.92/hpp")
#
ADD_LIBRARY(fun_obj STATIC ${source_list})
SET_TARGET_PROPERTIES(fun_obj
    PROPERTIES COMPILE_FLAGS "${extra_cxx_flags}"
)
