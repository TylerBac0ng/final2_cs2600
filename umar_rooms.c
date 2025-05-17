/**
 * umar_rooms.c - Implementation of MUD game rooms
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
		case 1:  //Room 1
			switch(direction) {
				case 'n': return 2;        // North - room 2 
				case 's': return 0;        // South - no exit 
				case 'e': return 0;        // East - no exit 
				case 'w': return 0;       // West - no exit
				default: return 0;
			}
		
		case 2:  // Room 2
			switch(direction) {
				case 'n': return 3;        // North - Room 3
				case 's': return 1;        // South - back to room 1
				case 'e': return 4;        // East - Room 4
				case 'w': return 0;        // West - no exit
				default: return 0;
			}
			
		case 3:  // Room 3
			switch(direction) {
				case 'n': return 0;        // North - no exit
				case 's': return 2;        // South - back to toom 2
				case 'e': return 0;        // East - no exit
				case 'w': return 5;        // West - rooom 5
				default: return 0;
			}
			
		case 4:  // Room 4 (CONNECTER)
			switch(direction) {
				case 'n': return 0;        // North - no exit
				case 's': return 0;        // South - no exit
				case 'e': return 0;        // East - no exit
				case 'w': return 3;        // West - back to room 3
				default: return 0;
			}
			
		case 5:  // Room 5 
			switch(direction) {
				case 'n': return 6;        // North - room 6
				case 's': return 4;        // South - back to room 4
				case 'e': return 0;        // East - no exit
				case 'w': return 0;        // West - no exit
				default: return 0;
			}
			
		case 6:  // Room 6
			switch(direction) {
				case 'n': return 7;        // North - room 7
				case 's': return 5;        // South - back to Room 5
				case 'e': return 0;        // East - no exit
				case 'w': return 0;        // West - no exit
				default: return 0;
			}
			
		case 7:  // Room 7
			switch(direction) {
				case 'n': return 0;        // North - no exit
				case 's': return 6;        // South - back to room 6
				case 'e': return 8;        // East - room 8
				case 'w': return 0;        // West - no exit
				default: return 0;
			}
			
		case 8:  // Room 8
			switch(direction) {
				case 'n': return 0;        // North - no exit
				case 's': return 0;        // South - no exit
				case 'e': return 9;        // East - Room 9
				case 'w': return 7;        // West - back to room 7
				default: return 0;
			}
			
		case 9:  // Room 9
			switch(direction) {
				case 'n': return 0;       // North - no ecit
				case 's': return 0;        // South - no exit
				case 'e': return 10;        // East - room 10
				case 'w': return 8;        // West - back to room 8
				default: return 0;
			}
			
		case 10:  // Room 10 (ITEM ROOM)
			switch(direction) {
				case 'n': return 0;        // North - no exit
				case 's': return 0;        // South - no exit
				case 'e': return 0;        // East - Reset to Room 1 (WIN)
				case 'w': return 0;        // West - no exit
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
	strcpy(rooms[0].north_desc, "you go north. Ruins of a humble church. A faint Site of Grace flickers.");
	strcpy(rooms[0].south_desc, "Collapsed walls block your path, overgrown with moss.");
	strcpy(rooms[0].east_desc, "Collapsed walls block your path, overgrown with moss.");
	strcpy(rooms[0].west_desc, "Collapsed walls block your path, overgrown with moss.");
	rooms[0].is_start_room = true;
	rooms[0].is_item_room = false;
	rooms[0].is_connector_room = false;
	rooms[0].connected_building_id = 2;  // Assuming building ID 2 when going west from Room 1

	// Room 2
	rooms[1].id = 2;
	strcpy(rooms[1].north_desc, "you go north. The path darkens beneath the canopy.");
	strcpy(rooms[1].south_desc, "you go south. You go back to the ruined church.");
	strcpy(rooms[1].east_desc, "you go east. You glimpse a glowing light between the trees.");
	strcpy(rooms[1].west_desc, "Thick roots entangle the forests edge.");
	rooms[1].is_start_room = false;
	rooms[1].is_item_room = false;
	rooms[1].is_connector_room = false;
	rooms[1].connected_building_id = 0;

	// Room 3
	rooms[2].id = 3;
	strcpy(rooms[2].north_desc, "Sheer cliffs prevent movement"); // Goes to Room 4
	strcpy(rooms[2].south_desc, "you go south. You go back to the mistwood outskirts"); // Returns to Room 2 - matching Room 2's north description
	strcpy(rooms[2].east_desc, "Sheer cliffs prevent movement");
	strcpy(rooms[2].west_desc, "You descend to the Siofra depths.");
	rooms[2].is_start_room = false;
	rooms[2].is_item_room = false;
	rooms[2].is_connector_room = false;
	

	// Room 4
	rooms[3].id = 4;
	strcpy(rooms[3].north_desc, "you go north, theres a shark circling a boat");
	strcpy(rooms[3].south_desc, "you go south, you enter a closet that leads to a bedroom"); // Returns to Room 3 - matching Room 3's north description
	strcpy(rooms[3].east_desc, "you turn east, the deep blue ocean seems empty");
	strcpy(rooms[3].west_desc, "you turn west, theres a cave that leads to a room"); // Goes to Room 5
	rooms[3].is_start_room = false;
	rooms[3].is_item_room = false;
	rooms[3].is_connector_room = true;
    rooms[2].connected_building_id = 3;  // Set this to the ID of the building it connects to

	// Room 5
	rooms[4].id = 5;
	strcpy(rooms[4].north_desc, "you go north. A crumbled archway opens into shimmering ruins.");
	strcpy(rooms[4].south_desc, "Impassable fog walls block the way.");
	strcpy(rooms[4].east_desc, "you go east. You go back to the Siofra River lift"); //Returns to room 4 - matching Room 4's west description
	strcpy(rooms[4].west_desc, "Impassable fog walls block the way."); // Goes to Room 6
	rooms[4].is_start_room = false;
	rooms[4].is_item_room = false;
	rooms[4].is_connector_room = false;

	// Room 6
	rooms[5].id = 6;
	strcpy(rooms[5].north_desc, "you go north. A collapsed stairway leads");
	strcpy(rooms[5].south_desc, "you go south. You go back to the river depts"); // Goes back to Room 6
	strcpy(rooms[5].east_desc, "Ancient buildings block your path."); // Returns to Room 5 - matching Room 5's west description
	strcpy(rooms[5].west_desc, "Ancient buildings bloc your path.");
	rooms[5].is_start_room = false;
	rooms[5].is_item_room = false;
	rooms[5].is_connector_room = false;

	// Room 7
	rooms[6].id = 7;
	strcpy(rooms[6].north_desc, "Rivers of ghostlight cut off your way."); // Returns to Room 6 - matching Room 6's south description
	strcpy(rooms[6].south_desc, "you go south. You go back to the eternal city."); // Goes to Room 8
	strcpy(rooms[6].east_desc, "A hidden path winds toward the surface.");
	strcpy(rooms[6].west_desc, "Rivers of ghostlight cut off your way.");
	rooms[6].is_start_room = false;
	rooms[6].is_item_room = false;
	rooms[6].is_connector_room = false;

	// Room 8
	rooms[7].id = 8;
	strcpy(rooms[7].north_desc, "Jagged rock and debris block the path"); // Returns to Room 7 - matching Room 7's south description
	strcpy(rooms[7].south_desc, "General Radahn, demigod of gravity stares you down.");
	strcpy(rooms[7].east_desc, "you go east. Following the roots of the grand tree."); // Goes to Room 9
	strcpy(rooms[7].west_desc, "you go west. You go back to the hallowhorn grounds");
	rooms[7].is_start_room = false;
	rooms[7].is_item_room = false;
	rooms[7].is_connector_room = false;

	// Room 9
	rooms[8].id = 9;
	strcpy(rooms[8].north_desc, "The root walls are too thick to pass."); // Goes to Room 10
	strcpy(rooms[8].south_desc, "you go south. A faint golden glow shines from the peak");
	strcpy(rooms[8].east_desc, "The root walls are too thick to pass.");
	strcpy(rooms[8].west_desc, "you go west. You go back to the crate of radahn."); // Returns to Room 8 - matching Room 8's east description
	rooms[8].is_start_room = false;
	rooms[8].is_item_room = false;
	rooms[8].is_connector_room = false;

	// Room 10
	rooms[9].id = 10;
	strcpy(rooms[9].north_desc, "you go north, theres a twisty backroad");
	strcpy(rooms[9].south_desc, "you go south, park your car and head back home"); // Returns to Room 9 - matching Room 9's north description
	strcpy(rooms[9].east_desc, "you turn east, you find a steak in the middle console and eat it (GAME OVER) ask to reset?"); // Game over - return to Room 1
	strcpy(rooms[9].west_desc, "you turn west, theres a racetrack with other cars");
	rooms[9].is_start_room = false;
	rooms[9].is_item_room = true;  // This is the item room where the game ends
	rooms[9].is_connector_room = false;
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

const char* get_room_overview(int current_room) {
	switch (current_room) {
		case 1:
			return "Church of Elleh- A ruined chapel open to the sky.";
		case 2:
			return "Mistwood Outskirts- Gnarled trees and howling wolves fill the silent forest.";
		case 3:
			return "Siofra River Lift- The lift hums with ghostly energy.";
		case 5:
			return "Blaidd's Watch- Blaidd leans silently on his greatsword, watching the stars fall.";
		case 4:
			return "Siofra River Depths- Ancestral spirits roam in silence.";
		case 6:
			return "Nokron, Eternal City- Ruins glimmer under eternal starlight.";
		case 7:
			return "Hallowhorn Grounds- A sacred arena of the Ancestral Spirit.";
		case 8:
			return "Crater of General Radahn- Meteoric debris surrounds you.";
		case 9:
			return "Deeproot Depths- Massive tree roots twist above.";
		case 10:
			return "Prince of Death's Throne- You grasp The Rune of Death. (YOU WIN)";
		default:
			return "You are in an unknown place. Something is wrong.";
	}
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
