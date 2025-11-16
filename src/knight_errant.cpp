#include "../include/knight_errant.h"

#include "../include/visitor_simulate_fight.h"

std::string KnightErrant::info() const {
    return "knight errant(" + std::to_string(this->x) + ' ' + std::to_string(this->y) + ") " + '[' + this->unique_name + ']';
}
bool KnightErrant::accept(std::shared_ptr <Npc> npc_ptr) const {
    VisitorFightKnightErrant visitor_fight_knight_errant;
    return visitor_fight_knight_errant.fight(npc_ptr);
}

unsigned int KnightErrant::get_distance_step() const {
    return this->distance_step;
}

unsigned int KnightErrant::get_distance_kill() const {
    return this->distance_kill;
}
