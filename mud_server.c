/**
 * MUD (Multi-User Dungeon) Game Server
 * CS 2600 Final Project
 * 
 * This program implements a text-based game server that handles player movement
 * through rooms using socket communication (UDP) and sends room descriptions
 * using MQTT. The program runs as a background service on a GCP VM instance.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mosquitto.h>
#include <syslog.h>
#include "rooms.h"

#define UDP_PORT 8888
#define MAX_BUFFER_SIZE 1024
#define MAX_PLAYERS 10
#define MAX_BUILDINGS 4

// MQTT Configuration
#define MQTT_HOST "localhost"
#define MQTT_PORT 1883
#define MQTT_TOPIC_PREFIX "mud/player/"
#define MQTT_QOS 1
#define MQTT_KEEPALIVE 60

// Player structure
typedef struct {
    int id;
    int current_building;
    int current_room;
    struct sockaddr_in addr;
    char mqtt_topic[100];
    bool is_active;
} Player;

// Global variables
Room buildings[MAX_BUILDINGS][MAX_ROOMS];
Player players[MAX_PLAYERS];
int num_players = 0;
int sock_fd;
struct mosquitto *mosq = NULL;
bool running = true;
int building_order[MAX_BUILDINGS]; // Maps logical building ID to physical array index

// Each team member's room initialization functions
extern void initialize_rooms_building1(Room rooms[]); // Julian's rooms
extern void initialize_rooms_building2(Room rooms[]); // Tyler's rooms
extern void initialize_rooms_building3(Room rooms[]); // Ally's rooms
extern void initialize_rooms_building4(Room rooms[]); // Umar's rooms

// Function prototypes
void initialize_buildings();
void randomize_building_order();
void process_command(char *buffer, struct sockaddr_in client_addr);
void handle_movement(int player_id, char direction);
void send_room_description(int player_id);
void publish_mqtt_message(const char *topic, const char *message);
void mosquitto_log_callback(struct mosquitto *mosq, void *obj, int level, const char *str);
void signal_handler(int sig);
int get_player_id(struct sockaddr_in addr);
void add_player(struct sockaddr_in addr);
void send_udp_response(struct sockaddr_in addr, const char* message);

/**
 * Initialize the MQTT client
 */
int initialize_mqtt() {
    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        syslog(LOG_ERR, "Error: Out of memory when creating MQTT client");
        return -1;
    }

    mosquitto_log_callback_set(mosq, mosquitto_log_callback);

    int rc = mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, MQTT_KEEPALIVE);
    if (rc != MOSQ_ERR_SUCCESS) {
        syslog(LOG_ERR, "Error: Could not connect to MQTT broker: %s", mosquitto_strerror(rc));
        return -1;
    }

    rc = mosquitto_loop_start(mosq);
    if (rc != MOSQ_ERR_SUCCESS) {
        syslog(LOG_ERR, "Error: Could not start MQTT loop: %s", mosquitto_strerror(rc));
        return -1;
    }

    return 0;
}

/**
 * Initialize the UDP socket
 */
int initialize_socket() {
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        syslog(LOG_ERR, "Error creating socket");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(UDP_PORT);

    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        syslog(LOG_ERR, "Error binding socket to port %d", UDP_PORT);
        close(sock_fd);
        return -1;
    }

    return 0;
}

/**
 * Randomizes the building order
 * This shuffles the logical-to-physical mapping of buildings
 */
void randomize_building_order() {
    // Initialize with default order
    for (int i = 0; i < MAX_BUILDINGS; i++) {
        building_order[i] = i;
    }
    
    // Fisher-Yates shuffle
    for (int i = MAX_BUILDINGS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = building_order[i];
        building_order[i] = building_order[j];
        building_order[j] = temp;
    }
    
    // Log the new order
    char order_str[100] = "";
    for (int i = 0; i < MAX_BUILDINGS; i++) {
        char temp[20];
        sprintf(temp, "B%d->pos%d ", i+1, building_order[i]+1);
        strcat(order_str, temp);
    }
    syslog(LOG_INFO, "New building order: %s", order_str);
}

