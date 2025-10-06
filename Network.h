#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <ostream>

// базовий пристрій (динамічний поліморфізм)
class Device {
protected:
    std::string name_;
public:
    explicit Device(std::string n) : name_(std::move(n)) {}
    virtual ~Device() = default;

    const std::string& name() const { return name_; }
    virtual std::string type() const = 0;
};

class Router : public Device {
public:
    using Device::Device;
    std::string type() const override { return "Router"; }
};

class Switch : public Device {
public:
    using Device::Device;
    std::string type() const override { return "Switch"; }
};

class Host : public Device {
public:
    using Device::Device;
    std::string type() const override { return "Host"; }
};

// параметри каналу зв'язку
struct Link {
    double latencyMs{1.0};
    double bandwidthMbps{100.0};
    bool isUp{true};

    Link() = default;
    Link(double lat, double bw, bool up = true)
        : latencyMs(lat), bandwidthMbps(bw), isUp(up) {}
};

inline std::ostream& operator<<(std::ostream& os, const Link& l) {
    os << "{lat=" << l.latencyMs
       << ", bw=" << l.bandwidthMbps
       << ", up=" << (l.isUp ? "true" : "false") << "}";
    return os;
}

// примітивний пакет
struct Packet {
    std::string source;
    std::string destination;
    int ttl{10};
    int sizeBytes{1024};

    Packet(std::string s, std::string d, int t = 10, int sz = 1024)
        : source(std::move(s)), destination(std::move(d)), ttl(t), sizeBytes(sz) {}
};

#endif //NETWORK_H