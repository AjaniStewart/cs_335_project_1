/*******************************************************************************
  Title          : tree_collection.h
  Author         : Ajani Stewart
  Created on     : June 1, 2019
  Description    : The interface file for the TreeCollection class
  Purpose        : To provide a minimal interface to be used to interact with 
                   the Tree species.
  Usage          : 
  Build with     : 
*******************************************************************************/
#ifndef _TREE_COLLECTION_H_
#define _TREE_COLLECTION_H_


#include <string>
#include <list>
#include <iostream>
// #include <unordered_map>

#include "__tree_collection.h"
#include "AvlTree.h"
#include "tree.h"
#include "tree_species.h"


//for documentation of the functions, go to __TreeCollection.h
class TreeCollection : public __TreeCollection {
public:

  TreeCollection();

  int total_tree_count();

  int count_of_tree_species( const std::string& species_name );

  int count_of_tree_species_in_boro( const std::string& spc_name, 
                                      const std::string& boro_name);
                                    
  int get_counts_of_trees_by_boro ( const std::string& spc_name, boro tree_count[5] );

  int count_of_trees_in_boro( const std::string& boro_name );

  int add_tree( Tree& new_tree );

  void print_all_species( std::ostream& out ) const;

  void print( std::ostream& out ) const;

  std::list<std::string> get_matching_species( const std::string& spc_name ) const;

  std::list<std::string> get_all_in_zipcode( int zipcode ) const;

  std::list<std::string> get_all_near( double latitude, double longitude, double distance ) const;

private:
  AvlTree<Tree> trees;
  TreeSpecies tree_species;
  // std::unordered_map<std::string, int> species_map;

  //should use enum class but dont want to keep writing static_cast
  enum Boros {
    BRONX = 0,
    MANHATTAN,
    BROOKLYN,
    QUEENS,
    STATEN_ISLAND,
    BORO_COUNT
  };

  size_t size = 0;

};

#endif /* _TREE_COLLECTION_H_ */