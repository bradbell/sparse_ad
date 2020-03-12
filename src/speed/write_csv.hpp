// vim: set expandtab:
# ifndef SRC_SPEED_WRITE_CSV_HPP
# define SRC_SPEED_WRITE_CSV_HPP

# include <fstream>
# include <string>
# include <cppad/utility/vector.hpp>

void write_csv(
    const std::string&                csv_file   ,
    const CppAD::vector<std::string>& name       ,
    const CppAD::vector<std::string>& value      ,
    const std::string&                method     ,
    const std::string&                problem    ,
    bool                              colpack    ,
    bool                              indirect   ,
    bool                              optimize   ,
    bool                              tape       ,
    bool                              reverse    ,
    bool                              onepass    ,
    bool                              correct    ,
    size_t                            n          ,
    size_t                            m          ,
    size_t                            nnz        ,
    double                            sec
);

# endif
