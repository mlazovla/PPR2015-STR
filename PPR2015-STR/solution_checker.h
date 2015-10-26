#ifndef CHECKER_H
#define CHECKER_H
/* solution_checker.h
 * Trida jejiz objekty slouzi jako kontejnery pro uchovani
 * aktualnich stavu a jejich overovani pri reseni problemu minimalniho
 * Steinerova stromu. */
#include "graph.h"
#include <iostream>

class Checker {
  private:
    // Graf, v nemz resime ulohu.
    const Graph *parent;
    // Podgraf reflektujici aktualni stav pri hledani reseni ulohy.
    Graph *current_sub;
    
    /* Terminalni mnozina uzlu. Uchovava info o tom, ktery z uzlu
      celeho grafu se nachazi v podgrafu reflektujicim dany stav.
      terminal_set[i] == 0 znamena, ze i-ty uzel grafu se v podgrafu
      nenachazi. V opacnem pripade se tam i-ty uzel nachazi */
    bool *terminal_set;
    /* Popisky uzlu podgrafu reflektujiciho dany stav. Zde je
      obsazena informace o tom, jake uzly rodicovskeho grafu odpovidaji
      jednotlivym uzlum podgrafu. Tj.
      current_sub_label[i] = j znamena, ze i-ty uzel podgrafu je
      j-tym uzlem rodicovskeho grafu. */
    int *current_sub_label;
    
    // Tesna dolni mez pro danou ulohu
    int min_price;
    // Graf reflektujici aktualne nejlepsi nalezene reseni.
    const Graph *best;
    // Popisky uzlu aktualne nejlepsiho reseni.
    int *best_sub_label;
    // Pocet vrcholu grafu reflektujiciho aktualni stav.
    int current_count;
    
    // Pomocna metoda - tiskne matici incidence grafu
    void print_graph(const Graph *g) const {
      for (int i = 0; i < g->get_vertex_count(); i++) {
        std::cout << std::endl;
        for (int j = 0; j < g->get_vertex_count(); j++) {
          std::cout << g->get(i, j) << " ";
        }
      }
      std::cout << std::endl;
    }
  
  public:
    Checker(const Graph *parent, int *terminal_nodes, int terminal_set_count);
    ~Checker();
    bool add_vertex(int n);
    bool remove_last_vertex();
    bool process_current_state();
    
    void remove_cycles_from_best() {
      Graph *tree = new Graph(*best);
      tree->remove_cycles();
      best = tree;
    }
    
    int get_parent_size() {
      return parent->get_vertex_count();
    }
    int get_sub_size() {
      return current_count;
    }
    bool can_be_better() {
      return best->get_vertex_count() - current_count > 1;
    }
    bool contains_vertex(int n) {
      return terminal_set[n];
    }   
    
    void print_parent() {
      print_graph(parent);
    }
    
    void print_sub() {
      std::cout << std::endl;
      for (int i = 0; i < current_count; i++)
        std::cout << current_sub_label[i] << " ";
      std::cout << "\n-----------------------";
      print_graph(current_sub);
    }
    
    void print_vertex() {
      for (int i = 0; i < parent->get_vertex_count(); i++) {
        if (terminal_set[i])
          std::cout << i << " ";
      }
      std::cout << "\n";
    }
    
    void print_best() {
      std::cout << std::endl;
      for (int i = 0; i < best->get_vertex_count(); i++)
        std::cout << best_sub_label[i] << " ";
      std::cout << "\n-----------------------";
      print_graph(best);
    }
};
#endif