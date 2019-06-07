/*******************************************************************************
  Title          : tree.h
  Author         : Ajani Stewart
  Created on     : May 27, 2019
  Description    : The implementation file for the Tree class
  Purpose        : To provide a minimal interface to be used to interact with 
                   the Tree objects.
  Usage          : 
  Build with     : -std=c++11
  Modifications  : March 5, 2019, by Stewart Weiss
                   The data dictionary description and column positions for
                   latitude and longitude were incorrect. They were corrected.
 
*******************************************************************************/

#include <iostream>
#include <vector>
#include <string>

#include "tree.h"

typedef std::vector<std::string> string_array;

string_array split( std::string s, char delimiter = ',', size_t size = -1 ) {
  string_array string_vector;
  if (size > -1) {
    string_vector.reserve(size);
  }
  std::string current_string = "";
  for ( size_t i = 0; i < s.size(); ++i ) {
    if (s[i] != delimiter) {
      current_string += s[i];
    } else {
      string_vector.push_back(current_string);
      current_string = "";
    }
  }
  return string_vector; 
}

//TODO: check for valid values
Tree::Tree(const std::string& str) {
  auto data_vector = split(str, ',', 40);

  tree_id = std::stoi(data_vector[0]);
  tree_dbh = std::stoi(data_vector[3]);
  status = data_vector[6];
  health = data_vector[7];
  spc_common = data_vector[10];
  zipcode = std::stoi(data_vector[25]);
  address = data_vector[24];
  boroname = data_vector[29];
  latitude = std::stod(data_vector[38]);
  longitude = std::stod(data_vector[39]);
}

Tree::Tree(int id, int diam,  std::string stat, std::string hlth, std::string name, 
        int zip, std::string addr, std::string boro, double lat, double longtd)
        : tree_id(id), tree_dbh(diam), status(stat), health(hlth), spc_common(name),
        zipcode(zip), address(addr), boroname(boro), latitude(lat), longitude(longtd) { }

std::string Tree::common_name() const { return spc_common; }

std::string Tree::borough_name() const { return boroname; }

std::string Tree::nearest_address() const { return address; }

std::string Tree::life_status() const { return status; }

std::string Tree::tree_health() const { return health; }

int Tree::id() const { return tree_id; }

int Tree::zip_code() const { return zipcode; }

int Tree::diameter() const { return tree_dbh; }

void Tree::get_position( double& lat, double& lon ) const {
  lat = latitude;
  lon = longitude;
}

std::string Tree::pad_zipcode() const {
  std::string z = std::to_string(zipcode);
  while ( z.size() < 5) {
    z.insert(0, "0");
  }
  return z;
}
std::ostream& operator<<( std::ostream& out, const Tree& t) {
  out << t.spc_common << "," << t.tree_id << "," << t.tree_dbh << "," << t.status << ",";
  out << t.health << "," << t.address << "," << t.pad_zipcode() << "," << t.boroname << ",";
  out << t.latitude << "," << t.longitude;
  return out;
}

int Tree::compare_trees( const Tree& t ) const {
  if ( this->spc_common.size() > t.spc_common.size() )
    return 1;
  else if ( this-> spc_common.size() < t.spc_common.size() )
    return -1;
  else {
    for ( size_t i = 0; i < t.spc_common.size(); ++i ) {
      char this_i = tolower(this->spc_common[i]);
      char other_i = tolower(t.spc_common[i]);
      if ( this_i != other_i && (this_i != '-'
            || this_i != ' ') && (other_i != '-' || other_i != ' ')) {
              return this_i > other_i ? 1 : -1;
            }
    }
  }
  return 0;
}

bool samename( const Tree& t1, const Tree& t2 ) {
  return t1.compare_trees(t2) == 0;
}

bool islessname( const Tree& t1, const Tree& t2 ) {
  return t1.compare_trees(t2) == -1;
}

bool operator==( const Tree& t1, const Tree& t2) {
  return samename(t1,t2) && t1.tree_id == t2.tree_id;
}

bool operator<( const Tree& t1, const Tree& t2 ) {
  int result = t1.compare_trees(t2);
  if (result == -1) return true;
  else if (result == 0) return t1.tree_id < t2.tree_id;
  else return false;
}