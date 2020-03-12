// vim: set expandtab:

// utility that runs a single test
# include <cppad/utility/test_boolofvoid.hpp>
# include <cppad/utility/thread_alloc.hpp>

// List of external examples
extern bool dficfj_ok(void);
extern bool dierfj_ok(void);
extern bool deptfg_ok(void);
extern bool dgl1fg_ok(void);
//
extern bool dficfj_extra(void);


// main program that runs the examples
int main(void)
{   // Run
    std::string group = "example";
    size_t      width = 20;
    CppAD::test_boolofvoid Run(group, width);

    Run( dficfj_ok,       "dficfj_ok" );
    Run( dierfj_ok,       "dierfj_ok" );
    Run( deptfg_ok,       "deptfg_ok" );
    Run( dgl1fg_ok,       "dgl1fg_ok" );
    //
    Run( dficfj_extra,      "dficfj_extra" );

    // check for memory leak
    bool memory_ok = CppAD::thread_alloc::free_all();
    bool        ok = Run.summary(memory_ok);
    //
    return static_cast<int>( ! ok );
}
