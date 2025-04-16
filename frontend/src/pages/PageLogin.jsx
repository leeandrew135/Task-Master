/**
 * @file PageLogin.jsx
 * @brief Page component to display the login screen.
 *
 * This component is a page that is rendered based on the route.
 * The login is the default route and is the first screen shown on proejct load.
 *
 * @author Andrew Lee
 * @author Robin Lee
 * @author Luca Duarte
 */

import { React, useState } from "react";
import { useNavigate, Link } from "react-router-dom";
import "../styles/StylesPageLogin.css";
import SignUp from "../components/Home/SignUp";

const PageLogin = () => {
  // STATE VARIABLES
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [failedLogin, setFailedLogin] = useState("");
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [isLoading, setIsLoading] = useState(false);

  /// ROUTING FUNCTIONS
  const navigate = useNavigate();
  /**
   * @brief Passes a parameter to the navigator, changing the route.
   *
   * This function takes the specified string parameter,
   * and passes it to the navigator to change the current route.
   *
   * @param {*} route
   */
  const changeRoute = (route) => {
    navigate(route);
  };

  /**
   * @brief Verifies the attempted login.
   *
   * This function checks whether the username and password given in the input fields,
   * exists as a user in the database through an API call.
   * It is called on the login screen when the button is pressed.
   *
   * While the function does not have parameters or a return,
   * it uses the state variables of the component for the API call,
   * and depending on the response, either the route changes, or there is a login failed message.
   */
  const verifyLogin = async () => {
    /// Check for empty input fields
    if (!username || !password) {
      setFailedLogin("Please enter both username and password");
      setTimeout(() => setFailedLogin(""), 2000);
      return;
    }

    setIsLoading(true);

    /// Call the API and process response for login.
    try {
      /// API call.
      const response = await fetch("http://localhost:8080/auth/login", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          login: username,
          password: password,
        }),
      });

      /// If login valid, localStorage the data, and change route
      if (response.status === 200) {
        const userData = await response.json();
        localStorage.setItem("userData", JSON.stringify(userData));
        changeRoute("/home");
      } else if (response.status === 401) {
        setFailedLogin("Invalid username/email or password");
        setPassword("");
      } else if (response.status === 400) {
        setFailedLogin("Invalid request format");
      } else {
        setFailedLogin("Login failed. Please try again.");
      }
    } catch (error) {
      console.error("Login error: ", error);
      setFailedLogin("Network error. Please try again later.");
    } finally {
      setIsLoading(false);

      if (failedLogin) {
        setTimeout(() => {
          setFailedLogin("");
        }, 2000);
      }
    }
  };

  /**
   * @brief Take userData, and attempt to create account with API call.
   *
   * This function takes packaged userData as a parameter,
   * and formats the parameters to send to the API, to add a new account.
   *
   * While there is no return, a message will be displayed,
   * depending on the contents of the response.
   *
   * @param {*} userData
   */
  const handleSignUp = async (userData) => {
    try {
      /// API Call.
      const response = await fetch("http://localhost:8080/users", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          name: userData.username,
          email: userData.email,
          password: userData.password,
        }),
      });

      /// Store the response data.
      let data;
      try {
        data = await response.json();
      } catch (e) {
        console.error("Failed to parse response:", e);
        setFailedLogin("Server error. Please try again.");
        setTimeout(() => setFailedLogin(""), 3000);
        return;
      }

      /// If valid response, display account created message.
      if (response.ok) {
        setIsModalOpen(false);
        setFailedLogin("Account created successfully! Please login.");
        setTimeout(() => setFailedLogin(""), 3000);
      } else {
        alert(data.message || "Signup failed. Please try again.");
      }
    } catch (error) {
      console.error("Signup error: ", error);
      alert("Network error. Please try again later.");
    }
  };

  // DEFINE COMPONENT HERE
  return (
    <section className="Login">
      <div className="container-background-login">
        {/* HEADER ICON */}
        <div className="container-icon">Task Master</div>

        {/* CONTAINER FOR LOGIN */}
        <div className="container-input">
          {/* USERNAME/EMAIL FIELD */}
          <input
            type="text"
            className="input-field"
            placeholder="Username or Email"
            value={username}
            onChange={(e) => setUsername(e.target.value)} // UPDATE ON EVENT
            onKeyPress={(e) => e.key === "Enter" && verifyLogin()}
          />

          {/* PASSWORD FIELD */}
          <input
            type="password"
            className="input-field"
            placeholder="Password"
            value={password}
            onChange={(e) => setPassword(e.target.value)} // UPDATE ON EVENT
          />

          {/* LOGIN BUTTON */}
          <button className="button-login-submit" onClick={verifyLogin}>
            Login
          </button>

          {/* SIGN UP MESSAGE */}
          <p>
            Not signed up yet?
            <span onClick={() => setIsModalOpen(true)}> Sign up here!</span>
          </p>
        </div>

        {/* CONDITIONAL LOGIN FAIL MESSAGE */}
        <text className="message-login-fail">{failedLogin}</text>
      </div>

      {/* SIGN UP MODAL */}
      {isModalOpen && (
        <SignUp onSave={handleSignUp} onCancel={() => setIsModalOpen(false)} />
      )}
    </section>
  );
};

export default PageLogin;
