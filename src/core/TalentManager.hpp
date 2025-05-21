#pragma once

#include <string>
#include <vector>
#include <set>
#include <unordered_map>

namespace imagined {

enum class SkillType {
    THREE_D_DESIGN,
    APP_DESIGN,
    WEB_DESIGN,
    MOTION_DESIGN,
    BRAND_IDENTITY,
    VIDEO_PRODUCTION,
    TWO_D_ANIMATION,
    PITCH_DECK,
    AD_GRAPHICS,
    PACKAGING_DESIGN
};

enum class ExperienceLevel {
    JUNIOR,
    MID_LEVEL,
    SENIOR,
    LEAD
};

struct Talent {
    std::string id;
    std::string name;
    std::string email;
    std::set<SkillType> skills;
    ExperienceLevel experienceLevel;
    std::vector<std::string> completedProjects;
    double hourlyRate;
    bool isAvailable;
    std::string timezone;
    std::string preferredLanguage;
};

class TalentManager {
public:
    TalentManager();
    ~TalentManager();

    // Talent CRUD operations
    std::string addTalent(const Talent& talent);
    bool updateTalent(const std::string& talentId, const Talent& talent);
    bool removeTalent(const std::string& talentId);
    Talent getTalent(const std::string& talentId);

    // Skill management
    bool addSkill(const std::string& talentId, SkillType skill);
    bool removeSkill(const std::string& talentId, SkillType skill);
    std::vector<Talent> getTalentsBySkill(SkillType skill);

    // Availability management
    bool updateAvailability(const std::string& talentId, bool isAvailable);
    std::vector<Talent> getAvailableTalents();

    // Project assignment
    bool assignProject(const std::string& talentId, const std::string& projectId);
    bool removeProject(const std::string& talentId, const std::string& projectId);
    std::vector<std::string> getTalentProjects(const std::string& talentId);

    // Search and filtering
    std::vector<Talent> searchTalents(const std::string& query);
    std::vector<Talent> getTalentsByExperienceLevel(ExperienceLevel level);
    std::vector<Talent> getTalentsByHourlyRateRange(double minRate, double maxRate);

private:
    std::unordered_map<std::string, Talent> talents_;
    // Add more private members as needed
};

} // namespace imagined 