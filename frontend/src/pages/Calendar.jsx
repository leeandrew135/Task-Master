import "../styles/Calendar.css";
import { useState, useEffect } from "react";
import { useNavigate } from "react-router-dom";

/**
 * @file Calendar.jsx
 * @component Calendar
 * @brief A calendar component that displays the current month with user tasks on each day.
 *
 * This component fetches a user's tasks from the backend and displays them inside
 * a calendar layout. Tasks are grouped by due dates, and the calendar includes
 * navigation for previous and next months.
 *
 * @returns {JSX.Element} A full interactive calendar with tasks displayed on appropriate days.
 *
 * @author Ali Robin
 */
const Calendar = () => {
  const [currentDate, setCurrentDate] = useState(new Date());
  const [tasks, setTasks] = useState([]);

  // Get logged-in user info from localStorage
  const userData = JSON.parse(localStorage.getItem("userData"));

  const navigate = useNavigate();

  // Extract current year/month info
  const year = currentDate.getFullYear();
  const month = currentDate.getMonth();

  // Determine first weekday and total days in current month
  const firstDayOfMonth = new Date(year, month, 1);
  const startingDay = firstDayOfMonth.getDay();
  const daysInMonth = new Date(year, month + 1, 0).getDate();

  /**
   * @brief Navigate to the previous month.
   */
  const handlePrevMonth = () => setCurrentDate(new Date(year, month - 1, 1));

  /**
   * @brief Navigate to the next month.
   */
  const handleNextMonth = () => setCurrentDate(new Date(year, month + 1, 1));

  /**
   * @brief Fetch user tasks from backend and store them in state.
   *
   * This fetches tasks associated with the current user using their ID from localStorage.
   */
  const fetchTasks = async () => {
    try {
      const res = await fetch(
        `http://localhost:8080/users/${userData.id}/tasks`
      );
      if (res.ok) {
        const data = await res.json();
        setTasks(data);
      } else {
        console.error("Failed to fetch tasks");
      }
    } catch (err) {
      console.error("Error fetching tasks:", err);
    }
  };

  // Fetch tasks when component first mounts
  useEffect(() => {
    fetchTasks();
  }, []);

  /**
   * @brief Get all tasks due on a specific day.
   *
   * @param {number} day - Day of the month (1–31)
   * @returns {Array} An array of tasks due on the given day
   */
  const getTasksForDay = (day) => {
    const monthStr = String(month + 1).padStart(2, "0");
    const dayStr = String(day).padStart(2, "0");
    const dateStr = `${year}-${monthStr}-${dayStr}`;
    return tasks.filter((task) => task.due_date?.startsWith(dateStr));
  };

  // Build array of day cells for rendering
  let calendarDays = [];

  // Add blank cells before the first day of the month
  for (let i = 0; i < startingDay; i++) {
    calendarDays.push({ day: "", tasks: [] });
  }

  // Add actual days with tasks
  for (let day = 1; day <= daysInMonth; day++) {
    calendarDays.push({ day, tasks: getTasksForDay(day) });
  }

  // Pad end of calendar to make it 6 full weeks (7 * 6 = 42 cells)
  while (calendarDays.length < 42) {
    calendarDays.push({ day: "", tasks: [] });
  }

  return (
    <section className="Calendar">
      {/* Back Button */}
      <div className="calendar-back-button-container">
        <button
          className="calendar-back-button"
          onClick={() => navigate("/home")}
        >
          ⬅ Back
        </button>
      </div>

      {/* Header */}
      <div className="calendar-header">
        <button onClick={handlePrevMonth}>&lt;</button>
        <h2>
          {currentDate.toLocaleString("default", { month: "long" })} {year}
        </h2>
        <button onClick={handleNextMonth}>&gt;</button>
      </div>

      <div className="day-labels">
        {["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"].map((label) => (
          <div key={label}>{label}</div>
        ))}
      </div>

      <div className="calendar-grid">
        {calendarDays.map(({ day, tasks }, index) => (
          <div key={index} className={`day-cell ${day === "" ? "empty" : ""}`}>
            {day && <div className="date-number">{day}</div>}
            {tasks.map((task) => (
              <div key={task.id} className="task-name">
                {task.title}
              </div>
            ))}
          </div>
        ))}
      </div>
    </section>
  );
};

export default Calendar;
