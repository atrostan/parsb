#include "SlashBurn.h"
#include "benchmark.h"
#include "bitmap.h"
#include "command_line.h"
#include "ips4o.hpp"
#include "spray.hpp"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <iostream>
#include <omp.h>
#include <random>

int main(int argc, char *argv[]) {
  CLApp cli(argc, argv, "connected-components-afforest");
  if (!cli.ParseArgs())
    return -1;
  Builder b(cli);
  fmt::print("cli.out_filename(): {}\n", cli.out_filename());
  fmt::print("cli.symmetrize(): {}\n", cli.symmetrize());
  Graph g = b.MakeGraph();
  fmt::print("g.num_nodes(): {}\n", g.num_nodes());
  fmt::print("g.num_edges(): {}\n", g.num_edges());
  fmt::print("g.num_edges_directed(): {}\n", g.num_edges_directed());
  fmt::print("g.directed: {}\n", g.directed());
  int n_neighbour_rounds = 2;
  float percent = cli.percent();
  Bitmap bmap(g.num_nodes());

  // create an array of vectors
  uint32_t num_nodes = g.num_nodes();
  uint num_threads = cli.num_threads();
  fmt::print("num_threads: {}\n", num_threads);
  // for (int i = 0; i < 5; ++i){
    SlashBurn sb = SlashBurn(g, n_neighbour_rounds, percent, bmap, num_threads);
    fmt::print("parsb completed in: {} ms\n", sb.time);
  // }
  // check that all vertex IDs have been assigned
  // #pragma omp parallel for
  uint32_t invalid = -1;
  for (uint32_t i = 0; i < num_nodes; ++i) {
    if (sb.perm[i] == invalid) {
      fmt::print("Unassigned: {}\n", i);
      return 1;
    }
  }
  sb.write_permutation(cli.out_filename());

  return 0;
}
