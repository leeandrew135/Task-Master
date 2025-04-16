import React from "react";
import Kanban from "./Kanban"; // Adjust the path if needed
import { useParams } from "react-router-dom";

/**
 * @file PageProject.jsx
 * @component PageProject
 * @brief Renders the Kanban board for a specific project.
 *
 * This component retrieves the project ID from the route parameters
 * and passes control to the Kanban component for rendering project-specific content.
 *
 * Example route: /projects/:projectId
 *
 * @returns {JSX.Element} The Kanban board for the specified project.
 *
 * @author Andrew, Robin
 */
const PageProject = () => {
  const { projectId } = useParams();

  console.log("PageProject got projectId:", projectId);

  return <Kanban />;
};

export default PageProject;
