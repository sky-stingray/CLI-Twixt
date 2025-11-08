#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"songs.h"
#include"album.h"

#define MAX_LINE 256
#define LOG_FILE "data/log.txt"
#define SONG_FILE "data/songs.txt"


typedef struct PlaylistNode {
    Song *song;
    struct PlaylistNode *next;
    struct PlaylistNode *prev;
} PlaylistNode;

typedef struct Playlist {
    PlaylistNode *head;
    PlaylistNode *current;
    int count;
} Playlist;

Song *SONGS = NULL;
Playlist PLAYLIST = {NULL, NULL, 0};
Album *ALBUM = NULL;

void addSongToPlaylist(Song* song) {
    PlaylistNode *node = malloc(sizeof(PlaylistNode));
    node->song = song;
    node->next = node->prev = NULL;

    if(PLAYLIST.head == NULL) {
        PLAYLIST.head = PLAYLIST.current = node;
        node->next = node->prev = node;
    }

    else {
        PlaylistNode *tail = PLAYLIST.head->prev;;
        tail->next = node;
        node->prev = tail;
        node->next = PLAYLIST.head;
        PLAYLIST.head->prev = node;
    }
    PLAYLIST.count++;
}

void playNext() {
    if(PLAYLIST.current == NULL) {
        printf("Playlist is empty.\n");
        return;
    }

    PLAYLIST.current = PLAYLIST.current->next;
    Song *song = PLAYLIST.current->song;
    int minutes = song->duration / 60;
    int seconds = song->duration % 60;
    printf("Now playing: %s - %s (%d:%02d)\n", song->title, song->artist, minutes, seconds);

}   

void playPrev() {
    if(PLAYLIST.current == NULL) {
        printf("Playlist is empty.\n");
        return;
    }

    PLAYLIST.current = PLAYLIST.current->prev;
    Song *song = PLAYLIST.current->song;
    int minutes = song->duration / 60;
    int seconds = song->duration % 60;
    printf("Now playing: %s - %s (%d:%02d)\n", song->title, song->artist, minutes, seconds);
}

void removeSongFromPlaylist(char* title) {
    if(PLAYLIST.current == NULL) {
        printf("Playlist is empty.\n");
        return;
    }

    PlaylistNode *node = PLAYLIST.head;
    do {
        if(strcasecmp(node->song->title, title) == 0) {
            if(PLAYLIST.count == 1) {
                PLAYLIST.head = PLAYLIST.current = NULL;
            }
            
            else {
                node->prev->next = node->next;
                node->next->prev = node->prev;
                if(node == PLAYLIST.head) {
                    PLAYLIST.head = node->next;
                }
                
                if(node == PLAYLIST.current) {
                    PLAYLIST.current = node->next;
                }
            }
            free(node);
            PLAYLIST.count--;
            printf("Removed '%s' from playlist\n", title);
            return;
        }
        node = node->next;
    } while (node != PLAYLIST.head);

    printf("Song '%s' not found in playlist.\n", title);
}

void showCurrentSong() {
    if(PLAYLIST.count == 0) {
        printf("Playlist is empty.\n");
        return;
    }

    Song* song = PLAYLIST.current->song;
    int minutes = song->duration/60;
    int seconds = song->duration%60;
    printf("Current: %s - %s (%d:%02d)\n", song->title, song->artist, minutes, seconds);
}

void addAlbumToPlaylist(const char *name) {
    Album *album = findAlbum(&ALBUM, name);
    if(!album) {
        printf("Album '%s' not found.\n", name);
        return;
    }
    
    if(!album->songs) {
        printf("Album '%s' is empty.\n", name);
        return;
    }
    
    int count = 0;
    Song *temp = album->songs;
    while(temp) {
        addSongToPlaylist(temp);
        count++;
        temp = temp->next;
    }
    printf("Added %d songs from '%s' to playlist\n", count, name);
}

void trim_cmd(char* cmd) {
    if(!cmd) return;
    cmd[strcspn(cmd, "\n")] = '\0';
    int len = strlen(cmd);
    while(len > 0 && (cmd[len-1] == ' ' || cmd[len-1] == '\t')) {
        cmd[--len] = '\0';
    }
}

void logCommand(const char* cmd) {
    FILE* spLog = fopen(LOG_FILE, "a");
    if(!spLog) return; //Error handling, such as creating a new file.

    time_t t = time(NULL);
    char ts[100];
    strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", localtime(&t));

    fprintf(spLog, "[%s] %s\n", ts, cmd);
    fclose(spLog);
    return;
}

void showLog() {
    FILE* spLog = fopen(LOG_FILE, "r");
    if(!spLog) return; //Error handling, such as creating a new file.

    char cmd[MAX_LINE];
    while (fgets(cmd, sizeof(cmd), spLog)) printf("%s", cmd);
    fclose(spLog);
    return;
}

