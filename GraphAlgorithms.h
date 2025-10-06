#ifndef GRAPHALGORITHMS_H
#define GRAPHALGORITHMS_H

#include "Graph.h"
#include <queue>
#include <stack>
#include <limits>
#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

// базовий поліморфний інтерфейс (динамічний поліморфізм)
template <typename TNode, typename TEdge>
class GraphAlgorithm {
public:
    virtual void run(const Graph<TNode, TEdge>& g, const TNode& start) = 0;
    virtual ~GraphAlgorithm() = default;
};

// BFS
template <typename TNode, typename TEdge>
class BFS : public GraphAlgorithm<TNode, TEdge> {
public:
    void run(const Graph<TNode, TEdge>& g, const TNode& start) override {
        std::queue<TNode> q;
        std::set<TNode> visited;

        q.push(start);
        visited.insert(start);

        std::cout << "BFS: ";
        while (!q.empty()) {
            TNode u = q.front(); q.pop();
            std::cout << u << " ";
            std::vector<TNode> neigh = g.getNeighbors(u);
            for (const auto& v : neigh) {
                if (!visited.count(v)) {
                    visited.insert(v);
                    q.push(v);
                }
            }
        }
        std::cout << "\n";
    }
};

// DFS
template <typename TNode, typename TEdge>
class DFS : public GraphAlgorithm<TNode, TEdge> {
public:
    void run(const Graph<TNode, TEdge>& g, const TNode& start) override {
        std::stack<TNode> st;
        std::set<TNode> visited;

        st.push(start);
        std::cout << "DFS: ";
        while (!st.empty()) {
            TNode u = st.top(); st.pop();
            if (visited.count(u)) continue;
            visited.insert(u);
            std::cout << u << " ";

            std::vector<TNode> neigh = g.getNeighbors(u);
            for (const auto& v : neigh) st.push(v);
        }
        std::cout << "\n";
    }
};

// Dijkstra (із підтримкою "витягувача ваги")
template <typename TNode>
class Dijkstra {
private:
    std::unordered_map<TNode, double> distance_;
    std::unordered_map<TNode, TNode> previous_;

public:
    // випадок: якщо TEdge - число
    template <typename TEdge>
    void run(const Graph<TNode, TEdge>& g, const TNode& start) {
        auto weight = [](const TEdge& e) -> double { return static_cast<double>(e); };
        run(g, start, weight);
    }

    // універсальний випадок: вага дістається функтором edgeWeight(edgeData)
    template <typename TEdge, typename F>
    void run(const Graph<TNode, TEdge>& g, const TNode& start, F edgeWeight) {
        distance_.clear();
        previous_.clear();

        std::vector<TNode> nodes = g.getNodes();
        for (const auto& n : nodes) distance_[n] = std::numeric_limits<double>::infinity();
        if (distance_.find(start) == distance_.end()) return;
        distance_[start] = 0.0;

        auto cmp = [&](const TNode& a, const TNode& b) { return distance_[a] > distance_[b]; };
        std::priority_queue<TNode, std::vector<TNode>, decltype(cmp)> pq(cmp);
        pq.push(start);

        while (!pq.empty()) {
            TNode u = pq.top(); pq.pop();

            std::vector<std::pair<TNode, TEdge>> edges = g.getEdges(u);
            for (const auto& pr : edges) {
                const TNode& v = pr.first;
                const TEdge& ed = pr.second;
                double w = edgeWeight(ed);
                double alt = distance_[u] + w;
                if (alt < distance_[v]) {
                    distance_[v] = alt;
                    previous_[v] = u;
                    pq.push(v);
                }
            }
        }
    }

    std::vector<TNode> buildPath(const TNode& target) const {
        std::vector<TNode> path;
        auto it = distance_.find(target);
        if (it == distance_.end() || !std::isfinite(it->second)) return path;

        TNode cur = target;
        while (true) {
            path.push_back(cur);
            auto pit = previous_.find(cur);
            if (pit == previous_.end()) break;
            cur = pit->second;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    double distTo(const TNode& target) const {
        auto it = distance_.find(target);
        return (it == distance_.end()) ? std::numeric_limits<double>::infinity() : it->second;
    }
};

#endif //GRAPHALGORITHMS_H