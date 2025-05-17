#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Rooms
{
    int room_id;
    char *room_name;
    char *room_description;
    struct Rooms *north_button;
    struct Rooms *east_button;
    struct Rooms *south_button;
    struct Rooms *west_button;
    int item_flag;
    char *description_item;

} Rooms;

typedef struct user
{
    int user_id;
    Rooms *room_current_in;
    int found_item;
} user;

Rooms *start_room = NULL;
Rooms *connector_room = NULL;
Rooms *item_room = NULL;
Rooms *ten_rooms[10];

// creates room
Rooms *room(int id, const char *name, const char description_of_room, int item, int description_of_item)
{
    Rooms *make_room = (Rooms *)malloc(sizeof(Rooms));

    // if fail to create room, program terminates
    if (make_room == NULL)
    {
        fprintf(stderr, "Fail to allocate memory so could not make room");
        exit(1);
    }

    make_room->room_id = id;
    make_room->room_name = name;
    make_room->room_description = description_of_room;
    make_room->item_flag = item;
    make_room->description_item = description_of_item;

    return make_room;
}

// set up format for 10 rooms
void setup()
{
    srand(time(NULL));
    ten_rooms[1] = room(1, "CPP Library", "Shelves occupied with books and tables reserved with students being academic filled this library ", 0, NULL);
    ten_rooms[2] = room(2, "Brick Dorms", "Brick walls, freshmans running down the the hall, and the occasional cockroaches filled the Montecito dorms you are in", 0, NULL);
    ten_rooms[3] = room(3, "Indoor Japanese Garden", "Fresh breeze swirling throughout the green scenery fills the japanese gardenn you are in with calmness", 0, NULL);
    ten_rooms[4] = room(4, "Building 8", "Programmers fill the hallway in this building you are in on floors 1 and 3, preparing for battle in their CS class", 0, NULL);
    ten_rooms[5] = room(5, "BRIC", "Packed at 8 PM at night with college students getting in a late night workout to avoid the rumors of freshman 15 in the BRIC you are in", 0, NULL);
    ten_rooms[6] = room(6, "Dining hall", "This place you are in is filled with hungry people, a long line waiting for those customized, delicious sandwhiches, and its your lucky day with the ice cream machine working!", 0, NULL);
    ten_rooms[7] = room(7, "Parking structure", "Lots of cars line up along the cement walls, making it a battle to find parking closest to the stairs in this strucutre you are in", 0, NULL);
    ten_rooms[8] = room(8, "Building 8, Room 245", "This room you are in has a guest speaker standing behind the podium, educating on how to break into the tech industry and students filling every seat ready to learn in the CSS club", 1, "The last slice of free, cheesy pizza in a dominoes pizza box placed the front of the room");
    ten_rooms[9] = room(9, "Farm Store", "Homemade jams fill the wicker baskets and new flavor homemade ice cream fill the freezers take up the store you are in", 0, NULL);
    ten_rooms[10] = room(10, "Bookstore", "Filled with CPP merchandise, billy bronco himself in the form of keychains, stuffed animals, cups, and more, you find yourself in the bookstore to represent the school.", 0, NULL);
}

// connects room for user to navigate into
void navigate_rooms(Rooms *current_room, Rooms *next_room, char navigate)
{
    switch (navigate)
    {
    case 'N':
    case 'n':
        if (current_room)
            current_room->north_button = next_room;
        next_room->south_button = current_room;
        break;
    case 'E':
    case 'e':
        current_room->east_button = next_room;
        next_room->west_button = current_room;
        break;
    case 'S':
    case 's':
        current_room->south_button = next_room;
        next_room->north_button = current_room;
        break;
    case 'W':
    case 'w':
        current_room->west_button = next_room;
        next_room->east_button = current_room;
        break;
    default:
        fprintf(stderr, "Please try again, invalid input of: %c\n", navigate);
    }
}

void link_rooms()
{
    navigate_rooms(ten_rooms[1], ten_rooms[2], 'N');
    navigate_rooms(ten_rooms[1], ten_rooms[2], 'n');
    navigate_rooms(ten_rooms[1], ten_rooms[3], 'E');
}
