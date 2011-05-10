#include "lvdb.h"
#include "leveldb/db.h"
#include "leveldb/write_batch.h"

using namespace leveldb;

#define SLICE(s) Slice(s.data, s.size)

struct lvdb_db {
    DB *db;
    Status status;
};

struct lvdb_batch {
    LVDB_DB *db;
    WriteBatch *batch;
};

LVDB_DB *lvdb_open(const char *name) {
    LVDB_DB *db = new LVDB_DB;
    Options options;
    options.create_if_missing = true;
    db->status = DB::Open(options, std::string(name), &db->db);
    return db;
}

void lvdb_close(LVDB_DB *db) {
    delete db->db;
    delete db;
}

bool lvdb_ok(LVDB_DB *db) {
    return db->status.ok();
}

bool lvdb_not_found(LVDB_DB *db) {
    return db->status.IsNotFound();
}

const char *lvdb_status(LVDB_DB *db) {
    return db->status.ToString().c_str();
}

LVDB_SLICE lvdb_get(LVDB_DB *db, LVDB_SLICE key) {
    std::string value;
    db->status = db->db->Get(ReadOptions(), SLICE(key), &value);
    LVDB_SLICE ret = {value.data(), value.size()};
    return ret;
}

void lvdb_put(LVDB_DB *db, LVDB_SLICE key, LVDB_SLICE value) {
    db->status = db->db->Put(WriteOptions(), SLICE(key), SLICE(value));
}

void lvdb_delete(LVDB_DB *db, LVDB_SLICE key) {
    db->status = db->db->Delete(WriteOptions(), SLICE(key));
}

LVDB_BATCH *lvdb_batch_start(LVDB_DB *db) {
    LVDB_BATCH *batch = new LVDB_BATCH;
    batch->db = db;
    batch->batch = new WriteBatch;
    return batch;
}

void lvdb_batch_put(LVDB_BATCH *batch, LVDB_SLICE key, LVDB_SLICE value) {
    batch->batch->Put(SLICE(key), SLICE(value));
}

void lvdb_batch_delete(LVDB_BATCH *batch, LVDB_SLICE key) {
    batch->batch->Delete(SLICE(key));
}

void lvdb_batch_commit(LVDB_BATCH *batch) {
    LVDB_DB *db = batch->db;
    db->status = db->db->Write(WriteOptions(), batch->batch);
    delete batch->batch;
    delete batch;
}

void lvdb_batch_cancel(LVDB_BATCH *batch) {
    delete batch->batch;
    delete batch;
}
