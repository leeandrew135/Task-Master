#ifndef TODOLIST_H
#define TODOLIST_H

#include <vector>
#include <string>
#include "Task.h"

/**
 * @class TodoList
 * @brief Manages a categorized collection of tasks using a Kanban-style system.
 *
 * The TodoList class organizes tasks into four categories: backlog, doing, review, and done.
 * It supports creating, reading, updating, deleting, and filtering tasks based on category or priority.
 * Internally, each category is stored as a separate vector of Task objects.
 *
 * This class uses helper methods for task lookup and supports status transitions.
 *
 * @author Robin
 */
class TodoList {
private:
    std::vector<Task> backlog;  /**< Tasks that are not yet started */
    std::vector<Task> doing;    /**< Tasks currently in progress */
    std::vector<Task> review;   /**< Tasks awaiting review */
    std::vector<Task> done;     /**< Completed tasks */

    /**
     * @brief Find a task by ID across all categories.
     *
     * @param taskID The unique identifier of the task to find.
     * @return A pointer to the Task if found, nullptr otherwise.
     */
    Task* findTask(int taskID);

public:
    /**
     * @brief Default constructor.
     */
    TodoList() = default;

    /**
     * @brief Default destructor.
     */
    ~TodoList() = default;

    /**
     * @brief Add a new task to the backlog.
     *
     * @param task The task to add.
     */
    void createTask(const Task& task);

    /**
     * @brief Display the task with the given ID.
     *
     * @param taskID The ID of the task to read.
     */
    void readTask(int taskID) const;

    /**
     * @brief Update the status/category of a task.
     *
     * Moves the task to a different internal category vector.
     *
     * @param taskID The ID of the task to update.
     * @param newStatus The new status/category to move the task to.
     * @return true if the task was updated successfully, false if not found.
     */
    bool updateStatus(int taskID, const std::string& newStatus);

    /**
     * @brief Delete a task by ID from any category.
     *
     * @param taskID The ID of the task to delete.
     * @return true if the task was deleted successfully, false if not found.
     */
    bool deleteTask(int taskID);

    /**
     * @brief Filter tasks by category.
     *
     * @param category One of "backlog", "doing", "review", or "done".
     * @return A vector of tasks that belong to the specified category.
     */
    std::vector<Task> filterTask(const std::string& category) const;

    /**
     * @brief Filter tasks by priority.
     *
     * @param priority The priority level to filter by.
     * @return A vector of tasks that match the given priority.
     */
    std::vector<Task> filterByPriority(const std::string& priority) const;
};

#endif // TODOLIST_H
