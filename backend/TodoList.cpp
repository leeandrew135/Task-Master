/**
* @file TodoList.cpp
 * @brief Implementation of the TodoList class.
 *
 * This file contains the implementation of the TodoList class,
 * which manages categorized task lists (Backlog, Doing, Review, Done).
 *
 * The class allows for task creation, reading, status updates, deletion,
 * and filtering by category or priority.
 *
 *
 * A helper function is also included to retrieve a task by ID from any category.
 *
 * @author Robin
 */


#include "TodoList.h"
#include <iostream>

/**
 * @brief Find a task by its ID across all task categories.
 *
 * @param taskID The ID of the task to find.
 * @return Pointer to the Task if found, otherwise nullptr.
 *
 * @author Robin
 */
Task* TodoList::findTask(int taskID)
{
    for (auto& task : backlog)
        if (task.getTaskID() == taskID) return &task;
    for (auto& task : doing)
        if (task.getTaskID() == taskID) return &task;
    for (auto& task : review)
        if (task.getTaskID() == taskID) return &task;
    for (auto& task : done)
        if (task.getTaskID() == taskID) return &task;

    return nullptr;
}

/**
 * @brief Create a task and add it to the backlog category.
 *
 * @param task The task to be added.
 */
void TodoList::createTask(const Task& task)
{
    backlog.push_back(task);
}

/**
 * @brief Display a task's details by its ID.
 *
 * Searches through all categories to find and display the task.
 *
 * @param taskID The ID of the task to display.
 */
void TodoList::readTask(int taskID) const
{
    const Task* task = nullptr;

    for (const auto& t : backlog)
        if (t.getTaskID() == taskID) task = &t;
    for (const auto& t : doing)
        if (t.getTaskID() == taskID) task = &t;
    for (const auto& t : review)
        if (t.getTaskID() == taskID) task = &t;
    for (const auto& t : done)
        if (t.getTaskID() == taskID) task = &t;

    if (task)
        task->displayTask();
    else
        std::cout << "Task not found.\n";
}

/**
 * @brief Change the status/category of a task.
 *
 * Moves the task to the specified category and updates its status.
 *
 * @param taskID The ID of the task to update.
 * @param newStatus The new status (category) of the task.
 * @return true if the task was updated successfully, false otherwise.
 */
bool TodoList::updateStatus(int taskID, const std::string& newStatus)
{
    Task* task = findTask(taskID);
    if (!task) return false;

    // Copy and update the task
    Task updatedTask = *task;
    updatedTask.editTaskStatus(newStatus);

    // Remove from current category
    deleteTask(taskID);

    // Add to new category
    if (newStatus == "Backlog") backlog.push_back(updatedTask);
    else if (newStatus == "Doing") doing.push_back(updatedTask);
    else if (newStatus == "Review") review.push_back(updatedTask);
    else if (newStatus == "Done") done.push_back(updatedTask);

    return true;
}

/**
 * @brief Delete a task by its ID from all categories.
 *
 * @param taskID The ID of the task to delete.
 * @return true if the task was found and deleted, false otherwise.
 */
bool TodoList::deleteTask(int taskID)
{
    auto removeTask = [&](std::vector<Task>& tasks)
    {
        auto it = std::remove_if(tasks.begin(), tasks.end(),
            [&](const Task& t) { return t.getTaskID() == taskID; });

        if (it != tasks.end())
        {
            tasks.erase(it, tasks.end());
            return true;
        }
        return false;
    };

    return removeTask(backlog) || removeTask(doing) || removeTask(review) || removeTask(done);
}

/**
 * @brief Get all tasks from a specific category.
 *
 * @param category The category name ("Backlog", "Doing", "Review", "Done").
 * @return A vector of tasks in the specified category.
 */
std::vector<Task> TodoList::filterTask(const std::string& category) const
{
    if (category == "Backlog") return backlog;
    else if (category == "Doing") return doing;
    else if (category == "Review") return review;
    else if (category == "Done") return done;
    return {};
}

/**
 * @brief Get all tasks that match a given priority.
 *
 * @param priority The priority to filter by (e.g., "High", "Medium", "Low").
 * @return A vector of tasks with the specified priority.
 */
std::vector<Task> TodoList::filterByPriority(const std::string& priority) const
{
    std::vector<Task> result;

    auto filter = [&](const std::vector<Task>& tasks)
    {
        for (const auto& t : tasks)
            if (t.getTaskPriority() == priority)
                result.push_back(t);
    };

    filter(backlog);
    filter(doing);
    filter(review);
    filter(done);

    return result;
}
