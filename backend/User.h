#ifndef USER_H
#define USER_H

#include <string>
#include <map>
#include <memory>

class Project;

/**
 * @class User
 * @brief Represents a user with associated profile information and projects.
 *
 * The User class manages user-related data such as ID, name, email, and password,
 * along with a collection of projects linked to the user. It provides functionality
 * to add or remove projects and retrieve user details.
 *
 * @author Robin
 */
class User {
private:
    int userID;    /**< Unique identifier for the user */
    std::string userName;    /**< The user's name */
    std::string userEmail;   /**< The user's email address */
    std::string userPassword; /**< The user's password */
    std::map<int, std::shared_ptr<Project>> userProjects; /**< Projects owned by the user, keyed by project ID */

public:
    /**
     * @brief Construct a new User object.
     *
     * @param userID The unique ID for the user.
     * @param userName The name of the user.
     * @param userEmail The email address of the user.
     * @param userPassword The password of the user.
     */
    User(int userID, const std::string& userName, const std::string& userEmail, const std::string& userPassword);

    /**
     * @brief Associate a project with the user.
     *
     * @param projectID The unique ID of the project.
     * @param project A shared pointer to the Project instance.
     */
    void addProject(int projectID, const std::shared_ptr<Project>& project);

    /**
     * @brief Remove a project from the user's list.
     *
     * @param projectID The ID of the project to be removed.
     */
    void removeProject(int projectID);

    /**
     * @brief Get the user's unique ID.
     *
     * @return The user's ID as an integer.
     */
    int getUserID() const;

    /**
     * @brief Get the user's name.
     *
     * @return The user's name as a string.
     */
    std::string getUserName() const;

    /**
     * @brief Get the user's email.
     *
     * @return The user's email as a string.
     */
    std::string getUserEmail() const;

    /**
     * @brief Get the user's password.
     *
     * @return The user's password as a string.
     */
    std::string getUserPassword() const;

    /**
     * @brief Get the user's associated projects.
     *
     * @return A map of project IDs to shared Project pointers.
     */
    std::map<int, std::shared_ptr<Project>> getUserProjects() const;
};

#endif // USER_H