void parseCommand(char *cmd) {
    if(strcmp(cmd, "exit") == 0) {
        exit(0);
    }
    else if (strcmp(cmd, "help") == 0) {
        printf("\n=== Available Commands ===\n");
        printf("  list songs                          - List all songs in library\n");
        printf("  list albums                         - List all albums\n");
        printf("  view album <name>                   - View songs in an album\n");
        printf("  create album <name>                 - Create a new album\n");
        printf("  add song <title> to album <name>    - Add song to album\n");
        printf("  delete song <title> from album <name> - Remove song from album\n");
        printf("  save album <name>                   - Save album to disk\n");
        printf("  add album <name> to playlist        - Add all songs from album to playlist\n");
        printf("  add song <title> to playlist        - Add a song to playlist\n");
        printf("  play next                           - Play next song\n");
        printf("  play prev                           - Play previous song\n");
        printf("  current                             - Show current song\n");
        printf("  remove song <title> from playlist   - Remove song from playlist\n");
        printf("  show log                            - Display command history\n");
        printf("  exit                                - Exit application\n\n");
    }

    else if(strcmp(cmd, "list songs") == 0) {
        listSongs(SONGS);
    }

    else if(strcmp(cmd, "show logs") == 0) {
        showLog();
    }

    else if(strncmp(cmd, "view album ", 11) == 0) {
        viewAlbum(cmd + 11, &ALBUM);
    }

    else if(strncmp(cmd, "create album ", 13) == 0) {
        char *name = cmd + 13;
        if(findAlbum(&ALBUM, name)) {
            printf("Album '%s' already exists.\n", name);
        } else {
            Album *album = createAlbum(name);
            album->next = ALBUM;
            ALBUM = album;
            printf("Created album '%s'\n", name);
        }
    }

    else if(strcmp(cmd, "current") == 0) {
        showCurrentSong();
    }

    else if(strcmp(cmd, "play next") == 0) {
        playNext();
    }

    else if(strcmp(cmd, "play prev") == 0) {
        playPrev();
    }

    else if(strncmp(cmd, "remove song ", 12) == 0) {
        char *remaining = cmd + 12;
        char *fromPlaylist = strstr(remaining, " from playlist");
        char *fromAlbum = strstr(remaining, " from album ");

        if(fromPlaylist) {
            *fromPlaylist = 0;
            removeSongFromPlaylist(remaining);
        } 
        else if (fromAlbum){
            *fromAlbum = 0;
            char *albumName = fromAlbum + 12;
            Album *album = findAlbum(&ALBUM, albumName);

            if(!album) {
                printf("Album '%s' not found. Create it first.\n", albumName);
            } 
            else {
                deleteSongFromAlbum(album, remaining);
            }
        }
        else {
            printf("Invalid command format. Use: remove song <title> from playlist\n");
        }
    }

    else if(strncmp(cmd, "add song ", 9) == 0) {
        char *remaining = cmd + 9;
        char *toPlaylist = strstr(remaining, " to playlist");
        char *toAlbum = strstr(remaining, " to album ");

        if(toPlaylist) {
            *toPlaylist = '\0';
            Song *song = findSongByTitle(SONGS, remaining);
            if(!song) {
                printf("Song '%s' not found in library.\n", remaining);
            } else {
                addSongToPlaylist(song);
                printf("Added '%s' to playlist\n", remaining);
            }
        }

        else if(toAlbum) {
            *toAlbum = '\0';
            char *albumName = toAlbum + 10;
            Song *song = findSongByTitle(SONGS, remaining);
            Album *album = findAlbum(&ALBUM, albumName);
            
            if(!song) {
                printf("Song '%s' not found in library.\n", remaining);
            } else if(!album) {
                printf("Album '%s' not found. Create it first.\n", albumName);
            } else {
                addSongToAlbum(song, album);
                printf("Added '%s' to album '%s'\n", remaining, albumName);
            }
        }
    }

    else if(strcmp(cmd, "list albums") == 0) {
        listAlbums(ALBUM);
    }

    else if(strncmp(cmd, "save album ", 11) == 0) {
        char *albumName = cmd + 11;
        Album *album = findAlbum(&ALBUM, albumName);
        if(album) {
            saveAlbum(album);
        } 
        
        else {
            printf("Album '%s' not found.\n", albumName);
        }
    }

    else if(strncmp(cmd, "add album ", 10) == 0) {
        char *remaining = cmd + 10;
        char *toPlaylist = strstr(remaining, " to playlist");
        *toPlaylist = '\0';

        addAlbumToPlaylist(remaining);

    }

    else {
        printf("'%s': Command not found. Type 'help' for available commands.\n", cmd);
    }
}

int main(int argc, char **argv) {
    SONGS = loadSongsFromFile(SONG_FILE);


printf(
        "\033[36m\n"
        "         ____     _    _             _                 _ \n"
        "        / ___|___| | _(_)_ __   __ _| |   _   _  ___  | |\n"
        "       | |   / _ \\ |/ / | '_ \\ / _\\`| |  | | | |/ _ \\ | |\n"
        "       | |__|  __/   <| | | | | (_| | |__| |_| |  __/ |_|\n"
        "        \\____\\___|_|\\_\\_|_| |_|\\__, |\\____\\__,_|\\___| (_)\n"
        "                                |___/                    \n"
        "             \033[0;96m╔═════════════════════════════════════╗\n"
        "             ║     🎵  C - U N P L U G G E D       ║\n"
        "             ╚═════════════════════════════════════╝\033[0m\n"
        "\n"
        "                \033[36m♪ Relax. Play. Groove. Unplug. ♪\033[0m\n"
    );


    printf("\nWelcome to 🎸 C-Unplugged!\n\nType 'help' to see available commands.\n\n");
    
    char cmd[MAX_LINE];
    while(1) {
        printf(">");
        fflush(stdout);

        if(!fgets(cmd, sizeof(cmd), stdin)) break;
        if(strlen(cmd) == 1) continue;

        trim_cmd(cmd);

        logCommand(cmd);
        parseCommand(cmd);
    }
    return 0;
}