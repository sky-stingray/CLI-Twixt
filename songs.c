#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "songs.h"

Song* loadSongsFromFile(const char *filename) {
    FILE *spSongs = fopen(filename, "r");
    if(!spSongs) {
        printf("Could not load songs from %s \n", filename);
        return NULL;
    }

    Song *head = NULL, *tail = NULL;
    char line[256];
    while (fgets(line, sizeof(line), spSongs)) {
        Song *s = malloc(sizeof(Song));
        if (sscanf(line, "%99[^|]|%99[^|]|%d", s->title, s->artist, &s->duration) != 3) {
            free(s); 
            continue;
        }
        s->next = NULL;
        if (!head) head = tail = s;
        else { 
            tail->next = s;
            tail = s;
        }
    }
    fclose(spSongs);
    return head;
}

void listSongs(Song *head) {
    if(!head) {
        printf("No songs in library.\n");
        return;
    }


    Song *temp = head;

    printf("------------------------------------------------------------------- \n");
    printf("| %-25s | %-20s | %s     |\n", "Title", "Artist", "Duration");
    printf("------------------------------------------------------------------- \n");
    while(temp != NULL) {
        int minutes = temp->duration/60;
        int seconds = temp->duration%60;

        printf("| %-25s | %-20s | %d:%02d         |\n", temp->title, temp->artist, minutes, seconds);

        temp = temp->next;

    }
    printf("------------------------------------------------------------------- \n");
}

Song* findSongByTitle(Song *head, const char *title) {
    Song *temp = head;
    while(temp) {
        if(strcasecmp(temp->title, title) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}