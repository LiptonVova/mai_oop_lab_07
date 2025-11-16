#include "../include/core.h"

#include <thread>

#include "../include/visitor_simulate_fight.h"

void FightFunctor::add_event(std::shared_ptr<Npc> attacker, std::shared_ptr<Npc> defender) {
    events.emplace(attacker, defender);
}

void FightFunctor::operator()() {
    while (true) {
        // брать из очереди ивент
        // если произошло убийство
        // то надо поменять в defender поле alive на false

        while (!events.empty()) {
            FightEvent current_event = events.front();
            events.pop();

            std::lock_guard<std::mutex> lock(mtx);
            std::shared_ptr<Npc>& attacker = current_event.attacker;
            std::shared_ptr<Npc>& defender = current_event.defender;
            if (attacker->is_alive()) {
                if (defender->is_alive()) {
                    if (attacker->accept(defender)) {
                        defender->make_dead();
                    }
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

bool is_murder_available(std::shared_ptr<Npc> attacker, std::shared_ptr<Npc> defender) {
    const unsigned int max_x = std::max(attacker->get_x(), defender->get_x());
    const unsigned int min_x = std::min(attacker->get_x(), defender->get_x());
    const unsigned int max_y = std::max(attacker->get_y(), defender->get_y());
    const unsigned int min_y = std::min(attacker->get_y(), defender->get_y());

    const unsigned int distance_in_a_square = (max_x - min_x) * (max_x - min_x) + (max_y - min_y) * (max_y - min_y);
    if (distance_in_a_square > attacker->get_distance_kill() * attacker->get_distance_kill()) {
        return false;
    }
    unsigned int power_attack = (std::rand() % 6) + 1;
    unsigned int power_defender = (std::rand() % 6) + 1;

    if (power_attack > power_defender) {
        return true;
    }

    return false;
}

void MoveFunctor::operator()() {
    while (true) {
        // постоянно проходиться по сету npc
        // если кто-то умер, но он все равно в сете - удалить
        // если позволяет дистанция, то создать ивент в FightFunctor

        for (auto &attacker : set_npc) {
            std::lock_guard<std::mutex> lock(mtx);

            if (!attacker->is_alive()) {
                set_npc.erase(attacker);
            }

            // логика шага
            int shift_x = std::rand() % MAX_VALUE;
            int shift_y = std::rand() % MAX_VALUE;
            attacker->move(shift_x, shift_y, MAX_VALUE);

            for (auto &defender : set_npc) {
                if (!defender->is_alive()) {
                    set_npc.erase(defender);
                }

                if (is_murder_available(attacker, defender)) {
                    // какая-то логика убийства
                    fight_functor_.add_event(attacker, defender);
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


void start_fight(std::set<std::shared_ptr<Npc> > &set_npc, const int distance) {
    std::set dead_list = simulate_fight(set_npc, distance);

    for (auto &dead_npc : dead_list) {
        set_npc.erase(dead_npc);
    }
}

std::set<std::shared_ptr<Npc> > generate_npc(const int MAX_VALUE) {
    std::set<std::shared_ptr<Npc> > set_npc;
    const int N = 50;
    for (int i = 0; i < N; i++) {
        const int rand_type = std::rand() % 3;
        TypeNpc type = static_cast<TypeNpc>(rand_type);
        std::string name = "";
        switch (rand_type) {
            case 0:
                name = "dragon";
                break;
            case 1:
                name = "frog";
                break;
            case 2:
                name = "knight errant";
                break;
        }

        int x = std::rand() % MAX_VALUE;
        int y = std::rand() % MAX_VALUE;
        std::shared_ptr<Npc> npc = FactoryNpc::create_npc(type, name, x, y);
        set_npc.insert(npc);
    }

    return set_npc;
}


void start_programm() {
    // основной поток
    // сгенерировать npc
    // запустить два потока FightFunctor и MoveFunctor
    // генерировать карту через определенные промежутки времени

    const int MAX_VALUE = 500;

    std::set<std::shared_ptr<Npc> > set_npc = generate_npc(MAX_VALUE);







}