/**
 * Initialize all buildings and their rooms by calling each team member's init function
 */
void initialize_buildings() {
    // Call each team member's initialization function
    initialize_rooms_building1(buildings[0]);
    initialize_rooms_building2(buildings[1]);
    initialize_rooms_building3(buildings[2]);
    initialize_rooms_building4(buildings[3]);
    
    // Randomize the building order
    randomize_building_order();
    
    syslog(LOG_INFO, "All buildings and rooms initialized with randomized order");
}

/**
 * Get player ID by address
 */
int get_player_id(struct sockaddr_in addr) {
    for (int i = 0; i < num_players; i++) {
        if (players[i].is_active && 
            players[i].addr.sin_addr.s_addr == addr.sin_addr.s_addr && 
            players[i].addr.sin_port == addr.sin_port) {
            return i;
        }
    }
    return -1;
}

/**
 * Send a UDP response to a client
 */
void send_udp_response(struct sockaddr_in addr, const char* message) {
    sendto(sock_fd, message, strlen(message), 0, 
           (struct sockaddr *)&addr, sizeof(addr));
}

/**
 * Add a new player
 */
void add_player(struct sockaddr_in addr) {
    if (num_players < MAX_PLAYERS) {
        // Pick a random logical building
        int logical_building = rand() % MAX_BUILDINGS;
        
        // Convert to physical building using the building order
        int physical_building = building_order[logical_building];
        
        int room_idx = -1;
        
        // Find a start room in that building
        for (int i = 0; i < MAX_ROOMS; i++) {
            if (buildings[physical_building][i].is_start_room) {
                room_idx = i;
                break;
            }
        }
        
        if (room_idx == -1) {
            // If no start room found, use room 1
            room_idx = 0;
        }
        
        players[num_players].id = num_players;
        players[num_players].addr = addr;
        players[num_players].current_building = physical_building;
        players[num_players].current_room = buildings[physical_building][room_idx].id;
        players[num_players].is_active = true;
        
        // Create MQTT topic for this player
        sprintf(players[num_players].mqtt_topic, "%s%d", MQTT_TOPIC_PREFIX, num_players);
        
        syslog(LOG_INFO, "New player added with ID %d, starting in building %d (physical location %d), room %d", 
               num_players, logical_building + 1, physical_building + 1, players[num_players].current_room);
        
        // Send the player ID via UDP
        char id_message[20];
        sprintf(id_message, "player:%d", num_players);
        send_udp_response(addr, id_message);
        
        // Send the initial room description via MQTT
        send_room_description(num_players);
        
        num_players++;
    } else {
        syslog(LOG_WARNING, "Maximum number of players reached, connection rejected");
    }
}

/**
 * Process the received command
 */
void process_command(char *buffer, struct sockaddr_in client_addr) {
    // Get player ID or add new player
    int player_id = get_player_id(client_addr);
    
    // Check if this is a new player request
    if (strncmp(buffer, "new", 3) == 0) {
        if (player_id == -1) {
            add_player(client_addr);
        } else {
            syslog(LOG_INFO, "Existing player %d requested new game", player_id);
            // Just send the current room description again
            send_room_description(player_id);
        }
        return;
    }
    
    // All other commands require an existing player
    if (player_id == -1) {
        syslog(LOG_WARNING, "Command from unknown player: %s", buffer);
        return;
    }
    
    // Check for reset command
    if (strncmp(buffer, "reset", 5) == 0) {
        syslog(LOG_INFO, "Player %d requested game reset", player_id);
        
        // Randomize building order for a new game layout
        randomize_building_order();
        
        // Pick a random logical building
        int logical_building = rand() % MAX_BUILDINGS;
        
        // Convert to physical building using the building order
        int physical_building = building_order[logical_building];
        
        int room_idx = -1;
        
        // Find a start room in that building
        for (int i = 0; i < MAX_ROOMS; i++) {
            if (buildings[physical_building][i].is_start_room) {
                room_idx = i;
                break;
            }
        }
        
        if (room_idx != -1) {
            players[player_id].current_building = physical_building;
            players[player_id].current_room = buildings[physical_building][room_idx].id;
            
            syslog(LOG_INFO, "Player %d reset to building %d (physical location %d), room %d", 
                   player_id, logical_building + 1, physical_building + 1, players[player_id].current_room);
            
            // Send updated room description
            send_room_description(player_id);
        }
        return;
    }
    
    // Handle movement commands
    if (strlen(buffer) > 0) {
        char direction = buffer[0];
        switch (direction) {
            case 'N':
            case 'n':
                handle_movement(player_id, 'n');
                break;
            case 'S':
            case 's':
                handle_movement(player_id, 's');
                break;
            case 'E':
            case 'e':
                handle_movement(player_id, 'e');
                break;
            case 'W':
            case 'w':
                handle_movement(player_id, 'w');
                break;
            default:
                syslog(LOG_INFO, "Player %d sent invalid command: %s", player_id, buffer);
                // Send error message via MQTT
                publish_mqtt_message(players[player_id].mqtt_topic, 
                                   "Invalid command. Use N, S, E, W to move.");
        }
    }
}

