#ifndef LAB_06_VISITOR_SIMULATE_FIGHT_H
#define LAB_06_VISITOR_SIMULATE_FIGHT_H

#include <set>
#include <memory>

#include "npc.h"
#include "dragon.h"
#include "knight_errant.h"
#include "frog.h"

class IVisitorFight {
public:
    // абстрактный класс visitor
    virtual bool fight(std::shared_ptr <Npc> defender) = 0;

    virtual ~IVisitorFight() = default;
};

struct VisitorFightDragon final : IVisitorFight  {
    // конкретный визитор - атакующий дракон
    bool fight(std::shared_ptr <Npc> defender) override;
};

class VisitorFightFrog final : public IVisitorFight {
public:
    VisitorFightFrog() = default;
    // конкретный визитор - атакующий жаба
    bool fight(std::shared_ptr <Npc> defender) override;
};

struct VisitorFightKnightErrant final : IVisitorFight {
    // конкретный визитор - атакующий странствующий рыцарь
    bool fight(std::shared_ptr <Npc> defender) override;
};



#endif //LAB_06_VISITOR_SIMULATE_FIGHT_H