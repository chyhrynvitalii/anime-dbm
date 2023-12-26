add_executable(anime-dbm
        anime-dbm.c
        anime-dbm-dialog.c
        anime-dbm-dialogs.h
        anime-dbm-engine.c
        anime-dbm-engine.h)

target_link_libraries(anime-dbm PUBLIC
        file
        get
        maths
        str)