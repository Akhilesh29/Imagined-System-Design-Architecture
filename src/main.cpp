#include "core/ProjectManager.hpp"
#include "core/TalentManager.hpp"
#include "core/ResourceAllocator.hpp"
#include <iostream>
#include <iomanip>

using namespace imagined;

void printProject(const Project& project) {
    std::cout << "Project ID: " << project.id << std::endl;
    std::cout << "Name: " << project.name << std::endl;
    std::cout << "Status: " << static_cast<int>(project.status) << std::endl;
    std::cout << "Type: " << static_cast<int>(project.type) << std::endl;
    std::cout << "Budget: $" << std::fixed << std::setprecision(2) << project.budget << std::endl;
    std::cout << "Team Size: " << project.assignedTeamMembers.size() << std::endl;
    std::cout << "------------------------" << std::endl;
}

void printTalent(const Talent& talent) {
    std::cout << "Talent ID: " << talent.id << std::endl;
    std::cout << "Name: " << talent.name << std::endl;
    std::cout << "Skills: " << talent.skills.size() << std::endl;
    std::cout << "Experience Level: " << static_cast<int>(talent.experienceLevel) << std::endl;
    std::cout << "Hourly Rate: $" << std::fixed << std::setprecision(2) << talent.hourlyRate << std::endl;
    std::cout << "Available: " << (talent.isAvailable ? "Yes" : "No") << std::endl;
    std::cout << "------------------------" << std::endl;
}

int main() {
    std::cout << "Imagined Studio System Demo\n" << std::endl;

    // Initialize managers
    ProjectManager projectManager;
    TalentManager talentManager;
    ResourceAllocator resourceAllocator(projectManager, talentManager);

    // Create a project
    Project project;
    project.name = "Website Redesign";
    project.type = ProjectType::WEB_DESIGN;
    project.status = ProjectStatus::PENDING;
    project.budget = 50000.0;
    project.description = "Complete website redesign for client";
    
    std::string projectId = projectManager.createProject(project);
    std::cout << "Created project with ID: " << projectId << std::endl;

    // Create a talent
    Talent talent;
    talent.name = "John Doe";
    talent.email = "john@example.com";
    talent.skills.insert(SkillType::WEB_DESIGN);
    talent.experienceLevel = ExperienceLevel::SENIOR;
    talent.hourlyRate = 75.0;
    talent.isAvailable = true;
    talent.timezone = "UTC-5";
    talent.preferredLanguage = "English";

    std::string talentId = talentManager.addTalent(talent);
    std::cout << "Added talent with ID: " << talentId << std::endl;

    // Assign talent to project
    projectManager.assignTeamMember(projectId, talentId);
    std::cout << "Assigned talent to project" << std::endl;

    // Create resource allocation request
    AllocationRequest request;
    request.projectId = projectId;
    request.requiredSkills = {"1"}; // WEB_DESIGN
    request.requiredTeamSize = 1;
    request.startDate = std::chrono::system_clock::now();
    request.endDate = request.startDate + std::chrono::hours(24 * 30); // 30 days
    request.budget = 50000.0;

    // Allocate resources
    AllocationResult result = resourceAllocator.allocateResources(request);
    std::cout << "Resource allocation result: " << (result.success ? "Success" : "Failed") << std::endl;
    std::cout << "Message: " << result.message << std::endl;

    // Print project details
    std::cout << "\nProject Details:" << std::endl;
    printProject(projectManager.getProject(projectId));

    // Print talent details
    std::cout << "\nTalent Details:" << std::endl;
    printTalent(talentManager.getTalent(talentId));

    return 0;
} 