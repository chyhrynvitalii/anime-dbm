#pragma once

enum ent_var {
    NO_ENT_VAR,
    TITLE,
    STATUS,
    SCORE,
    PROG
};

enum sort_ord {
    NO_SORT_ORD,
    ASC,
    DESC
};

typedef struct {
    char *title;
    char *status;
    float score;
    unsigned prog;
} ent;

typedef struct {
    ent *ents;
    unsigned ent_count;
} db;

int get_dbname(char *);