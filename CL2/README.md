# To-Do List API (C++)

Простой REST API сервис для управления списком задач, реализованный на C++.
Проект выполнен в рамках коллоквиума.

## Стек технологий
*   **Язык:** C++
*   **Сервер:** [cpp-httplib](https://github.com/yhirose/cpp-httplib)
*   **JSON:** [nlohmann/json](https://github.com/nlohmann/json)
*   **База данных:** Хранение в оперативной памяти (`std::map`).

## Функционал (CRUD)
API поддерживает следующие операции:

*   `GET /tasks` — Получить список всех задач.
*   `POST /tasks` — Создать новую задачу.
*   `GET /tasks/{id}` — Получить задачу по ID.
*   `PUT /tasks/{id}` — Полное обновление задачи.
*   `PATCH /tasks/{id}` — Обновление статуса задачи.
*   `DELETE /tasks/{id}` — Удаление задачи.

## Структура JSON
Пример объекта задачи:
```json
{
  "id": 1,
  "title": "Купить молоко",
  "description": "3.2% жирности",
  "status": "todo"
}