/**
 * Handle player movement
 */
void handle_movement(int player_id, char direction) {
    if (player_id < 0 || player_id >= num_players || !players[player_id].is_active) {
        return;
    }
    
    int physical_building = players[player_id].current_building;
    int current_room = players[player_id].current_room;
    
    // Get next room based on current room and direction using the building's logic
    int next_room = get_next_room(current_room, direction);
    
    // Special case: -1 means transport to another building (connector room)
    if (next_room == -1) {
        // Find the room index
        int room_idx = current_room - 1;  // Room IDs start at 1
        
        // Check if this is a connector room
        if (room_idx >= 0 && room_idx < MAX_ROOMS && 
            buildings[physical_building][room_idx].is_connector_room) {
            
            // Get logical connected building ID (1-based)
            int logical_next_building = buildings[physical_building][room_idx].connected_building_id - 1;
            
            // Validate logical next building ID
            if (logical_next_building >= 0 && logical_next_building < MAX_BUILDINGS) {
                // Convert to physical building index using the building order
                int physical_next_building = building_order[logical_next_building];
                
                // Find a start room in the next building
                int start_room_idx = 0;
                for (int i = 0; i < MAX_ROOMS; i++) {
                    if (buildings[physical_next_building][i].is_start_room) {
                        start_room_idx = i;
                        break;
                    }
                }
                
                // Transport player to the new building's start room
                players[player_id].current_building = physical_next_building;
                players[player_id].current_room = buildings[physical_next_building][start_room_idx].id;
                
                syslog(LOG_INFO, "Player %d moved from building %d to building %d (physical position %d)", 
                       player_id, physical_building + 1, logical_next_building + 1, physical_next_building + 1);
                
                // Send the new room description
                send_room_description(player_id);
                return;
            }
        }
    }
    
    // Normal movement within the same building
    if (next_room > 0) {
        players[player_id].current_room = next_room;
        
        // Get description for the direction moved
        const char* desc = get_room_description(buildings[physical_building], current_room, direction);
        
        // Check if this is an item room (game win condition)
        int next_room_idx = next_room - 1;
        if (next_room_idx >= 0 && next_room_idx < MAX_ROOMS && 
            buildings[physical_building][next_room_idx].is_item_room) {
            // Game won!
            syslog(LOG_INFO, "Player %d found the item in building %d, room %d", 
                   player_id, physical_building + 1, next_room);
        }
        
        // Send the new room description
        send_room_description(player_id);
    } else {
        // No valid exit in that direction
        char message[100];
        sprintf(message, "You can't go that way. Try another direction.");
        publish_mqtt_message(players[player_id].mqtt_topic, message);
    }
}

/**
 * Send room description to player
 */
