#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <string>
#include "crow.h"

/**
 * @class Task
 * @brief Represents a task with properties such as name, date, status, priority, and description.
 *
 * The Task class provides methods to create, edit, view, and convert task information to JSON format.
 * It supports basic getter and setter functionality for all relevant fields.
 * 
 * @author Robin
 */
class Task
{
private:
    int taskID;                  /**< Unique identifier for the task */
    std::string taskName;        /**< Name or title of the task */
    std::string taskDate;        /**< Date assigned to the task */
    std::string taskStatus;      /**< Current status (e.g., pending, done) */
    std::string taskDesc;        /**< Description of the task */
    std::string taskPriority;    /**< Priority level of the task */

public:
    /**
     * @brief Construct a new Task object.
     * 
     * Initializes a task with the given ID, name, date, status, priority, and optional description.
     * 
     * @param taskID The unique task ID.
     * @param taskName The name of the task.
     * @param taskDate The date associated with the task.
     * @param taskStatus The status of the task.
     * @param taskPriority The task's priority.
     * @param taskDesc (Optional) A description of the task.
     */
    Task(int taskID, std::string taskName, std::string taskDate, std::string taskStatus, std::string taskPriority, std::string taskDesc = "");

    /**
     * @brief Get the task's ID.
     * @return Task ID as an integer.
     */
    int getTaskID() const;

    /**
     * @brief Get the task's name.
     * @return Task name as a string.
     */
    std::string getTaskName() const;

    /**
     * @brief Get the task's date.
     * @return Task date as a string.
     */
    std::string getTaskDate() const;

    /**
     * @brief Get the task's status.
     * @return Task status as a string.
     */
    std::string getTaskStatus() const;

    /**
     * @brief Get the task's description.
     * @return Task description as a string.
     */
    std::string getTaskDesc() const;

    /**
     * @brief Get the task's priority.
     * @return Task priority as a string.
     */
    std::string getTaskPriority() const;

    /**
     * @brief Edit the name of the task.
     * @param newName The new task name.
     */
    void editTaskName(const std::string &newName);

    /**
     * @brief Edit the date of the task.
     * @param newDate The new task date.
     */
    void editTaskDate(const std::string &newDate);

    /**
     * @brief Edit the status of the task.
     * @param newStatus The new task status.
     */
    void editTaskStatus(const std::string &newStatus);

    /**
     * @brief Edit the task's description.
     * @param newDesc The new description.
     */
    void editTaskDesc(const std::string &newDesc);

    /**
     * @brief Edit the task's priority.
     * @param newPriority The new priority value.
     */
    void editTaskPriority(const std::string &newPriority);

    /**
     * @brief Print task details to standard output.
     */
    void displayTask() const;

    /**
     * @brief Convert the task into JSON format.
     * 
     * Uses crow's JSON writer to serialize the task object.
     * 
     * @return A crow::json::wvalue object containing the task's data.
     */
    crow::json::wvalue toJSON() const;
};

#endif // TASK_H