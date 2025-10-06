#ifndef NETWORKSIMULATOR_H
#define NETWORKSIMULATOR_H

#include "Graph.h"
#include "GraphAlgorithms.h"
#include "Network.h"
#include <unordered_map>
#include <memory>
#include <iostream>

class NetworkSimulator {
private:
    // граф: вузли - імена пристроїв; ребра - Link
    Graph<std::string, Link> net_;

    // сховище пристроїв (для демо)
    std::unordered_map<std::string, std::unique_ptr<Device>> devices_;

public:
    // створення пристроїв
    void addRouter(const std::string& name) { ensureNode(name); devices_[name] = std::make_unique<Router>(name); }
    void addSwitch(const std::string& name) { ensureNode(name); devices_[name] = std::make_unique<Switch>(name); }
    void addHost  (const std::string& name) { ensureNode(name); devices_[name] = std::make_unique<Host>(name); }

    // додавання каналу
    void addLink(const std::string& a, const std::string& b, const Link& link, bool bidir = true) {
        ensureNode(a); ensureNode(b);
        net_.addEdge(a, b, link);
        if (bidir) net_.addEdge(b, a, link);
    }

    // проста демо-топологія
    void buildDemo() {
        addRouter("R1");
        addSwitch("S1");
        addHost("H1");
        addHost("H2");

        addLink("R1", "S1", Link{1.0, 100.0});
        addLink("S1", "H1", Link{2.0, 100.0});
        addLink("S1", "H2", Link{5.0, 50.0});
        addLink("R1", "H2", Link{10.0, 100.0}); // альтернативний довший шлях
    }

    void print() const {
        std::cout << "Topology:\n";
        net_.printGraph();
    }

    // пошук маршруту між src і dst
    // useLatency=true: мінімізуємо латентність
    // useLatency=false: мінімізуємо (латентність + час передачі великого пакета)
    std::vector<std::string> route(const std::string& src,
                                   const std::string& dst,
                                   int packetSizeBytes = 1024,
                                   bool useLatency = true) const {
        Dijkstra<std::string> dj;

        auto weightLatency = [](const Link& e) -> double {
            if (!e.isUp) return 1e12;
            return e.latencyMs;
        };

        auto weightXferTime = [packetSizeBytes](const Link& e) -> double {
            if (!e.isUp || e.bandwidthMbps <= 0.0) return 1e12;
            // приблизний час на лінку (ms) = latency + (bytes*8)/(Mbps)*1e3
            double txMs = (packetSizeBytes * 8.0) / (e.bandwidthMbps * 1'000'000.0) * 1e3;
            return e.latencyMs + txMs;
        };

        if (useLatency) {
            dj.run(net_, src, weightLatency);
        } else {
            dj.run(net_, src, weightXferTime);
        }
        return dj.buildPath(dst);
    }

    // емуляція відправки пакета хоп-за-хопом із TTL
    void send(Packet p, bool useLatency = true) const {
        std::vector<std::string> path = route(p.source, p.destination, p.sizeBytes, useLatency);
        if (path.empty()) {
            std::cout << "Маршрут не знайдено\n";
            return;
        }

        std::cout << "Маршрут: ";
        for (std::size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i] << (i + 1 < path.size() ? " -> " : "\n");
        }

        std::cout << "Відправка пакета (ttl=" << p.ttl << ", size=" << p.sizeBytes << "B):\n";
        for (std::size_t i = 0; i + 1 < path.size() && p.ttl > 0; ++i) {
            std::cout << "  " << path[i] << " -> " << path[i + 1] << "\n";
            --p.ttl;
        }
        if (p.ttl <= 0 && path.size() > 1) {
            std::cout << "TTL вичерпано, пакет втрачено\n";
        } else {
            std::cout << "Пакет доставлено до " << p.destination << "\n";
        }
    }

private:
    void ensureNode(const std::string& n) {
        // addNode нічого не зламає, якщо вузол уже існує
        net_.addNode(n);
    }
};

#endif //NETWORKSIMULATOR_H