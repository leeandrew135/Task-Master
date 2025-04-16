/**
 * @file Comment.h
 * @brief Declaration of the Comment class.
 *
 * This file defines the Comment class, but we didn't use it as long as I know
 *
 * @author Robin
 */

#ifndef COMMENT_H
#define COMMENT_H

#include <string>

/**
 * @class Comment
 * did not get used so fine to skip. I did add commnents just in case, but it is not used at all
 */
class Comment {
private:
    int commentID;               /**< Unique identifier for the comment */
    std::string commentBody;     /**< Text content of the comment */
    std::string commentDate;     /**< Date the comment was created */
    std::string commentStatus;   /**< Status of the comment (e.g., "active", "deleted") */
    int commentUser;             /**< ID of the user who posted the comment */

public:
    /**
     * @brief Constructs a Comment object.
     *
     * Initializes the comment with an ID, body, date, status, and user ID.
     *
     * @param commentID ID of the comment.
     * @param commentBody Text content of the comment.
     * @param commentDate Date the comment was posted.
     * @param commentStatus Status of the comment.
     * @param commentUser ID of the user who made the comment.
     */
    Comment(int commentID, const std::string& commentBody, const std::string& commentDate,
            const std::string& commentStatus, int commentUser);

    /**
     * @brief Get the ID of the comment.
     * @return The comment ID.
     */
    int getCommentID() const;

    /**
     * @brief Get the content/body of the comment.
     * @return The comment body as a string.
     */
    std::string getCommentBody() const;

    /**
     * @brief Get the date of the comment.
     * @return The comment date as a string.
     */
    std::string getCommentDate() const;

    /**
     * @brief Get the current status of the comment.
     * @return The comment status as a string.
     */
    std::string getCommentStatus() const;

    /**
     * @brief Get the user ID of the comment's author.
     * @return The user ID.
     */
    int getCommentUser() const;

    /**
     * @brief Edit the comment body.
     * @param newBody The new text to replace the current body.
     */
    void editCommentBody(const std::string& newBody);

    /**
     * @brief Edit the comment date.
     * @param newDate The new date string.
     */
    void editCommentDate(const std::string& newDate);

    /**
     * @brief Edit the comment status.
     * @param newStatus The new status string (e.g., "active", "deleted").
     */
    void editCommentStatus(const std::string& newStatus);

    /**
     * @brief Perform a soft delete on the comment.
     *
     * Marks the comment as deleted by changing its status and replacing
     * the body text with a placeholder message.
     */
    void deleteComment();
};

#endif // COMMENT_H