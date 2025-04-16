import React from "react";
import "./styles/ButtonProjectSelect.css";

/**
 * @file ButtonProjectSelect.jsx
 * @component ButtonProjectSelect
 * @brief A clickable UI element used to select a project by name.
 *
 * This component displays the project name and triggers an action when clicked.
 * It is typically used in a list of projects, such as inside a project selection modal.
 *
 * @param {string} projectName - The name/title of the project to display.
 * @param {Function} onClick - Callback triggered when the project button is clicked.
 *
 * @returns {JSX.Element} A styled, clickable project selection box.
 *
 * @example
 * <ButtonProjectSelect projectName="Marketing Website" onClick={() => handleSelect(id)} />
 *
 * @author Andrew
 */
const ButtonProjectSelect = ({ projectName, onClick }) => {
  return (
    <div className="container-button-project-select" onClick={onClick}>
      <span className="component-project-select-title">{projectName}</span>
    </div>
  );
};

export default ButtonProjectSelect;
