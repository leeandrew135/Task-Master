/**
 * @file server.cpp
 * @brief Crow + SQLite3-based Task Manager Backend.
 *
 * This file implements a RESTful backend server using the Crow web framework and SQLite3
 * to manage the app
 *
 * @author Ethan, Robin, Luca
 */

#include "crow.h"
#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include "crow/middlewares/cors.h"

sqlite3 *db;

/**
 * @brief Executes a raw SQL command on the SQLite3 database.
 *
 * If an error occurs, it logs the error to stderr.
 *
 * @param sql The raw SQL query to execute.
 * @return int Returns SQLITE_OK (0) if successful, or another SQLite error code.
 */
int executeSQL(const char *sql)
{
    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    return rc;
}

int main()
{
    // Enable CORS
    crow::App<crow::CORSHandler> app;

    // Customize CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();
    cors
        .global()
        .headers("*")
        .methods("GET"_method, "POST"_method, "PUT"_method, "DELETE"_method, "OPTIONS"_method)
        .prefix("/")
        .origin("*");

    // Open SQLite database
    if (sqlite3_open("taskmaster.db", &db))
    {
        std::cerr << "Can't open DB: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // Enforce foreign key constraints
    executeSQL("PRAGMA foreign_keys = ON;");

    // Create schema if not exists
    const char *createSchema = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            email TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL
        );

        CREATE TABLE IF NOT EXISTS projects (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            deadline TEXT NOT NULL,
            date TEXT,
            completion_status BOOLEAN DEFAULT 0
        );

        CREATE TABLE IF NOT EXISTS user_projects (
            user_id INTEGER,
            project_id INTEGER,
            PRIMARY KEY (user_id, project_id),
            FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
            FOREIGN KEY (project_id) REFERENCES projects(id) ON DELETE CASCADE
        );

        CREATE TABLE IF NOT EXISTS tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            description TEXT,
            due_date TEXT,
            priority INTEGER DEFAULT 1,
            status TEXT DEFAULT 'pending',
            project_id INTEGER,
            FOREIGN KEY (project_id) REFERENCES projects(id) ON DELETE CASCADE
        );

        CREATE TABLE IF NOT EXISTS comments (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            body TEXT NOT NULL,
            date TEXT,
            status TEXT DEFAULT 'active',
            user_id INTEGER,
            task_id INTEGER,
            FOREIGN KEY (user_id) REFERENCES users(id),
            FOREIGN KEY (task_id) REFERENCES tasks(id)
        );
    )";
    executeSQL(createSchema);

    // Basic route to confirm server is running
    CROW_ROUTE(app, "/")([]
                         { return "Server is running!"; });
    CROW_ROUTE(app, "/favicon.ico")([]
                                    { return crow::response(404); });

    // ---------------------- TASKS ROUTES ----------------------

    // Get all tasks with optional filtering by status, project_id, or priority
    CROW_ROUTE(app, "/tasks").methods(crow::HTTPMethod::Get)([](const crow::request &req)
                                                             {
        std::ostringstream query;
        std::string status = req.url_params.get("status") ? req.url_params.get("status") : "";
        std::string project_id = req.url_params.get("project_id") ? req.url_params.get("project_id") : "";
        std::string priority = req.url_params.get("priority") ? req.url_params.get("priority") : "";

        query << "SELECT * FROM tasks";
        if (!status.empty() || !project_id.empty() || !priority.empty()) {
            query << " WHERE ";
            bool hasCond = false;
            if (!status.empty()) { query << "status='" << status << "'"; hasCond = true; }
            if (!project_id.empty()) { if (hasCond) query << " AND "; query << "project_id=" << project_id; hasCond = true; }
            if (!priority.empty()) { if (hasCond) query << " AND "; query << "priority=" << priority; }
        }
        query << ";";

        sqlite3_stmt* stmt;
        crow::json::wvalue::list tasks;

        if (sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                crow::json::wvalue task;
                task["id"] = sqlite3_column_int(stmt, 0);
                task["title"] = (const char*)sqlite3_column_text(stmt, 1);
                task["description"] = (const char*)sqlite3_column_text(stmt, 2);
                task["due_date"] = (const char*)sqlite3_column_text(stmt, 3);
                task["priority"] = sqlite3_column_int(stmt, 4);
                task["status"] = (const char*)sqlite3_column_text(stmt, 5);
                task["project_id"] = sqlite3_column_int(stmt, 6);
                tasks.push_back(task);
            }
            sqlite3_finalize(stmt);
            return crow::response(crow::json::wvalue(tasks));
        }
        return crow::response(500, "Failed to query tasks."); });

    // Create a new task
    CROW_ROUTE(app, "/tasks").methods(crow::HTTPMethod::Post)([](const crow::request &req)
                                                              {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        std::ostringstream query;
        query << "INSERT INTO tasks (title, description, due_date, priority, status, project_id) VALUES ("
              << "'" << body["title"].s() << "', "
              << "'" << body["description"].s() << "', "
              << "'" << body["due_date"].s() << "', "
              << body["priority"].i() << ", "
              << "'" << body["status"].s() << "', "
              << body["project_id"].i() << ");";

        if (executeSQL(query.str().c_str()) == SQLITE_OK)
            {
            // Get the last inserted row and return it
            int last_id = sqlite3_last_insert_rowid(db);
            std::ostringstream getQuery;
            getQuery << "SELECT * FROM tasks WHERE id = " << last_id << ";";

            sqlite3_stmt* stmt;
            if (sqlite3_prepare_v2(db, getQuery.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                if (sqlite3_step(stmt) == SQLITE_ROW)
                {
                    crow::json::wvalue task;
                    task["id"] = sqlite3_column_int(stmt, 0);
                    task["title"] = (const char*)sqlite3_column_text(stmt, 1);
                    task["description"] = (const char*)sqlite3_column_text(stmt, 2);
                    task["due_date"] = (const char*)sqlite3_column_text(stmt, 3);
                    task["priority"] = sqlite3_column_int(stmt, 4);
                    task["status"] = (const char*)sqlite3_column_text(stmt, 5);
                    task["project_id"] = sqlite3_column_int(stmt, 6);

                    sqlite3_finalize(stmt);
                    return crow::response(201, task);
                }
                sqlite3_finalize(stmt);
            }
            return crow::response(500, "Task created but failed to fetch it.");
        }

        return crow::response(500, "Failed to insert task."); });

    // Update a task
    CROW_ROUTE(app, "/tasks/<int>").methods(crow::HTTPMethod::Put)([](const crow::request &req, crow::response &res, int id)
                                                                   {
        auto body = crow::json::load(req.body);
        if (!body) {
            res.code = 400;
            res.write("Invalid JSON");
            return res.end();
        }

        std::ostringstream query;
        query << "UPDATE tasks SET ";

        if (body.has("title")) query << "title='" << body["title"].s() << "', ";
        if (body.has("description")) query << "description='" << body["description"].s() << "', ";
        if (body.has("due_date")) query << "due_date='" << body["due_date"].s() << "', ";
        if (body.has("priority")) query << "priority=" << body["priority"].i() << ", ";
        if (body.has("status")) query << "status='" << body["status"].s() << "', ";
        if (body.has("project_id")) query << "project_id=" << body["project_id"].i() << ", ";

        std::string q = query.str();
        q = q.substr(0, q.size() - 2); // Remove trailing comma
        q += " WHERE id=" + std::to_string(id) + ";";
        if (executeSQL(q.c_str()) == SQLITE_OK) {
            res.code = 200;
            res.write("Task updated successfully");
        } else {
            res.code = 500;
            res.write("Update failed");
        }
        res.end(); });

    // Delete a task
    CROW_ROUTE(app, "/tasks/<int>").methods(crow::HTTPMethod::Delete)([](int id)
                                                                      {
        std::ostringstream query;
        query << "DELETE FROM tasks WHERE id=" << id << ";";
        if (executeSQL(query.str().c_str()) == SQLITE_OK)
            return crow::response(204);
        return crow::response(500); });

    // ---------------------- USERS ROUTES ----------------------

    // Create a new user
    CROW_ROUTE(app, "/users").methods(crow::HTTPMethod::Post)([](const crow::request &req)
                                                              {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, crow::json::wvalue({{"message", "Invalid JSON format"}}).dump());

        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, "Select id FROM users where email = ?", -1, &stmt, nullptr) == SQLITE_OK) {
            std::string email = body["email"].s(); // Convert to std::string first
            sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                sqlite3_finalize(stmt);
                crow::json::wvalue response ({{"message", "User account already exists"}});
                return crow::response(409, response.dump());
            }
            sqlite3_finalize(stmt);
        }

        std::ostringstream query;
        query << "INSERT INTO users (name, email, password) VALUES ('"
              << body["name"].s() << "', '" << body["email"].s() << "', '" << body["password"].s() << "');";
        if (executeSQL(query.str().c_str()) == SQLITE_OK)
            return crow::response(201, crow::json::wvalue({{"message", "User created successfully"}}));
        return crow::response(500); });

    // Get all users
    CROW_ROUTE(app, "/users").methods(crow::HTTPMethod::Get)([]
                                                             {
        sqlite3_stmt* stmt;
        crow::json::wvalue::list users;
        if (sqlite3_prepare_v2(db, "SELECT * FROM users", -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                crow::json::wvalue u;
                u["id"] = sqlite3_column_int(stmt, 0);
                u["name"] = (const char*)sqlite3_column_text(stmt, 1);
                u["email"] = (const char*)sqlite3_column_text(stmt, 2);
                users.push_back(u);
            }
            sqlite3_finalize(stmt);
            return crow::response(crow::json::wvalue(users));
        }
        return crow::response(500); });

    // Get a user by email
    CROW_ROUTE(app, "/users/email/<string>").methods(crow::HTTPMethod::Get)([](const std::string &email)
                                                                            {
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, "SELECT * FROM users WHERE email = ?", -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                crow::json::wvalue u;
                u["id"] = sqlite3_column_int(stmt, 0);
                u["name"] = (const char*)sqlite3_column_text(stmt, 1);
                u["email"] = (const char*)sqlite3_column_text(stmt, 2);
                
                sqlite3_finalize(stmt);
                return crow::response(u);
            } else {
                sqlite3_finalize(stmt);
                return crow::response(404, "User not found");
            }
        }
        return crow::response(500, "Database error"); });

    // Delete a user
    CROW_ROUTE(app, "/users/<int>").methods(crow::HTTPMethod::Delete)([](int id)
                                                                      {
        std::ostringstream query;
        query << "DELETE FROM users WHERE id=" << id << ";";
        if (executeSQL(query.str().c_str()) == SQLITE_OK)
            return crow::response(204);
        return crow::response(500); });

    // ---------------------- LOGIN ROUTE ----------------------
    CROW_ROUTE(app, "/auth/login").methods(crow::HTTPMethod::Post)([](const crow::request &req)
                                                                   {
        auto json = crow::json::load(req.body);

        if (!json || !json.has("login") || !json.has("password")) {
            return crow::response(400, "Invalid request format");
        }

        std::string login = json["login"].s();
        std::string password = json["password"].s();

        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, "SELECT id, name, email, password FROM users WHERE email = ? OR name = ?", -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, login.c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                int id = sqlite3_column_int(stmt, 0);
                const char* name = (const char*)sqlite3_column_text(stmt, 1);
                const char* email = (const char*)sqlite3_column_text(stmt, 2);
                const char* storedPassword = (const char*)sqlite3_column_text(stmt, 3);

                if (password == std::string(storedPassword)) {
                    crow::json::wvalue result;
                    result["id"] = id;
                    result["name"] = name;
                    result["email"] = email;

                    sqlite3_finalize(stmt);
                    return crow::response(result);
                } else {
                    sqlite3_finalize(stmt);
                    return crow::response(401, "Invalid credentials");
                }
            } else {
                sqlite3_finalize(stmt);
                return crow::response(401, "User not found");
            }
        } 
    
        return crow::response(500, "Database error"); });

    // ---------------------- PROJECTS ROUTES ----------------------

    // Create a new project
    CROW_ROUTE(app, "/projects").methods(crow::HTTPMethod::Post)([](const crow::request &req)
{
    auto body = crow::json::load(req.body);
    if (!body) return crow::response(400, "Invalid JSON");

    std::ostringstream query;
    query << "INSERT INTO projects (deadline, date, completion_status) VALUES ('"
          << body["deadline"].s() << "', '" << body["date"].s() << "', "
          << (body["completion_status"].b() ? 1 : 0) << ");";

    if (executeSQL(query.str().c_str()) == SQLITE_OK)
    {
        int last_id = sqlite3_last_insert_rowid(db);

        std::ostringstream selectQuery;
        selectQuery << "SELECT * FROM projects WHERE id = " << last_id << ";";

        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, selectQuery.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                crow::json::wvalue project;
                project["id"] = sqlite3_column_int(stmt, 0);
                project["deadline"] = (const char*)sqlite3_column_text(stmt, 1);
                project["date"] = (const char*)sqlite3_column_text(stmt, 2);
                project["completion_status"] = sqlite3_column_int(stmt, 3);

                sqlite3_finalize(stmt);
                return crow::response(201, project);
            }
            sqlite3_finalize(stmt);
        }

        return crow::response(500, "Project created but failed to fetch it");
    }

    return crow::response(500, "Failed to insert project");
});

    // Get all projects
    CROW_ROUTE(app, "/projects").methods(crow::HTTPMethod::Get)([]
                                                                {
        sqlite3_stmt* stmt;
        crow::json::wvalue::list projects;
        if (sqlite3_prepare_v2(db, "SELECT * FROM projects", -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                crow::json::wvalue p;
                p["id"] = sqlite3_column_int(stmt, 0);
                p["deadline"] = (const char*)sqlite3_column_text(stmt, 1);
                p["date"] = (const char*)sqlite3_column_text(stmt, 2);
                p["completion_status"] = sqlite3_column_int(stmt, 3);
                projects.push_back(p);
            }
            sqlite3_finalize(stmt);
            return crow::response(crow::json::wvalue(projects));
        }
        return crow::response(500); });

    // Update a project
    CROW_ROUTE(app, "/projects/<int>").methods(crow::HTTPMethod::Put)([](const crow::request &req, crow::response &res, int id)
                                                                      {
        auto body = crow::json::load(req.body);
        if (!body) {
            res.code = 400;
            res.write("Invalid JSON");
            return res.end();
        }

        std::ostringstream query;
        if (body.has("deadline")) query << "deadline='" << body["deadline"].s() << "', ";
        if (body.has("date")) query << "date='" << body["date"].s() << "', ";
        if (body.has("completion_status")) query << "completion_status=" << body["completion_status"].b() << ", ";

        std::string q = query.str();
        q = q.substr(0, q.size() - 2);
        q += " WHERE id=" + std::to_string(id) + ";";

        if (executeSQL(q.c_str()) == SQLITE_OK) {
            res.code = 200;
            res.write("Project updated successfully");
        } else {
            res.code = 500;
            res.write("Update failed");
        }
        res.end(); });

    // Delete a project
    CROW_ROUTE(app, "/projects/<int>").methods(crow::HTTPMethod::Delete)([](int id)
                                                                         {
        std::ostringstream query;
        query << "DELETE FROM projects WHERE id=" << id << ";";
        if (executeSQL(query.str().c_str()) == SQLITE_OK)
            return crow::response(204);
        return crow::response(500); });


    // get projects a user is working on
    CROW_ROUTE(app, "/users/<int>/projects").methods("GET"_method)([](int user_id) {
    sqlite3_stmt* stmt;
    std::ostringstream query;
    query << "SELECT projects.id, deadline, date, completion_status FROM projects "
          << "JOIN user_projects ON projects.id = user_projects.project_id "
          << "WHERE user_projects.user_id = " << user_id << ";";

    crow::json::wvalue::list projectList;
    if (sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            crow::json::wvalue project;
            project["id"] = sqlite3_column_int(stmt, 0);
            project["deadline"] = (const char*)sqlite3_column_text(stmt, 1);
            project["date"] = (const char*)sqlite3_column_text(stmt, 2);
            project["completion_status"] = sqlite3_column_int(stmt, 3);
            projectList.push_back(project);
        }
        sqlite3_finalize(stmt);
        return crow::response(crow::json::wvalue(projectList));
    }
    return crow::response(500, "Database error");
});

    // Get all tasks for a user across their projects
    CROW_ROUTE(app, "/users/<int>/tasks").methods("GET"_method)([](int user_id) {
        std::ostringstream query;
        query << R"(
        SELECT tasks.id, tasks.title, tasks.description, tasks.due_date, tasks.priority, tasks.status, tasks.project_id
        FROM tasks
        JOIN user_projects ON tasks.project_id = user_projects.project_id
        WHERE user_projects.user_id = )" << user_id << ";";

        sqlite3_stmt* stmt;
        crow::json::wvalue::list taskList;

        if (sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                crow::json::wvalue task;
                task["id"] = sqlite3_column_int(stmt, 0);
                task["title"] = (const char*)sqlite3_column_text(stmt, 1);
                task["description"] = (const char*)sqlite3_column_text(stmt, 2);
                task["due_date"] = (const char*)sqlite3_column_text(stmt, 3);
                task["priority"] = sqlite3_column_int(stmt, 4);
                task["status"] = (const char*)sqlite3_column_text(stmt, 5);
                task["project_id"] = sqlite3_column_int(stmt, 6);
                taskList.push_back(task);
            }
            sqlite3_finalize(stmt);
            return crow::response(crow::json::wvalue(taskList));
        }

        return crow::response(500, "Failed to fetch tasks for user");
    });

    // get user_projects
    CROW_ROUTE(app, "/user_projects").methods("POST"_method)([](const crow::request &req) {
    auto body = crow::json::load(req.body);
    if (!body || !body.has("user_id") || !body.has("project_id"))
        return crow::response(400, "Invalid JSON");

    std::ostringstream query;
    query << "INSERT INTO user_projects (user_id, project_id) VALUES ("
          << body["user_id"].i() << ", " << body["project_id"].i() << ");";

    if (executeSQL(query.str().c_str()) == SQLITE_OK)
        return crow::response(201, "User assigned to project");

    return crow::response(500, "Failed to assign user to project");
});

    // debug route to delete all projects
    CROW_ROUTE(app, "/debug/delete_all_projects").methods("GET"_method)
([] {
    const char* sql = "DELETE FROM projects;";
    if (executeSQL(sql) == SQLITE_OK) {
        return crow::response(200, "All projects deleted (debug route)");
    } else {
        return crow::response(500, "Failed to delete projects");
    }
});
    // debug route to delete all tasks
    CROW_ROUTE(app, "/debug/delete_all_tasks").methods("GET"_method)
([] {
    const char* sql = "DELETE FROM tasks;";
    if (executeSQL(sql) == SQLITE_OK) {
        return crow::response(200, "All projects deleted (debug route)");
    } else {
        return crow::response(500, "Failed to delete projects");
    }
});

    // ---------------------- SERVER SETUP ----------------------
    app.port(8080).multithreaded().run();
    sqlite3_close(db);
    return 0;


}
