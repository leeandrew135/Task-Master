import React from "react";
import { Link } from "react-router-dom";
import "./styles/StylesTaskHome.css";

/**
 * @file TaskHome.jsx
 * @component TaskHome
 * @brief A visual component that displays a task's name and description with an action button.
 *
 * This component is used in the home view or dashboard to preview a task’s title and description.
 * The `• • •` button can be hooked to show more details, options, or actions (e.g., edit, delete).
 *
 * @param {string} taskName - The title of the task.
 * @param {string} taskDesc - A short description of the task.
 *
 * @returns {JSX.Element} A stylized box displaying the task with a detail button.
 *
 * @author Andrew
 */
const TaskHome = ({ taskName, taskDesc, dueDate, projectName }) => {
  return (
    <div className="container-component-task">
      {/* TEXT SECTION */}
      <div className="component-task-text">
        <div>
          <text className="component-task-date">Due by {dueDate}</text>
        </div>
        <div>
          <text className="component-task-title">
            {taskName} | Project: {projectName}
          </text>
        </div>
        <div>
          <text className="component-task-desc">{taskDesc}</text>
        </div>
      </div>
    </div>
  );
};

export default TaskHome;
