/*******************************************************************************
  Title          : main.cpp
  Author         : Stewart Weiss
  Created on     : February 18, 2019
  Description    : The main program for Project1, processing NYC Tree Data
  Purpose        : reads the tree data file and a command file, applying
                   commands to the tree data.
  Usage          : project1  datafile  commandfile
  Build with     : g++ -o project1 main.cpp tree.cpp tree_collection.cpp \
                        tree_species.cpp command.cpp utilities.cpp avl.cpp
  Modifications  : 
  
  
  License        :
    Copyright (c) 2019 Stewart Weiss

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
                   
 
*******************************************************************************/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <locale>
#include <string>
#include <cstring>
#include <vector>
#include <list>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <errno.h>
#include <limits.h>

#include "tree.h"
#include "tree_collection.h"
#include "command.cpp"

using namespace std;

/******************************************************************************
Data Dictionary for Input File:
 Position   Type   Name        Description
 1          int    tree_id;    unique id that  identifies the tree
 4          int    tree_dbh;   specifies tree diameter
 7          string status;     valid values: ”Alive”, ”Dead”, ”Stump”, or the 
                               empty string 
 8          string health;     valid values: ”Good”, ”Fair”, ”Poor”, or the 
                               empty string
 10         string spc_common; the common name of the tree, such as “white oak” 
                               or a possibly empty string
 25         string  address;   the nearest street address to the tree
 26         int    zipcode;    positive five digit integer (This means that any 
                               number from 0 to 99999 is acceptable. The values 
                               that are shorter are treated as if they had 
                               leading zeroes
 30         string boroname;   valid values: ”Manhattan”, ”Bronx”, ”Brooklyn”, 
                               ”Queens”, ”Staten Island”
 40         double latitude     spatial coordinate specifying position in 
                               New York State state plane coordinates
 41         double longitude     spatial coordinate specifying position in 
                               New York State state plane coordinates

 ******************************************************************************/

const string boro_name[5] = {
     "Bronx","Manhattan","Brooklyn","Queens","Staten Island"
      };


class comma_numpunct : public std::numpunct<char>
{
  protected:
    virtual char do_thousands_sep() const
    {
        return ',';
    }

    virtual std::string do_grouping() const
    {
        return "\03";
    }
};




