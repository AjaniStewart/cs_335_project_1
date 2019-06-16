/*******************************************************************************
  Title          : tree_species.h
  Author         : Ajani Stewart
  Created on     : May 27, 2019
  Description    : The interface file for the TreeSpecies class
  Purpose        : To provide a minimal interface to be used to interact with 
                   the Tree species.
  Usage          : 
  Build with     : 
*******************************************************************************/

#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "__tree_species.h"

class TreeSpecies : public __TreeSpecies {
public:
  TreeSpecies() = default;

  void print_all_species( std::ostream& out ) const;

  int number_of_species() const;

  int add_species( const string& species );

  std::list<std::string> get_matching_species( const std::string& partial_name ) const;

  bool contains( const std::string& s);

private:
  std::vector<std::string> species;
};

bool is_matching_species( const std::string& spc_name, const std::string& partial_name );
