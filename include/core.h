#ifndef LAB_06_CORE_H
#define LAB_06_CORE_H

#include <set>
#include <queue>

#include "npc.h"


struct FightEvent {
    std::shared_ptr<Npc> attacker;
    std::shared_ptr<Npc> defender;
};

class FightFunctor {
private:
    std::queue<FightEvent> events;
public:
    FightFunctor() = default;
    void add_event(std::shared_ptr<Npc> attacker, std::shared_ptr<Npc> defender);
    void operator()();
};


class MoveFunctor {
private:
    std::set<std::shared_ptr<Npc> > set_npc;
public:
    MoveFunctor() = delete;
    explicit MoveFunctor(const std::set<std::shared_ptr<Npc> > &set_npc) : set_npc(set_npc) {};
    void operator()();
};


void start_fight(std::set<std::shared_ptr<Npc> > &set_npc, const int distance);
void start_programm();

#endif //LAB_06_CORE_H