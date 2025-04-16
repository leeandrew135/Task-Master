/**
 * @file Kanban.jsx
 * @brief React Kanban board for managing tasks within a project.
 *
 * This component renders a dynamic Kanban board using dnd-kit for drag-and-drop,
 * allows adding tasks and users to a project, and provides sorting/filtering options.
 *
 * @component Kanban
 * @author Ali Robin
 */

import "../styles/Kanban.css";
import { useState, useEffect } from "react";
import Modal from "../components/Home/Modal";
import KanbanColumn from "../components/Home/KanbanColumn";
import { useParams } from "react-router-dom";
import {
    DndContext,
    closestCorners,
    PointerSensor,
    useSensor,
    useSensors,
    DragOverlay,
    useDroppable
} from '@dnd-kit/core';

/**
 * @component Kanban
 * @brief Main Kanban board UI with drag-and-drop task management.
 *
 * @returns {JSX.Element} The rendered Kanban board.
 */
const Kanban = () => {
    const { projectId } = useParams();

    const [tasks, setTasks] = useState([]);
    const [isModalOpen, setIsModalOpen] = useState(false);
    const [activeTask, setActiveTask] = useState(null);
    const [users, setUsers] = useState([]);
    const [selectedUserId, setSelectedUserId] = useState("");

    // Fetch users for project association
    useEffect(() => {
        fetch("http://localhost:8080/users")
            .then(res => res.json())
            .then(data => setUsers(data))
            .catch(err => console.error("Failed to fetch users", err));
    }, []);

    // Fetch tasks associated with the project
    useEffect(() => {
        if (!projectId) return;

        fetch(`http://localhost:8080/tasks?project_id=${projectId}`)
            .then(res => res.json())
            .then(data => setTasks(data))
            .catch(err => console.error("Failed to fetch tasks", err));
    }, [projectId]);

    /**
     * @function handleSortByPriority
     * @brief Sorts tasks by ascending priority (lower = higher).
     */
    const handleSortByPriority = () => {
        const sorted = [...tasks].sort((a, b) => a.priority - b.priority);
        setTasks(sorted);
    };

    /**
     * @function handleSortByDueDate
     * @brief Sorts tasks by due date (earliest first).
     */
    const handleSortByDueDate = () => {
        const sorted = [...tasks].sort((a, b) => new Date(a.due_date) - new Date(b.due_date));
        setTasks(sorted);
    };

    /**
     * @function handleAddUserToProject
     * @brief Sends POST request to associate a user with the current project.
     */
    const handleAddUserToProject = async () => {
        if (!selectedUserId || !projectId) return;

        try {
            const res = await fetch("http://localhost:8080/user_projects", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({
                    user_id: parseInt(selectedUserId),
                    project_id: parseInt(projectId)
                })
            });

            if (res.ok) {
                alert("User added to project!");
            } else {
                const errMsg = await res.text();
                alert("Failed to add user: " + errMsg);
            }
        } catch (err) {
            console.error(err);
            alert("Error adding user to project");
        }
    };

    /**
     * @function addTask
     * @brief Adds a new task to the project via a POST request.
     *
     * @param {string} title Task title
     * @param {string} description Task description
     * @param {number} priority Task priority (lower is higher)
     * @param {string} due_date Due date string
     */
    const addTask = async (title, description, priority, due_date) => {
        try {
            const response = await fetch("http://localhost:8080/tasks", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify({
                    title,
                    description,
                    priority,
                    due_date,
                    status: "backlog",
                    project_id: parseInt(projectId)
                }),
            });

            if (response.status === 201) {
                const newTask = await response.json();
                setTasks(prev => [...prev, newTask]);
            } else if (response.status === 204) {
                const refreshed = await fetch(`http://localhost:8080/tasks?project_id=${projectId}`);
                const data = await refreshed.json();
                setTasks(data);
            } else {
                alert("Failed to create task");
            }
        } catch (error) {
            console.error("Error creating task: ", error);
            alert("An error occurred while creating the task.");
        }
    };

    const sensors = useSensors(useSensor(PointerSensor));

    /**
     * @function handleDragStart
     * @brief Stores the task being dragged.
     *
     * @param {object} event dnd-kit drag start event
     */
    const handleDragStart = (event) => {
        const { active } = event;
        const draggedTask = tasks.find(t => t.id.toString() === active.id);
        setActiveTask(draggedTask);
    };

    /**
     * @function handleDragEnd
     * @brief Handles moving or deleting tasks after drag-and-drop.
     *
     * @param {object} event dnd-kit drag end event
     */
    const handleDragEnd = async (event) => {
        const { active, over } = event;
        if (!over) return;

        const taskId = active.id.toString();
        const newStatus = over.id;

        if (newStatus === 'delete') {
            await fetch(`http://localhost:8080/tasks/${taskId}`, { method: "DELETE" });
            setTasks(tasks.filter(task => task.id.toString() !== taskId));
        } else {
            const updatedTasks = tasks.map(task =>
                task.id.toString() === taskId ? { ...task, status: newStatus } : task
            );
            setTasks(updatedTasks);
            await fetch(`http://localhost:8080/tasks/${taskId}`, {
                method: "PUT",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ status: newStatus })
            });
        }
        setActiveTask(null);
    };

    /**
     * @component DeleteZone
     * @brief A droppable area for removing tasks.
     *
     * @returns {JSX.Element} The delete zone column.
     */
    const DeleteZone = () => {
        const { setNodeRef } = useDroppable({
            id: "delete"
        });

        return (
            <div ref={setNodeRef} className="kanban-column delete-zone">
                <h2>Delete</h2>
            </div>
        );
    };

    return (
        <section className="Kanban">
            <div className="kanban-header">
                <h1>Project Name</h1>
                <div className="kanban-actions">
                    <button onClick={() => setIsModalOpen(true)}>＋</button>
                    <button onClick={handleSortByPriority}>Sort by Priority</button>
                    <button onClick={handleSortByDueDate}>Sort by Due Date</button>
                    <button onClick={handleAddUserToProject}>Add User</button>
                    <select
                        value={selectedUserId}
                        onChange={(e) => setSelectedUserId(e.target.value)}
                    >
                        <option value="">Select user to add</option>
                        {users.map(user => (
                            <option key={user.id} value={user.id}>
                                {user.name} ({user.email})
                            </option>
                        ))}
                    </select>
                </div>
            </div>

            <DndContext
                sensors={sensors}
                collisionDetection={closestCorners}
                onDragStart={handleDragStart}
                onDragEnd={handleDragEnd}
            >
                <div className="kanban-columns">
                    <KanbanColumn title="backlog" tasks={tasks.filter(t => t.status === "backlog")} />
                    <KanbanColumn title="inProgress" tasks={tasks.filter(t => t.status === "inProgress")} />
                    <KanbanColumn title="completed" tasks={tasks.filter(t => t.status === "completed")} />
                    <DeleteZone />
                </div>

                <DragOverlay>
                    {activeTask ? (
                        <div className="kanban-card">
                            <h3>{activeTask.title}</h3>
                        </div>
                    ) : null}
                </DragOverlay>

            </DndContext>

            {isModalOpen && <Modal onSave={addTask} onClose={() => setIsModalOpen(false)} />}
        </section>
    );
};

export default Kanban;