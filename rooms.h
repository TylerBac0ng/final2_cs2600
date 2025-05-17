/**
 * rooms.h - Header file for MUD game rooms
 * 
 * This file contains the room structure definition and function prototypes
 * for room-related operations.
 */
#ifndef ROOMS_H
#define ROOMS_H
#include <stdbool.h>
#define MAX_ROOMS 10
#define MAX_DESCRIPTION_LENGTH 256

// Room structure - extended to include building connections
typedef struct {
    int id;
    char north_desc[MAX_DESCRIPTION_LENGTH];
    char south_desc[MAX_DESCRIPTION_LENGTH];
    char east_desc[MAX_DESCRIPTION_LENGTH];
    char west_desc[MAX_DESCRIPTION_LENGTH];
    bool is_start_room;
    bool is_item_room;
    bool is_connector_room;
    int connected_building_id;  // Added to maintain connectivity between buildings
} Room;

// Function prototypes
// These are implemented by each team member for their respective buildings
void initialize_rooms_building1(Room rooms[]);
void initialize_rooms_building2(Room rooms[]);
void initialize_rooms_building3(Room rooms[]);
void initialize_rooms_building4(Room rooms[]);

int get_next_room(int current_room, char direction);
const char* get_room_description(Room rooms[], int current_room, char direction);

// Building connection functions
bool is_connector_room(Room rooms[], int room_id);
int get_connected_building(Room rooms[], int room_id);

#endif /* ROOMS_H */
