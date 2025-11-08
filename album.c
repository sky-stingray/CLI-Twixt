#include "album.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALBUM_DIR "data/albums/"

Album* createAlbum(const char* name) {
    Album *album = malloc(sizeof(Album));
    strcpy(album->name, name);
    album->songs = NULL;
    album->next = NULL;
    return album;
}

void addSongToAlbum(Song* song, Album* album) {
    Song *newSong = malloc(sizeof(Song));
    strcpy(newSong->title, song->title);
    strcpy(newSong->artist, song->artist);
    newSong->duration = song->duration;
    newSong->next = NULL;

    if (!album->songs) {
        album->songs = newSong;
    } 
    
    else {
        Song *temp = album->songs;
        while(temp->next) temp = temp->next;
        temp->next = newSong;
    }
}

void deleteSongFromAlbum(Album *album, const char *title) {
    if(!album->songs) return;
    
    Song *temp = album->songs;
    Song *prev = NULL;
    
    while(temp) {
        if(strcasecmp(temp->title, title) == 0) {
            if(prev) {
                prev->next = temp->next;
            } else {
                album->songs = temp->next;
            }
            free(temp);
            printf("Removed '%s' from album '%s'\n", title, album->name);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Song '%s' not found in album '%s'\n", title, album->name);
}

Album* loadAlbum(const char *name) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s%s.txt", ALBUM_DIR, name);
    
    FILE *fp = fopen(filepath, "r");
    if(!fp) return NULL;
    
    Album *album = createAlbum(name);
    char line[256];
    Song *tail = NULL;
    
    while(fgets(line, sizeof(line), fp)) {
        Song *s = malloc(sizeof(Song));
        if(sscanf(line, "%99[^|]|%99[^|]|%d", s->title, s->artist, &s->duration) != 3) {
            free(s);
            continue;
        }
        s->next = NULL;
        
        if(!album->songs) {
            album->songs = tail = s;
        } else {
            tail->next = s;
            tail = s;
        }
    }
    fclose(fp);
    return album;
}

Album* findAlbum(Album **head, const char *name) {
    Album *temp = *head;
    while(temp) {
        if(strcasecmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    if(temp == NULL) {
        temp = loadAlbum(name);
        if(temp) {
            temp->next = *head;
            *head = temp;
            printf("Loaded album '%s' from disk\n", (*head)->name);
        }
        return temp;
    }
    return NULL;
}

void saveAlbum(Album *album) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s%s.txt", ALBUM_DIR, album->name);
    
    FILE *fp = fopen(filepath, "w");
    if(!fp) {
        printf("Error: Could not save album '%s'\n", album->name);
        return;
    }
    
    Song *temp = album->songs;
    while(temp) {
        fprintf(fp, "%s|%s|%d\n", temp->title, temp->artist, temp->duration);
        temp = temp->next;
    }
    fclose(fp);
    printf("Album '%s' saved successfully!\n", album->name);
}

void viewAlbum(const char* name, Album **ALBUM) {
    Album *album = findAlbum(ALBUM, name);
    if(!album) {
        album = loadAlbum(name);
        if(!album) {
            printf("Album '%s' not found.\n", name);
            return;
        }
    }

    printf("\nSongs in Album [%s]:\n", album->name);
    if(!album->songs) {
        printf("  (empty)\n\n");
        return;
    }
    
    Song *temp = album->songs;
    int count = 1;
    while(temp) {
        int minutes = temp->duration / 60;
        int seconds = temp->duration % 60;
        printf("%d. %s - %s (%d:%02d)\n", count++, temp->title, temp->artist, minutes, seconds);
        temp = temp->next;
    }
    printf("\n");
}

void listAlbums(Album *head) {
    if(!head) {
        printf("No albums in your library.\n");
        return;
    }
    
    printf("\nAlbums in your library:\n");
    Album *temp = head;
    int count = 1;
    while(temp) {
        int songCount = 0;
        Song *s = temp->songs;
        while(s) { songCount++; s = s->next; }
        printf("%d. %s (%d songs)\n", count++, temp->name, songCount);
        temp = temp->next;
    }
    printf("\n");
}