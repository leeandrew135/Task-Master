import "../../styles/Kanban.css";
import { useDroppable } from "@dnd-kit/core";
import DraggableTask from "./DraggableTask";

/**
 * @file KanbanColumn.jsx
 * @component KanbanColumn
 * @brief A droppable column in the Kanban board that holds draggable tasks.
 *
 * Each column represents a task status (e.g., Backlog, Doing, Review, Done),
 * and is configured as a droppable area using `@dnd-kit`. Tasks passed in
 * via props are rendered inside the column as draggable cards.
 *
 * @param {string} title - The title of the column (used as droppable ID).
 * @param {Array} tasks - An array of task objects to render in this column.
 *
 * @returns {JSX.Element} A styled droppable Kanban column with task items.
 *
 * @see DraggableTask
 * @author Ali Robin
 */
const KanbanColumn = ({ title, tasks }) => {
  const { setNodeRef } = useDroppable({
    id: title,
  });

  return (
    <div className="kanban-column" ref={setNodeRef} id={title}>
      <h2>{title}</h2>
      {tasks.map((task) => (
        <DraggableTask key={task.id} task={task} />
      ))}
    </div>
  );
};

export default KanbanColumn;
