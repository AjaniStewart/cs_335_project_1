/*******************************************************************************
  Title          : cmmd_test_driver.cpp
  Author         : Stewart Weiss
  Created on     : March 13, 2018
  Description    : Tests the command.cpp file
  Purpose        : Shows how member class constructors are called.
  Usage          : cmmd_test_driver  commandfile outputfile
                   
  Build with     : g++ -Wall -g -o commandtester commandtester.cpp command.cpp
  Modifications  : 

  Notes
  This program can be used to test the Command class implementation.
  Create a file with a sequence of commands and compare the output of this
  program to the command file.  It should be identical except for whitespace.
 
*******************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "command.h"

using namespace std;


int main( int argc,  char* argv[] )
{

    ifstream  fin;    
    ofstream  fout;    
    Command   command;
    string    treename;
    int       zipcode;
    double    latitude, longitude, distance;
    bool      result;

    if ( argc < 3 ) {
        cerr << "Usage: " << argv[0] << " commandfile  outputfile\n";
        exit(1);
    }  

    fin.open( argv[1]);
    if ( !fin ) {
        cerr << "Could not open" << argv[1] << " for reading.\n";
        exit(1);
    }  

	fout.open(argv[2]);
    if ( fout.fail() ) {
        cerr << "Could not open " << argv[2] << " for writing.\n";
        exit(1);
    }

    while ( ! fin.eof() ) {
        if ( ! command.get_next(fin) ) {
            if ( ! fin.eof() )
                cerr << "Could not get next command.\n";
            return 1;
        }
        command.get_args(treename, zipcode, latitude, longitude, 
                         distance, result);

        switch ( command.type_of() )
        {
            case tree_info_cmmd:
                fout << "tree_info_cmmd " << treename << endl;
                break;
            case listall_names_cmmd:
                fout << "listall_names_cmmd " << endl;
                break;
            case list_near_cmmd:
                fout << "list_near_cmmd " << latitude << " " << longitude
                     << " " << distance  << endl;
                break;
            case listall_inzip_cmmd:
                fout << "listall_inzip_cmmd " << zipcode << endl;               
                break;
            case bad_cmmd:
                    fout << "bad command " << endl;
                    break;
            default:
                    break;
        }
    }
    fout.close();
}


