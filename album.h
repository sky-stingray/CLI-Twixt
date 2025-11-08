#ifndef ALBUM_H
#define ALBUM_H

#include <stddef.h>
#include "songs.h"

typedef struct Album {
    char name[100];
    Song *songs;
    struct Album *next;
} Album;

Album* createAlbum(const char* name);
void addSongToAlbum(Song* song, Album* album);
void deleteSongFromAlbum(Album *album, const char *title);
Album* loadAlbum(const char *name);
Album* findAlbum(Album **head, const char *name);
void saveAlbum(Album *album);
void viewAlbum(const char* name, Album **ALBUM);
void listAlbums(Album *head);

#endif