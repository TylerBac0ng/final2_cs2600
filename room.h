#ifndef ROOMS_H
#define ROOMS_H

#include <stdbool.h>

#define MAX_ROOMS 10
#define MAX_DESCRIPTION_LENGTH 256

// room structure
typedef struct
{
    int id;
    char north_desc[MAX_DESCRIPTION_LENGTH];
    char south_desc[MAX_DESCRIPTION_LENGTH];
    char east_desc[MAX_DESCRIPTION_LENGTH];
    char west_desc[MAX_DESCRIPTION_LENGTH];
    bool is_start_room;
    bool is_item_room;
    bool is_connector_room;
} Room;

// Function prototypes
void initialize_rooms(Room rooms[]);
int get_next_room(int current_room, char direction);
const char *get_room_description(Room rooms[], int current_room, char direction);
void randomize_building_order(Room rooms[], int building_order[]);

#endif /* ROOMS_H */
