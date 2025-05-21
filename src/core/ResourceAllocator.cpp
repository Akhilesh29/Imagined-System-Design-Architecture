#include "ResourceAllocator.hpp"
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace imagined {

ResourceAllocator::ResourceAllocator(ProjectManager& projectManager, TalentManager& talentManager)
    : projectManager_(projectManager), talentManager_(talentManager) {}

ResourceAllocator::~ResourceAllocator() {}

std::string ResourceAllocator::addResource(const Resource& resource) {
    // Generate a unique resource ID
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
    
    Resource newResource = resource;
    newResource.id = uuid;
    resources_[uuid] = newResource;
    return uuid;
}

bool ResourceAllocator::updateResource(const std::string& resourceId, const Resource& resource) {
    if (resources_.find(resourceId) == resources_.end()) {
        return false;
    }
    resources_[resourceId] = resource;
    return true;
}

bool ResourceAllocator::removeResource(const std::string& resourceId) {
    return resources_.erase(resourceId) > 0;
}

Resource ResourceAllocator::getResource(const std::string& resourceId) {
    if (resources_.find(resourceId) == resources_.end()) {
        throw std::runtime_error("Resource not found");
    }
    return resources_[resourceId];
}

AllocationResult ResourceAllocator::allocateResources(const AllocationRequest& request) {
    AllocationResult result;
    result.success = false;
    
    // Validate project exists
    try {
        projectManager_.getProject(request.projectId);
    } catch (const std::runtime_error&) {
        result.message = "Project not found";
        return result;
    }
    
    // Find matching talents
    std::vector<std::string> matchingTalentIds = findMatchingTalents(request.requiredSkills);
    if (matchingTalentIds.size() < request.requiredTeamSize) {
        result.message = "Insufficient matching talents";
        return result;
    }
    
    // Find available resources
    std::vector<Resource> availableResources = getAvailableResources();
    std::vector<std::string> allocatedResourceIds;
    
    for (const auto& resource : availableResources) {
        if (isResourceAvailable(resource, request.startDate, request.endDate)) {
            allocatedResourceIds.push_back(resource.id);
            resources_[resource.id].currentProjectId = request.projectId;
            resources_[resource.id].isAvailable = false;
            resources_[resource.id].lastUsed = std::chrono::system_clock::now();
        }
    }
    
    // Allocate talents to project
    for (size_t i = 0; i < request.requiredTeamSize; ++i) {
        talentManager_.assignProject(matchingTalentIds[i], request.projectId);
        result.allocatedTalentIds.push_back(matchingTalentIds[i]);
    }
    
    result.allocatedResourceIds = allocatedResourceIds;
    result.success = true;
    result.message = "Resources allocated successfully";
    return result;
}

bool ResourceAllocator::deallocateResources(const std::string& projectId) {
    bool success = false;
    
    for (auto& pair : resources_) {
        if (pair.second.currentProjectId == projectId) {
            pair.second.currentProjectId = "";
            pair.second.isAvailable = true;
            success = true;
        }
    }
    
    return success;
}

bool ResourceAllocator::updateResourceAvailability(const std::string& resourceId, bool isAvailable) {
    if (resources_.find(resourceId) == resources_.end()) {
        return false;
    }
    resources_[resourceId].isAvailable = isAvailable;
    return true;
}

std::vector<Resource> ResourceAllocator::getAvailableResources() {
    std::vector<Resource> result;
    for (const auto& pair : resources_) {
        if (pair.second.isAvailable) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<Resource> ResourceAllocator::getResourcesByProject(const std::string& projectId) {
    std::vector<Resource> result;
    for (const auto& pair : resources_) {
        if (pair.second.currentProjectId == projectId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<Resource> ResourceAllocator::getResourcesByType(const std::string& type) {
    std::vector<Resource> result;
    for (const auto& pair : resources_) {
        if (pair.second.type == type) {
            result.push_back(pair.second);
        }
    }
    return result;
}

void ResourceAllocator::optimizeResourceAllocation() {
    // Implement resource optimization logic
    // This could include:
    // 1. Balancing resource utilization
    // 2. Minimizing resource conflicts
    // 3. Optimizing for cost efficiency
    // 4. Improving resource availability
}

std::vector<std::string> ResourceAllocator::getUnderutilizedResources() {
    std::vector<std::string> result;
    for (const auto& pair : resources_) {
        if (calculateResourceUtilization(pair.first) < 0.3) { // 30% utilization threshold
            result.push_back(pair.first);
        }
    }
    return result;
}

std::vector<std::string> ResourceAllocator::getOverutilizedResources() {
    std::vector<std::string> result;
    for (const auto& pair : resources_) {
        if (calculateResourceUtilization(pair.first) > 0.9) { // 90% utilization threshold
            result.push_back(pair.first);
        }
    }
    return result;
}

bool ResourceAllocator::isResourceAvailable(const Resource& resource,
                                          const std::chrono::system_clock::time_point& startDate,
                                          const std::chrono::system_clock::time_point& endDate) {
    if (!resource.isAvailable) {
        return false;
    }
    
    // Check if resource is already allocated during the requested time period
    if (!resource.currentProjectId.empty()) {
        try {
            Project currentProject = projectManager_.getProject(resource.currentProjectId);
            if (currentProject.deadline > startDate) {
                return false;
            }
        } catch (const std::runtime_error&) {
            // If project not found, consider resource available
            return true;
        }
    }
    
    return true;
}

std::vector<std::string> ResourceAllocator::findMatchingTalents(const std::vector<std::string>& requiredSkills) {
    std::vector<std::string> result;
    std::vector<Talent> availableTalents = talentManager_.getAvailableTalents();
    
    for (const auto& talent : availableTalents) {
        bool hasAllSkills = true;
        for (const auto& skill : requiredSkills) {
            // Convert skill string to SkillType enum
            // This is a simplified version - in reality, you'd need proper skill mapping
            SkillType skillType = static_cast<SkillType>(std::stoi(skill));
            if (talent.skills.find(skillType) == talent.skills.end()) {
                hasAllSkills = false;
                break;
            }
        }
        if (hasAllSkills) {
            result.push_back(talent.id);
        }
    }
    
    return result;
}

double ResourceAllocator::calculateResourceUtilization(const std::string& resourceId) {
    if (resources_.find(resourceId) == resources_.end()) {
        return 0.0;
    }
    
    const Resource& resource = resources_[resourceId];
    auto now = std::chrono::system_clock::now();
    
    // Calculate utilization based on time allocated to projects
    // This is a simplified version - in reality, you'd need more sophisticated metrics
    if (resource.currentProjectId.empty()) {
        return 0.0;
    }
    
    try {
        Project currentProject = projectManager_.getProject(resource.currentProjectId);
        auto totalDuration = currentProject.deadline - resource.lastUsed;
        auto elapsedDuration = now - resource.lastUsed;
        
        return std::min(1.0, std::max(0.0, 
            std::chrono::duration<double>(elapsedDuration).count() / 
            std::chrono::duration<double>(totalDuration).count()));
    } catch (const std::runtime_error&) {
        return 0.0;
    }
}

} // namespace imagined 