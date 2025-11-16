#ifndef LAB_06_OBSERVER_H
#define LAB_06_OBSERVER_H

#include <filesystem>
#include <iostream>
#include <fstream>
#include <memory>

#include "npc.h"


class Observer {
public:
    virtual void update(const Npc &attacker, const Npc &defender) = 0;
    Observer() = default;
    Observer(const Observer &) = default;
    virtual ~Observer() = default;
};

class FileObserver final : public Observer {
private:
    std::ofstream log_file;
public:
    void update(const Npc &attacker, const Npc &defender) override;
};

class StdinObserver final : public Observer {
public:
    void update(const Npc &attacker, const Npc &defender) override;
};

#endif //LAB_06_OBSERVER_H