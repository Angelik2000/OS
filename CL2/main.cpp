#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "httplib.h" 
#include "json.hpp"  

using json = nlohmann::json;
using namespace std;

// Структура задачи
struct Task {
    int id;
    string title;
    string description;
    string status;
};

// Макрос для JSON
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Task, id, title, description, status)

// База данных
map<int, Task> tasks_db;
int next_id = 1;

int main() {
    httplib::Server svr;

    cout << "Start server..." << endl;

    // 1. GET /tasks
    svr.Get("/tasks", [](const httplib::Request& req, httplib::Response& res) {
        vector<Task> all_tasks;
        for (auto const& pair : tasks_db) {
            all_tasks.push_back(pair.second);
        }
        res.set_content(json(all_tasks).dump(), "application/json");
        });

    // 2. POST /tasks
    svr.Post("/tasks", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_body = json::parse(req.body);

            Task new_task;
            new_task.id = next_id++;
            new_task.title = json_body.at("title").get<string>();
            // Используем .value() для безопасного получения данных
            new_task.description = json_body.value("description", "");
            new_task.status = json_body.value("status", "todo");

            tasks_db[new_task.id] = new_task;

            res.status = 201;
            res.set_content(json(new_task).dump(), "application/json");
        }
        catch (...) {
            res.status = 400;
        }
        });

    // 3. GET /tasks/{id}
    svr.Get(R"(/tasks/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        int id = stoi(req.matches[1]);

        if (tasks_db.count(id)) {
            res.set_content(json(tasks_db[id]).dump(), "application/json");
        }
        else {
            res.status = 404;
            res.set_content("{}", "application/json");
        }
        });

    // 4. PUT /tasks/{id}
    svr.Put(R"(/tasks/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        int id = stoi(req.matches[1]);

        if (tasks_db.count(id)) {
            try {
                auto json_body = json::parse(req.body);
                // Получаем ссылку на задачу
                Task& task = tasks_db[id];

                task.title = json_body.at("title").get<string>();
                task.description = json_body.at("description").get<string>();
                task.status = json_body.at("status").get<string>();

                res.set_content(json(task).dump(), "application/json");
            }
            catch (...) {
                res.status = 400;
            }
        }
        else {
            res.status = 404;
        }
        });

    // 5. DELETE /tasks/{id}
    svr.Delete(R"(/tasks/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        int id = stoi(req.matches[1]);

        if (tasks_db.count(id)) {
            tasks_db.erase(id);
            res.status = 200;
            res.set_content("{\"status\": \"deleted\"}", "application/json");
        }
        else {
            res.status = 404;
        }
        });

    // 6. PATCH /tasks/{id}
    svr.Patch(R"(/tasks/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        int id = stoi(req.matches[1]);

        if (tasks_db.count(id)) {
            auto json_body = json::parse(req.body);

            if (json_body.contains("status")) {
                tasks_db[id].status = json_body["status"];
            }
            res.set_content(json(tasks_db[id]).dump(), "application/json");
        }
        else {
            res.status = 404;
        }
        });

    cout << "Server running on http://localhost:8081" << endl;
    svr.listen("0.0.0.0", 8081);

    return 0;
}