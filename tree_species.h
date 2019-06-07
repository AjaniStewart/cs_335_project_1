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

#ifndef TREE_SPECIES_H
#define TREE_SPECIES_H

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

  std::vector<std::string>::iterator begin();

  std::vector<std::string>::iterator end();

  std::string& operator[](int index);

  bool contains( const std::string& s);

private:
  std::vector<std::string> species;
};

#endif //TREE_SPECIES_H