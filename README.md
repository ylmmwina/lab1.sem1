# OOP Lab 1 — Комп'ютерні мережі + Графи

### Мета
Показати базові ідеї ООП на практиці:
- класи, інкапсуляція;
- успадкування та **динамічний поліморфізм** (віртуальні методи);
- **статичний поліморфізм** (шаблони/`templates`);
- робота зі структурами даних та алгоритмами на графах.

### Обраний варіант + відповідність загальним вимогам
- **Перелік 1:** (****) Графи + **Дейкстра** → разом **5★**
- **Перелік 2:** **Комп’ютерні мережі** (рівні a, b, c)
    - a) класи даних (`Device`, `Router`, `Switch`, `Host`, `Link`, `Packet`);
    - b) алгоритми (маршрутизація через `RoutingAlgorithm`, реалізація `DijkstraRouting`);
    - c) інтеграція (читання/запис топології файлом).

### Структура проєкту
(усі файли лежать у кореневій директорії)

| Файл | Зміст |
|------|-------|
| **Graph.h** | Шаблонний граф `Graph<TNode, TEdge>`: додавання/видалення вершин/ребер, сусіди, друк, очищення. |
| **GraphAlgorithms.h** | `GraphAlgorithm` (абстр.), `BFS`, `DFS`, `WeightedEdge`, `Dijkstra` з відновленням шляху. |
| **Network.h** | Ієрархія `Device → Router/Switch/Host`, а також `Link` (latency/bandwidth/reliability) і `Packet`. |
| **NetworkSimulator.h** | Ієрархія `RoutingAlgorithm → DijkstraRouting` і клас `NetworkSimulator` (побудова мережі, пошук маршруту, симуляція, I/O). |
| **main.cpp** | Демо: BFS/DFS на простому графі; Дейкстра; маршрутизація та передача пакета в мережі. |

### Підрахунок елементів
**Класи / типи (14):**  
`Graph`, `GraphAlgorithm`(абстр.), `BFS`, `DFS`, `WeightedEdge`, `Dijkstra`,  
`Device`(абстр.), `Router`, `Switch`, `Host`, `Link`, `Packet`,  
`RoutingAlgorithm`(абстр.), `DijkstraRouting`, `NetworkSimulator`.

**Поля (сукупно 20):**
- `Graph`: `adjacency`, `directed_` (2)
- `Device`: `id_`, `name_` (2), `Router`: `routingHint_`(1), `Host`: `address_`(1)
- `Link`: `latencyMs`, `bandwidthMbps`, `reliability` (3)
- `Packet`: `source_`, `destination_`, `ttl_`, `sizeBytes_`, `hops_` (5)
- `Dijkstra`: `dist`, `parent` (2)
- `NetworkSimulator`: `graph_`, `devices_` (2)

**Нетривіальні методи (30+):**
- `Graph.hpp` — 9 методів (8 нетривіальних: `addEdge/removeNode/removeEdge/...`).
- `GraphAlgorithms.hpp` — 5 (BFS::run, DFS::run, Dijkstra::run, Dijkstra::getPathTo, GraphAlgorithm::run абстр.).
- `Network.hpp` — 6 (поліморфні `kind()`, `Link::costForBytes`, `Packet::addHop`).
- `NetworkSimulator.hpp` — 10 (`route`, `addDevice`, `connect`, `buildDemo`, `findRoute`, `sendPacket`, `saveTopology`, `loadTopology`, `printDevices`).  
  Разом: **~29 нетривіальних + абстрактні**.

**Ієрархії успадкування (2):**
1) `Device → Router / Switch / Host` (≥3 класи).
2) `RoutingAlgorithm → DijkstraRouting` (ієрархія для поліморфного вибору алгоритму). 

*(додатково) Патерн «алгоритм» для BFS/DFS можна викликати через базовий `GraphAlgorithm<TNode,TEdge>*`.

**Поліморфізм (~3 приклади):**
- **Динамічний:** `Device::kind()` (різні типи), `RoutingAlgorithm::route()` → `DijkstraRouting`.
- **Статичний:** шаблонні `Graph<TNode,TEdge>`, `Dijkstra<TNode>` (компілятор підставляє типи).

**Інкапсуляція:**
- Поля — `private/protected`.
- Інтерфейси — через чисто віртуальні методи.
- `const`-коректність у геттерах і методах.

