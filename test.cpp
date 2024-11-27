
#include <iostream>
#include "sqlite3.h"


void executeSQL(sqlite3* db, const char* sql) {
   char* errMsg = nullptr;
   int rc = sqlite3_exec(db, sql, nullptr, 0, &errMsg);
   if (rc != SQLITE_OK) {
       std::cerr << "SQL error: " << errMsg << std::endl;
       sqlite3_free(errMsg);
   }
}

int main(int argc, char* argv[])
{
    const char* dbName = "encrypted.db";
    const char* password = "your_password";

    sqlite3* db;
    int rc;

    rc = sqlite3_open(dbName, &db);
    if (rc) {
       std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
       return rc;
    }

    const char* keySQL = "PRAGMA key = '123456';";
    executeSQL(db, keySQL);

    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT);";
    executeSQL(db, createTableSQL);

    const char* insertSQL = "INSERT INTO users (name) VALUES ('Alice'), ('Bob');";
    executeSQL(db, insertSQL);

    const char* selectSQL = "SELECT * FROM users;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) == SQLITE_OK) {
       while (sqlite3_step(stmt) == SQLITE_ROW) {
           int id = sqlite3_column_int(stmt, 0);
           const char* name = (const char*)sqlite3_column_text(stmt, 1);
           std::cout << "ID: " << id << ", Name: " << name << std::endl;
       }
       sqlite3_finalize(stmt);
    }
    else {
       std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);

    return 0;
}