#include "../include/factory_npc.h"

std::shared_ptr<Npc> FactoryNpc::create_npc(const TypeNpc &type, const std::string &name, unsigned int x, unsigned int y) {
    switch (type) {
        case TypeNpc::dragon:
            return std::make_shared<Dragon>(x, y, name);
        case TypeNpc::frog:
            return std::make_shared<Frog>(x, y, name);
        case TypeNpc::knight_errant:
            return std::make_shared<KnightErrant>(x, y, name);
    }
    return nullptr;
}

std::shared_ptr<Npc> FactoryNpc::create_npc_from_file(const TypeNpc &type, std::ifstream& in) {
    switch (type) {
        case TypeNpc::dragon:
            static Dragon dragon;

            if (in.is_open()) in >> dragon;
            else throw std::logic_error("File not found");

            return std::make_shared<Dragon>(dragon);
        case TypeNpc::frog:
            static Frog frog;

            if (in.is_open()) in >> frog;
            else throw std::logic_error("File not found");

            return std::make_shared<Frog>(frog);
        case TypeNpc::knight_errant:
            static KnightErrant knight_errant;

            if (in.is_open()) in >> knight_errant;
            else throw std::logic_error("File not found");

            return std::make_shared<KnightErrant>(knight_errant);
    }
    return nullptr;
}