#include <iostream>
#include "Database.h"
#include "DiaryApp.h"

int main() {
    Database db("database/diary.db");
    if (!db.open()) return 1;
    db.createTable();

    // Seed only if empty
    auto existing = db.getAllEntries();
    if (existing.empty()) {
        std::cout << "Database empty — please run Day 2 first.\n";
        return 1;
    }

    DiaryApp app(db);
    app.run();

    db.close();
    return 0;
}