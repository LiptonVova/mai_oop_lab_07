#ifndef LAB_06_DRAGON_H
#define LAB_06_DRAGON_H

#include <memory>

#include "npc.h"

class Dragon final : public Npc {
private:
    int distance_step = 50;
    int distance_kill = 50;
public:
    Dragon() : Npc() {};
    Dragon(const unsigned int x, const unsigned int y, const std::string &name) : Npc(x, y, name) {};

    Dragon(Dragon &&other) noexcept : Npc(std::move(other)) {};
    Dragon(const Dragon &other) = default;

    bool accept(std::shared_ptr <Npc> npc_ptr) const override;

    std::string info() const override;
};

#endif //LAB_06_DRAGON_H