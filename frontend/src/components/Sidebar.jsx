import "../styles/Sidebar.css";
import { useState } from "react";
import { useNavigate } from "react-router-dom";

/**
 * @file Sidebar.jsx
 * @component Sidebar
 * @brief A collapsible sidebar menu for navigation and logout.
 *
 * This component overlays a sidebar with navigation buttons:
 * - Home: Navigates to /home
 * - Calendar: Navigates to /calendar
 * - Sign Out: Clears user data and returns to login screen
 *
 * Clicking the dark background triggers `onClick` to close the sidebar.
 *
 * @param {Function} onClick - Function called when the background is clicked (used to close the sidebar).
 *
 * @returns {JSX.Element} A modal-style sidebar for in-app navigation.
 *
 * @example
 * <Sidebar onClick={() => setSidebarOpen(false)} />
 *
 * @author Andrew
 */
const Sidebar = ({ onClick }) => {
  const navigate = useNavigate();

  /**
   * @brief Change route using React Router.
   * @param {string} route - Target route to navigate to.
   */
  const changeRoute = (route) => {
    navigate(route);
  };

  /**
   * @brief Handle background click to close the sidebar.
   */
  const backgroundClicked = () => {
    onClick();
  };

  return (
    <div>
      <div className="sidebar-background" onClick={backgroundClicked}>
        <div className="container-sidebar" onClick={(e) => e.stopPropagation()}>
          <button
            className="sidebar-button-nav"
            onClick={() => changeRoute("/home")}
          >
            Home
          </button>

          <button
            className="sidebar-button-nav"
            onClick={() => changeRoute("/calendar")}
          >
            Calendar
          </button>

          <button
            className="sidebar-button-nav"
            onClick={() => {
              localStorage.removeItem("userData");
              navigate("/");
            }}
          >
            Sign Out
          </button>

          {/* <button className="sidebar-button-nav" onClick={() => changeRoute("/project")}>Projects</button> */}
        </div>
      </div>
    </div>
  );
};

export default Sidebar;
