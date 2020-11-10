# include <cstdlib>
# include <cppad/speed/uniform_01.hpp>
# include <src/typedef.hpp>
# include <src/fun_obj/fun_dficfj.hpp>
# include <src/fun_obj/fun_dierfj.hpp>
# include <src/fun_obj/fun_deptfg.hpp>
# include <src/fun_obj/fun_dgl1fg.hpp>
# include "write_csv.hpp"
# include "setup.hpp"
# include "test.hpp"
/*
------------------------------------------------------------------------------
{xsrst_begin speed_global}

.. include:: ../preamble.rst

{xsrst_spell
    coloring
    globals
    ptr
    onepass
    nnz
}

Speed Program Global Variables
##############################

Initialization
**************
The following globals are set by the speed initialization routine
before any other subroutines are called.

global_problem_ptr
==================
minpack2 representation of the problem:
{xsrst_code cpp} */
fun_base* global_problem_ptr;
/* {xsrst_code}

global_size
===========
size for this problem:
{xsrst_code cpp} */
size_t global_size;
/* {xsrst_code}

global_colpack
==============
use colpack coloring:
{xsrst_code cpp} */
bool global_colpack;
/* {xsrst_code}

global_indirect
===============
use indirect computation
{xsrst_code cpp} */
bool global_indirect;

