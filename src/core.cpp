#include "../include/core.h"

#include "../include/visitor_simulate_fight.h"

void FightFunctor::add_event(std::shared_ptr<Npc> attacker, std::shared_ptr<Npc> defender) {
    events.emplace(attacker, defender);
}

void FightFunctor::operator()() {
    while (true) {
        // брать из очереди ивент
        // если произошло убийство
        // то надо поменять в defender поле alive на false
    }
}

void MoveFunctor::operator()() {
    while (true) {
        // постоянно проходиться по сету npc
        // если кто то умер, но он все равно в сете - удалить
        // если позволяет дистанция, то создать ивент в FightFunctor
    }
}


void start_fight(std::set<std::shared_ptr<Npc> > &set_npc, const int distance) {
    std::set dead_list = simulate_fight(set_npc, distance);

    for (auto &dead_npc : dead_list) {
        set_npc.erase(dead_npc);
    }
}


void start_programm() {
    // основной поток
    // сгенерировать npc
    // запустить два потока FightFunctor и MoveFunctor
    // генерировать карту через определенные промежутки времени



}