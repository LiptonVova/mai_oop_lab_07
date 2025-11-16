#ifndef LAB_06_FACTORY_NPC_H
#define LAB_06_FACTORY_NPC_H

#include <memory>
#include <fstream>

#include "npc.h"
#include "dragon.h"
#include "frog.h"
#include "knight_errant.h"


enum class TypeNpc { dragon, frog, knight_errant };

class FactoryNpc {
public:
    static std::shared_ptr<Npc> create_npc(const TypeNpc &type, const std::string &name, unsigned int x = 0, unsigned int y = 0);
    static std::shared_ptr<Npc> create_npc_from_file(const TypeNpc& type, std::ifstream& in);
};

#endif //LAB_06_FACTORY_NPC_H