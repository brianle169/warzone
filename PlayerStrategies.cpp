#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "Orders.h"
#include "Player.h"
#include "PlayerStrategies.h"
#include "Cards.h"
#include "GameEngine.h"

using namespace std;

PlayerStrategies::PlayerStrategies(Player *player) : p(player) {}

AggressivePlayerStrategy::AggressivePlayerStrategy(Player *player) : PlayerStrategies(player) {}

// BenevolentPlayerStrategy class implementation
vector<Territory *> BenevolentPlayerStrategy::toAttack()
{
    cout << "Available territories to attack: " << endl;
    unordered_map<string, Territory *> attackableTerritories = this->p->getAttackableTerritories();
    for (const auto &pair : attackableTerritories)
    {
        cout << "- " << pair.first << " (" << pair.second->getArmies() << ")" << endl;
    }
    cout << endl;

    cout << "Benevolent player does not attack any territories." << endl;

    return vector<Territory *>();
}
vector<Territory *> BenevolentPlayerStrategy::toDefend()
{
    cout << "Available territories to defend: " << endl;
    unordered_map<string, Territory *> defendableTerritories = this->p->getDefendableTerritories();
    for (const auto &pair : defendableTerritories)
    {
        cout << "- " << pair.first << " (" << pair.second->getArmies() << ")" << endl;
    }
    cout << endl;

    std::vector<Territory *> sortTer = *this->p->getTerritories();

    // sort terr vector from least to most armies
    std::sort(sortTer.begin(), sortTer.end(),
              [](Territory *a, Territory *b)
              {
                  return a->getArmies() < b->getArmies();
              });
    // defend the weakest terr
    this->p->getDefending()->clear();
    this->p->getDefending()->push_back(sortTer[0]);

    cout << "Benevolent player defends its weakest territory" << endl;
    return *this->p->getDefending();
}
void BenevolentPlayerStrategy::issueOrder()
{
    // Implementation for Benevolent player issuing an order
    cout << "\n==========================" << endl;
    cout << "===== BENEVOLENT PLAYER ===== ";
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

        // deploy all armies to weakest terretory
        Order *deployOrder = new Deploy(this->p, defendingTerritories[0], this->p->getReinforcementPool());
        this->p->setReinforcementPool(0);

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
    if (!this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AdvancePhase)))
    {
        std::vector<Territory *> sortTer = *this->p->getTerritories();

        // sort terr vector from least to most armies
        std::sort(sortTer.begin(), sortTer.end(),
                  [](Territory *a, Territory *b)
                  {
                      return a->getArmies() < b->getArmies();
                  });

        Order *advanceOrder = nullptr;

        // find highest differences of armies in adjacent terrotories and equalize armies
        int s = sortTer.size();
        for (size_t weakIdx = 0; weakIdx < sortTer.size(); weakIdx++)
        {
            Territory *weak = sortTer[weakIdx];
            for (int j = s - 1; j > (int)weakIdx; j--)
            { // Start at last valid index (s-1)
                if (sortTer[j]->isEdge(weak))
                { // Now j is always valid (0 to s-1)
                    int numStrong = sortTer[j]->getArmies();
                    int numWeak = weak->getArmies();
                    int armiesToMove = numStrong - ((numWeak + numStrong) / 2);
                    if (armiesToMove > 0)
                    {
                        advanceOrder = new Advance(this->p, armiesToMove, sortTer[j], weak);
                        break;
                    }
                }
            }
            if (advanceOrder != nullptr)
                break;
        }
        // for (auto weak : sortTer){
        //     for (s; s > 0; s--){
        //         if (sortTer[s]->isEdge(weak)){
        //             int numStrong = sortTer[s]->getArmies();
        //             int numWeak = weak->getArmies();
        //             advanceOrder = new Advance(this->p, numStrong-((numWeak+numStrong)/2), sortTer[s], weak);
        //         }
        //     }
        // }
        this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AdvancePhase)) = true;
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
        // resort terrs
        std::vector<Territory *> sortTer = *this->p->getTerritories();

        std::sort(sortTer.begin(), sortTer.end(),
                  [](Territory *a, Territory *b)
                  {
                      return a->getArmies() < b->getArmies();
                  });

        SpCard cardToPlay;
        string name;
        if (this->p->getHand()->includes("Airlift"))
        {

            name = "Airlift";
            // equalize strongest and weakest territories
            int numStrong = sortTer.back()->getArmies();
            int numWeak = sortTer[0]->getArmies();
            Airlift *airliftOrder = new Airlift(this->p, numStrong - ((numWeak + numStrong) / 2), sortTer.back(), sortTer[0]);

            airliftOrder->Attach(std::make_shared<LogObserver>());
            this->p->getOrdersList()->addOrder(airliftOrder);
            GameEngine::getCardDeck()->add(this->p->getHand()->getCard("Airlift")); // Add card back to deck
            this->p->getHand()->remove(this->p->getHand()->getCardIndex(name));     // Remove the card from hand after playing
        }

        this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::OtherPhase)) = true;
        // Add the card back to the deck

        return;
    }
    else
    {
        cout << "All possible orders have been issued for this turn." << endl;
    }
}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *player) : PlayerStrategies(player) {}

