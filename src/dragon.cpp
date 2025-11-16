#include "../include/dragon.h"

#include "../include/visitor_simulate_fight.h"

std::string Dragon::info() const {
    return "dragon(" + std::to_string(this->x) + ' ' + std::to_string(this->y) + ") " + '[' + this->unique_name + ']';
}
bool Dragon::accept(std::shared_ptr <Npc> npc_ptr) const {
    VisitorFightDragon visitor_fight_dragon;
    return visitor_fight_dragon.fight(npc_ptr);
}