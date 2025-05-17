#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <syslog.h>
#include "rooms.h"

// navigate to next room depending on direction
int get_next_room(int current_room, char direction)
{
    switch (current_room)
    {
    case 1: // room 1
        switch (direction)
        {
        case 'n':
            return 2; // go to room 2
        case 's':
            return 0; // south is dead end
        case 'e':
            return 4; // go to room 4
        case 'w':
            return 0; // west is dead end
        default:
            return 0;
        }

    case 2: // room 2
        switch (direction)
        {
        case 'n':
            return 0; // north is dead end
        case 's':
            return 1; // go back to room 1
        case 'e':
            return 3; // go to room 3
        case 'w':
            return 0; // west is a dead end
        default:
            return 0;
        }

    case 3: // room 3
        switch (direction)
        {
        case 'n':
            return 0; // north is dead end
        case 's':
            return 4; // go to room 4
        case 'e':
            return 0; // east is dead end
        case 'w':
            return 2; // go back to room 2
        default:
            return 0;
        }

    case 4: // room 4
        switch (direction)
        {
        case 'n':
            return 3; // go back to room 3
        case 's':
            return 0; // south is dead end
        case 'e':
            return 5; // go to room 5
        case 'w':
            return 1; // go back to start room 1
        default:
            return 0;
        }

    case 5: // room 5
        switch (direction)
        {
        case 'n':
            return 0; // north is dead end
        case 's':
            return 6; // go to room 6
        case 'e':
            return 0; // east is dead end
        case 'w':
            return 4; // go back to room 4
        default:
            return 0;
        }

    case 6: // room 6
        switch (direction)
        {
        case 'n':
            return 5; // go back to room 5
        case 's':
            return 9; // go to room 9
        case 'e':
            return 7; // go to room 7
        case 'w':
            return 0; // west is dead end
        default:
            return 0;
        }

    case 7: // room 7
        switch (direction)
        {
        case 'n':
            return 0; // north is dead end
        case 's':
            return 8; // go to room 8
        case 'e':
            return 0; // east is dead end
        case 'w':
            return 6; // go back to room 6
        default:
            return 0;
        }

    case 8: // room 8
        switch (direction)
        {
        case 'n':
            return 7; // go back to room 7
        case 's':
            return 0; // south is dead end
        case 'e':
            return -1; // east is dead end
        case 'w':
            return 9; // go to room 9
        default:
            return 0;
        }

    case 9: // room 9
        switch (direction)
        {
        case 'n':
            return 6; // go back to room 6
        case 's':
            return 10; // go to room 10
        case 'e':
            return 8; // go back to room 8
        case 'w':
            return 0; // west is a dead end
        default:
            return 0;
        }

    case 10: // room 10
        switch (direction)
        {
        case 'n':
            return 9; // go back to room 9
        case 's':
            return 0; // south is dead end
        case 'e':
            return 0; // east is dead end
        case 'w':
            return 1; // go back to room 1
        default:
            return 0;
        }

    default:
        return 0; // Invalid room
    }
}