NeutralPlayerStrategy::NeutralPlayerStrategy(Player *player) : PlayerStrategies(player) {}

// CheaterPlayerStrategy class implementation

CheaterPlayerStrategy::CheaterPlayerStrategy(Player *player) : PlayerStrategies(player) {}

// CheaterPlayerStrategy::toAttack(): returns all adjacent enemy territories, allowing the cheater player to attack all of them.
vector<Territory *> CheaterPlayerStrategy::toAttack()
{
    vector<Territory *> attackableTerritories;
    for (Territory *t : *(this->p->getTerritories()))
    {
        for (Territory *neighbor : t->getEdges())
        {
            // If the neighboring territory is owned by another player, and not already in the list, add it to the attackable territories.
            if (neighbor->getPlayer() != this->p &&
                std::find(attackableTerritories.begin(), attackableTerritories.end(), neighbor) == attackableTerritories.end())
            {
                attackableTerritories.push_back(neighbor);
            }
        }
    }
    return attackableTerritories;
}

// CheaterPlayerStrategy::toDefend(): even though the cheater player doesn't defend, we will return the list of it's own territories for consistency.
vector<Territory *> CheaterPlayerStrategy::toDefend()
{
    // Cheater player does not defend any territories.
    vector<Territory *> defendableTerritories = *(this->p->getTerritories());
    return defendableTerritories;
}

// CheaterPlayerStrategy::issueOrder(): automatically conquers all adjacent enemy territories without issuing traditional orders. This is all the cheater player does during its turn.
void CheaterPlayerStrategy::issueOrder()
{
    cout << "\n==========================" << endl;
    cout << "=== Cheater Player " << (this->p->getName()) << "'s turn ===" << endl;
    cout << *this->p;
    cout << "==========================" << endl;
    cout << endl;

    if (!this->p->isDoneIssuingOrder())
    {
        // Cheater player automatically conquers all adjacent enemy territories. It will only do this once.
        cout << "Cheater Player " << this->p->getName() << " is conquering all adjacent enemy territories!" << endl;
        vector<Territory *> territoriesToConquer = this->p->toAttack();
        for (Territory *t : territoriesToConquer)
        {
            cout << "Cheater Player " << this->p->getName() << " conquers territory " << t->getName() << "!" << endl;
            t->getPlayer()->removeTerritory(t); // remove territory from current owner
            t->setPlayer(this->p);              // set new owner to cheater player
            this->p->addTerritory(t);           // add territory to cheater player's list
        }
        // Mark all phases as completed since the cheater player does not issue traditional orders.
        // This ensures that the cheat player will not attempt to issue any further orders in this turn.
        // And also prevent the cheater from conquering all the territories again in the next call.
        this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AttackDefendPhase)) = true;
        this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::DeployPhase)) = true;
        this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AdvancePhase)) = true;
        this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::OtherPhase)) = true;
    }
    cout << "Cheater Player " << this->p->getName() << " has completed its turn." << endl;
}

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

// Aggressive player wants to attack all attackable territories
vector<Territory *> AggressivePlayerStrategy::toAttack()
{
    // Get adjacent territories
    unordered_map<string, Territory *> attackableTerritoriesMap = this->p->getAttackableTerritories();

    // List them in console
    cout << "Available territories to attack: " << endl;
    for (const auto &pair : attackableTerritoriesMap)
    {
        cout << "- " << pair.first << " (" << pair.second->getArmies() << ")" << endl;
    }
    cout << endl;

    this->p->getAttacking()->clear();
    // Extract territories into vector
    for (auto &pair : attackableTerritoriesMap)
    {
        this->p->getAttacking()->push_back(pair.second);
    }

    cout << "Choosing all territories." << endl;

    // Return vector of countries to attack
    return *(this->p->getAttacking());
}

// Aggressive player only defends its strongest territory
vector<Territory *> AggressivePlayerStrategy::toDefend()
{

    // Get defendable territories
    cout << "Available territories to defend: " << endl;
    unordered_map<string, Territory *> defendableTerritories = this->p->getDefendableTerritories();

    // List them in console
    for (const auto &pair : defendableTerritories)
    {
        cout << "- " << pair.first << " (" << pair.second->getArmies() << ")" << endl;
    }
    cout << endl;

    vector<Territory *> territories = *(this->p->getTerritories());

    int strongestTerritoryIndex = 0;

    cout << "Choosing strongest territory." << endl;
    // Find territory with the most armies
    for (int i = 0; i < territories.size(); i++)
    {
        if (territories[i]->getArmies() > territories[strongestTerritoryIndex]->getArmies())
        {
            strongestTerritoryIndex = i;
        }
    }

    // vector<Territory*> defendingTerritories;

    // defendingTerritories.push_back(territories[strongestTerritoryIndex]);
    this->p->getDefending()->clear();
    this->p->getDefending()->push_back(territories[strongestTerritoryIndex]);
    cout << "The strongest (and thus only defendable) territory is at index " << strongestTerritoryIndex << endl;
    return *(this->p->getDefending());
    // return defendingTerritories;
}