/* {xsrst_code}

global_optimize
===============
was this AD function optimized
{xsrst_code cpp} */
bool global_optimize;
/* {xsrst_code}

global_setup
============
is taping of function AD operations included in timing
{xsrst_code cpp} */
bool global_setup;
/* {xsrst_code}

global_reverse
==============
are we using reverse mode (otherwise forward mode)
{xsrst_code cpp} */
bool global_reverse;
/* {xsrst_code}

global_onepass
==============
are we using one pass of the tape to compute entire Jacobian
{xsrst_code cpp} */
bool global_onepass;
/* {xsrst_code}

global_correct
==============
are we checking for correctness and result of check
{xsrst_code cpp} */
bool global_correct;
/* {xsrst_code}

global_x
========
argument value at which we are evaluation function and Jacobian
{xsrst_code cpp} */
d_vector global_x;
/* {xsrst_code}

global_nnz
**********
This value, set at the end of every test, is the number of non-zeros
int the sparsity pattern detected by the implementation:
{xsrst_code cpp} */
size_t global_nnz;
/* {xsrst_code}

global_correct_ok
*****************
If *global_correct* is true this value is set at the end of every test
and is true (fails) if the correctness test passes (fails):
If *global_correct* is fails, this value is never set or used.
{xsrst_code cpp} */
bool global_correct_ok;
/* {xsrst_code}
{xsrst_end speed_global}
*/
// -----------------------------------------------------------------------
namespace {
    using std::cerr;
    // -----------------------------------------------------------------------
    // initialize
    void initialize(
        std::string implement     ,
        fun_base* problem_ptr  ,
        size_t    size         ,
        bool      colpack      ,
        bool      indirect     ,
        bool      optimize     ,
        bool      setup         ,
        bool      reverse      ,
        bool      onepass      ,
        bool      correct      )
    {   // simple variables
        global_problem_ptr = problem_ptr;
        global_size        = size;
        global_colpack     = colpack;
        global_indirect    = indirect;
        global_optimize    = optimize;
        global_setup        = setup;
        global_reverse     = reverse;
        global_onepass     = onepass;
        global_correct     = correct;
        global_x           = problem_ptr->start();
        //
        // perturb the starting point with random noise
        size_t n = global_x.size();
        d_vector noise(n);
        CppAD::uniform_01(n, noise);
        for(size_t j = 0; j < n; ++j)
            global_x[j] = global_x[j] + (noise[j] - 0.5) / 10.0;
        //
        // is this a Hessian problem
        bool hessian = problem_ptr->size_range() == 1;
        //
        // initialize error message as empty
        std::string msg = "";
        //
        // illegal colpack cases
        if( colpack )
        {   if( implement == "subgraph" ) msg =
                "implement is subgraph and --colpack is present";
            if( implement == "subcg" ) msg =
                "implement is subcg and --colpack is present";
        }
        else
        {   if( implement == "adolc" ) msg =
                "implement is adolc and --colpack missing";
        }
        // illegal indirect cases
        if( indirect )
        {   if( implement != "adolc" ) msg =
                "implement is not adolc and --indirect is present";
            if( ! hessian ) msg =
                "--indirect is present for a Jacobian problem";
        }
        // illegal optimize cases
        if( optimize )
        {   if( implement == "adolc" )  msg =
                "implement is adolc and --optimize is present";
        }
        // illegal reverse cases
        if( reverse )
        {   if( implement == "adolc" && hessian ) msg =
                "implement is adolc for Hessian problem and --reverse present";
            if( implement == "cppad" && onepass  ) msg =
                "implement is cppad, --reverse present, and --onepass  present";
        }
        else
        {   if( implement == "subgraph" ) msg =
                "implement is subgraph and --reverse missing";
            if( implement == "subcg" ) msg =
                "implement is subcg and --reverse missing";
        }
        // illegal onepass  cases
        if( onepass  )
        {   if( hessian ) msg =
                "--onepass  present for a Hessian problem";
            if( implement == "subgraph" ) msg =
                "implement is subgraph and --onepass  present";
            if( implement == "subcg" ) msg =
                "implement is subcg and --onepass  present";
            if( implement == "cppad" && reverse ) msg =
                "implement is cppad, --reverse present, and --onepass  present";
        }
        else
        {   if( implement == "adolc" && ! hessian ) msg =
                "implement is adolc and --onepass  missing for Jacobian problem";
        }
        //
        // exit if any error case above was found
        if( msg != "" )
        {   cerr << "speed: " << msg << "\n";
            std::exit(1);
        }
        //
        if( ! global_setup )
        {   // must run setup before first because not inclued in timing
            if( implement == "adolc" )
                setup_adolc();
            if( implement == "cppad" )
                setup_cppad();
            if( implement == "subgraph" )
                setup_subgraph();
            if( implement == "cppadcg" )
                setup_cppadcg();
            if( implement == "subcg" )
                setup_subcg();
        }
    }
    // -----------------------------------------------------------------------
    // run_test
    void run_test(
        const std::string&                csv_file ,
        const CppAD::vector<std::string>& name     ,
        const CppAD::vector<std::string>& value    ,
        const std::string&                implement,
        const std::string&                problem  ,
        void               (*test)(size_t repeat)  )
    {   assert( name.size() == value.size() );
        //
        double time_min = 1.0;
        double sec = CppAD::time_test(test,  time_min);
        size_t n   = global_x.size();
        size_t m   = global_problem_ptr->size_range();
        size_t nnz = global_nnz;
        assert( nnz != 0 );
        //
        write_csv(
            csv_file,
            name,
            value,
            implement,
            problem,
            global_colpack,
            global_indirect,
            global_optimize,
            global_setup,
            global_reverse,
            global_onepass,
            global_correct,
            n,
            m,
            nnz,
            sec
        );
        return;
    }
    // -----------------------------------------------------------------------
    // check_arg
    size_t check_arg(
        const std::string&                arg_name        ,
        const std::string&                arg_value       ,
        const CppAD::vector<std::string>& possible_value  )
    {   for(size_t i = 0; i < possible_value.size(); ++i)
        {   if( possible_value[i] == arg_value )
                return i;
        }
        cerr << "speed: " << arg_name << " not one of the following:\n";
        cerr << possible_value[0];
        for(size_t i = 1; i < possible_value.size(); ++i)
            cerr << ", " << possible_value[i];
        cerr << "\n";
        std::exit(1);
        return possible_value.size();
    }
    // -----------------------------------------------------------------------
}
// ----------------------------------------------------------------------------
// main
int main(int argc, char* argv[])
{   std::string usage = "speed csv_file\\\n\t";
    usage += "name_1=value_1 ... name_k=value_k\\\n\t";
    usage += "implement problem size \\\n\t";
    usage += "[--colpack] [--indirect] [--optimize] [--setup] ";
    usage += "[--reverse] [--onepass] [--correct]\n";
    usage += "csv_file:   is the csv file where the resutls are written\n";
    usage += "name_k:     k-th extra column name in the csv file\n";
    usage += "value_k:    value for the k-th extra colum in the csv file\n";
    usage += "method:     double, adolc, cppad, subgraph or cppadcg\n";
    usage += "problem:    dficfj, dierfj, deptfg, or dgl1fg\n";
    usage += "size:       number of sub-intervals\n";
    usage += "--colpack:  use Colpack for coloring algorithm\n";
    usage += "--indirect: use indirect method for adolc Hessians\n";
    usage += "--optimize: optimize the cppad function object\n";
    usage += "--setup:    include setup in timing\n";
    usage += "--reverse:  use reverse (instead of forward) mode\n";
    usage += "--onepass:  only use one pass of the tape to compute Jacobian\n";
    usage += "--correct:  include a correctnes check\n";
    if( argc < 5 )
    {   cerr << usage;
        std::exit(1);
    }
    size_t iarg = 0;
    //
    // csv_file
    std::string csv_file = argv[++iarg];
    //
    // name_k=value_k
    CppAD::vector<std::string> name, value;
    std::string name_value = argv[iarg + 1];
    size_t pos             = name_value.find("=");
    while( pos != std::string::npos )
    {   size_t len = name_value.size();
        name.push_back( name_value.substr(0, pos) );
        value.push_back( name_value.substr(pos+1, len) );
        ++iarg;
        if( size_t(argc) < iarg + 4 )
        {   cerr << usage;
            std::exit(1);
        }
        name_value = argv[iarg + 1];
        pos = name_value.find("=");
    }
    //
    // implement, implement_vec, implement_index
    std::string implement = argv[++iarg];
    CppAD::vector<std::string> implement_vec(5);
    implement_vec[0] = "adolc";
    implement_vec[1] = "cppad";
    implement_vec[2] = "subgraph";
    implement_vec[3] = "cppadcg";
    implement_vec[4] = "subcg";
    size_t implement_index = check_arg("implement", implement, implement_vec);
    //
    // problem, problem_vec
    std::string problem = argv[++iarg];
    CppAD::vector<std::string> problem_vec(4);
    problem_vec[0] = "dficfj";
    problem_vec[1] = "dierfj";
    problem_vec[2] = "deptfg";
    problem_vec[3] = "dgl1fg";
    check_arg("problem", problem, problem_vec);
    //
    // size
    if( std::atoi( argv[++iarg] ) <= 0 )
    {   cerr << "speed: size is less than or equal zero\n";
        std::exit(1);
    }
    size_t size = size_t( std::atoi( argv[iarg] ) );
    //
    // option list
    bool colpack  = false;
    bool indirect = false;
    bool optimize = false;
    bool setup    = false;
    bool reverse  = false;
    bool onepass  = false;
    bool correct  = false;
    while(iarg + 1 < size_t(argc) )
    {   std::string option = argv[++iarg];
        if ( option == "--colpack" )
            colpack = true;
        else if ( option == "--indirect" )
            indirect = true;
        else if ( option == "--optimize" )
            optimize = true;
        else if ( option == "--setup" )
            setup = true;
        else if( option == "--reverse" )
            reverse = true;
        else if ( option == "--onepass" )
            onepass = true;
        else if ( option == "--correct" )
            correct = true;
        else
        {   cerr << "speed: " << argv[iarg] << " is not a valid option\n";
            std::exit(1);
        }
    }
    //
    // test_jac
    typedef void (*test_function)(size_t repeat);
    CppAD::vector<test_function> test_jac(implement_vec.size());
    test_jac[0] = test_adolc_jac;
    test_jac[1] = test_cppad_jac;
    test_jac[2] = test_subgraph_jac;
    test_jac[3] = test_cppadcg_jac;
    test_jac[4] = test_subcg_jac;
    // test_hes
    CppAD::vector<test_function> test_hes(implement_vec.size());
    test_hes[0] = test_adolc_hes;
    test_hes[1] = test_cppad_hes;
    test_hes[2] = test_subgraph_hes;
    test_hes[3] = test_cppadcg_hes;
    test_hes[4] = test_subcg_hes;
    //
    if( problem == "dficfj" )
    {   fun_dficfj fun_obj(size);
        fun_base* problem_ptr = static_cast<fun_base*>( &fun_obj );
        initialize(
            implement,
            problem_ptr,
            size,
            colpack,
            indirect,
            optimize,
            setup,
            reverse,
            onepass,
            correct
        );
        assert( global_x.size() == 8 * size );
        assert( fun_obj.size_range() == global_x.size() );
        run_test(
            csv_file,
            name,
            value,
            implement,
            problem,
            test_jac[implement_index]
        );
    }
    else if( problem == "dierfj" )
    {   fun_dierfj fun_obj(size);
        fun_base* problem_ptr = static_cast<fun_base*>( &fun_obj );
        initialize(
            implement,
            problem_ptr,
            size,
            colpack,
            indirect,
            optimize,
            setup,
            reverse,
            onepass,
            correct
        );
        assert( global_x.size() == 15 * size + 3 );
        assert( fun_obj.size_range() == global_x.size() );
        run_test(
            csv_file,
            name,
            value,
            implement,
            problem,
            test_jac[implement_index]
        );
    }
    else if( problem == "deptfg" )
    {   fun_deptfg fun_obj(size, size);
        fun_base* problem_ptr = static_cast<fun_base*>( &fun_obj );
        initialize(
            implement,
            problem_ptr,
            size,
            colpack,
            indirect,
            optimize,
            setup,
            reverse,
            onepass,
            correct
        );
        assert( global_x.size() == size * size );
        assert( fun_obj.size_range() == 1 );
        run_test(
            csv_file,
            name,
            value,
            implement,
            problem,
            test_hes[implement_index]
        );
    }
    else if( problem == "dgl1fg" )
    {   fun_dgl1fg fun_obj(size);
        fun_base* problem_ptr = static_cast<fun_base*>( &fun_obj );
        initialize(
            implement,
            problem_ptr,
            size,
            colpack,
            indirect,
            optimize,
            setup,
            reverse,
            onepass,
            correct
        );
        assert( global_x.size() == size );
        assert( fun_obj.size_range() == 1 );
        run_test(
            csv_file,
            name,
            value,
            implement,
            problem,
            test_hes[implement_index]
        );
    }
    if( correct )
    {   if( ! global_correct_ok )
        {   cerr << "speed: correctness check failed\n";
            std::exit(1);
        }
    }
    std::cout << "speed: OK\n";
    std::exit(0);
}
