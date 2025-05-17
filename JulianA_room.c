/**
 * JulianA_rooms.c - Implementation of MUD game rooms
 * 
 * This file contains the initialization and manipulation of my rooms
 * for the MUD game.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <syslog.h>
#include "rooms.h"

/**
 * Get the next room based on current room and direction
 * Returns the ID of the next room, or 0 if no exit in that direction
 */
int get_next_room(int current_room, char direction) {
    switch(current_room) {
        case 1:  // Room 1
            switch(direction) {
                case 'n': return 2;        // North - Room 2
                case 's': return 0;       // South - no exit
                case 'e': return 0;        // East - no exit
                case 'w': return 0;        // West - no exit
                default: return 0;
            }
        
        case 2:  // Room 2
            switch(direction) {
                case 'n': return 0;        // North - no exit
                case 's': return 1;        // South - Room 1
                case 'e': return 5;        // East - Room 5
                case 'w': return 3;        // West - Room 3
                default: return 0;
            }
            
        case 3:  // Room 3
            switch(direction) {
                case 'n': return 4;        // North - Room 4
                case 's': return 0;        // South - no exit
                case 'e': return 2;        // East - Room 2
                case 'w': return 0;        // West - no exit
                default: return 0;
            }
            
        case 4:  // Room 4
            switch(direction) {
                case 'n': return 0;        // North - no exit
                case 's': return 3;        // South - Room 3
                case 'e': return 0;        // East - no exit
                case 'w': return 0;        // West - no exit
                default: return 0;
            }
            
        case 5:  // Room 5
            switch(direction) {
                case 'n': return 6;        // North - Room 6
                case 's': return 0;        // South - no exit
                case 'e': return 0;        // East - no exit
                case 'w': return 2;        // West - Room 2
                default: return 0;
            }
            
        case 6:  // Room 6
            switch(direction) {
                case 'n': return 8;        // North - Room 8
                case 's': return 5;        // South - Room 5
                case 'e': return 7;        // East - Room 7
                case 'w': return 0;        // West - no exit
                default: return 0;
            }
            
        case 7:  // Room 7
            switch(direction) {
                case 'n': return 0;        // North - no exit
                case 's': return 0;        // South - no exit
                case 'e': return 0;        // East - no exit
                case 'w': return 6;        // West - Room 6
                default: return 0;
            }
            
        case 8:  // Room 8
            switch(direction) {
                case 'n': return 0;        // North - no exit
                case 's': return 6;        // South - Room 6
                case 'e': return 0;        // East - no exit
                case 'w': return 9;        // West - Room 9
                default: return 0;
            }
            
        case 9:  // Room 9
            switch(direction) {
                case 'n': return 10;       // North - Room 10
                case 's': return 0;        // South - no exit
                case 'e': return 0;        // East - no exit
                case 'w': return 0;        // West - no exit
                default: return 0;
            }
            
        case 10:  // Room 10
            switch(direction) {
                case 'n': return -1;        // North - connector room
                case 's': return 9;        // South - no exit
                case 'e': return 0;        // East - no exit
                case 'w': return 0;        // West - no exit]']
                default: return 0;
            }
            
        default:
            return 0;  // Invalid room
    }
}

/**
 * Initialize the room data based on the provided diagram
 */
