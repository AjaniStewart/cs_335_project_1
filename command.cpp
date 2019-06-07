/******************************************************************************
  Title          : command.cpp
  Author         : Stewart Weiss
  Created on     : February 18, 2019
  Description    : Implementation of the Command class
  Purpose        : Represents a Command object
  Usage          : 
  Build with     : g++ -c command.cpp
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

******************************************************************************/

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "command.h"

#define MAXBUF 4096


void die( string s )
{
    std::cerr << "\t" << s << ".";
}


Command::Command () : type(null_cmmd) {} 


bool Command::get_next (istream & in ) 
{
    string firstword;
    string rest_of_line;
    char line[MAXBUF];
    bool result = false;
    
    istringstream iss;

    if ( in.good() ) {
        in.getline(line, MAXBUF);
        if ( in.eof() )
            return false;
        iss.str(line);
        iss >> firstword;
        if ( !iss ) {
            std::cerr << line << ": ";
            die(" Error in command file syntax");
            return false;
        }
        if ( firstword.compare("tree_info") == 0 ) {
            getline(iss,rest_of_line);
            if ( !iss ) {
                std::cerr << line << ": ";
                die(" Missing tree to find for tree_info command");
                return false;
            }
            this->type = tree_info_cmmd;
            this->tree_to_find = rest_of_line;
        } else if ( firstword.compare("listall_names") == 0 ) {
            this->type = listall_names_cmmd;
        } else if ( firstword.compare("listall_inzip") == 0  ) {
            iss >> this->zip;
            if ( !iss ) {
                std::cerr << line << ": ";
                die(" Missing zip code for listall_inzip command");
                return false;
            }
            this->type     = listall_inzip_cmmd;
        }          
        else if  ( firstword.compare("list_near") == 0 ) {
            this->type = list_near_cmmd;
            iss >> latitude;
            if ( !iss ) {
                std::cerr << line << ": ";
                die ( " Failed to get latitude argument for save_by_loc command");
                return false;
                }
            if ( (latitude <= -90) || (latitude >= 90) ) {
                die ( " Latitude must be in range (-90,90)");
                return false;
                }

            iss >> longitude;
            if ( !iss ) {
                std::cerr << line << ": ";
                die ( " Failed to get longitude argument for list_near command");
                return false;
                }

            if ( (longitude < -180) || (longitude > 180) ) {
                die ( " Longitude must be in range [-180,180]");
                return false;
                }

            iss >> distance;
            if ( !iss ) {
                std::cerr << line << ": ";
                die ( " Failed to get distance argument for list_near command");
                return false;
                }
            if ( 0 > distance ) {
                die ( " Distance argument for list_near command is negative");
                return false;
            }
        }
        else if ( firstword.compare("print_all") == 0 ) {
            this->type = print_all_cmmd;
        }
        else if ( firstword.compare("remove_stumps") == 0 ) {
            this->type = remove_stumps_cmmd;
        }
        else
            this->type = bad_cmmd;       
    }
    else {
        this->type = null_cmmd;
    }
    result = true;
    return result;
}




Command_type Command::type_of () const
{
    return type;
}

void  Command::get_args (
            string    & arg_tree_to_find,
            int       & arg_zip,
            double    & arg_latitude, 
            double    & arg_longitude, 
            double    & arg_distance,
            bool      & result
            ) const
{
    result = true;
    if ( tree_info_cmmd == type )
        arg_tree_to_find = tree_to_find;
    else if ( listall_inzip_cmmd == type )
        arg_zip = zip;
    else if ( list_near_cmmd == type ) {
        arg_latitude  = latitude;
        arg_longitude = longitude;
        arg_distance  = distance;
    }
    else
        result = false;
}


