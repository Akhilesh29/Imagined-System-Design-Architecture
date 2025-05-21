#include "ProjectManager.hpp"
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>

namespace imagined {

ProjectManager::ProjectManager() {}

ProjectManager::~ProjectManager() {}

std::string ProjectManager::createProject(const Project& project) {
    // Generate a unique project ID
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
    
    Project newProject = project;
    newProject.id = uuid;
    projects_[uuid] = newProject;
    return uuid;
}

bool ProjectManager::updateProject(const std::string& projectId, const Project& project) {
    if (projects_.find(projectId) == projects_.end()) {
        return false;
    }
    projects_[projectId] = project;
    return true;
}

bool ProjectManager::deleteProject(const std::string& projectId) {
    return projects_.erase(projectId) > 0;
}

Project ProjectManager::getProject(const std::string& projectId) {
    if (projects_.find(projectId) == projects_.end()) {
        throw std::runtime_error("Project not found");
    }
    return projects_[projectId];
}

bool ProjectManager::updateProjectStatus(const std::string& projectId, ProjectStatus newStatus) {
    if (projects_.find(projectId) == projects_.end()) {
        return false;
    }
    projects_[projectId].status = newStatus;
    return true;
}

std::vector<Project> ProjectManager::getProjectsByStatus(ProjectStatus status) {
    std::vector<Project> result;
    for (const auto& pair : projects_) {
        if (pair.second.status == status) {
            result.push_back(pair.second);
        }
    }
    return result;
}

bool ProjectManager::assignTeamMember(const std::string& projectId, const std::string& teamMemberId) {
    if (projects_.find(projectId) == projects_.end()) {
        return false;
    }
    auto& project = projects_[projectId];
    if (std::find(project.assignedTeamMembers.begin(), 
                  project.assignedTeamMembers.end(), 
                  teamMemberId) == project.assignedTeamMembers.end()) {
        project.assignedTeamMembers.push_back(teamMemberId);
        return true;
    }
    return false;
}

bool ProjectManager::removeTeamMember(const std::string& projectId, const std::string& teamMemberId) {
    if (projects_.find(projectId) == projects_.end()) {
        return false;
    }
    auto& project = projects_[projectId];
    auto it = std::find(project.assignedTeamMembers.begin(), 
                       project.assignedTeamMembers.end(), 
                       teamMemberId);
    if (it != project.assignedTeamMembers.end()) {
        project.assignedTeamMembers.erase(it);
        return true;
    }
    return false;
}

std::vector<Project> ProjectManager::getProjectsByClient(const std::string& clientId) {
    std::vector<Project> result;
    for (const auto& pair : projects_) {
        if (pair.second.clientId == clientId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<Project> ProjectManager::getProjectsByType(ProjectType type) {
    std::vector<Project> result;
    for (const auto& pair : projects_) {
        if (pair.second.type == type) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<Project> ProjectManager::getUpcomingDeadlines(int daysThreshold) {
    std::vector<Project> result;
    auto now = std::chrono::system_clock::now();
    auto threshold = std::chrono::hours(24 * daysThreshold);
    
    for (const auto& pair : projects_) {
        if (pair.second.status != ProjectStatus::COMPLETED && 
            pair.second.status != ProjectStatus::CANCELLED) {
            auto timeUntilDeadline = pair.second.deadline - now;
            if (timeUntilDeadline <= threshold && timeUntilDeadline > std::chrono::hours(0)) {
                result.push_back(pair.second);
            }
        }
    }
    
    // Sort by deadline
    std::sort(result.begin(), result.end(), 
              [](const Project& a, const Project& b) {
                  return a.deadline < b.deadline;
              });
    
    return result;
}

} // namespace imagined 