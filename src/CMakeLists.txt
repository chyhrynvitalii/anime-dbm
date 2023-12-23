add_executable(anime-db
        anime-db.c
        anime-db-dialog.c
        anime-db-dialog.h
        anime-db-engine.c
        anime-db-engine.h)

target_link_libraries(anime-db PUBLIC
        file
        get
        maths
        str)