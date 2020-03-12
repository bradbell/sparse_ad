// vim: set expandtab:
/*
$begin cppadcg_library$$
$spell
    CppAD
    cppadcg
    cg
$$

$section CppADCodeGen Dynamic Libraries$$

$head Syntax$$
$codei%cppadcg_library %fun_name%(%cg_fun%, %file_name%)
%$$
$codei%cppadcg_library %fun_name%(%file_name%)
%$$
$codei%cppadcg_library %fun_name%()
%$$
$icode%fun_name%.swap(%other_fun%)
%$$
$icode%y% = %fun_name%(%x%)
%$$

$head Prototype$$
$srcthisfile%
    0%// BEGIN_CTOR_AD_FUN_FILE_NAME%// END_CTOR_AD_FUN_FILE_NAME%1
%$$
$srcthisfile%
    0%// BEGIN_CTOR_FILE_NAME%// END_CTOR_FILE_NAME%1
%$$
$srcthisfile%
    0%// BEGIN_CTOR_VOID%// END_CTOR_VOID%1
%$$
$srcthisfile%
    0%// BEGIN_SWAP_OTHER_FUN%// END_SWAP_OTHER_FUN%1
%$$
$srcthisfile%
    0%// BEGIN_FUN_NAME_X%// END_FUN_NAME_X%1
%$$

$head fun_name$$
This is the name of the $code cppadcg_library$$ object.

$head other_fun$$
This is the name of another $code cppadcg_library$$ object.

$head cg_fun$$
This is a CppAD function object that corresponds to a function
$latex f : \B{R}^n \rightarrow \B{R}^m$$.
If this arguments is present in the constructor,
a new dynamic library is created.

$head file_name$$
This is the absolute or relative path for the
file that contains the dynamic library.
It does not include the files extension at the end that is used
for dynamic libraries on this system.
If $icode cg_fun$$ is not present in the constructor,
it must have been present in a previous constructor with the same
$icode file_name$$.

$head swap$$
This exchanges the library in $icode fun_name$$ with the library in
$icode other_fun$$.

$head x$$
is a vector of size $icode n$$ specifying the argument value
at which the function will be evaluated.

$head y$$
This return value has size $icode m$$ and is the value of $latex f(x)$$.


$end
*/

# include "cppadcg_library.hpp"

// BEGIN_CTOR_AD_FUN_FILE_NAME
cppadcg_library::cppadcg_library(
    CppAD::ADFun<cg_double>& cg_fun    ,
    const std::string&       file_name )
// END_CTOR_AD_FUN_FILE_NAME
{
    // Generate source code
    CppAD::cg::ModelCSourceGen<double> cgen(cg_fun, "model");
    CppAD::cg::ModelLibraryCSourceGen<double> libcgen(cgen);

    // Compile source, create the library file, and load the library
    CppAD::cg::DynamicModelLibraryProcessor<double> proc(libcgen, file_name);
    CppAD::cg::ClangCompiler<double> compiler;
    bool loadLib = true;
    dynamic_lib_ = proc.createDynamicLibrary(compiler, loadLib);
    //
    // create the model object
    model_        = dynamic_lib_->model("model");
}
// BEGIN_CTOR_FILE_NAME
cppadcg_library::cppadcg_library(
    const std::string&       file_name )
// END_CTOR_FILE_NAME
{   // file name plus extension used for dynamic libraries on this system
    std::string file_name_ext = file_name +
        CppAD::cg::system::SystemInfo<>::DYNAMIC_LIB_EXTENSION;

    // load the library
    CppAD::cg::DynamicLib<double>* ptr =
        new CppAD::cg::LinuxDynamicLib<double>(file_name_ext);
    dynamic_lib_  = std::unique_ptr< CppAD::cg::DynamicLib<double> >(ptr);
    //
    // create the model object
    model_        = dynamic_lib_->model("model");
}
// BEGIN_CTOR_VOID
cppadcg_library::cppadcg_library(void)
// END_CTOR_VOID
{ }
// BEGIN_SWAP_OTHER_FUN
void cppadcg_library::swap(cppadcg_library& other_fun)
// END_SWAP_OTHER_FUN
{   std::swap(dynamic_lib_, other_fun.dynamic_lib_);
    std::swap(model_, other_fun.model_ );
}
// BEGIN_FUN_NAME_X
d_vector cppadcg_library::operator()(const d_vector& x)
// END_FUN_NAME_X
{   return model_-> ForwardZero(x);
}
