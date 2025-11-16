#include "../include/frog.h"

#include "../include/visitor_simulate_fight.h"

std::string Frog::info() const {
    return "frog(" + std::to_string(this->x) + ' ' + std::to_string(this->y) + ") " + '[' + this->unique_name + ']';
}

bool Frog::accept(std::shared_ptr <Npc> npc_ptr) const {
    VisitorFightFrog visitor;
    return visitor.fight(npc_ptr);
}

unsigned int Frog::get_distance_step() const {
    return this->distance_step;
}

unsigned int Frog::get_distance_kill() const {
    return this->distance_kill;
}