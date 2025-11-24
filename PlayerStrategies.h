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
    virtual string getStrategyName() const = 0;

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
    string getStrategyName() const override;
};

class BenevolentPlayerStrategy : public PlayerStrategies
{
public:
    BenevolentPlayerStrategy(Player *player);
    ~BenevolentPlayerStrategy() override;
    void issueOrder() override;
    vector<Territory *> toAttack() override;
    vector<Territory *> toDefend() override;
    string getStrategyName() const override;

};

class NeutralPlayerStrategy : public PlayerStrategies
{
public:
    NeutralPlayerStrategy(Player *player);
    ~NeutralPlayerStrategy() override;
    void issueOrder() override;
    vector<Territory *> toAttack() override;
    vector<Territory *> toDefend() override;
    string getStrategyName() const override;
};

class CheaterPlayerStrategy : public PlayerStrategies
{
public:
    CheaterPlayerStrategy(Player *player);
    ~CheaterPlayerStrategy() override;
    void issueOrder() override;
    vector<Territory *> toAttack() override;
    vector<Territory *> toDefend() override;
    string getStrategyName() const override;

};

class HumanPlayerStrategy : public PlayerStrategies
{
public:
    HumanPlayerStrategy(Player *player);
    ~HumanPlayerStrategy() override;
    void issueOrder() override;
    vector<Territory *> toAttack() override;
    vector<Territory *> toDefend() override;
    string getStrategyName() const override;

};

void testPlayerStrategies();
