#ifndef LAB_06_FROG_H
#define LAB_06_FROG_H

#include <memory>

#include "npc.h"

class Frog final: public Npc {
public:
    Frog() : Npc() {};
    Frog(const unsigned int x, const unsigned int y, const std::string &name) : Npc(x, y, name) {};

    Frog(Frog &&other) noexcept : Npc(std::move(other)) {};
    Frog(Frog &other) = default;

    bool accept(std::shared_ptr <Npc> npc_ptr) const override;

    std::string info() const override;
};

#endif //LAB_06_FROG_H