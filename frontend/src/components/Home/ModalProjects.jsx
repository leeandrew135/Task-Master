import "./styles/ModalProjects.css";
import "./styles/ButtonProjectSelect.css";
import { useState } from "react";
import { useNavigate } from "react-router-dom";
import ButtonProjectSelect from "./ButtonProjectSelect";

/**
 * @file ModalProjects.jsx
 * @component ModalProjects
 * @brief A modal that displays and manages user projects.
 *
 * This component allows users to:
 * - View a list of their projects
 * - Navigate to a project page
 * - Delete a project
 * - Create a new project and assign it to themselves
 *
 * The modal toggles between selection and creation modes.
 *
 * @param {Array} userProjects - Array of project objects associated with the user.
 * @param {Function} onClick - Callback triggered when the background is clicked (used to close modal).
 * @param {string} username - The name of the user.
 * @param {Function} refreshProjects - Function to refresh the project list after create/delete.
 * @param {Function} refreshTasks - Function to refresh the task list after deleting a project.
 *
 * @returns {JSX.Element} A full-screen modal UI for managing projects.
 *
 * @author Robin Andrew
 */
const ModalProjects = ({
  userProjects,
  onClick,
  username,
  refreshProjects,
  refreshTasks,
}) => {
  const navigate = useNavigate();

  /**
   * @brief Navigate to a different route.
   * @param {string} route - The target path to navigate to.
   */
  const changeRoute = (route) => {
    navigate(route);
  };

  /**
   * @brief Handle background click to close the modal.
   */
  const backgroundClicked = () => {
    onClick();
  };

  const [creatingNewProject, setCreatingNewProject] = useState(false);
  const [deadline, setDeadline] = useState("");
  const [date, setDate] = useState("");
  const [completed, setCompleted] = useState(false);
  const [selected, setSelected] = useState(false);

  /**
   * @brief Delete a project by its ID.
   * @param {number} projectId - The ID of the project to delete.
   */
  const deleteProject = async (projectId) => {
    const confirmDelete = window.confirm(
      "Are you sure you want to delete this project?"
    );
    if (!confirmDelete) return;

    try {
      const response = await fetch(
        `http://localhost:8080/projects/${projectId}`,
        {
          method: "DELETE",
        }
      );

      if (response.ok || response.status === 204) {
        alert("Project deleted.");
        refreshProjects();
        refreshTasks();
      } else {
        const errorText = await response.text();
        alert("Failed to delete project: " + errorText);
      }
    } catch (err) {
      console.error("Error deleting project:", err);
      alert("Network error occurred.");
    }
  };

  /**
   * @brief Create a new project and assign it to the current user.
   */
  const pushNewProject = async () => {
    const userData = JSON.parse(localStorage.getItem("userData"));
    if (!userData || !userData.id) {
      alert("User not found in local storage.");
      return;
    }

    try {
      const response = await fetch("http://localhost:8080/projects", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          deadline: deadline,
          date: date,
          completion_status: completed,
        }),
      });

      if (response.ok) {
        const createdProject = await response.json();

        const assignRes = await fetch("http://localhost:8080/user_projects", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify({
            user_id: userData.id,
            project_id: createdProject.id,
          }),
        });

        if (assignRes.ok) {
          alert("Project created and linked to your account!");
          setCreatingNewProject(false);
          refreshProjects();
        } else {
          const errorMsg = await assignRes.text();
          alert("Project created, but failed to assign user: " + errorMsg);
        }
      } else {
        const errorText = await response.text();
        alert("Failed to create project: " + errorText);
      }
    } catch (error) {
      console.error("Error creating or assigning project:", error);
      alert("Network error occurred.");
    }
  };

  return (
    <div className="modal-background" onClick={backgroundClicked}>
      <div
        className="container-home-modal"
        onClick={(e) => e.stopPropagation()}
      >
        {/* Modal Header */}
        <div className="container-project-modal">
          {creatingNewProject ? "Create New Project" : `${username} Projects`}
        </div>

        {/* Project List View */}
        {!creatingNewProject && (
          <div className="container-project-select">
            {userProjects.map((project, index) => (
              <div key={index} className="project-entry">
                <ButtonProjectSelect
                  className="button-project-select"
                  onClick={() => changeRoute(`/project/${project.id}`)}
                  projectName={project.id}
                />
                <button
                  className="button-delete-project"
                  onClick={() => deleteProject(project.id)}
                  title="Delete project"
                >
                  🗑️
                </button>
              </div>
            ))}
          </div>
        )}

        {/* Project Creation View */}
        {creatingNewProject && (
          <div className="container-project-select">
            <label className="field-labels" for="date">
              Date started:
            </label>
            <input
              type="date"
              className="input-new-project"
              placeholder="date"
              value={date}
              onChange={(e) => setDate(e.target.value)}
            />
            <label className="field-labels" for="date">
              Deadline:
            </label>
            <input
              type="date"
              className="input-new-project"
              placeholder="deadline"
              value={deadline}
              onChange={(e) => setDeadline(e.target.value)}
            />
            <div className="option-selection">
              <p>Is the project finished?</p>
              <div className="checkbox-group">
                <label className="checkbox-label">
                  <input
                    type="checkbox"
                    checked={completed}
                    onChange={() => setCompleted(!completed)}
                  />
                  Yes, it is finished
                </label>
              </div>
            </div>
            <button
              className="button-push-new-project"
              onClick={pushNewProject}
            >
              Create New Project
            </button>
          </div>
        )}

        {/* Toggle Button */}
        <button
          className="button-create-project"
          onClick={() => setCreatingNewProject(!creatingNewProject)}
        >
          {creatingNewProject ? "Select Project" : "Create New Project"}
        </button>
      </div>
    </div>
  );
};

export default ModalProjects;