// The aggressive player only plays airlift and bomb cards
void AggressivePlayerStrategy::issueOrder()
{

    vector<Territory *> attackingTerritories;
    vector<Territory *> defendingTerritories;
    Territory *strongestTerritory;

    // Attack/Defend phase
    if (!this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AttackDefendPhase)))
    {
        cout << "Choosing territories to attack and defend." << endl;
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

        // Protection against empty vector access crash
        if (defendingTerritories.empty())
        {
            cout << "No territories to defend. Skipping turn." << endl;
            this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::DeployPhase)) = true;
            this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AdvancePhase)) = true;
            this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::OtherPhase)) = true;
            return;
        }

        strongestTerritory = defendingTerritories[0];
    }

    // Deploy armies
    if ((this->p->getReinforcementPool()) > 0)
    {
        // Deploy all of the reinforcement pool to the strongest territory
        cout << "Deploying all reinforcement pool to the strongest territory" << endl;
        Order *deployOrder = new Deploy(this->p, strongestTerritory, this->p->getReinforcementPool());
        this->p->setReinforcementPool(0);
        deployOrder->Attach(std::make_shared<LogObserver>());
        this->p->getOrdersList()->addOrder(deployOrder);
        return;
    }
    else
    {
        cout << "Reinforcement pool is empty. No deploy orders can be issued." << endl;
        this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::DeployPhase)) = true;
    }
    // Advance armies
    if (!this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AdvancePhase)))
    {
        // Protection against empty attacking vector crash
        if (attackingTerritories.empty())
        {
            cout << "No territories to attack. Skipping advance phase." << endl;
            this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AdvancePhase)) = true;
            return;
        }

        // Select a random attackable territory to attack
        int randomIndex = rand() % attackingTerritories.size();
        auto *targetTerritory = attackingTerritories[randomIndex];

        Order *advanceOrder = new Advance(this->p, strongestTerritory->getArmies(), strongestTerritory, targetTerritory);
        if (advanceOrder != nullptr)
        {
            advanceOrder->Attach(std::make_shared<LogObserver>());
            this->p->getOrdersList()->addOrder(advanceOrder);
        }
        this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AdvancePhase)) = true;
        return;
    }
    else
    {
        cout << "Advance orders are finalized. Issuing other order types using cards." << endl;
    }

    // Play cards in hand
    if (!this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::OtherPhase)))
    {
        Hand *hand = this->p->getHand();

        // If there are no bomb cards or airlift cards, the player does not use those cards
        if (!hand->includes("BombCard"))
        {
            this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::OtherPhase)) = true;
            cout << "All aggressive orders have been issued for this turn." << endl;
            return;
        }
        int cardToPlayIndex = hand->getFirstIndexOf("BombCard");

        SpCard cardToPlay = hand->getCardAt(cardToPlayIndex);

        cardToPlay->play(*GameEngine::getCardDeck(), *(this->p));

        hand->remove(cardToPlayIndex); // Remove the card from hand after playing

        // Add the card back to the deck
        GameEngine::getCardDeck()->add(cardToPlay);
        this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::OtherPhase)) = true;
        return;
    }
}
// Neutral Player implementation

vector<Territory *> NeutralPlayerStrategy::toAttack()
{
    // Neutral player never attacks
    return vector<Territory *>();
}

vector<Territory *> NeutralPlayerStrategy::toDefend()
{
    // Neutral player never defends
    return vector<Territory *>();
}

void NeutralPlayerStrategy::issueOrder()
{
    // Neutral player does nothing
    cout << "Neutral player " << p->getName() << " does not issue orders.\n";

    // Mark all phases as complete to protect against infinite loop
    this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AttackDefendPhase)) = true;
    this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::DeployPhase)) = true;
    this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::AdvancePhase)) = true;
    this->p->getIssueOrderStatus()->at(static_cast<int>(IssuePhase::OtherPhase)) = true;
}

string HumanPlayerStrategy::getStrategyName() const
{
    return "HumanPlayerStrategy";
}

string AggressivePlayerStrategy::getStrategyName() const
{
    return "AggressivePlayerStrategy";
}

string BenevolentPlayerStrategy::getStrategyName() const
{
    return "BenevolentPlayerStrategy";
}

string NeutralPlayerStrategy::getStrategyName() const
{
    return "NeutralPlayerStrategy";
}

string CheaterPlayerStrategy::getStrategyName() const
{
    return "CheaterPlayerStrategy";
}

// DESTRUCTORS - Empty implementation, but needed for compilation
PlayerStrategies::~PlayerStrategies() {}
AggressivePlayerStrategy::~AggressivePlayerStrategy() {}
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}
NeutralPlayerStrategy::~NeutralPlayerStrategy() {}
CheaterPlayerStrategy::~CheaterPlayerStrategy() {}
HumanPlayerStrategy::~HumanPlayerStrategy() {}