#include <pksav.h>
#include <stdio.h>
#include <string.h>

typedef int (*Error_Handler)(enum pksav_error, const char *);

struct pksav_gen2_save loadSaveFromFile(char *path, Error_Handler error_handler)
{
    enum pksav_error err = PKSAV_ERROR_NONE;

    enum pksav_gen2_save_type save_type;
    err = pksav_gen2_get_file_save_type(path, &save_type);
    if (err != PKSAV_ERROR_NONE)
    {
        error_handler(err, "Error getting save type");
    }

    struct pksav_gen2_save save;
    err = pksav_gen2_load_save_from_file(path, &save);
    if (err != PKSAV_ERROR_NONE)
    {
        error_handler(err, "Error loading save");
    }
    return save;
}
void printTrainerBadges(struct pksav_gen2_save *save)
{
    enum pksav_gen2_johto_badge_mask johto_badges = *save->trainer_info.p_johto_badges;
    enum pksav_gen2_kanto_badge_mask kanto_badges = *save->trainer_info.p_kanto_badges;
    printf("Johto badges: ");
    if (johto_badges & PKSAV_GEN2_JOHTO_ZEPHYR_BADGE)
    {
        printf("Zephyr ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_HIVE_BADGE)
    {
        printf("Hive ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_PLAIN_BADGE)
    {
        printf("Plain ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_FOG_BADGE)
    {
        printf("Fog ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_STORM_BADGE)
    {
        printf("Storm ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_MINERAL_BADGE)
    {
        printf("Mineral ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_GLACIER_BADGE)
    {
        printf("Glacier ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_RISING_BADGE)
    {
        printf("Rising ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_BOULDER_BADGE)
    {
        printf("Boulder ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_CASCADE_BADGE)
    {
        printf("Cascade ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_THUNDER_BADGE)
    {
        printf("Thunder ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_RAINBOW_BADGE)
    {
        printf("Rainbow ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_SOUL_BADGE)
    {
        printf("Soul ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_MARSH_BADGE)
    {
        printf("Marsh ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_VOLCANO_BADGE)
    {
        printf("Volcano ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_EARTH_BADGE)
    {
        printf("Earth ");
    }

    printf("\n");
}
void printTrainerData(struct pksav_gen2_save *save)
{
    // print trainer name
    char trainer_name[8];
    pksav_gen2_import_text(save->trainer_info.p_name, trainer_name, 7);
    printf("Trainer name: %s\n", trainer_name);

    // print trainer id
    uint16_t trainer_id = pksav_bigendian16(*save->trainer_info.p_id);
    printf("Trainer ID: %u\n", trainer_id);

    // print trainer gender
    uint8_t trainer_gender = *save->trainer_info.p_gender;
    printf("Trainer Gender: %s\n", trainer_gender ? "F" : "M");

    // print trainer badges
    printTrainerBadges(save);
}
// void changePartyPokemonNicknameAtIndex(pksav_gen2_save_t *save, int pokemon_index, char *new_name)
// {
//     char pokemon_name[11];
//     pksav_text_to_gen2(new_name, save->pokemon_party->nicknames[pokemon_index], 10);
//     pksav_text_from_gen2(save->pokemon_party->nicknames[pokemon_index], pokemon_name, 10);
//     printf("Pokemon new name: %s\n", pokemon_name);
// }
void saveToFile(struct pksav_gen2_save *save, char *path, Error_Handler error_handler)
{
    enum pksav_error err = PKSAV_ERROR_NONE;
    err = pksav_gen2_save_save(path, save);
    pksav_gen2_free_save(save);

    if (err != PKSAV_ERROR_NONE)
    {
        error_handler(err, "Error saving save");
    } else {
        printf("Saved to %s\n", path);
    }
}

void printPartyPokemonAtIndex(struct pksav_gen2_save *save, int pokemon_index)
{
    // print nickname
    char pokemon_nickname[11];
    pksav_gen2_import_text(save->pokemon_storage.p_party->nicknames[pokemon_index], pokemon_nickname, 10);
    printf("%d. %s\n", pokemon_index + 1, pokemon_nickname);

    // // print species
    // uint8_t species = save->pokemon_storage.p_party->species[pokemon_index];
    // printf("%d. %d\n", pokemon_index + 1, species);
}
void printParty(struct pksav_gen2_save *save)
{
    printf("Party:\n");
    for (int i = 0; i < 6; i++)
    {
        printPartyPokemonAtIndex(save, i);
    }
}
void swapPartyPokemonAtIndices(struct pksav_gen2_save *save, int pokemon_index1, int pokemon_index2)
{
    // swap nickname
    char tmp_nickname1[11];
    char tmp_nickname2[11];
    pksav_gen2_import_text(save->pokemon_storage.p_party->nicknames[pokemon_index1], tmp_nickname1, 10);
    pksav_gen2_import_text(save->pokemon_storage.p_party->nicknames[pokemon_index2], tmp_nickname2, 10);
    pksav_gen2_export_text(tmp_nickname2, save->pokemon_storage.p_party->nicknames[pokemon_index1], 10);
    pksav_gen2_export_text(tmp_nickname1, save->pokemon_storage.p_party->nicknames[pokemon_index2], 10);
    save->pokemon_storage.p_party->nicknames[pokemon_index1][strlen(tmp_nickname2)] = 0x50;
    save->pokemon_storage.p_party->nicknames[pokemon_index2][strlen(tmp_nickname1)] = 0x50;

    // swap party
    struct pksav_gen2_party_pokemon tmp_pokemon = save->pokemon_storage.p_party->party[pokemon_index1];
    save->pokemon_storage.p_party->party[pokemon_index1] = save->pokemon_storage.p_party->party[pokemon_index2];
    save->pokemon_storage.p_party->party[pokemon_index2] = tmp_pokemon;

    // swap species
    uint8_t tmp_species = save->pokemon_storage.p_party->species[pokemon_index1];
    save->pokemon_storage.p_party->species[pokemon_index1] = save->pokemon_storage.p_party->species[pokemon_index2];
    save->pokemon_storage.p_party->species[pokemon_index2] = tmp_species;

    // swap otnames
    char tmp_otname1[8];
    char tmp_otname2[8];
    pksav_gen2_import_text(save->pokemon_storage.p_party->otnames[pokemon_index1], tmp_otname1, 7);
    pksav_gen2_import_text(save->pokemon_storage.p_party->otnames[pokemon_index2], tmp_otname2, 7);
    pksav_gen2_export_text(tmp_otname2, save->pokemon_storage.p_party->otnames[pokemon_index1], 7);
    pksav_gen2_export_text(tmp_otname1, save->pokemon_storage.p_party->otnames[pokemon_index2], 7);
    save->pokemon_storage.p_party->otnames[pokemon_index1][strlen(tmp_otname2)] = 0x50;
    save->pokemon_storage.p_party->otnames[pokemon_index2][strlen(tmp_otname1)] = 0x50;
}

void swapPokemonAtIndexBetweenSaves(struct pksav_gen2_save* player1_save, struct pksav_gen2_save* player2_save, int selected_index1, int selected_index2)
{
    // swap nickname
    char tmp_nickname1[11];
    char tmp_nickname2[11];
    pksav_gen2_import_text(player1_save->pokemon_storage.p_party->nicknames[selected_index1], tmp_nickname1, 10);
    pksav_gen2_import_text(player2_save->pokemon_storage.p_party->nicknames[selected_index2], tmp_nickname2, 10);
    pksav_gen2_export_text(tmp_nickname2, player1_save->pokemon_storage.p_party->nicknames[selected_index1], 10);
    pksav_gen2_export_text(tmp_nickname1, player2_save->pokemon_storage.p_party->nicknames[selected_index2], 10);
    player1_save->pokemon_storage.p_party->nicknames[selected_index1][strlen(tmp_nickname2)] = 0x50;
    player2_save->pokemon_storage.p_party->nicknames[selected_index2][strlen(tmp_nickname1)] = 0x50;

    // swap party
    struct pksav_gen2_party_pokemon tmp_pokemon = player1_save->pokemon_storage.p_party->party[selected_index1];
    player1_save->pokemon_storage.p_party->party[selected_index1] = player2_save->pokemon_storage.p_party->party[selected_index2];
    player2_save->pokemon_storage.p_party->party[selected_index2] = tmp_pokemon;

    // swap species
    uint8_t tmp_species = player1_save->pokemon_storage.p_party->species[selected_index1];
    player1_save->pokemon_storage.p_party->species[selected_index1] = player2_save->pokemon_storage.p_party->species[selected_index2];
    player2_save->pokemon_storage.p_party->species[selected_index2] = tmp_species;

    // swap otnames
    char tmp_otname1[8];
    char tmp_otname2[8];
    pksav_gen2_import_text(player1_save->pokemon_storage.p_party->otnames[selected_index1], tmp_otname1, 7);
    pksav_gen2_import_text(player2_save->pokemon_storage.p_party->otnames[selected_index2], tmp_otname2, 7);
    pksav_gen2_export_text(tmp_otname2, player1_save->pokemon_storage.p_party->otnames[selected_index1], 7);
    pksav_gen2_export_text(tmp_otname1, player2_save->pokemon_storage.p_party->otnames[selected_index2], 7);
    player1_save->pokemon_storage.p_party->otnames[selected_index1][strlen(tmp_otname2)] = 0x50;
    player2_save->pokemon_storage.p_party->otnames[selected_index2][strlen(tmp_otname1)] = 0x50;
}

int error_handler(enum pksav_error error, const char *message)
{
    printf("%s\n", message);
    exit(1);
}

int main(int argc, char *argv[])
{
    char *player1_savefile = "../rom/pk-crystal.sav";
    char *player2_savefile = "../rom/pk-crystal_player2.sav";

    struct pksav_gen2_save save_player2 = loadSaveFromFile(player2_savefile, &error_handler);
    printTrainerData(&save_player2);
    printParty(&save_player2);

    struct pksav_gen2_save save_player1 = loadSaveFromFile(player1_savefile, &error_handler);
    printTrainerData(&save_player1);
    printParty(&save_player1);

    swapPokemonAtIndexBetweenSaves(&save_player1, &save_player2, 0, 0);

    printParty(&save_player1);
    printParty(&save_player2);

    if (strcmp(argv[0], "no-write") == 0) {} else saveToFile(&save_player1, player1_savefile, &error_handler);
    if (strcmp(argv[0], "no-write") == 0) {} else saveToFile(&save_player2, player2_savefile, &error_handler);
    return 0;
}
