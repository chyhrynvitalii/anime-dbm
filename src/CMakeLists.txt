add_executable(anime-dbm
        anime-dbm-db.c
        anime-dbm-db.h
        anime-dbm-dialog.c
        anime-dbm-dialog.h
        anime-dbm-rec.c
        anime-dbm-rec.h
        anime-dbm.c)

target_link_libraries(anime-dbm PUBLIC
        file
        get
        maths
        str)