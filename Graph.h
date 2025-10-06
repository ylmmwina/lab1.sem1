#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

template <typename TNode, typename TEdge>
class Graph {
private:
  std::map<TNode, std::vector<std::pair<TNode, TEdge>>> adjacency;

public:
  void addNode(const TNode& node) {
    adjacency[node];
  }

  void addEdge(const TNode& from, const TNode& to, const TEdge& edge) {
    adjacency[from].push_back({to, edge});
  }

  void removeNode(const TNode& node) {
    adjacency.erase(node);
    for (auto& p : adjacency) {
      auto& neighbors = p.second;
      neighbors.erase(
          std::remove_if(neighbors.begin(), neighbors.end(),
                    [&](const std::pair<TNode, TEdge>& pr){ return pr.first == node; }),
          neighbors.end()
      );
    }
  }

  std::vector<TNode> getNeighbors(const TNode& node) const {
    std::vector<TNode> result;
    auto it = adjacency.find(node);
    if (it != adjacency.end()) {
      for (const auto& pr : it->second)
        result.push_back(pr.first);
    }
    return result;
  }

  std::vector<TNode> getNodes() const {
    std::vector<TNode> nodes;
    nodes.reserve(adjacency.size());
    for (const auto& p : adjacency)
      nodes.push_back(p.first);
    return nodes;
  }

  std::vector<std::pair<TNode, TEdge>> getEdges(const TNode& node) const {
    auto it = adjacency.find(node);
    if (it != adjacency.end())
      return it->second;
    return {};
  }

  void printGraph() const {
    for (const auto& p : adjacency) {
      const TNode& node = p.first;
      const auto& neighbors = p.second;
      std::cout << node << " -> ";
      for (const auto& pr : neighbors)
        std::cout << "(" << pr.first << ", edge=" << pr.second << ") ";
      std::cout << "\n";
    }
  }
};

#endif // GRAPH_H
