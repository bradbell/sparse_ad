# vim: set expandtab:
# usage: cmake \
#   -D adolc_prefix=value \
#   -D colpack_prefix=value \
#   -D cppad_prefix=value \
#   -D extra_cxx_flags=value \
#   ..
#
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(sparsity_paper)
SET(sparsity_paper_version "20200331")
#
# command line definitions
SET(cmd_define_list
    adolc_prefix
    colpack_prefix
    cppad_prefix
    extra_cxx_flags
)
FOREACH(variable ${cmd_define_list} )
    #
    # check command line argument is present
    IF( NOT ${variable} )
        MESSAGE(FATAL_ERROR "cmake: did not find -D ${variable}=value")
    ELSE( NOT ${variable} )
        MESSAGE(STATUS "${variable}=${${variable}}")
    ENDIF( NOT ${variable} )
ENDFOREACH(variable ${cmd_define_list} )
#
#
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR} SYSTEM ${adolc_prefix}/include)
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR} SYSTEM ${colpack_prefix}/include)
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR} ${cppad_prefix}/include)
#
FOREACH(package adolc colpack cppad)
    SET(prefix ${${package}_prefix})
    SET(found NO)
    FOREACH(dir lib lib64)
        IF( EXISTS ${prefix}/${dir} )
            LINK_DIRECTORIES( ${prefix}/${dir})
            SET(found YES)
        ENDIF( EXISTS ${prefix}/${dir} )
    ENDFOREACH(dir lib lib64)
    IF( NOT found )
        MESSAGE(FATAL_ERROR
            "Cannot find lib or lib64 under ${package}_prefix=${prefix}. "
            "It seems that bin/get_${package}.sh has not been run "
            "with built_type=${CMAKE_BUILD_TYPE} in bin/run_cmake.sh."
        )
    ENDIF( NOT found )
ENDFOREACH(package adolc colpack cppad)
#
# Check for cppadcg
IF( NOT EXISTS "${cppad_prefix}/include/cppad/cg/cppadcg.hpp" )
        MESSAGE(FATAL_ERROR
            "Cannot find ${cppad_prefix}/include/cppad/cg/cppadcg.hpp. "
            "It seems that bin/get_cppadcg.sh has not been run "
            "with built_type=${CMAKE_BUILD_TYPE} in bin/run_cmake.sh."
        )
ENDIF( NOT EXISTS "${cppad_prefix}/include/cppad/cg/cppadcg.hpp" )
#
ADD_SUBDIRECTORY(minpack2/tprobs.92)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(example)
