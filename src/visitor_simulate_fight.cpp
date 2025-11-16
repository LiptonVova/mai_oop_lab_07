#include "../include/visitor_simulate_fight.h"

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

