#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include "ProjectManager.hpp"
#include "TalentManager.hpp"

namespace imagined {

struct Resource {
    std::string id;
    std::string name;
    std::string type;
    bool isAvailable;
    std::chrono::system_clock::time_point lastUsed;
    std::string currentProjectId;
};

struct AllocationRequest {
    std::string projectId;
    std::vector<std::string> requiredSkills;
    int requiredTeamSize;
    std::chrono::system_clock::time_point startDate;
    std::chrono::system_clock::time_point endDate;
    double budget;
};

struct AllocationResult {
    bool success;
    std::vector<std::string> allocatedTalentIds;
    std::vector<std::string> allocatedResourceIds;
    std::string message;
};

class ResourceAllocator {
public:
    ResourceAllocator(ProjectManager& projectManager, TalentManager& talentManager);
    ~ResourceAllocator();

    // Resource management
    std::string addResource(const Resource& resource);
    bool updateResource(const std::string& resourceId, const Resource& resource);
    bool removeResource(const std::string& resourceId);
    Resource getResource(const std::string& resourceId);

    // Resource allocation
    AllocationResult allocateResources(const AllocationRequest& request);
    bool deallocateResources(const std::string& projectId);
    
    // Resource availability
    bool updateResourceAvailability(const std::string& resourceId, bool isAvailable);
    std::vector<Resource> getAvailableResources();
    
    // Resource tracking
    std::vector<Resource> getResourcesByProject(const std::string& projectId);
    std::vector<Resource> getResourcesByType(const std::string& type);
    
    // Resource optimization
    void optimizeResourceAllocation();
    std::vector<std::string> getUnderutilizedResources();
    std::vector<std::string> getOverutilizedResources();

private:
    ProjectManager& projectManager_;
    TalentManager& talentManager_;
    std::map<std::string, Resource> resources_;
    
    // Helper methods
    bool isResourceAvailable(const Resource& resource, 
                           const std::chrono::system_clock::time_point& startDate,
                           const std::chrono::system_clock::time_point& endDate);
    std::vector<std::string> findMatchingTalents(const std::vector<std::string>& requiredSkills);
    double calculateResourceUtilization(const std::string& resourceId);
};

} // namespace imagined 