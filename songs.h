#ifndef SONGS_H
#define SONGS_H

#include <stddef.h>

typedef struct Song {
    char title[100];
    char artist[100];
    char album[100];
    int duration;
    struct Song *next;
} Song;

Song* loadSongsFromFile(const char *filename);
void listSongs(Song *head);
Song* findSongByTitle(Song *head, const char *title);

#endif