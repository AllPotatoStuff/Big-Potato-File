#ifndef DATABASE_H__
#define DATABASE_H__

#include "sqlite3.h"
#include <string>
#include <vector>
#include "index/fileinfo.h"

/**
 * @class IndexDatabase
 * @brief Encapsulate the SQLite connexion and writting operations
 *        for the file indexation.
 */
class IndexDatabase {
private:
  sqlite3 *m_db = nullptr;
  sqlite3_stmt *m_insertStmt = nullptr;

  void openDatabase(const std::string &dbPath);
  void createSchema();
  void prepareStatements();

public:
  explicit IndexDatabase(const std::string &dbPath);
  ~IndexDatabase();

  IndexDatabase(const IndexDatabase &) = delete;
  IndexDatabase &operator=(const IndexDatabase &) = delete;

  void insertBatch(const std::vector<FileInfo> &batch);
};

#endif // DATABASE_H__