#ifndef LAB_06_CORE_H
#define LAB_06_CORE_H

#include <set>
#include <queue>
#include <string>


#include "npc.h"
#include "factory_npc.h"


struct FightEvent {
    std::shared_ptr<Npc> attacker;
    std::shared_ptr<Npc> defender;
};

class FightFunctor {
private:
    std::queue<FightEvent> events;
    std::mutex mtx;
public:
    FightFunctor() = default;
    void add_event(std::shared_ptr<Npc> attacker, std::shared_ptr<Npc> defender);
    void operator()();
};


class MoveFunctor {
private:
    std::set<std::shared_ptr<Npc> > set_npc;
    std::mutex mtx;
    FightFunctor fight_functor_;
    const int MAX_VALUE;

public:
    MoveFunctor() = delete;
    explicit MoveFunctor(const std::set<std::shared_ptr<Npc> > &set_npc, const int MAX_VALUE) :
        set_npc(set_npc), MAX_VALUE(MAX_VALUE) {};
    void operator()();
};


void start_fight(std::set<std::shared_ptr<Npc> > &set_npc, const int distance);
void start_programm();

#endif //LAB_06_CORE_H