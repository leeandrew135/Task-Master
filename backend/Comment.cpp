/**
 * @file Comment.cpp
 * @brief Implementation of the Comment class.
 *
 * This file contains the method definitions for the Comment class which
 * should have represented a user comment in the system. we just used strings so this never really got used
 * I was thinking of deleting it, but I was too scared of possible errors
 * @author Robin
 */

#include "Comment.h"

/**
 * @brief Constructs a Comment object.
 *
 * Initializes a Comment with an ID, body, date, status, and the user who made it.
 *
 * @param commentID ID of the comment.
 * @param commentBody Text content of the comment.
 * @param commentDate Date the comment was posted.
 * @param commentStatus Current status of the comment (e.g., active, deleted).
 * @param commentUser ID of the user who posted the comment.
 */
Comment::Comment(int commentID, const std::string& commentBody, const std::string& commentDate,
                 const std::string& commentStatus, int commentUser)
    : commentID(commentID), commentBody(commentBody), commentDate(commentDate),
      commentStatus(commentStatus), commentUser(commentUser) {}

// ========================
// Getters
// ========================

/**
 * @brief Get the ID of the comment.
 *
 * @return int The comment ID.
 */
int Comment::getCommentID() const { return commentID; }

/**
 * @brief Get the body text of the comment.
 *
 * @return std::string The content of the comment.
 */
std::string Comment::getCommentBody() const { return commentBody; }

/**
 * @brief Get the date the comment was posted.
 *
 * @return std::string The date of the comment.
 */
std::string Comment::getCommentDate() const { return commentDate; }

/**
 * @brief Get the status of the comment.
 *
 * @return std::string The status (e.g., "active", "deleted").
 */
std::string Comment::getCommentStatus() const { return commentStatus; }

/**
 * @brief Get the user ID associated with the comment.
 *
 * @return int The user ID.
 */
int Comment::getCommentUser() const { return commentUser; }

// ========================
// Setters
// ========================

/**
 * @brief Edit the content of the comment.
 *
 * @param newBody The new body text for the comment.
 */
void Comment::editCommentBody(const std::string& newBody) {
    commentBody = newBody;
}

/**
 * @brief Edit the date of the comment.
 *
 * @param newDate The new date for the comment.
 */
void Comment::editCommentDate(const std::string& newDate) {
    commentDate = newDate;
}

/**
 * @brief Edit the status of the comment.
 *
 * @param newStatus The new status (e.g., "active", "deleted").
 */
void Comment::editCommentStatus(const std::string& newStatus) {
    commentStatus = newStatus;
}

// ========================
// Soft Delete
// ========================

/**
 * @brief Marks the comment as deleted.
 *
 * Performs a "soft delete" by setting the comment status to "deleted"
 * and replacing the body content with a placeholder.
 */
void Comment::deleteComment() {
    commentStatus = "deleted";
    commentBody = "[Deleted]";
}