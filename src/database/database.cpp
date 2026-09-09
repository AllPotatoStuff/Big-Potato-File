#include "database/database.h"
#include <iostream>
#include <stdexcept>

IndexDatabase::IndexDatabase(const std::string &dbPath) {
  openDatabase(dbPath);
  createSchema();
  prepareStatements();
}

IndexDatabase::~IndexDatabase() {
  if (m_insertStmt)
    sqlite3_finalize(m_insertStmt);
  if (m_db)
    sqlite3_close(m_db);
}

void IndexDatabase::openDatabase(const std::string &dbPath) {
  if (sqlite3_open(dbPath.c_str(), &m_db) != SQLITE_OK) {
    std::string msg = m_db ? sqlite3_errmsg(m_db) : "unknown error";
    if (m_db) {
      sqlite3_close(m_db);
      m_db = nullptr;
    }
    throw std::runtime_error("Impossible d'ouvrir la DB: " + msg);
  }

  sqlite3_exec(m_db, "PRAGMA journal_mode=WAL;", nullptr, nullptr, nullptr);
  sqlite3_exec(m_db, "PRAGMA synchronous=NORMAL;", nullptr, nullptr, nullptr);
}

void IndexDatabase::createSchema() {
  const char *createTable = "CREATE TABLE IF NOT EXISTS files ("
                            "path TEXT PRIMARY KEY,"
                            "name TEXT NOT NULL,"
                            "size INTEGER NOT NULL,"
                            "modifiedAt INTEGER,"
                            "createdAt INTEGER,"
                            "isDirectory INTEGER,"
                            "hash TEXT,"
                            "flags INTEGER"
                            ");";
  char *errMsg = nullptr;
  if (sqlite3_exec(m_db, createTable, nullptr, nullptr, &errMsg) != SQLITE_OK) {
    std::string msg = errMsg ? errMsg : "unknown error";
    sqlite3_free(errMsg);
    throw std::runtime_error("Erreur création table: " + msg);
  }
}

void IndexDatabase::prepareStatements() {
  const char *insertSql =
      "INSERT OR REPLACE INTO files "
      "(path, name, size, modifiedAt, createdAt, isDirectory, hash, flags) "
      "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

  if (sqlite3_prepare_v2(m_db, insertSql, -1, &m_insertStmt, nullptr) !=
      SQLITE_OK) {
    throw std::runtime_error(std::string("Erreur prepare insert: ") +
                             sqlite3_errmsg(m_db));
  }
}

void IndexDatabase::insertBatch(const std::vector<FileInfo> &batch) {
  if (batch.empty())
    return;

  sqlite3_exec(m_db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

  for (const auto &f : batch) {
    sqlite3_reset(m_insertStmt);
    sqlite3_bind_text(m_insertStmt, 1, f.path.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_insertStmt, 2, f.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(m_insertStmt, 3, static_cast<sqlite3_int64>(f.size));
    sqlite3_bind_int64(m_insertStmt, 4, f.modifiedAt);
    sqlite3_bind_int64(m_insertStmt, 5, f.createdAt);
    sqlite3_bind_int(m_insertStmt, 6, f.isDirectory ? 1 : 0);
    if (f.hash.has_value())
      sqlite3_bind_text(m_insertStmt, 7, f.hash->c_str(), -1, SQLITE_TRANSIENT);
    else
      sqlite3_bind_null(m_insertStmt, 7);
    sqlite3_bind_int(m_insertStmt, 8, f.flags);

    if (sqlite3_step(m_insertStmt) != SQLITE_DONE) {
      std::cerr << "Erreur insert pour " << f.path << ": "
                << sqlite3_errmsg(m_db) << "\n";
    }
  }

  sqlite3_exec(m_db, "COMMIT;", nullptr, nullptr, nullptr);
}