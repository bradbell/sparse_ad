/*
{xsrst_begin speed_write_csv}

.. include:: ../preamble.rst

{xsrst_spell
    csv
    onepass
    nnz
}

Writes One Row of the Speed Csv Output File
###########################################

Syntax
******

| ``write_csv`` (
| |tab| *csv_file* ,
| |tab| *name* ,
| |tab| *value* ,
| |tab| *implement* ,
| |tab| *problem* ,
| |tab| *colpack* ,
| |tab| *indirect* ,
| |tab| *optimize* ,
| |tab| *setup* ,
| |tab| *reverse* ,
| |tab| *onepass* ,
| |tab| *correct* ,
| |tab| *n* ,
| |tab| *m* ,
| |tab| *nnz* ,
| |tab| *sec*
| )

Prototype
*********
{xsrst_file
    // BEGIN_PROTOTYPE
    // END_PROTOTYPE
}

csv_file
********
is the name of the csv file.
If the file does not exist the header is written to the file.
Otherwise, the header is checked to make sure it is as expected.
The data corresponding to this call is written as one row at the
end of the file.

header
======
The header for the csv file is its first row.
The header row begins with

| |tab| *first_name* , ... , *last_name* , ``implement,problem`` ,

where *first_name* is the first component of *name* ,
and *last_name* is the last component of *name* .
The header row ends with

| |tab| ``colpack,indirect,optimize,setup,reverse,onepass,correct,n,m,nnz,sec``

There are no spaces in the header row.

name, value
***********
The vectors *name* and *value* have the same size.
The *i*-th element of *name* is the header entry
corresponding to the *i*-th element of *value* .

implement
*********
This is the value written in the *implement* column.

problem
*******
This is the value written in the problem column.

options
*******
The option arguments are
*colpack* ,
*indirect* ,
*optimize* ,
*setup* ,
*reverse* ,
*onepass* ,
*correct* .
For each option, if it is true (false)
the string ``true`` ( ``false`` ) is written in the corresponding column.

n, m, nnz
*********
These values are converted to strings and written in
the corresponding columns.

sec
***
This value is converted to a string using fixed point notation
and ten digits after the decimal point.

{xsrst_end speed_write_csv}
*/
# include <iostream>
# include <iomanip>
# include "write_csv.hpp"

// BEGIN_PROTOTYPE
void write_csv(
    const std::string&                csv_file   ,
    const CppAD::vector<std::string>& name       ,
    const CppAD::vector<std::string>& value      ,
    const std::string&                implement  ,
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
    header += "implement,problem,";
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
    << implement          << ","
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
