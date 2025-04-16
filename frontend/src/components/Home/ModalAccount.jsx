import "./styles/ModalAccount.css";
import { useState } from "react";
import { useNavigate } from "react-router-dom";
import ButtonProjectSelect from "./ButtonProjectSelect";

/**
 * @file ModalAccount.jsx
 * @component ModalAccount
 * @brief A modal for displaying or managing account-related features (e.g., profile, settings).
 *
 * This modal appears over the screen and can be closed by clicking the background.
 * Currently, it renders an empty container but can be expanded to include account options.
 *
 * @param {Function} onClick - Callback to close the modal (triggered by background click).
 * @param {string} username - The current user's username (can be displayed later if needed).
 *
 * @returns {JSX.Element} A basic modal wrapper for account-related content.
 *
 * @author Andrew
 */
const ModalAccount = ({ onClick, username }) => {
  /**
   * @brief Handle click on background to close modal.
   */
  const backgroundClicked = () => {
    onClick();
  };

  return (
    <div className="modal-background" onClick={backgroundClicked}>
      <div
        className="container-home-modal"
        onClick={(e) => e.stopPropagation()}
      >
        {/* Account modal content can go here */}
      </div>
    </div>
  );
};

export default ModalAccount;
