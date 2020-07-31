# ifndef SRC_UTILITY_CPPADCG_LIBRARY_HPP
# define SRC_UTILITY_CPPADCG_LIBRARY_HPP

# include <cppad/cg/cppadcg.hpp>
# include <src/typedef.hpp>

class cppadcg_library {
private:
    // dynamic_lib_
    std::unique_ptr< CppAD::cg::DynamicLib<double> > dynamic_lib_;
    //
    // model_ (contains a reference to dynamic_lib_)
    std::unique_ptr< CppAD::cg::GenericModel<double> > model_;
public:
    // fun_name( cg_fun, file_name)
    cppadcg_library(
        CppAD::ADFun<cg_double>& cg_fun    ,
        const std::string&       file_name
    );
    //
    // fun_name( file_name )
    cppadcg_library(const std::string& file_name);
    //
    // fun_name()
    cppadcg_library(void);
    //
    // swap(other_fun)
    void swap(cppadcg_library& other_fun);
    //
    // y = fun_name(x)
    d_vector operator()(const d_vector& x);
};

# endif
