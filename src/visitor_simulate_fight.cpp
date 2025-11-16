#include "../include/visitor_simulate_fight.h"

bool validate_fight(const std::shared_ptr<Npc> &npc_1, const std::shared_ptr<Npc> &npc_2, const int distance) {
    if (npc_1 == npc_2) {
        return false;
    }

    unsigned int max_x = std::max(npc_1->get_x(), npc_2->get_x());
    unsigned int min_x = std::min(npc_1->get_x(), npc_2->get_x());
    unsigned int max_y = std::max(npc_1->get_y(), npc_2->get_y());
    unsigned int min_y = std::min(npc_1->get_y(), npc_2->get_y());
    if (!(max_x - min_x <= distance && max_y - min_y <= distance)) {
        return false;
    }
    return true;
}

std::set<std::shared_ptr<Npc>> simulate_fight(const std::set<std::shared_ptr<Npc> > &set_npc, const int distance) {
    std::set<std::shared_ptr<Npc> > dead_list;

    for (auto& attacker : set_npc) {
        for (auto &defender : set_npc) {
            if (!validate_fight(attacker, defender, distance)) continue;

            if (attacker->accept(defender)) {
                dead_list.insert(defender);
                attacker->notify(*defender);
            }
        }
    }
    return dead_list;
}

bool VisitorFightDragon::fight(std::shared_ptr <Npc> defender) {
    // атакующий дракон
    if (dynamic_pointer_cast<KnightErrant>(defender)) {
        // если защищающийся рыцарь
        return true;
    }
    return false;
}

bool VisitorFightFrog::fight(std::shared_ptr <Npc> defender) {
    // атакующий жаба - всегда побеждает
    return true;
}

bool VisitorFightKnightErrant::fight(std::shared_ptr <Npc> defender) {
    // атакующий рыцарь
    if (dynamic_pointer_cast<Dragon>(defender)) {
        // если защищающийся дракон
        return true;
    }
    return false;
}

