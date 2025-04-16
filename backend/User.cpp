#include "User.h"
#include "Project.h"

/**
 * @brief Construct a new User object.
 *
 * Initializes a user with the given ID, name, email, and password.
 *
 * @param userID The unique ID of the user.
 * @param userName The name of the user.
 * @param userEmail The email address of the user.
 * @param userPassword The user's password.
 *
 * @author Robin
 */
User::User(int userID, const std::string& userName, const std::string& userEmail, const std::string& userPassword)
    : userID(userID), userName(userName), userEmail(userEmail), userPassword(userPassword) {}

/**
 * @brief Add a project to the user's list of projects.
 *
 * @param projectID The ID of the project.
 * @param project A shared pointer to the Project object.
 */
void User::addProject(int projectID, const std::shared_ptr<Project>& project)
{
    userProjects[projectID] = project;
}

/**
 * @brief Remove a project from the user's list by ID.
 *
 * Does nothing if the project is not found.
 *
 * @param projectID The ID of the project to remove.
 */
void User::removeProject(int projectID)
{
    if (userProjects.find(projectID) != userProjects.end()) {
        userProjects.erase(projectID);
    }
}

/**
 * @brief Get the user's ID.
 * @return The user ID as an integer.
 */
int User::getUserID() const { return userID; }

/**
 * @brief Get the user's name.
 * @return The user name as a string.
 */
std::string User::getUserName() const { return userName; }

/**
 * @brief Get the user's email address.
 * @return The user email as a string.
 */
std::string User::getUserEmail() const { return userEmail; }

/**
 * @brief Get the user's password.
 * @return The user password as a string.
 */
std::string User::getUserPassword() const { return userPassword; }

/**
 * @brief Get all projects associated with this user.
 *
 * @return A map of project IDs to shared pointers to Project objects.
 */
std::map<int, std::shared_ptr<Project>> User::getUserProjects() const {
    return userProjects;
}