// set up room data
void initialize_rooms(Room rooms[])
{

    // room 1
    rooms[0].id = 1;
    strcpy(rooms[0].north_desc, "you go north, you find a room with mario and bowser have a brawl in the kingdom");
    strcpy(rooms[0].south_desc, "you go south, you hit a dead end");
    strcpy(rooms[0].east_desc, "you turn east, there's a room with ice cream trucks waiting for people to buy ");
    strcpy(rooms[0].west_desc, "you turn west, you are stuck in an endless dead end loop");
    rooms[0].is_start_room = true;
    rooms[0].is_item_room = false;
    rooms[0].is_connector_room = false;

    // room 2
    rooms[1].id = 2;
    strcpy(rooms[1].north_desc, "you go north, you cannot open the door");
    strcpy(rooms[1].south_desc, "you go south, you find room with a stool with a feather on it");
    strcpy(rooms[1].east_desc, "you turn east, you find room with a mirror reflecting you as a child");
    strcpy(rooms[1].west_desc, "you turn west, you hit a dead end");
    rooms[1].is_start_room = false;
    rooms[1].is_item_room = false;
    rooms[1].is_connector_room = false;

    // room 3
    rooms[2].id = 3;
    strcpy(rooms[2].north_desc, "you go north, you hit a wall with chip paint");
    strcpy(rooms[2].south_desc, "you go south, you see a room with an ice cream truck waiting for customers");
    strcpy(rooms[2].east_desc, "you turn east, you see a locked window");
    strcpy(rooms[2].west_desc, "you turn west, you see a room with bowser fighting mario in his kingdom");
    rooms[2].is_start_room = false;
    rooms[2].is_item_room = false;
    rooms[2].is_connector_room = false;

    // room 4
    rooms[3].id = 4;
    strcpy(rooms[3].north_desc, "you go north, you enter a room and see you as a child in the mirror");
    strcpy(rooms[3].south_desc, "you go south, you hit a wall with flower growing on it");
    strcpy(rooms[3].east_desc, "you turn east, there's a room with a bucket of korean fried chicken on a stool");
    strcpy(rooms[3].west_desc, "you turn west, there's a room and you see a feather on a stool");
    rooms[3].is_start_room = false;
    rooms[3].is_item_room = false;
    rooms[3].is_connector_room = false;

    // room 5
    rooms[4].id = 5;
    strcpy(rooms[4].north_desc, "you go north, there's nothing but a dead end");
    strcpy(rooms[4].south_desc, "you go south, you see a room filled  chickens flying everywhere");
    strcpy(rooms[4].east_desc, "you turn east, you hit a metal wall");
    strcpy(rooms[4].west_desc, "you turn west, theres a room with no line in front of the ice cream truck");
    rooms[4].is_start_room = false;
    rooms[4].is_item_room = false;
    rooms[4].is_connector_room = false;

    // room 6
    rooms[5].id = 6;
    strcpy(rooms[5].north_desc, "you go north, you see room with a stool with a bucket of korean fried chicken");
    strcpy(rooms[5].south_desc, "you go south, you find a room with people camping under the starry sky");
    strcpy(rooms[5].east_desc, "you turn east, you see a room with people surfing on the ocean waves");
    strcpy(rooms[5].west_desc, "you turn west, you find a locked purple door");
    rooms[5].is_start_room = false;
    rooms[5].is_item_room = false;
    rooms[5].is_connector_room = false;

    // room 7
    rooms[6].id = 7;
    strcpy(rooms[6].north_desc, "you go north, you hit a dead end and going nowhere");
    strcpy(rooms[6].south_desc, "you go south, you find a room with an empty coffee table");
    strcpy(rooms[6].east_desc, "you turn east, its just a window");
    strcpy(rooms[6].west_desc, "you turn west, and there's a room with chicken flying all over");
    rooms[6].is_start_room = false;
    rooms[6].is_item_room = false;
    rooms[6].is_connector_room = false;

    // room 8
    rooms[7].id = 8;
    strcpy(rooms[7].north_desc, "you go north, you see a room with people surfing");
    strcpy(rooms[7].south_desc, "you go south, you hit a smooth wall");
    strcpy(rooms[7].east_desc, "you turn east, you missed the chance to open the door");
    strcpy(rooms[7].west_desc, "you turn west, and its a room of people camping");
    rooms[7].is_start_room = false;
    rooms[7].is_item_room = false;
    rooms[7].is_connector_room = true;

    // room 9
    rooms[8].id = 9;
    strcpy(rooms[8].north_desc, "you go north, and its a room filled with chickens flying");
    strcpy(rooms[8].south_desc, "you go south, you see a room with seaside bakery");
    strcpy(rooms[8].east_desc, "you turn east, and its a room with an empty coffee table");
    strcpy(rooms[8].west_desc, "you turn west, you hit a dead end");
    rooms[8].is_start_room = false;
    rooms[8].is_item_room = false;
    rooms[8].is_connector_room = false;

    // room 10
    rooms[9].id = 10;
    strcpy(rooms[9].north_desc, "Congrats! you found the golden chicken! Click any button to reset!");
    strcpy(rooms[9].south_desc, "Congrats! you found the golden chicken! Click any button to reset!");
    strcpy(rooms[9].east_desc, "Congrats! you found the golden chicken! Click any button to reset!");
    strcpy(rooms[9].west_desc, "Congrats! you found the golden chicken! Click any button to reset!");
    rooms[9].is_start_room = false;
    rooms[9].is_item_room = true; // This is the item room where the game ends
    rooms[9].is_connector_room = false;
}

// get description of room based on current room and navigation
const char *get_room_description_building3(Room rooms[], int current_room, char direction)
{
    // find the room index
    int room_idx = current_room - 1;

    // make sure index of room is valid
    if (room_idx < 0 || room_idx >= MAX_ROOMS)
    {
        return "Invalid room!";
    }

    // return to a description based on direction
    switch (direction)
    {
    case 'n':
        return rooms[room_idx].north_desc;
    case 's':
        return rooms[room_idx].south_desc;
    case 'e':
        return rooms[room_idx].east_desc;
    case 'w':
        return rooms[room_idx].west_desc;
    default:
        return "Invalid direction!";
    }
}

// randomize orders to building
void randomize_building_order(Room rooms[], int building_order[])
{
    // initialize the building order array
    for (int i = 0; i < MAX_ROOMS; i++)
    {
        building_order[i] = i;
    }

    for (int i = MAX_ROOMS - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = building_order[i];
        building_order[i] = building_order[j];
        building_order[j] = temp;
    }

    // make new building order
    char order_str[100] = "";
    for (int i = 0; i < MAX_ROOMS; i++)
    {
        char temp[10];
        sprintf(temp, "%d ", building_order[i] + 1);
        strcat(order_str, temp);
    }
    syslog(LOG_INFO, "New building order: %s", order_str);
}
