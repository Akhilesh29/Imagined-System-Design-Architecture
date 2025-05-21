#include "TalentManager.hpp"
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>
#include <cctype>

namespace imagined {

TalentManager::TalentManager() {}

TalentManager::~TalentManager() {}

std::string TalentManager::addTalent(const Talent& talent) {
    // Generate a unique talent ID
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    const char* hex = "0123456789abcdef";
    std::string uuid;
    uuid.reserve(36);
    
    for (int i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            uuid += '-';
        } else {
            uuid += hex[dis(gen)];
        }
    }
    
    Talent newTalent = talent;
    newTalent.id = uuid;
    talents_[uuid] = newTalent;
    return uuid;
}

bool TalentManager::updateTalent(const std::string& talentId, const Talent& talent) {
    if (talents_.find(talentId) == talents_.end()) {
        return false;
    }
    talents_[talentId] = talent;
    return true;
}

bool TalentManager::removeTalent(const std::string& talentId) {
    return talents_.erase(talentId) > 0;
}

Talent TalentManager::getTalent(const std::string& talentId) {
    if (talents_.find(talentId) == talents_.end()) {
        throw std::runtime_error("Talent not found");
    }
    return talents_[talentId];
}

bool TalentManager::addSkill(const std::string& talentId, SkillType skill) {
    if (talents_.find(talentId) == talents_.end()) {
        return false;
    }
    talents_[talentId].skills.insert(skill);
    return true;
}

bool TalentManager::removeSkill(const std::string& talentId, SkillType skill) {
    if (talents_.find(talentId) == talents_.end()) {
        return false;
    }
    return talents_[talentId].skills.erase(skill) > 0;
}

std::vector<Talent> TalentManager::getTalentsBySkill(SkillType skill) {
    std::vector<Talent> result;
    for (const auto& pair : talents_) {
        if (pair.second.skills.find(skill) != pair.second.skills.end()) {
            result.push_back(pair.second);
        }
    }
    return result;
}

bool TalentManager::updateAvailability(const std::string& talentId, bool isAvailable) {
    if (talents_.find(talentId) == talents_.end()) {
        return false;
    }
    talents_[talentId].isAvailable = isAvailable;
    return true;
}

std::vector<Talent> TalentManager::getAvailableTalents() {
    std::vector<Talent> result;
    for (const auto& pair : talents_) {
        if (pair.second.isAvailable) {
            result.push_back(pair.second);
        }
    }
    return result;
}

bool TalentManager::assignProject(const std::string& talentId, const std::string& projectId) {
    if (talents_.find(talentId) == talents_.end()) {
        return false;
    }
    auto& talent = talents_[talentId];
    if (std::find(talent.completedProjects.begin(), 
                  talent.completedProjects.end(), 
                  projectId) == talent.completedProjects.end()) {
        talent.completedProjects.push_back(projectId);
        return true;
    }
    return false;
}

bool TalentManager::removeProject(const std::string& talentId, const std::string& projectId) {
    if (talents_.find(talentId) == talents_.end()) {
        return false;
    }
    auto& talent = talents_[talentId];
    auto it = std::find(talent.completedProjects.begin(), 
                       talent.completedProjects.end(), 
                       projectId);
    if (it != talent.completedProjects.end()) {
        talent.completedProjects.erase(it);
        return true;
    }
    return false;
}

std::vector<std::string> TalentManager::getTalentProjects(const std::string& talentId) {
    if (talents_.find(talentId) == talents_.end()) {
        throw std::runtime_error("Talent not found");
    }
    return talents_[talentId].completedProjects;
}

std::vector<Talent> TalentManager::searchTalents(const std::string& query) {
    std::vector<Talent> result;
    std::string lowercaseQuery = query;
    std::transform(lowercaseQuery.begin(), lowercaseQuery.end(), 
                  lowercaseQuery.begin(), ::tolower);
    
    for (const auto& pair : talents_) {
        const auto& talent = pair.second;
        std::string lowercaseName = talent.name;
        std::transform(lowercaseName.begin(), lowercaseName.end(), 
                      lowercaseName.begin(), ::tolower);
        
        if (lowercaseName.find(lowercaseQuery) != std::string::npos) {
            result.push_back(talent);
        }
    }
    return result;
}

std::vector<Talent> TalentManager::getTalentsByExperienceLevel(ExperienceLevel level) {
    std::vector<Talent> result;
    for (const auto& pair : talents_) {
        if (pair.second.experienceLevel == level) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<Talent> TalentManager::getTalentsByHourlyRateRange(double minRate, double maxRate) {
    std::vector<Talent> result;
    for (const auto& pair : talents_) {
        if (pair.second.hourlyRate >= minRate && pair.second.hourlyRate <= maxRate) {
            result.push_back(pair.second);
        }
    }
    return result;
}

} // namespace imagined 