void send_room_description(int player_id) {
    if (player_id < 0 || player_id >= num_players || !players[player_id].is_active) {
        return;
    }
    
    int physical_building = players[player_id].current_building;
    int current_room = players[player_id].current_room;
    int room_idx = current_room - 1;  // Room IDs start at 1
    
    if (room_idx < 0 || room_idx >= MAX_ROOMS) {
        syslog(LOG_ERR, "Error: Invalid room index for player %d", player_id);
        return;
    }
    
    // Find the logical building ID from the physical location (for display purposes)
    int logical_building = -1;
    for (int i = 0; i < MAX_BUILDINGS; i++) {
        if (building_order[i] == physical_building) {
            logical_building = i;
            break;
        }
    }
    
    if (logical_building == -1) {
        logical_building = physical_building; // Fallback
    }
    
    // Create room description message
    char message[MAX_DESCRIPTION_LENGTH * 4 + 100];
    sprintf(message, "Room %d (Building %d)\n\nN: %s\nS: %s\nE: %s\nW: %s", 
            current_room, logical_building + 1,
            buildings[physical_building][room_idx].north_desc,
            buildings[physical_building][room_idx].south_desc,
            buildings[physical_building][room_idx].east_desc,
            buildings[physical_building][room_idx].west_desc);
    
    // Publish to player's MQTT topic
    publish_mqtt_message(players[player_id].mqtt_topic, message);
}

/**
 * Publish a message to an MQTT topic
 */
void publish_mqtt_message(const char *topic, const char *message) {
    int rc = mosquitto_publish(mosq, NULL, topic, strlen(message), message, MQTT_QOS, false);
    if (rc != MOSQ_ERR_SUCCESS) {
        syslog(LOG_ERR, "Error publishing to MQTT topic %s: %s", topic, mosquitto_strerror(rc));
    }
}

/**
 * MQTT logging callback
 */
void mosquitto_log_callback(struct mosquitto *mosq, void *obj, int level, const char *str) {
    switch (level) {
        case MOSQ_LOG_ERR:
            syslog(LOG_ERR, "MQTT Error: %s", str);
            break;
        case MOSQ_LOG_WARNING:
            syslog(LOG_WARNING, "MQTT Warning: %s", str);
            break;
        case MOSQ_LOG_NOTICE:
        case MOSQ_LOG_INFO:
            syslog(LOG_INFO, "MQTT Info: %s", str);
            break;
        default:
            syslog(LOG_DEBUG, "MQTT Debug: %s", str);
            break;
    }
}

/**
 * Signal handler
 */
void signal_handler(int sig) {
    switch (sig) {
        case SIGINT:
        case SIGTERM:
            syslog(LOG_INFO, "Received signal %d, shutting down...", sig);
            running = false;
            break;
        default:
            break;
    }
}

/**
 * Main function
 */
int main(int argc, char *argv[]) {
    // Initialize syslog
    openlog("mud_server", LOG_PID | LOG_CONS, LOG_DAEMON);
    syslog(LOG_INFO, "MUD Server starting...");
    
    // Seed random number generator
    srand(time(NULL));
    
    // Set up signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // Initialize rooms
    initialize_buildings();
    
    // Initialize MQTT
    if (initialize_mqtt() != 0) {
        syslog(LOG_ERR, "Failed to initialize MQTT. Exiting.");
        closelog();
        return EXIT_FAILURE;
    }
    
    // Initialize socket
    if (initialize_socket() != 0) {
        syslog(LOG_ERR, "Failed to initialize socket. Exiting.");
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        closelog();
        return EXIT_FAILURE;
    }
    
    syslog(LOG_INFO, "MUD Server running on UDP port %d", UDP_PORT);
    
    // Main loop
    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    while (running) {
        int recv_len = recvfrom(sock_fd, buffer, MAX_BUFFER_SIZE - 1, 0, 
                              (struct sockaddr *)&client_addr, &addr_len);
        
        if (recv_len > 0) {
            buffer[recv_len] = '\0';
            syslog(LOG_INFO, "Received command: %s from %s:%d", 
                   buffer, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            
            process_command(buffer, client_addr);
        }
    }
    
    // Cleanup
    close(sock_fd);
    
    mosquitto_disconnect(mosq);
    mosquitto_loop_stop(mosq, true);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    
    syslog(LOG_INFO, "MUD Server shutting down");
    closelog();
    
    return EXIT_SUCCESS;
}
