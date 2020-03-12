// vim: set expandtab:
/*
$begin speed_write_csv$$
$spell
    Csv
    colpack
    onepass
    nnz
$$

$section Writes One Row of the Speed Csv Output File$$

$head Syntax$$
$codei%write_csv(
    %csv_file% ,
    %name%     ,
    %value%    ,
    %method%   ,
    %problem%  ,
    %colpack%  ,
    %indirect% ,
    %optimize% ,
    %setup%    ,
    %reverse%  ,
    %onepass%  ,
    %correct%  ,
    %n%        ,
    %m%        ,
    %nnz%      ,
    %sec%
)%$$

$head Prototype$$
$srcthisfile%
    0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head csv_file$$
is the name of the csv file.
If the file does not exist the header is written to the file.
Otherwise, the header is checked to make sure it is as expected.
The data corresponding to this call is written as one row at the
end of the file.

$subhead header$$
The header for the csv file is its first row.
The header row begins with
$codei%
    %first_name%,%...%,%last_name%,method,problem,
%$$
where $icode first_name$$ is the first component of $icode name$$,
and $icode last_name$$ is the last component of $icode name$$.
The header row ends with
$codei%
    colpack,indirect,optimize,setup,reverse,onepass,correct,n,m,nnz,sec
%$$
There are no spaces in the header row.

$head name, value$$
The vectors $icode name$$ and $icode value$$ have the same size.
The $th i$$ element of $icode name$$ is the header entry
corresponding to the $th i$$ element of $icode value$$.

$head method$$
This is the value written in the method column.

$head problem$$
This is the value written in the problem column.

$head options$$
The option arguments are
$icode colpack$$,
$icode indirect$$,
$icode optimize$$,
$icode setup$$,
$icode reverse$$,
$icode onepass$$,
$icode correct$$.
For each option, if it is true (false)
the string $code true$$ ($code false$$) is written in the corresponding column.

$head n, m, nnz$$
These values are converted to strings and written in
the corresponding columns.

$head sec$$
This value is converted to a string using fixed point notation
and ten digits after the decimal point.

$end
*/
# include <iostream>
# include <iomanip>
# include "write_csv.hpp"

// BEGIN_PROTOTYPE
void write_csv(
    const std::string&                csv_file   ,
    const CppAD::vector<std::string>& name       ,
    const CppAD::vector<std::string>& value      ,
    const std::string&                method     ,
    const std::string&                problem    ,
    bool                              colpack    ,
    bool                              indirect   ,
    bool                              optimize   ,
    bool                              setup      ,
    bool                              reverse    ,
    bool                              onepass    ,
    bool                              correct    ,
    size_t                            n          ,
    size_t                            m          ,
    size_t                            nnz        ,
    double                            sec        )
// END_PROTOTYPE
{   assert( name.size() == value.size() );
    std::string header = "";
    for(size_t k = 0; k < name.size(); ++k)
        header += name[k] + ",";
    header += "method,problem,";
    header += "colpack,indirect,optimize,setup,reverse,onepass,correct,";
    header += "n,m,nnz,sec";
    //
    const char* file_name = csv_file.c_str();
    //
    CppAD::vector<std::string> bool2str(2);
    bool2str[0] = "false";
    bool2str[1] = "true";
    //
    std::ifstream ifile(file_name);
    if( ifile.is_open() )
    {   // check header
        std::string line;
        std::getline(ifile, line, '\n');
        if( line != header )
        {   std::cerr << file_name << " header is not equal to\n";
            std::cerr << header << "\n";
            std::exit(1);
        }
        ifile.close();
    }
    else
    {   std::ofstream ofile(file_name);
        ofile << header << "\n";
    }
    std::fstream afile(file_name, std::fstream::app );
    for(size_t k = 0; k < value.size(); ++k)
        afile << value[k] << ",";
    afile
    << method             << ","
    << problem            << ","
    << bool2str[colpack]  << ","
    << bool2str[indirect] << ","
    << bool2str[optimize] << ","
    << bool2str[setup]    << ","
    << bool2str[reverse]  << ","
    << bool2str[onepass]  << ","
    << bool2str[correct]  << ","
    << n       << ","
    << m       << ","
    << nnz     << ","
    << std::fixed << std::setprecision(10) << sec << "\n";
    //
    afile.close();
}
