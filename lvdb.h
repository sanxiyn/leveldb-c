#ifndef LVDB_H_
#define LVDB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    const char *data;
    size_t size;
} LVDB_SLICE;

typedef struct lvdb_db LVDB_DB;
typedef struct lvdb_batch LVDB_BATCH;

LVDB_DB *lvdb_open(const char *name);
void lvdb_close(LVDB_DB *db);

bool lvdb_ok(LVDB_DB *db);
bool lvdb_not_found(LVDB_DB *db);
const char *lvdb_status(LVDB_DB *db);

LVDB_SLICE lvdb_get(LVDB_DB *db, LVDB_SLICE key);
void lvdb_put(LVDB_DB *db, LVDB_SLICE key, LVDB_SLICE value);
void lvdb_delete(LVDB_DB *db, LVDB_SLICE key);

LVDB_BATCH *lvdb_batch_start(LVDB_DB *db);
void lvdb_batch_put(LVDB_BATCH *batch, LVDB_SLICE key, LVDB_SLICE value);
void lvdb_batch_delete(LVDB_BATCH *batch, LVDB_SLICE key);
void lvdb_batch_commit(LVDB_BATCH *batch);
void lvdb_batch_cancel(LVDB_BATCH *batch);

#ifdef __cplusplus
}
#endif

#endif
