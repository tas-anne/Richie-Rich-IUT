typedef enum {
    G_type_ABBANK,
    G_type_extra_internet_usage,
    G_type_STATION,
    G_type_TASK1,
    G_type_TASK2,
    G_type_TASK3,
    G_type_TASK4,
    G_type_TASK5,
    G_type_TASK6,
    G_type_PUNISHMENT,
    G_type_LAKE,
    G_type_SERVICE,
    G_type_LIBRARY_FEES,
    G_type_GOTO_VC_OFFICE,
    G_type_BROWN,
    G_type_LIGHTBLUE,
    G_type_PINK,
    G_type_ORANGE,
    G_type_RED,
    G_type_YELLOW,
    G_type_GREEN,
    G_type_BLUE

} G_Property_t;

typedef struct
{
    G_Property_t type;
    int id;
    int owner;
    int mortaged;
    int upgrades;
    int price;
    int upgradeCost;
    int rentCost[6];
} Game_Property;
