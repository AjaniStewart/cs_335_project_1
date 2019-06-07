/*******************************************************************************
  Title          : tree_collection.cpp
  Author         : Ajani Stewart
  Created on     : June 1, 2019
  Description    : The interface file for the TreeCollection class
  Purpose        : To provide a minimal interface to be used to interact with 
                   the Tree species.
  Usage          : 
  Build with     : -std=c++11 -lm
*******************************************************************************/
#include <unordered_set>

#include "tree_collection.h"
#include "tree.h"

TreeCollection::TreeCollection() : trees( Tree() ) { }

int TreeCollection::total_tree_count() {
  return size;
}

int TreeCollection::count_of_tree_species( const std::string& spc_name ) {
  // return species_map.count(spc_name) > 0 ? species_map[spc_name] : 0;
  return trees.countIf([&](const Tree& t) {
    return t.common_name() == spc_name;
  });
}

int TreeCollection::count_of_tree_species_in_boro( const std::string& spc_name, 
  const std::string& boro_name) {

  return trees.countIf([&](const Tree& t) {
    return t.common_name() == spc_name && t.borough_name() == boro_name;
  });   
}

int TreeCollection::get_counts_of_trees_by_boro ( const std::string& spc_name, boro tree_count[5] ) {
  for ( int b = BRONX; b < BORO_COUNT; ++b ) {
    switch (b) {
      case BRONX:
        tree_count[b].count = trees.countIf([&](const Tree& t){
          return t.borough_name() == "Bronx" && t.common_name() == spc_name;
        });
        tree_count[b].name = "Bronx";
        break;

      case MANHATTAN:
        tree_count[b].count = trees.countIf([&](const Tree& t){
          return t.borough_name() == "Manhattan" && t.common_name() == spc_name;
        });
        tree_count[b].name = "Manhattan";
        break;

      case BROOKLYN:
        tree_count[b].count = trees.countIf([&](const Tree& t){
          return t.borough_name() == "Brooklyn" && t.common_name() == spc_name;
        });
        tree_count[b].name = "Brooklyn";
        break;

      case QUEENS:
        tree_count[b].count = trees.countIf([&](const Tree& t){
          return t.borough_name() == "Queens" && t.common_name() == spc_name;
        });
        tree_count[b].name = "Queens";
        break;

      default:
        tree_count[b].count = trees.countIf([&](const Tree& t){
          return t.borough_name() == "Staten Island" && t.common_name() == spc_name;
        });
        tree_count[b].name = "Staten Island";
    }
  }

  int sum = 0;
  for (int i = 0; i < BORO_COUNT; ++i) {
    sum += tree_count[i].count;
  }
  return sum;
}

int TreeCollection::count_of_trees_in_boro( const std::string& boro_name ) {
  return trees.countIf([&boro_name](const Tree& t) {
    return t.borough_name() == boro_name;
  });
}

int TreeCollection::add_tree( Tree& tree ) {
  int result = trees.insert(tree);

  if (result) {
    if (!tree_species.contains(tree.common_name())) {
      tree_species.add_species(tree.common_name());
    }
    result++;
  }

  return result;
} 

void TreeCollection::print_all_species( std::ostream& os ) const {
  trees.printTreeToStream(os);
}

std::list<std::string> TreeCollection::get_matching_species( const std::string& s ) const {
  return tree_species.get_matching_species(s);
}

std::list<std::string> TreeCollection::get_all_in_zipcode( int zipcode ) const {
  auto trs = trees.collectIntoListIf([zipcode](const Tree& t) {
    return t.zip_code() == zipcode;
  });

  std::unordered_set<std::string> set;
  std::list<std::string> result;

  for ( const auto& e : trs ) {
    if (set.count(e.common_name()) == 0) {
      result.push_back(e.common_name());
      set.insert(e.common_name());
    }
  }

  return result;
}

std::list<std::string> TreeCollection::get_all_near( double lat, double lgt, double dntc ) const {

}