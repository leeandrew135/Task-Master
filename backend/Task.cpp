#include "crow.h"
#include "Task.h"

/**
 * @brief Construct a new Task object.
 * 
 * Initializes a task with the given parameters.
 * 
 * @param taskID The unique ID of the task. (primary key)
 * @param taskName The name or title of the task.
 * @param taskDate The date associated with the task.
 * @param taskStatus The current status (backlog, in progress, completed).
 * @param taskPriority The priority level of the task.
 * @param taskDesc A description of the task.
 * 
 * @author Robin
 */
Task::Task(int taskID, std::string taskName, std::string taskDate, std::string taskStatus, std::string taskPriority, std::string taskDesc)
    : taskID(taskID), taskName(taskName), taskDate(taskDate), taskStatus(taskStatus), taskPriority(taskPriority), taskDesc(taskDesc) {}

/**
 * @brief Get the task's unique ID.
 * 
 * @return The integer task ID.
 */
int Task::getTaskID() const
{
    return taskID;
}

/**
 * @brief Get the name of the task.
 * 
 * @return The task name as a string.
 */
std::string Task::getTaskName() const
{
    return taskName;
}

/**
 * @brief Get the date associated with the task.
 * 
 * @return The task date as a string.
 */
std::string Task::getTaskDate() const
{
    return taskDate;
}

/**
 * @brief Get the current status of the task.
 * 
 * @return The task status as a string.
 */
std::string Task::getTaskStatus() const
{
    return taskStatus;
}

/**
 * @brief Get the description of the task.
 * 
 * @return The task description as a string.
 */
std::string Task::getTaskDesc() const
{
    return taskDesc;
}

/**
 * @brief Get the priority level of the task.
 * 
 * @return The task priority as a string.
 */
std::string Task::getTaskPriority() const
{
    return taskPriority;
}

/**
 * @brief Edit the name of the task.
 * 
 * @param newName The new task name.
 */
void Task::editTaskName(const std::string &newName)
{
    taskName = newName;
}

/**
 * @brief Edit the date of the task.
 * 
 * @param newDate The new task date.
 */
void Task::editTaskDate(const std::string &newDate)
{
    taskDate = newDate;
}

/**
 * @brief Edit the status of the task.
 * 
 * @param newStatus The new task status.
 */
void Task::editTaskStatus(const std::string &newStatus)
{
    taskStatus = newStatus;
}

/**
 * @brief Edit the description of the task.
 * 
 * @param newDesc The new task description.
 */
void Task::editTaskDesc(const std::string &newDesc)
{
    taskDesc = newDesc;
}

/**
 * @brief Edit the priority of the task.
 * 
 * @param newPriority The new task priority.
 */
void Task::editTaskPriority(const std::string &newPriority)
{
    taskPriority = newPriority;
}

/**
 * @brief Display task information to standard output.
 * 
 * Outputs all task fields (ID, name, date, etc.) to `std::cout`.
 */
void Task::displayTask() const
{
    std::cout << "Task ID: " << taskID << "\n"
              << "Task Name: " << taskName << "\n"
              << "Task Date: " << taskDate << "\n"
              << "Task Status: " << taskStatus << "\n"
              << "Task Priority: " << taskPriority << "\n"
              << "Task Description: " << taskDesc << "\n";
}

/**
 * @brief Convert task to a JSON object.
 * 
 * This method packages all task fields into a JSON structure for output.
 * 
 * @return A crow::json::wvalue representing the task.
 */
crow::json::wvalue Task::toJSON() const
{
    crow::json::wvalue json;
    json["taskID"] = taskID;
    json["taskName"] = taskName;
    json["taskDate"] = taskDate;
    json["taskStatus"] = taskStatus;
    json["taskPriority"] = taskPriority;
    json["taskDesc"] = taskDesc;
    return json;
}