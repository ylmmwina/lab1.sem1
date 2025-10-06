#include "Graph.h"
#include "GraphAlgorithms.h"
#include "Network.h"
#include "NetworkSimulator.h"
#include <clocale>
#ifdef _WIN32
  #include <windows.h>
#endif
#include <iostream>

int main() {

#ifdef _WIN32
    // перевести консоль Windows на UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::cout << "OOP Lab 1 — Networks + Graphs (C++20)\n\n";

    // демонстрація Graph + BFS/DFS на простих типах
    Graph<int, int> g;
    g.addNode(1); g.addNode(2); g.addNode(3);
    g.addEdge(1, 2, 1);
    g.addEdge(2, 3, 1);
    g.addEdge(1, 3, 5);

    std::cout << "Simple graph:\n";
    g.printGraph();

    BFS<int, int> bfs;
    DFS<int, int> dfs;
    bfs.run(g, 1);
    dfs.run(g, 1);

    // Демонстрація Dijkstra на зваженому графі (int->double)
    Graph<std::string, double> wg;
    wg.addNode("A"); wg.addNode("B"); wg.addNode("C"); wg.addNode("D");
    wg.addEdge("A","B",4.0);
    wg.addEdge("A","C",2.0);
    wg.addEdge("C","D",3.0);
    wg.addEdge("B","D",10.0);

    Dijkstra<std::string> dj1;
    dj1.run(wg, "A"); // тут вага = саме значення ребра
    std::vector<std::string> p1 = dj1.buildPath("D");
    std::cout << "\nDijkstra A->D (weighted graph): ";
    if (p1.empty()) std::cout << "no path\n";
    else { for (const auto& v : p1) std::cout << v << " "; std::cout << "(dist=" << dj1.distTo("D") << ")\n"; }

    // мережа + маршрутизація
    NetworkSimulator sim;
    sim.buildDemo();
    std::cout << "\n";
    sim.print();

    // 1) найменша латентність
    std::vector<std::string> r1 = sim.route("R1", "H2", 1024, true);
    std::cout << "\nRoute by latency R1->H2: ";
    if (r1.empty()) std::cout << "no route\n";
    else { for (const auto& v : r1) std::cout << v << " "; std::cout << "\n"; }

    // 2) найшвидша передача великого пакета
    std::vector<std::string> r2 = sim.route("R1", "H2", 2'000'000, false); // ~2MB
    std::cout << "Route by xfer-time (2MB) R1->H2: ";
    if (r2.empty()) std::cout << "no route\n";
    else { for (const auto& v : r2) std::cout << v << " "; std::cout << "\n"; }

    // 3) відправка пакета
    Packet pkt("R1","H2", 10, 4096);
    sim.send(pkt, true);

    return 0;
}
