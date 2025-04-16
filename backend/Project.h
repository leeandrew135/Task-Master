/**
 * @file Project.h
 * @brief Declaration of the Project class.
 * 
 * The Project class represents a project with the todoList,
 * users, a creation date, a deadline, and a completion status. It provides
 * methods to manage tasks and users, modify deadlines, and check project status.
 * 
 * @author Luca Robin
 */

#ifndef PROJECT_H
#define PROJECT_H

#include <iostream>
#include <string>
#include <vector>
#include "TodoList.h"

class User;

/**
 * @class Project
 * @brief Represents a project with tasks, users, and progress tracking.
 * 
 * Each project tracks a to-do list, participating users, a creation date, a deadline,
 * and whether the project is completed. Provides functionality to add tasks/users and 
 * update key attributes.
 */
class Project
{
private:
    TodoList list;                  /**< To-do list of the project */
    std::vector<User*> userList;    /**< List of users assigned to the project */
    std::string date;               /**< Creation date of the project */
    std::string deadline;           /**< Deadline for the project */
    bool completionStatus;          /**< Indicates whether the project is complete */

public:
    /**
     * @brief Constructor for the Project class.
     * 
     * Initializes a project with a to-do list, a deadline, and an optional completion status.
     * The creation date is automatically set to the current date at instantiation.
     * 
     * @param list The TodoList associated with this project.
     * @param deadline Deadline date in YYYY-MM-DD format.
     * @param completionStatus Initial completion status (default is false).
     */
    Project(TodoList list, std::string deadline, bool completionStatus = false);

    /**
     * @brief Adds a new task to the project's task list.
     * 
     * @param name The name of the task to be added.
     */
    void addTask(std::string name);

    /**
     * @brief Adds a user to the project's user list.
     * 
     * @param user A pointer to the User to be added to the project.
     */
    void addUser(User* user);

    /**
     * @brief Updates the deadline of the project.
     * 
     * @param date The new deadline in YYYY-MM-DD format.
     */
    void changeDeadline(std::string date);

    /**
     * @brief Sets the completion status of the project.
     * 
     * @param status True if the project is complete, false otherwise.
     */
    void markComplete(bool status);

    /**
     * @brief Retrieves the creation date of the project.
     * 
     * @return A string representing the creation date in YYYY-MM-DD format.
     */
    std::string getDate() const;

    /**
     * @brief Retrieves the project's deadline.
     * 
     * @return A string representing the deadline in YYYY-MM-DD format.
     */
    std::string getDeadline() const;

    /**
     * @brief Checks whether the project is complete.
     * 
     * @return True if complete, false otherwise.
     */
    bool isComplete() const;

    /**
     * @brief Gets the list of users assigned to the project.
     * 
     * @return A vector of User pointers.
     */
    std::vector<User*> getUsers() const;

    /**
     * @brief Equality operator.
     * 
     * Projects are considered equal if they share the same deadline and creation date.
     * 
     * @param other The other Project to compare against.
     * @return True if the projects have the same deadline and date, false otherwise.
     */
    bool operator==(const Project& other) const
    {
        return (this->deadline == other.deadline && this->date == other.date);
    }
};

#endif // PROJECT_H
