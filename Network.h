#ifndef NETWORK_H
#define NETWORK_H

/*
КЛАСИ/ТИПИ У ФАЙЛІ:
  7) class Device (абстрактний) [КЛАС №7]
  8) class Router  : public Device [КЛАС №8]
  9) class Switch  : public Device [КЛАС №9]
 10) class Host    : public Device [КЛАС №10]
 11) struct Link [КЛАС/СТРУКТ. №11]
 12) class Packet [КЛАС №12]

ПОЛЯ:
  Device: id_, name_ - 2
  Router: routingHint_ (опц. поле) - 1
  Switch: (немає додаткових) - 0
  Host: address_ - 1
  Link: latencyMs, bandwidthMbps, rel - 3
  Packet: source, destination, ttl, size, hops - 5
  разом: 12

НЕТРИВІАЛЬНІ МЕТОДИ:
  (М15)  Device::kind() - чисто віртуальний (динамічний поліморфізм)
  (М16)  Router::kind() - поліморфне перевизначення
  (М17)  Switch::kind() - поліморфне перевизначення
  (М18)  Host::kind() - поліморфне перевизначення
  (М19)  Link::costForBytes(bytes) - час передавання (latency + size/bandwidth)
  (М20)  Packet::addHop(node) - лог запису послідовності вузлів
  разом: 6
*/

#include <iostream>
#include <string>
#include <vector>

// базовий пристрій
class Device {
protected:
    int         id_;
    std::string name_;
public:
    Device(int id, std::string name) : id_(id), name_(std::move(name)) {}
    virtual ~Device() = default;

    int id() const { return id_; } // геттер (тривіальний)
    const std::string& name() const { return name_; }

    // (М15) абстрактний поліморфний метод — тип пристрою
    virtual std::string kind() const = 0;
};

// похідні пристрої
class Router : public Device {
    // поле "натяк" для маршрутизації (не обов'язкове, просто приклад)
    std::string routingHint_;
public:
    Router(int id, std::string name) : Device(id, std::move(name)) {}
    std::string kind() const override { return "Router"; } // (М16)
};

class Switch : public Device {
public:
    Switch(int id, std::string name) : Device(id, std::move(name)) {}
    std::string kind() const override { return "Switch"; } // (М17)
};

class Host : public Device {
    std::string address_;
public:
    Host(int id, std::string name, std::string addr)
        : Device(id, std::move(name)), address_(std::move(addr)) {}
    const std::string& address() const { return address_; }
    std::string kind() const override { return "Host"; } // (М18)
};

// зв'язок/канал
struct Link {
    double latencyMs{1.0};
    double bandwidthMbps{100.0};
    double reliability{0.999};

    // (М19) оцінка часу передавання "bytes" з урахуванням latency та bandwidth
    // повертає секунди: latency (мс -> с) + (байти / (Мб/с * 1e6))
    double costForBytes(std::size_t bytes) const {
        double secondsLatency = latencyMs / 1000.0;
        double secondsPayload = (bandwidthMbps > 0.0)
                                ? (static_cast<double>(bytes) / (bandwidthMbps * 1'000'000.0))
                                : 1e9; // дуже велика вартість у разі 0 Mbps
        return secondsLatency + secondsPayload;
    }
};

// пакет
class Packet {
    std::string source_;
    std::string destination_;
    int         ttl_{8};
    std::size_t sizeBytes_{512};
    std::vector<std::string> hops_;
public:
    Packet(std::string src, std::string dst, int ttl = 8, std::size_t size = 512)
        : source_(std::move(src)), destination_(std::move(dst)),
          ttl_(ttl), sizeBytes_(size) {}

    const std::string& src() const { return source_; }
    const std::string& dst() const { return destination_; }
    int ttl() const { return ttl_; }
    std::size_t size() const { return sizeBytes_; }
    const std::vector<std::string>& hops() const { return hops_; }

    void decTTL() { --ttl_; } // тривіальний

    // (М20) запис чергового вузла в маршрут пакета
    void addHop(const std::string& nodeName) {
        hops_.push_back(nodeName);
    }
};

#endif //NETWORK_H