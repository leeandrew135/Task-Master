/**
 * @file Project.cpp
 * @brief Implementation of the Project class.
 *
 * This file is for the Project class
 * It adds tasks and users, modifying deadlines, and tracking project completion status.
 * It also includes a helper function to retrieve the current date.
 * Though we didn't use the current date and the completions status and the deadline.
 * We just couldn't think of a reason why we need those when we have the Task doing all that
 *
 * @author Luca Robin
 */

#include "Project.h"
#include "User.h"
#include <ctime>

/**
 * @brief Helper function to get the current system date.
 *
 * Gets the current date in the format YYYY-MM-DD.
 *
 * @return std::string Current date as a string.
 */
std::string getCurrentDate()
{
    time_t now = time(0);
    tm* localTime = localtime(&now);
    char buffer[11];
    strftime(buffer, 11, "%Y-%m-%d", localTime);
    return std::string(buffer);
}

/**
 * @brief Constructs a Project object with default date.
 *
 * Initializes the project with a to-do list, a deadline, and a completion status.
 * The date is set to the current date at the time of creation.
 *
 * @param list A TodoList object containing initial tasks.
 * @param deadline A string representing the project deadline (YYYY-MM-DD).
 * @param completionStatus Boolean indicating whether the project is complete.
 */
Project::Project(TodoList list, std::string deadline, bool completionStatus)
    : list(list), deadline(deadline), completionStatus(completionStatus) {
    date = getCurrentDate();
}

/**
 * @brief Adds a new task to the project's to-do list.
 *
 * The task is created with a random ID, the given name, today's date,
 * and default values for status and priority.
 *
 * @param name The name/title of the task.
 */
void Project::addTask(std::string name)
{
    Task newTask(rand(), name, getCurrentDate(), "Backlog", "Medium");
    list.createTask(newTask);
}

/**
 * @brief Adds a user to the project.
 *
 * Appends the given user pointer to the list of users assigned to the project.
 *
 * @param user Pointer to the User object to be added.
 */
void Project::addUser(User* user) {
    userList.push_back(user);
}

/**
 * @brief Changes the project's deadline.
 *
 * Updates the deadline to the new specified date string.
 *
 * @param newDeadline The new deadline in YYYY-MM-DD format.
 */
void Project::changeDeadline(std::string newDeadline) {
    deadline = newDeadline;
}

/**
 * @brief Marks the project as complete or incomplete.
 *
 * Updates the project's completion status.
 *
 * @param status True if the project is complete, false otherwise.
 */
void Project::markComplete(bool status) {
    completionStatus = status;
}

// ========================
// Getters
// ========================

/**
 * @brief Gets the creation date of the project.
 *
 * @return std::string The creation date in YYYY-MM-DD format.
 */
std::string Project::getDate() const { return date; }

/**
 * @brief Gets the project's deadline.
 *
 * @return std::string The deadline in YYYY-MM-DD format.
 */
std::string Project::getDeadline() const { return deadline; }

/**
 * @brief Checks if the project is complete.
 *
 * @return true if the project is complete, false otherwise.
 */
bool Project::isComplete() const { return completionStatus; }

/**
 * @brief Retrieves the list of users associated with the project.
 *
 * @return std::vector<User*> A vector of pointers to User objects.
 */
std::vector<User*> Project::getUsers() const {
    return userList;
}