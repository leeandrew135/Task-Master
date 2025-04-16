import "./styles/Modal.css";
import { useState } from "react";

/**
 * @file Modal.jsx
 * @component Modal
 * @brief A modal form used to create a new task.
 *
 * This component allows the user to input a task title, description, priority, and deadline.
 * It converts the priority to a numeric value and passes all data to the `onSave` callback.
 * Clicking outside the modal or pressing "Cancel" closes the form via `onClose`.
 *
 * @param {Function} onClose - Function to close the modal.
 * @param {Function} onSave - Function called when the task is submitted. Receives title, description, priority (int), and date.
 *
 * @returns {JSX.Element} A task creation modal UI with form fields.
 *
 * @author Robin Ali
 */
const Modal = ({ onClose, onSave }) => {
  const [title, setTitle] = useState("");
  const [description, setDescription] = useState("");
  const [priority, setPriority] = useState("Low");
  const [due_date, setDate] = useState("");

  /**
   * @brief Handle save button click.
   * Converts the priority string to a numeric value and calls onSave with task data.
   */
  const handleSave = () => {
    const priorityMap = {
      High: 1,
      Medium: 2,
      Low: 3,
    };

    const numericPriority = priorityMap[priority];

    onSave(title, description, numericPriority, due_date);

    // Reset form
    setTitle("");
    setDescription("");
    setPriority("Low");
    setDate("");

    onClose();
  };

  return (
    <div className="modal-backdrop" onClick={onClose}>
      <div className="modal-content" onClick={(e) => e.stopPropagation()}>
        <h2>Add New Task</h2>
        <form onSubmit={(e) => e.preventDefault()}>
          <div>
            <label>Title:</label>
            <input
              type="text"
              value={title}
              onChange={(e) => setTitle(e.target.value)}
              required
            />
          </div>

          <div>
            <label>Description:</label>
            <textarea
              value={description}
              maxLength="2000"
              onChange={(e) => setDescription(e.target.value)}
            />
          </div>

          <div>
            <label>Priority:</label>
            <select
              value={priority}
              onChange={(e) => setPriority(e.target.value)}
            >
              <option value="Low">Low</option>
              <option value="Medium">Medium</option>
              <option value="High">High</option>
            </select>
          </div>

          <div>
            <label>Deadline:</label>
            <input
              type="date"
              value={due_date}
              onChange={(e) => setDate(e.target.value)}
              required
            />
          </div>

          <div className="modal-actions">
            <button type="button" onClick={handleSave}>
              Save Task
            </button>
            <button type="button" onClick={onClose}>
              Cancel
            </button>
          </div>
        </form>
      </div>
    </div>
  );
};

export default Modal;
