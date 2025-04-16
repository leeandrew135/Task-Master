// components/Home/DraggableTask.js
import { useDraggable } from "@dnd-kit/core";
import { useState, useRef, useEffect } from "react";
/**
 * @file DraggableTask.js
 * @component DraggableTask
 * @brief A draggable Kanban card representing a task.
 *
 * This component uses `@dnd-kit/core` to make the task draggable.
 * It displays the task's title and visually updates when being dragged.
 *
 * @param {Object} task - The task object containing at least an `id` and `title`.
 *
 * @returns {JSX.Element} A draggable task card for use inside a Kanban column.
 *
 * @see useDraggable from @dnd-kit/core
 * @see KanbanColumn
 *
 * @example
 * <DraggableTask task={{ id: 1, title: "Fix login bug" }} />
 *
 * @author Robin
 */
const DraggableTask = ({ task }) => {
  const { attributes, listeners, setNodeRef, transform, isDragging } =
    useDraggable({
      id: task.id.toString(),
    });
  const [showTooltip, setShowTooltip] = useState(false);
  const cardRef = useRef(null);
  const [tooltipPosition, setTooltipPosition] = useState({ top: 0, left: 0 });

  const style = {
    transform: transform
      ? `translate(${transform.x}px, ${transform.y}px)`
      : undefined,
    opacity: isDragging ? 0.5 : 1,
  };

  useEffect(() => {
    if (showTooltip && cardRef.current) {
      const rect = cardRef.current.getBoundingClientRect();
      setTooltipPosition({
        top: rect.bottom + window.scrollY,
        left: rect.left + window.scrollX,
      });
    }
  }, [showTooltip]);

  const handleMouseEnter = () => {
    setShowTooltip(true);
  };

  const handleMouseLeave = () => {
    setShowTooltip(false);
  };

  return (
    <>
      <div
        ref={(node) => {
          setNodeRef(node);
          cardRef.current = node;
        }}
        style={style}
        {...listeners}
        {...attributes}
        className="kanban-card"
        onMouseEnter={handleMouseEnter}
        onMouseLeave={handleMouseLeave}
      >
        <h3>{task.title}</h3>
      </div>

      {showTooltip && !isDragging && (
        <div
          className="task-tooltip"
          style={{
            position: "absolute",
            top: `${tooltipPosition.top}px`,
            left: `${tooltipPosition.left}px`,
            backgroundColor: "white",
            border: "1px solid #ccc",
            borderRadius: "4px",
            padding: "10px",
            boxShadow: "0 2px 5px rgba(0,0,0,0.2)",
            zIndex: 1000,
            minWidth: "200px",
          }}
        >
          <h4>{task.title}</h4>
          <p>
            <strong>Description:</strong> {task.description || "No description"}
          </p>
          <p>
            <strong>Priority:</strong> {task.priority}
          </p>
          <p>
            <strong>Due Date:</strong>{" "}
            {task.due_date
              ? new Date(task.due_date).toLocaleDateString()
              : "No due date"}
          </p>
        </div>
      )}
    </>
  );
};

export default DraggableTask;
