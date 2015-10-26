#include "solution_checker.h"
#include <iostream>

using namespace std;

Checker::Checker(const Graph *parent, int *terminal_nodes,
  int terminal_set_count) {
  int size = parent->get_vertex_count();
  this->parent = parent;
  this->current_sub_label = new int[size];
  this->terminal_set = new bool[size];
  this->best_sub_label = new int[size];
  
  for (int i = 0; i < size; i++) {
    this->current_sub_label[i] = i;
    this->best_sub_label[i] = i;
    this->terminal_set[i] = false;
  }
  
  this->current_count = 0;
  this->current_sub = new Graph(0, size); 
  
  for (int i = 0; i < terminal_set_count; i++) {
    add_vertex(terminal_nodes[i]);
  }  
  
  this->min_price = this->current_count;
  this->best = this->parent;
}

Checker::~Checker() {
  delete(current_sub);
  delete(terminal_set);
  delete(current_sub_label);
  if (best != parent)
    delete(best);
  if (best_sub_label != current_sub_label)
    delete(best_sub_label);
}

bool Checker::add_vertex(int n) {
  int size = parent->get_vertex_count();
  if (n < 0 || size <= n) {
    // ERROR: Index outside of range
    return false;
  }
  if (terminal_set[n]) {
    // ERROR: Vertex is already there
    return false;
    
  }
  if (current_count == size) {
    // ERROR: Capacity full
    return false;
  }
  if (!current_sub->add_vertex()) {
    // ERROR: Could not add a vertex due to an error in
    // the underlying Graph structure
    return false;
  } 

  for (int i = 0; i < current_count; i++) {
     if (parent->get(n, current_sub_label[i]))
       current_sub->set(current_count, i);
     else
       current_sub->unset(current_count, i);
  }

  terminal_set[n] = true;
  current_sub_label[current_count] = n;
  current_count++;
  return true;
}

bool Checker::remove_last_vertex() {
  if (!current_sub->remove_last_vertex()) {
    // ERROR: Could not add a vertex due to an error in
    // the underlying Graph structure
    return false;
  }

  current_count--;
  terminal_set[current_sub_label[current_count]] = false;
}

bool Checker::process_current_state() {
  if (current_count < best->get_vertex_count()
    && current_sub->is_connected()) {
    if (best != parent)
      delete(best);
    
    best = new Graph(*current_sub);
    
    if (best_sub_label != current_sub_label)
      delete(best_sub_label);
      
    int *label = new int[parent->get_vertex_count()];
    for (int i = 0; i < parent->get_vertex_count(); i++)
      label[i] = current_sub_label[i];
    best_sub_label = label;
   
    if (min_price == current_count)
      return true;
    }
    
  return false;
}
