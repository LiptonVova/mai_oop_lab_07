#ifndef LAB_06_CORE_H
#define LAB_06_CORE_H

#include <set>
#include <queue>
#include <string>
#include <thread>
#include <chrono>
#include <shared_mutex>


#include "npc.h"
#include "factory_npc.h"


struct FightEvent {
    std::shared_ptr<Npc> attacker;
    std::shared_ptr<Npc> defender;
};

class FightFunctor {
private:
    std::queue<FightEvent> events;
    std::shared_ptr<bool> is_work_thread;
    std::shared_ptr<std::shared_mutex> mtx;
public:
    FightFunctor() = delete;
    FightFunctor(std::shared_ptr<bool> is_work_thread,  std::shared_ptr<std::shared_mutex> mtx) :
        is_work_thread(std::move(is_work_thread)), mtx(std::move(mtx)) {}
    FightFunctor(const FightFunctor &other);
    void add_event(std::shared_ptr<Npc> attacker, std::shared_ptr<Npc> defender);
    void operator()();
};


class MoveFunctor {
private:
    std::set<std::shared_ptr<Npc> > set_npc;
    FightFunctor fight_functor_;
    const int MAX_VALUE;
    std::shared_ptr<bool> is_work_thread;
    std::shared_ptr<std::shared_mutex> mtx;

public:
    MoveFunctor() = delete;
    explicit MoveFunctor(
        const std::set<std::shared_ptr<Npc> > &set_npc,
        const FightFunctor& fight_functor_,
        const int MAX_VALUE,
        std::shared_ptr<bool> is_work_thread,
        std::shared_ptr<std::shared_mutex> mtx
        ) : set_npc(set_npc), fight_functor_(fight_functor_), MAX_VALUE(MAX_VALUE), is_work_thread(std::move(is_work_thread)), mtx(std::move(mtx)) {};
    void operator()();
};


void start_fight(std::set<std::shared_ptr<Npc> > &set_npc, const int distance);
void start_programm();

#endif //LAB_06_CORE_H