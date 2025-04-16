/**
 * @file PageHome.jsx
 * @brief Page component to display everything on the home screen.
 *
 * This component is a page that is rendered based on the route.
 * It is accessed by the login, and this screen can be used to access
 * all other screens.
 *
 * @author Andrew Lee
 * @author Robin Lee
 * @author Luca Duarte
 * @author Ali Farhangi
 */

import React, { useState, useEffect } from "react";
import { Link, useLocation, useNavigate } from "react-router-dom";
import "../styles/StylesPageHome.css";
import { TaskHome, ModalProjects, ModalAccount } from "../components/Home";
import Sidebar from "../components/Sidebar.jsx";

/**
 * @component PageHome
 * @brief Displays the home screen with user tasks, filters, projects, and profile access.
 *
 * @returns {JSX.Element} The rendered homepage.
 */
const PageHome = () => {
  const userData = JSON.parse(localStorage.getItem("userData"));
  const username = userData?.name || "User";
  const navigate = useNavigate();

  const [isModalOpen, setIsModalOpen] = useState(false);
  const [isAccountModalOpen, setIsAccountModalOpen] = useState(false);
  const [isSidebarOpen, setIsSidebarOpen] = useState(false);
  const [allTasks, setAllTasks] = useState([]);
  const [filteredTasks, setFilteredTasks] = useState([]);
  const [filterStatus, setFilterStatus] = useState("All");
  const [userProjects, setUserProjects] = useState([]);

  // Maps display status to DB status values
  const statusMap = {
    All: "all",
    Backlog: "backlog",
    "In Progress": "inProgress",
    Completed: "completed",
  };

  /**
   * @brief Get all tasks from database by API call.
   *
   * This function gets all tasks for the user by the database,
   * then calls the methods to update the state variables.
   * The filteredTasks is initially set to all tasks,
   * and is changed based on the filter buttons being clicked.
   *
   * While there are no params and no return,
   * the param for the API call is given by the localStorage user info,
   * and instead of a return we display the tasks on the right side of the screen.
   */
  const fetchTasks = async () => {
    try {
      /// API call.
      const response = await fetch(
        `http://localhost:8080/users/${userData.id}/tasks`
      );

      /// If valid response, set the state variables.
      if (response.ok) {
        const data = await response.json();
        setAllTasks(data);
        setFilteredTasks(data); /// Initially show all tasks.
      } else {
        console.error("Failed to fetch tasks");
      }
    } catch (error) {
      console.error("Error fetching tasks:", error);
    }
  };

  /**
   * @brief Get all projects form database by API call.
   *
   * This function gets all projects for the user in the database,
   * then calls the method to update the state variable.
   *
   * While the function itself does not have param or return,
   * it uses the userData variable of the class, and then displays
   * the projects when the project modal is opened, depending on the response.
   */
  const fetchProjects = async () => {
    try {
      /// API call.
      const res = await fetch(
        `http://localhost:8080/users/${userData.id}/projects`
      );

      /// If valid response, set the state variable.
      if (res.ok) {
        const data = await res.json();
        setUserProjects(data);
      } else {
        console.error("Failed to fetch projects");
      }
    } catch (err) {
      console.error("Error fetching projects:", err);
    }
  };

  /**
   * @brief Function to set the displayed tasks on the home screen.
   *
   * When the filter button is clicked, we pass the status param
   * to display only the corresponding tasks on the home screen.
   * If the 'all' button is clicked, then we just set the filtered tasks
   * to be the same as all tasks.
   *
   * While there is no return, the displayed tasks change according
   * to the button that is pressed.
   *
   * @param {*} status
   */
  const applyFilter = (status) => {
    setFilterStatus(status);
    const dbStatus = statusMap[status];

    /// If filter is "all", then set as all tasks.
    if (dbStatus === "all") {
      setFilteredTasks(allTasks);
    }
    /// Otherwise, set as the specific status.
    else {
      setFilteredTasks(allTasks.filter((task) => task.status === dbStatus));
    }
  };

  const displayedTasks = filteredTasks.slice(0, 5);

  /**
   * @brief Automatically calls helper functions on component mount.
   *
   * The functions below are automatically called on component mount,
   * as it gets all of the tasks and users, and automatically sets them in the state variables.
   */
  useEffect(() => {
    fetchTasks();
    fetchProjects(); // Added this line
  }, []);

  // DEFINE COMPONENT HERE
  return (
    <div className="container-background">
      {/* HEADER BAR */}
      <div className="container-header">
        <div className="container-header-left">
          {/* SIDEBAR BUTTON */}
          <button
            className="button-sidebar button-logic"
            onClick={() => setIsSidebarOpen(true)}
          >
            ≡
          </button>

          {/* HEADER MESSAGE */}
          <div className="container-message">
            <text className="text-home-header">Task Master</text>
            <text className="text-home-subheader">Welcome {username}!</text>
          </div>
        </div>

        <div className="container-header-right">
          <text className="text-header-tasks">Tasks</text>

          {/* FILTER BUTTONS FOR TASKS */}
          <div className="container-filter">
            {["All", "Backlog", "In Progress", "Completed"].map((status) => (
              <button
                key={status}
                className={`button-filter button-logic ${
                  filterStatus === status ? "active" : ""
                }`}
                onClick={() => applyFilter(status)}
              >
                {status}
              </button>
            ))}
          </div>
        </div>
      </div>

      {/* PAGE BODY */}
      <div className="container-body">
        <div className="container-body-left">
          {/* PROJECT BUTTON */}
          <button
            className="button-home-main button-logic"
            onClick={() => setIsModalOpen(true)}
          >
            Projects
          </button>

          {/* CALENDAR BUTTON */}
          <button
            className="button-home-main button-logic"
            onClick={() => navigate("/calendar")}
          >
            Calendar
          </button>
        </div>

        <div className="container-body-right">
          {filteredTasks.length === 0 ? (
            <p>No tasks to display.</p>
          ) : (
            displayedTasks.map((task) => (
              <TaskHome
                key={task.id}
                taskName={task.title}
                taskDesc={task.description}
                dueDate={task.due_date}
                projectName={task.project_id}
              />
            ))
          )}
        </div>
      </div>

      {/* MODALS */}
      {/* PROJECT MODAL */}
      {isModalOpen && (
        <ModalProjects
          userProjects={userProjects}
          username={username}
          onClick={() => setIsModalOpen(false)}
          refreshProjects={fetchProjects}
          refreshTasks={fetchTasks}
        />
      )}

      {/* ACCOUNT MODAL */}
      {isAccountModalOpen && (
        <ModalAccount onClick={() => setIsAccountModalOpen(false)} />
      )}

      {/* SIDEBAR MODAL */}
      {isSidebarOpen && <Sidebar onClick={() => setIsSidebarOpen(false)} />}
    </div>
  );
};

export default PageHome;