void initialize_rooms(Room rooms[]) {
    // Room 1
    rooms[0].id = 1;
    strcpy(rooms[0].north_desc, "You run north, and eventually find an old, angry man imprisoned for 1000 years. He begs you to free him, but you refuse.");
    strcpy(rooms[0].south_desc, "You try to turn back and run, but the gorilla blocks your path.");
    strcpy(rooms[0].east_desc, "You try to run east, but the gorilla blocks your path.");
    strcpy(rooms[0].west_desc, "You try to run west, but the gorilla blocks your path.");
    rooms[0].is_start_room = true;
    rooms[0].is_item_room = false;
    rooms[0].is_connector_room = false;

    // Room 2
    rooms[1].id = 2;
    strcpy(rooms[1].north_desc, "The prisoner's cell takes up the entire north wall. His pleas are getting louder.");
    strcpy(rooms[1].south_desc, "You turn back, and run into the gorilla again. He seems angrier.");
    strcpy(rooms[1].east_desc, "You turn east, and find a room filled with small dogs. You pet a few.");
    strcpy(rooms[1].west_desc, "You turn west, and find a room entire made of cake. You eat some, but it makes you feel a little sick.");
    rooms[1].is_start_room = false;
    rooms[1].is_item_room = false;
    rooms[1].is_connector_room = false;

    // Room 3
    rooms[2].id = 3;
    strcpy(rooms[2].north_desc, "You go north, and enter a room filled knee-deep with milk. It's very gross.");
    strcpy(rooms[2].south_desc, "You turn south, only to find a wall made of cake.");
    strcpy(rooms[2].east_desc, "You turn east, and find an old, angry man imprisoned for 1000 years. He begs you to free him, but you refuse.");
    strcpy(rooms[2].west_desc, "You turn west, only to find a wall made of cake.");
    rooms[2].is_start_room = false;
    rooms[2].is_item_room = false;
    rooms[2].is_connector_room = true;

    // Room 4
    rooms[3].id = 4;
    strcpy(rooms[3].north_desc, "The north door is locked. Your pants are getting soaked with milk.");
    strcpy(rooms[3].south_desc, "You go south, and find a room entire made of cake. You eat some, but it makes you feel a little sick.");
    strcpy(rooms[3].east_desc, "The east wall looms over you. The milk is seeping into your shoes.");
    strcpy(rooms[3].west_desc, "The west wall looms over you. The milk is starting to smell.");
    rooms[3].is_start_room = false;
    rooms[3].is_item_room = false;
    rooms[3].is_connector_room = false;

    // Room 5
    rooms[4].id = 5;
    strcpy(rooms[4].north_desc, "You go open the door in front of you, and enter a room filled with cobwebs. It takes a while, but you free yourself and keep going.");
    strcpy(rooms[4].south_desc, "You turn south, but the puppies seem sad. You decide to stay a while.");
    strcpy(rooms[4].east_desc, "You turn east, but find a dead end. The puppies are barking at you.");
    strcpy(rooms[4].west_desc, "You turn west, and find an old, angry man imprisoned for 1000 years. He begs you to free him, but you refuse.");
    rooms[4].is_start_room = false;
    rooms[4].is_item_room = false;
    rooms[4].is_connector_room = false;

    // Room 6
    rooms[5].id = 6;
    strcpy(rooms[5].north_desc, "You go north, but set off a booby trap and get hit with an arrow. You keep going.");
    strcpy(rooms[5].south_desc, "You go south, and find a room filled with small dogs. You pet a few.");
    strcpy(rooms[5].east_desc, "You turn east, and find a portal to leave the dungeon. You win!");
    strcpy(rooms[5].west_desc, "You turn west, but find more cobwebs. You decide to go back.");
    rooms[5].is_start_room = false;
    rooms[5].is_item_room = false;
    rooms[5].is_connector_room = false;

    // Room 7
    rooms[6].id = 7;
    // Item room, no descriptions needed
    rooms[6].is_start_room = false;
    rooms[6].is_item_room = true;
    rooms[6].is_connector_room = false;

    // Room 8
    rooms[7].id = 8;
    strcpy(rooms[7].north_desc, "You go north, and find a giant ditch. You decide to go back.");
    strcpy(rooms[7].south_desc, "You go south, and enter a room filled with cobwebs. It takes a while, but you free yourself and keep going");
    strcpy(rooms[7].east_desc, "You try to open the east door, but it's locked.");
    strcpy(rooms[7].west_desc, "You take the west door, and find yourself in a pitch black room. You can't see anything.");
    rooms[7].is_start_room = false;
    rooms[7].is_item_room = false;
    rooms[7].is_connector_room = false;

    // Room 9
    rooms[8].id = 9;
    strcpy(rooms[8].north_desc, "You go north, and eventually find a long bridge in front of you. Do you cross it?");
    strcpy(rooms[8].south_desc, "You turn south, and walk for hours. You still can't see anything.");
    strcpy(rooms[8].east_desc, "You find a door, and go through it. You set off a booby trap and get hit with an arrow. You keep going.");
    strcpy(rooms[8].west_desc, "You turn east, and walk in pitch black for a few minutes. You can feel something breathing down your neck.");
    rooms[8].is_start_room = false;
    rooms[8].is_item_room = false;
    rooms[8].is_connector_room = false;

    // Room 10
    rooms[9].id = 10;
    strcpy(rooms[9].north_desc, "You cross the bridge, and find a castle. You decide to go inside.");
    strcpy(rooms[9].south_desc, "You turn back, and enter the darkness once again.");
    strcpy(rooms[9].east_desc, "There's nothing east of you. The bridge calls your namne.");
    strcpy(rooms[9].west_desc, "There's nothing west of you. The bridge calls your namne.");
    rooms[9].is_start_room = false;
    rooms[9].is_item_room = false; 
    rooms[9].is_connector_room = true;
}

/**
 * Get the room description based on the current room and direction
 */
const char* get_room_description(Room rooms[], int current_room, char direction) {
    // Find the room index (array index is room ID - 1)
    int room_idx = current_room - 1;
    
    // Make sure room index is valid
    if (room_idx < 0 || room_idx >= MAX_ROOMS) {
        return "Invalid room!";
    }
    
    // Return the appropriate description based on direction
    switch(direction) {
        case 'n': return rooms[room_idx].north_desc;
        case 's': return rooms[room_idx].south_desc;
        case 'e': return rooms[room_idx].east_desc;
        case 'w': return rooms[room_idx].west_desc;
        default: return "Invalid direction!";
    }
}

/**
 * Randomize the building order
 */
void randomize_building_order(Room rooms[], int building_order[]) {
	// Initialize the building order array if needed
	for (int i = 0; i < MAX_ROOMS; i++) {
		building_order[i] = i;
	}
	
	// Fisher-Yates shuffle algorithm
	for (int i = MAX_ROOMS - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int temp = building_order[i];
		building_order[i] = building_order[j];
		building_order[j] = temp;
	}
	
	// Log the new building order
	char order_str[100] = "";
	for (int i = 0; i < MAX_ROOMS; i++) {
		char temp[10];
		sprintf(temp, "%d ", building_order[i] + 1);
		strcat(order_str, temp);
	}
	syslog(LOG_INFO, "New building order: %s", order_str);
}

/**
 * Check if a room is a connector room
 */
bool is_connector_room(Room rooms[], int room_id) {
	if (room_id < 1 || room_id > MAX_ROOMS) {
		return false;
	}
	
	return rooms[room_id - 1].is_connector_room;
}

/**
 * Get the building ID that this room connects to
 * Returns 0 if the room doesn't connect to another building
 */
int get_connected_building(Room rooms[], int room_id) {
	if (room_id < 1 || room_id > MAX_ROOMS) {
		return 0;
	}
	
	if (!rooms[room_id - 1].is_connector_room) {
		return 0;
	}
	
	return rooms[room_id - 1].connected_building_id;
}