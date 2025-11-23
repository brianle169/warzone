#pragma once

#include <vector>
#include "Player.h"
#include "Map.h"

class PlayerStrategies
{
public:
    PlayerStrategies() = default;
    PlayerStrategies(Player *player);
    virtual ~PlayerStrategies() = 0;
    virtual void issueOrder() = 0;
    virtual vector<Territory *> toAttack() = 0;
    virtual vector<Territory *> toDefend() = 0;

protected:
    Player *p;
};

class AggressivePlayerStrategy : public PlayerStrategies
{
public:
    AggressivePlayerStrategy(Player *player);
    ~AggressivePlayerStrategy() override;
    void issueOrder() override;
    vector<Territory *> toAttack() override;
    vector<Territory *> toDefend() override;
};

class BenevolentPlayerStrategy : public PlayerStrategies
{
public:
    BenevolentPlayerStrategy(Player *player);
    ~BenevolentPlayerStrategy() override;
    void issueOrder() override;
    vector<Territory *> toAttack() override;
    vector<Territory *> toDefend() override;
};

class NeutralPlayerStrategy : public PlayerStrategies
{
public:
    NeutralPlayerStrategy(Player *player);
    ~NeutralPlayerStrategy() override;
    void issueOrder() override;
    vector<Territory *> toAttack() override;
    vector<Territory *> toDefend() override;
};

class CheaterPlayerStrategy : public PlayerStrategies
{
public:
    CheaterPlayerStrategy(Player *player);
    ~CheaterPlayerStrategy() override;
    void issueOrder() override;
    vector<Territory *> toAttack() override;
    vector<Territory *> toDefend() override;
};

class HumanPlayerStrategy : public PlayerStrategies
{
public:
    HumanPlayerStrategy(Player *player);
    ~HumanPlayerStrategy() override;
    void issueOrder() override;
    vector<Territory *> toAttack() override;
    vector<Territory *> toDefend() override;
};

void testPlayerStrategies();
