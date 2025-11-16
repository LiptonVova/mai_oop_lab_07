#ifndef LAB_06_KNIGHT_ERRANT_H
#define LAB_06_KNIGHT_ERRANT_H

#include <memory>

#include "npc.h"

class KnightErrant final: public Npc {
private:
    unsigned int distance_step = 30;
    unsigned int distance_kill = 10;
public:
    KnightErrant() : Npc() {};
    KnightErrant(const unsigned int x, const unsigned int y, const std::string &name) : Npc(x, y, name) {};

    KnightErrant(KnightErrant &&other) noexcept : Npc(std::move(other)) {};
    KnightErrant(const KnightErrant &other) = default;

    bool accept(std::shared_ptr <Npc> npc_ptr) const override;

    std::string info() const override;
};


#endif //LAB_06_KNIGHT_ERRANT_H