add_executable(anime-dbm
        anime-dbm.c
        anime-dbm-db.c
        anime-dbm-db.h
        anime-dbm-dialog.c
        anime-dbm-dialog.h
        anime-dbm-rec.c
        anime-dbm-rec.h
        anime-dbm-sort.c
        anime-dbm-sort.h)

target_link_libraries(anime-dbm PUBLIC
        file
        get
        maths
        str)