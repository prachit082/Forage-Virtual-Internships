#pragma once

#include <string>
#include <vector>
#include <memory>
class Player;
class Sponsor;
class NIL_Offer;

class OfferRequirement {
public:
    virtual ~OfferRequirement() = default;
    virtual void execute(const Player& player) const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getTimeCommitment() const = 0;
};

class SocialMediaPostRequirement : public OfferRequirement {
public:
    // Constructor to initialize the requirement.
    SocialMediaPostRequirement(const std::string& platform, const std::string& postContent);

    void execute(const Player& player) const override;
    std::string getDescription() const override;
    int getTimeCommitment() const override;

private:
    std::string platform;
    std::string postContent;
    int timeCommitmentHours;
};

class AppearanceRequirement : public OfferRequirement {
public:
    // Constructor.
    AppearanceRequirement(const std::string& eventName, const std::string& location, int durationHours);

    // Override base class methods.
    void execute(const Player& player) const override;
    std::string getDescription() const override;
    int getTimeCommitment() const override;

private:
    std::string eventName;
    std::string location;
    int durationHours;
};

class Sponsor {
public:
    // Constructor to create a new sponsor.
    Sponsor(const std::string& brandName, double budget);
    std::unique_ptr<NIL_Offer> createOffer(const Player& targetPlayer, double cashValue, std::vector<std::unique_ptr<OfferRequirement>> requirements);

    // Getter for the brand name.
    std::string getBrandName() const;

private:
    std::string brandName;
    double sponsorshipBudget;
};

class NIL_Offer {
public:
    // Constructor. Takes a reference to the sponsor and owns its requirements.
    NIL_Offer(const Sponsor& offeringSponsor, double cashValue, std::vector<std::unique_ptr<OfferRequirement>> requirements);

    // Getter for the cash value of the offer.
    double getCashValue() const;

    // Getter for the sponsoring brand's name.
    std::string getSponsorName() const;

    // Getter for the list of requirements (const access).
    const std::vector<std::unique_ptr<OfferRequirement>>& getRequirements() const;

private:
    const Sponsor& offeringSponsor;
    double cashValue;
    std::vector<std::unique_ptr<OfferRequirement>> requirements;
};

class Player {
public:
    // Constructor to initialize a player.
    Player(const std::string& name, int nilAppeal);
    void receiveOffer(std::unique_ptr<NIL_Offer> offer);
    void acceptOffer(size_t offerIndex);
    void viewOffers() const;
    std::string getName() const;

private:
    std::string playerName;
    int nilAppeal;
    float currentMorale;
    std::vector<std::unique_ptr<NIL_Offer>> availableOffers;
};