int main( int argc, char* argv[])
{

    ifstream        inputfile;
    ifstream        commandfile;
    TreeCollection  NYCTrees;
    string          tree_line;
    Tree            temp_tree;
    list<string>    matching_species;
    string          input_string;
    boro  tree_counts_by_borough[5] = { {0,"Bronx"},
                                        {0,"Manhattan"},
                                        {0,"Brooklyn"},
                                        {0,"Queens"},
                                        {0,"Staten Island"} };
    Command   command;
    string    treename;
    int       zipcode;
    double    latitude, longitude, distance;
    bool      result;
    string    prev;
    list<string>::iterator it;
    int freq;
/*    
    for (int i =0; i < argc; i++ ){
	cerr << argv[i] <<endl;
    }
*/ 
    if ( argc < 3 ) {
        cerr << "\n Usage: " << argv[0] << " input_file  command_file" << endl;
        exit(1);
    }

    inputfile.open(argv[1]);
    if ( inputfile.fail() ) {
        cerr << "Could not open data file " << argv[1] << " for reading" << endl;
        exit(1);
    }

    commandfile.open(argv[2]);
    if ( commandfile.fail() ) {
        cerr << "Could not open command file " << argv[2] << " for reading" << endl;
        exit(1);
    }


    int count = 0;
    int numtrees = 0;
    while( getline(inputfile, tree_line) ) {
       // Create a tree to insert into the TreeCollection
            Tree  temp_tree(tree_line);
            if ( 0 != temp_tree.id() ) {
                numtrees += NYCTrees.add_tree(temp_tree);
                count++;
                }
            else {
                cerr << "bad data" << endl;
            }
    }
    
    inputfile.close();

    // Create a locale to use that puts commas in long integers
    locale comma_locale(std::locale(), new comma_numpunct());

    // Save the original locale for when we do not want those commas
    locale orig_locale = cout.getloc();

    // tell cout to use our new locale.
    //cout.imbue(comma_locale);

    while ( ! commandfile.eof() ) {
        if ( ! command.get_next(commandfile) ) {
            if ( ! commandfile.eof() ) {
                cerr << "Error getting command.\n";
                continue;
            }
            else
                return 1;
        }
        command.get_args(treename, zipcode, latitude, longitude, 
                         distance, result);

        cout << "Command: " ;
        switch ( command.type_of() )
        {
            case tree_info_cmmd:
                // tell cout to use our new locale.
                cout.imbue(comma_locale);

                cout << "tree_info " << treename << endl;
                matching_species = NYCTrees.get_matching_species(treename);
                if ( matching_species.size() == 0 )
                    cout << "There are no matching species.\n";
                else {
                    cout << "The matching species are: \n";
                    for ( list<string>::iterator it = matching_species.begin();
                                            it != matching_species.end(); ++it )
                        cout << "\t" << *it << endl;
                    cout << "Popularity in the city:\n";
                    
                    int total = 0;
                    for ( int i = 0; i < 5; i++ )
                        tree_counts_by_borough[i].count = 0;

                    // Get total numbers of all matching species by boro
                    for ( list<string>::iterator it = matching_species.begin();
                                            it != matching_species.end(); ++it )
                        total += NYCTrees.get_counts_of_trees_by_boro(*it, tree_counts_by_borough);

                    // Print NYC total first, then print by boro
                    double percentage;
                    int   total_in_city = NYCTrees.total_tree_count();
                    percentage = total_in_city > 0? (double) 100.00 *total / total_in_city:0;
                    cout << "\t" 
                         << left << setw(15) << "New York City" 
                         << right << setw(12) << total 
                         << "  (" << right << setw(12) << total_in_city << ")" 
                         << right << setw(12) << fixed << setprecision(2)
                         << percentage << "%"
                         << endl;     

                    for ( int i = 0; i < 5; i++ ) {
                        int boro_total = NYCTrees.count_of_trees_in_boro(boro_name[i]);
                        percentage = boro_total > 0? 
                                 (double) 100.00 * tree_counts_by_borough[i].count / boro_total:0;
                        cout << "\t" 
                             << left << setw(15) << boro_name[i]
                             << right << setw(12) << tree_counts_by_borough[i].count 
                             << "  (" << right << setw(12) << boro_total << ")" 
                             << right << setw(12) << fixed << setprecision(2)
                             << percentage << "%"
                             << endl;     
                    }
                }
                cout.imbue(orig_locale);
                break;
                
            case listall_names_cmmd:
                cout << "listall_names"  << endl;
                NYCTrees.print_all_species(cout);
                break;
                
            case print_all_cmmd:
                cout << "print_all"  << endl;
                NYCTrees.print(cout);
                break;
                
            case remove_stumps_cmmd:
                cout << "remove_stumps"  << endl;
                
                break;
            case list_near_cmmd:
                cout << "list_near "  << fixed << setprecision(6) << latitude << " " << longitude << " " << distance << endl;
                cout.imbue(comma_locale);
                matching_species = NYCTrees.get_all_near(latitude, longitude, distance); 
                it = matching_species.begin();
                freq = 0;
                prev = "";
                while ( it != matching_species.end() ) {
                    if ( prev == *it )
                        freq++;
                    else {
                        if ( prev != "" ) 
                            cout << "\t" 
                                 << left << setw(22) << prev
                                 << right << setw(8) << freq << endl;
                        freq = 1;
                        prev = *it;
                    }
                    it++;
                }
                if ( prev != "" ) 
                    cout << "\t" << left << setw(22) << prev
                         << right << setw(8) << freq << endl;
                cout.imbue(orig_locale);
                break;
                
            case listall_inzip_cmmd:
                cout << "listall_inzip " << zipcode << endl;
                cout.imbue(comma_locale);
                matching_species = NYCTrees.get_all_in_zipcode(zipcode);      
                it = matching_species.begin();
                freq = 0;
                prev = "";
                while ( it != matching_species.end() ) {
                    if ( prev == *it )
                        freq++;
                    else {
                        if ( prev != "" ) 
                            cout << "\t" 
                                 << left << setw(22) << prev
                                 << right << setw(8) << freq << endl;
                        freq = 1;
                        prev = *it;
                    }
                    it++;

                }
                if ( prev != "" ) 
                    cout << "\t" << left << setw(22) << prev
                         << right << setw(8) << freq << endl;
                cout.imbue(orig_locale);
                break;
            case bad_cmmd:
                    cerr << "bad command" << endl;
                    break;
            default:
                    break;
        }
    }
    commandfile.close();
    return 0;
}





