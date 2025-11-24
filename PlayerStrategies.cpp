#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "Orders.h"
#include "Player.h"
#include "PlayerStrategies.h"

PlayerStrategies::PlayerStrategies(Player *player) : p(player) {}

AggressivePlayerStrategy::AggressivePlayerStrategy(Player *player) : PlayerStrategies(player) {}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *player) : PlayerStrategies(player) {}

NeutralPlayerStrategy::NeutralPlayerStrategy(Player *player) : PlayerStrategies(player) {}

CheaterPlayerStrategy::CheaterPlayerStrategy(Player *player) : PlayerStrategies(player) {}

// HumanPlayerStrategy class implementation

HumanPlayerStrategy::HumanPlayerStrategy(Player *player) : PlayerStrategies(player) {}

vector<Territory *> HumanPlayerStrategy::toAttack()
{
    cout << "Available territories to attack: " << endl;
    unordered_map<string, Territory *> attackableTerritories = this->p->getAttackableTerritories();
    for (const auto &pair : attackableTerritories)
    {
        cout << "- " << pair.first << " (" << pair.second->getArmies() << ")" << endl;
    }
    cout << endl;

    string input = "";
    do
    {
        cout << "Select territories to attack (Enter 'x' to finish) \n>> ";
        cin >> input;
        if (input == "x" || input == "X")
            break;
        try
        {
            Territory *terr = attackableTerritories.at(input);
            if (std::find(this->p->getAttacking()->begin(), this->p->getAttacking()->end(), terr) != this->p->getAttacking()->end())
            {
                cout << "Territory " << terr->getName() << " has already been selected for attack." << endl;
                continue;
            }
            cout << "Territory " << terr->getName() << " selected for attack." << endl;
            this->p->getAttacking()->push_back(terr); // insert the territory pointer into attacking vector
            this->p->displayTerritories(*this->p->getAttacking());
        }
        catch (const out_of_range &)
        {
            cout << "Invalid territory. Please try again." << endl;
        }
    } while (true);
    return *this->p->getAttacking();
}

vector<Territory *> HumanPlayerStrategy::toDefend()
{
    cout << "Available territories to defend: " << endl;
    unordered_map<string, Territory *> defendableTerritories = this->p->getDefendableTerritories();
    for (const auto &pair : defendableTerritories)
    {
        cout << "- " << pair.first << " (" << pair.second->getArmies() << ")" << endl;
    }
    cout << endl;

    string input = "";
    do
    {
        cout << "Select territories to defend (Enter 'x' to finish) \n>> ";
        cin >> input;
        if (input == "x" || input == "X")
            break;
        try
        {
            Territory *terr = defendableTerritories.at(input);
            if (std::find(this->p->getDefending()->begin(), this->p->getDefending()->end(), terr) != this->p->getDefending()->end())
            {
                cout << "Territory " << terr->getName() << " has already been selected for defend." << endl;
                continue;
            }
            cout << "Territory " << terr->getName() << " selected for defend." << endl;
            this->p->getDefending()->push_back(terr); // insert the territory pointer into defending vector
            this->p->displayTerritories(*this->p->getDefending());
        }
        catch (const out_of_range &)
        {
            cout << "Invalid territory. Please try again." << endl;
        }
    } while (true);
    return *this->p->getDefending();
}

void HumanPlayerStrategy::issueOrder()
{
    // Implementation for human player issuing an order
    cout << "\n==========================" << endl;
    cout << "=== Player " << (this->p->getName()) << "'s turn ===" << endl;
    cout << *this->p;
    this->p->displayOrdersList(this->p->getOrdersList());
    cout << "==========================" << endl;
    cout << endl;

    vector<Territory *> attackingTerritories;
    vector<Territory *> defendingTerritories;
    // 2. Decide territories to attack and defend.
    if (!this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AttackDefendPhase)))
    {
        attackingTerritories = this->toAttack();
        defendingTerritories = this->toDefend();
        this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AttackDefendPhase)) = true;
        return;
    }
    else
    {
        cout << "Choosing territories to attack and defend has been completed." << endl;
        attackingTerritories = *(this->p->getAttacking());
        defendingTerritories = *(this->p->getDefending());
    }
    // 3. After choosing the territories to attack and defend, we will issue
    // deploy orders on the defending territories until the reinforcement pool is empty.
    if ((this->p->getReinforcementPool()) > 0)
    {
        // if the reinforcement pool is not empty, we only have one order option: Deploy
        Order *deployOrder = this->p->deploy(defendingTerritories);
        deployOrder->Attach(std::make_shared<LogObserver>());
        this->p->getOrdersList()->addOrder(deployOrder);
        return;
    }
    else
    {
        cout << "Reinforcement pool is empty. No deploy orders can be issued." << endl;
        this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::DeployPhase)) = true;
    }
    // 4. After deploying all reinforcements, we can issue other types of orders.
    // *Note: the following orders are optional depending on what the player wants to do.
    // That means they can skip issuing these orders if they want to
    if (!this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AdvancePhase)))
    {
        // If advance phase orders have not been issued, we can issue them now.
        Order *advanceOrder = this->p->advance(attackingTerritories, defendingTerritories);
        if (advanceOrder != nullptr)
        {
            advanceOrder->Attach(std::make_shared<LogObserver>());
            this->p->getOrdersList()->addOrder(advanceOrder);
        }
        return;
    }
    else
    {
        cout << "Advance orders are finalized. Now you can issue other types using corresponding cards." << endl;
    }

    // 5. Finally, we can issue other types of orders based on the cards in hand.
    if (!this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::OtherPhase)))
    {
        this->p->cardOrder();
    }
    else
    {
        cout << "All possible orders have been issued for this turn." << endl;
    }
}

//Neutral Player implementation


vector <Territory*> NeutralPlayerStrategy::toAttack() {
    // Neutral player never attacks
    return vector<Territory*>();
}

vector <Territory*> NeutralPlayerStrategy::toDefend() {
    // Neutral player never defends 
    return vector<Territory*>();
}

void NeutralPlayerStrategy::issueOrder() {
    // Neutral player does nothing
    cout << "Neutral player " << p->getName() << " does not issue orders.\n";
}

string HumanPlayerStrategy::getStrategyName() const {
    return "HumanPlayerStrategy";
}

string AggressivePlayerStrategy::getStrategyName() const {
    return "AggressivePlayerStrategy";
}

string BenevolentPlayerStrategy::getStrategyName() const {
    return "BenevolentPlayerStrategy";
}

string NeutralPlayerStrategy::getStrategyName() const {
    return "NeutralPlayerStrategy";
}

string CheaterPlayerStrategy::getStrategyName() const {
    return "CheaterPlayerStrategy";
}