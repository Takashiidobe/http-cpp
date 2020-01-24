#include "dependencies/httplib.hpp"
#include "dependencies/json.hpp"
#include "dependencies/sqlite_orm.hpp"

#include <iostream>

using namespace std;
using json = nlohmann::json;
using namespace httplib;
using namespace sqlite_orm;

struct User {
  int m_id;
  string m_first_name;
  string m_last_name;
  User() = default;
  User(string first_name, string last_name) {
    m_first_name = first_name;
    m_last_name = last_name;
  }
};

int main() {
  auto storage = make_storage(
      "sql.db",
      make_table("users",
        make_column("id", &User::m_id, autoincrement(), primary_key()),
        make_column("first_name", &User::m_first_name),
        make_column("last_name", &User::m_last_name)));

  Server app;

  app.Get("/hi", [&](const Request& req, Response& res) {
      User user("John", "Doe");
      storage.insert(user);
      string j = json{{"message", "Hello World"}}.dump();
      res.set_content(j, "application/json");
      });

  app.Post("/user", [&](const Request& req, Response& res) {
      json body = json::parse(req.body);
      const string first_name = body["firstName"];
      const string last_name = body["lastName"];

      User user = User(first_name, last_name);
      storage.insert(user);
      string j = json{{"message", "Created User: " + first_name}}.dump();
      res.set_content(j, "application/json");
      });

  app.Delete("/user", [&](const Request& req, Response& res) {
      const string id = req.params.find("id")->second;
      storage.remove<User>(id);
      string j = json{{"message", "Deleted user with id: " + id}}.dump();
      res.set_content(j, "application/json");
      });

  app.listen("localhost", 1234);
}
