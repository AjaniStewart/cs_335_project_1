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
#include <cmath>
#include <iostream>
#include <locale>

#include "tree_collection.h"
#include "tree_species.h"
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

std::string remove_leading_whitespace(const std::string& s) {
  int i = 0;
  for (; i < s.size(); ++i) {
    if (!std::isspace(s[i])) 
      break;
  }

  return s.substr(i);
}

int TreeCollection::get_counts_of_trees_by_boro ( const std::string& spc_name, boro tree_count[5] ) {
  std::string ns = remove_leading_whitespace(spc_name);
  std::cout << "GET_COUNTS_OF_TREES_BY_BORO\n";
  // for ( int b = BRONX; b < BORO_COUNT; ++b ) {
  //   switch (b) {
      // case BRONX:
        //std::cout << tree_count[BRONX].name << "\n";
        tree_count[BRONX].count = trees.countIf([&](const Tree& t){
          //std::cout << t.borough_name() << "\n";
          return t.borough_name() == "bronx" && is_matching_species(t.common_name(),ns);
        });
        // break;

      // case MANHATTAN:
        //std::cout << tree_count[MANHATTAN].name << "\n";
        tree_count[MANHATTAN].count = trees.countIf([&](const Tree& t){
          // std::cout << t.borough_name() << "\n";
          return t.borough_name() == "manhattan" && is_matching_species(t.common_name(),ns);
        });
        // break;

      // case BROOKLYN:
        //std::cout << tree_count[BROOKLYN].name << "\n";
        tree_count[BROOKLYN].count = trees.countIf([&](const Tree& t){
          // std::cout << t.borough_name() << "\n";
          return t.borough_name() == "brooklyn" && is_matching_species(t.common_name(),ns);
        });
        // break;

      // case QUEENS:
        //std::cout << tree_count[QUEENS].name << "\n";
        tree_count[QUEENS].count = trees.countIf([&](const Tree& t){
          // std::cout << t.borough_name() << "\n";
          return t.borough_name() == "queens" && is_matching_species(t.common_name(),ns);
        });
        // break;

      // default:
        //std::cout << tree_count[STATEN_ISLAND].name << "\n";
        tree_count[STATEN_ISLAND].count = trees.countIf([&](const Tree& t){
          // std::cout << t.borough_name() << "\n";
          return t.borough_name() == "staten island" && is_matching_species(t.common_name(),ns);
        });
  //   }
  // }

  int sum = 0;
  for (int i = 0; i < BORO_COUNT; ++i) {
    sum += tree_count[i].count;
  }
  return sum;
}

std::string tolower(const std::string& s);

int TreeCollection::count_of_trees_in_boro( const std::string& boro_name ) {
  return trees.countIf([&boro_name](const Tree& t) {
    // std::cout << "TreeCollection::count_of_trees_in_boro: ";
    // std::cout << "Comparing \"" << t.borough_name() << "\" and " << boro_name  << "\n";
    auto ns = tolower(boro_name);
    return t.borough_name() == ns;
  });
}

int TreeCollection::add_tree( Tree& tree ) {
  int result = trees.insert(tree);

  if (result) {
    if (!tree_species.contains(tree.common_name())) {
      tree_species.add_species(tree.common_name());
      // std::cout << "add_tree: adding " << tree << "\n";
    }
    size++;
  }
  //std::cout << "size of tree collection is " << size << "\n";

  return result;
} 

void TreeCollection::print_all_species( std::ostream& os ) const {
  tree_species.print_all_species(os);
}

void TreeCollection::print ( std::ostream& out ) const {
  trees.printTreeToStream(out);
}


std::list<std::string> 
TreeCollection::get_matching_species( const std::string& s ) const {
  std::string ns = remove_leading_whitespace(s);
  // temp is a list of tree references
  // auto temp =  trees.findAllIf([&](const Tree& t) {
  //   std::cout << "comparing: " << t.common_name() << " and " << s <<"\n";
  //   return is_matching_species(t.common_name(),ns);
  // }, 
  // [&](const Tree& t) {
  //   Tree dummy(-1,-1,"","",ns,0,"","",0,0);
  //   int result = compare_trees(dummy,t);
  //   // switch (result)
  //   // {
  //   // case 1:
  //   //   std::cout << "result is 1, going right!\n";
  //   //   break;
  //   // case  0:
  //   //   std::cout << "result is 0, going nowhere!\n";
  //   //   break;
  //   // case -1:
  //   //   std::cout << "result is -1, going left!\n";
  //   //   break;
  //   // }
  //   return result;
  // });

  //below "works" but is inefficient

  auto temp = trees.collectIntoListIf([&ns](const Tree& t) {
    //std::cout << t.common_name() << "\t\t" << ns << " ";
    //std::cout << std::boolalpha;
    //std::cout << is_matching_species(t.common_name(),ns) << "\n";
    return is_matching_species(t.common_name(),ns);
  });

  std::unordered_set<std::string> set;
  std::list<std::string> result;

  for ( const auto& e : temp ) {
    if (set.count(e.common_name()) == 0) {
      result.push_back(e.common_name());
      set.insert(e.common_name());
    }
  }
  return result;
}

std::list<std::string> TreeCollection::get_all_in_zipcode( int zipcode ) const {
  auto trs = trees.collectIntoListIf([zipcode](const Tree& t) {
    return t.zip_code() == zipcode;
  });

  //std::unordered_set<std::string> set;
  std::list<std::string> result;

  for ( const auto& e : trs ) {
    //if (set.count(e.common_name()) == 0) {
      result.push_back(e.common_name());
      //set.insert(e.common_name());
    //}
  }

  return result;
}

constexpr double R = 6372.8; //radius of the earth in km
constexpr double TO_RAD = M_PI / 180.0; //conversion of degrees to rads

double haversine( double lat1, double lon1, double lat2, double lon2 ) {
  lat1 = TO_RAD * lat1;
  lat2 = TO_RAD * lat2;
  lon1 = TO_RAD * lon1;
  lon2 = TO_RAD * lon2;
  double dLat = (lat2 - lat1)/2;
  double dLon = (lon2 - lon1)/2;
  double a = sin(dLat);
  double b = sin(dLon);

  return 2 * R * asin(sqrt(a*a + cos(lat1)*cos(lat2)*b*b));
}

std::list<std::string> TreeCollection::get_all_near( double lat, double lgt, double dntc ) const {
  auto trs = trees.collectIntoListIf([&]( const Tree& t) {
    double t_lat, t_lon;
    t.get_position(t_lat,t_lon);
    return haversine( lat, lgt, t_lat, t_lon ) <= dntc;
  });

  //std::unordered_set<std::string> set;
  std::list<std::string> result;

  for ( const auto& e : trs ) {
    //if (set.count(e.common_name()) == 0) {
      result.push_back(e.common_name());
      //set.insert(e.common_name());
    //}
  }

  return result;
}