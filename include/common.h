#ifndef COMMON_H
#define COMMON_H

#include <pksav.h>

typedef int (*Error_Handler)(enum pksav_error, const char *);

struct TrainerInfo
{
    char trainer_name[8];
    uint16_t trainer_id;
    uint8_t trainer_gender;
    uint8_t trainer_badges[2];
    struct pksav_gen2_pokemon_party *pokemon_party;
};

typedef enum
{
    EBadge_Region_Johto,
    EBadge_Region_Kanto
} BadgeRegion;

struct TrainerSelection
{
    int trainer_id;
    int pokemon_index;
    int trainer_index;
};

#endif // COMMON_H
