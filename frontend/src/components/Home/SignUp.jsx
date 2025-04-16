import "./styles/Modal.css";
import { useState } from "react";

/**
 * @file SignUp.jsx
 * @component SignUp
 * @brief A modal component that handles user account registration.
 *
 * This component allows new users to sign up by entering a username, email, password,
 * and password confirmation. It checks for email uniqueness before submitting
 * the new user data via the `onSave` callback.
 *
 * @param {Function} onSave - Callback function called with user data when registration is valid.
 * @param {Function} onCancel - Callback function to close the modal without registering.
 *
 * @returns {JSX.Element} A modal interface for user signup.
 *
 * @author Luca
 */
const SignUp = ({ onSave, onCancel }) => {
  const [newUsername, setNewUsername] = useState("");
  const [newPassword, setNewPassword] = useState("");
  const [confirmPassword, setConfirmPassword] = useState("");
  const [email, setEmail] = useState("");
  const [error, setError] = useState("");
  const [isChecking, setIsChecking] = useState(false);

  /**
   * @brief Handle user input validation and signup request.
   */
  const handleSubmit = async () => {
    if (!newUsername || !newPassword || !email) {
      setError("Please fill in all fields");
      return;
    }

    if (newPassword !== confirmPassword) {
      setError("Passwords do not match");
      return;
    }

    setIsChecking(true);
    setError("");

    try {
      // Check if user with this email already exists
      const response = await fetch(
        `http://localhost:8080/users/email/${encodeURIComponent(email)}`,
        {
          method: "GET",
          headers: {
            "Content-Type": "application/json",
          },
        }
      );

      if (response.status === 200) {
        setError("An account with this email already exists");
        setIsChecking(false);
        return;
      } else if (response.status === 404) {
        // Email is available; proceed with account creation
        onSave({
          username: newUsername,
          password: newPassword,
          email: email,
        });
      } else {
        throw new Error("Error checking existing user");
      }
    } catch (error) {
      console.error("Error during signup: ", error);
      setError("An unexpected error occurred. Please try again.");
    } finally {
      setIsChecking(false);
    }
  };

  return (
    <div className="modal-backdrop">
      <div className="modal-content" onClick={(e) => e.stopPropagation()}>
        <h2>Create Account</h2>

        <input
          type="text"
          placeholder="Username"
          value={newUsername}
          onChange={(e) => setNewUsername(e.target.value)}
        />

        <input
          type="email"
          placeholder="Email"
          value={email}
          onChange={(e) => setEmail(e.target.value)}
        />

        <input
          type="password"
          placeholder="Password"
          value={newPassword}
          onChange={(e) => setNewPassword(e.target.value)}
        />

        <input
          type="password"
          placeholder="Confirm Password"
          value={confirmPassword}
          onChange={(e) => setConfirmPassword(e.target.value)}
        />

        {/* Error message */}
        {error && <p className="error-message">{error}</p>}

        <div className="button-container">
          <button onClick={handleSubmit} disabled={isChecking}>
            {isChecking ? "Checking..." : "Create Account"}
          </button>
          <button onClick={onCancel} disabled={isChecking}>
            Cancel
          </button>
        </div>
      </div>
    </div>
  );
};

export default SignUp;
