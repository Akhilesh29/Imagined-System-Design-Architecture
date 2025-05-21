#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <unordered_map>

namespace imagined {

enum class ProjectStatus {
    PENDING,
    IN_PROGRESS,
    REVIEW,
    COMPLETED,
    CANCELLED
};

enum class ProjectType {
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

struct Project {
    std::string id;
    std::string name;
    std::string clientId;
    ProjectType type;
    ProjectStatus status;
    std::chrono::system_clock::time_point deadline;
    std::vector<std::string> assignedTeamMembers;
    std::string projectManager;
    double budget;
    std::string description;
};

class ProjectManager {
public:
    ProjectManager();
    ~ProjectManager();

    // Project CRUD operations
    std::string createProject(const Project& project);
    bool updateProject(const std::string& projectId, const Project& project);
    bool deleteProject(const std::string& projectId);
    Project getProject(const std::string& projectId);
    
    // Project status management
    bool updateProjectStatus(const std::string& projectId, ProjectStatus newStatus);
    std::vector<Project> getProjectsByStatus(ProjectStatus status);
    
    // Team management
    bool assignTeamMember(const std::string& projectId, const std::string& teamMemberId);
    bool removeTeamMember(const std::string& projectId, const std::string& teamMemberId);
    
    // Project tracking
    std::vector<Project> getProjectsByClient(const std::string& clientId);
    std::vector<Project> getProjectsByType(ProjectType type);
    std::vector<Project> getUpcomingDeadlines(int daysThreshold);

private:
    std::unordered_map<std::string, Project> projects_;
    // Add more private members as needed
};

} // namespace imagined 