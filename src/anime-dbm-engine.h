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

extern const size_t title_len, status_len, score_len, prog_len;

extern const char *ent_printf_csv;
extern const char *ent_printf_toml;
extern const char *ent_scanf_csv;
extern const char *title_printf_toml;

int get_dbname(char *);
ent *alloc_ent();
void free_ent(ent *);
int get_ent(ent *);
int append_ent(char *, ent *);
int get_ent_num(char *);
int scan_db(char *, const char *, ent **, int);
int printf_ent(ent *, const char *);
int list_titles(ent **, const char *, int);
ent *get_ent_w_match_title(ent **ents, const char *title, int ent_num);