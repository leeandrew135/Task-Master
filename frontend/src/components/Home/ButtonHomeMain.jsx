import React from "react";
import { Link } from "react-router-dom";

/**
 * @file ButtonHomeMain.jsx
 * @component ButtonHomeMain
 * @brief A button that navigates to the home page.
 *
 * This component renders a styled button that links to the root route ("/").
 * It can be used in navigation bars, menus, or anywhere a "Home" button is needed.
 *
 * @returns {JSX.Element} A React Router <Link> styled as a primary button.
 *
 * @example
 * <ButtonHomeMain />
 *
 * @author Andrew
 */
const ButtonHomeMain = () => {
  return (
    <Link to="/" className="btn btn-primary">
      Home
    </Link>
  );
};

export default ButtonHomeMain;
