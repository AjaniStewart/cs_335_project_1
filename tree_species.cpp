/*******************************************************************************
  Title          : tree.h
  Author         : Ajani Stewart
  Created on     : May 27, 2019
  Description    : The implementation file for the TreeSpecies class
  Purpose        : To provide a minimal interface to be used to interact with 
                   the Tree species.
  Usage          : 
  Build with     : -std=c++11
*******************************************************************************/


#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

#include "tree_species.h"

void TreeSpecies::print_all_species( std::ostream& out ) const {

  std::for_each(species.begin(), species.end(), [&](const std::string& s) {
    out << s << '\n';
  });
}

int TreeSpecies::number_of_species() const { return species.size(); }

int TreeSpecies::add_species( const std::string& s ) {
  auto it = std::find(species.begin(), species.end(), s);
  if (it == species.end()) {
    // std::cout << "adding " << s << "\n";
    species.push_back(s);
    std::sort( species.begin(), species.end() );
    return 1;
  }
  return 0;
}

//checks if they are the same, ignoring case
// hyphens and spaces are treated as the same character
bool is_same_species( const std::string& s_name, const std::string& p_name ) {
  if ( s_name.size() != p_name.size() )
    return false;
  
  for ( size_t i = 0; i < s_name.size(); ++i ) {
    char p_i = p_name[i];
    char s_i = s_name[i];
    if ( s_i != p_i && (s_i != '-' && s_i != ' ') 
        && (p_i != '-' && p_i != ' ')) {
          return false;
        }
  }
  return true;
}

std::vector<std::string> split_on_space_or_hyphen(const std::string& s) {
  std::vector<std::string> result;
  std::string cur_str;

  std::for_each(s.begin(), s.end(), [&](char c) {
    if (c == '-' || c == ' ') {
      result.push_back(cur_str);
      cur_str = "";
    } else {
      cur_str += c;
    }
  });
  result.push_back(cur_str);
  return result;
}

bool contains_word( const std::string& s_name, const std::string& p_name ) {
  auto string_array = split_on_space_or_hyphen(s_name);

  for (size_t i = 0; i < string_array.size(); ++i) {
    if (is_same_species(string_array[i], p_name))
      return true;
  }

  return false;
}

std::string alternate_hyphens_and_spaces( const std::string& s) {
  std::string r = "";

  for (size_t i = 0; i < s.size(); ++i) {
    if (s[i] == ' ')
      r += '-';
    else if (s[i] == '-')
      r += ' ';
    else
      r += s[i];
  }
  return r;
}


bool contains_subsequence( const std::string& s_name, const std::string& p_name ) {
  if (p_name.size() > s_name.size()) return false;

  auto result = s_name.find(p_name);
  if (result == s_name.npos) {
    result = s_name.find(alternate_hyphens_and_spaces(p_name));
    if (result == s_name.npos){
      return false;
    }
  }

  if (result == 0 && (s_name[p_name.size()] == ' ' || s_name[p_name.size()] == '-')) {
    return true;
  } else {
    if (result + p_name.size() >= s_name.size())
      return s_name[result - 1] == ' ' || s_name[result - 1] == '-';
    else 
      return false;
  }
}

std::string tolower(const std::string& s) {
    std::string k;
    std::for_each(s.begin(), s.end(), [&](char c) {
      k += std::tolower(c); 
    });
    return k;
}

bool is_matching_species( const std::string& spc_name, const std::string& partial_name ) {
  // case 1: spc_name == partial name
  // case 2: p_name is one word -> then p_name is exactly one of the words on s_name
  // case 3: if p_name isnt one word -> then p_name is a subsequence of s_name
  auto s_name = tolower(spc_name);
  auto p_name = tolower(partial_name);
  
  return is_same_species(s_name, p_name) || contains_word(s_name, p_name)
         || contains_subsequence(s_name, p_name);
}

std::list<std::string> TreeSpecies::get_matching_species( const std::string& partial_name ) const {
  std::list<std::string> list;

  std::for_each(species.begin(), species.end(), [&](const string& s) {
    if (is_matching_species(s, partial_name))
      list.push_back(s);
  });
  return list;
}

bool TreeSpecies::contains( const std::string& s ) {
  return std::binary_search(species.begin(), species.end(), s);
}