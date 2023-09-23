#include <pksav.h>
#include <stdio.h>

struct pksav_gen2_save loadSaveFromFile(char *path)
{
    // bool is_gen2 = false;
    // err = pksav_file_is_gen2_save(path, true, &is_gen2);
    // if (!is_gen2)
    // {
    //     printf("Save is not a Gen 2 save file!\n");
    // }
    // if (err != PKSAV_ERROR_NONE)
    // {
    //     printf("Error checking save: %s\n", pksav_strerror(err));
    // }

    struct pksav_gen2_save save;
    pksav_gen2_load_save_from_file(path, &save);
    // if (err != PKSAV_ERROR_NONE)
    // {
    //     printf("Error loading save: %s\n", pksav_strerror(err));
    // }
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

    // print trainer money
    // size_t money = 0;
    // pksav_import_bcd(save.trainer_info.p_money, 3, &money);
    // printf("Trainer money: %zu\n", money);

    struct pksav_gen2_time timePlayed = *save->save_time.p_time_played;
    printf("Time played: %u:%u:%u\n", timePlayed.hours, timePlayed.minutes, timePlayed.seconds);
}
// void changePartyPokemonNicknameAtIndex(pksav_gen2_save_t *save, int pokemon_index, char *new_name)
// {
//     char pokemon_name[11];
//     pksav_text_to_gen2(new_name, save->pokemon_party->nicknames[pokemon_index], 10);
//     pksav_text_from_gen2(save->pokemon_party->nicknames[pokemon_index], pokemon_name, 10);
//     printf("Pokemon new name: %s\n", pokemon_name);
// }
void saveToFile(struct pksav_gen2_save *save, char *path)
{
    pksav_gen2_save_save(path, save);
    pksav_gen2_free_save(save);
}
// void printPartySpeciesAtIndex(pksav_gen2_save_t *save, int pokemon_index)
// {
//     uint8_t species = save->pokemon_party->party[pokemon_index].pc.species;
//     printf("Pokemon species: %u\n", species);
// }
// void printPartyPokemonAtIndex(pksav_gen2_save_t *save, int pokemon_index)
// {
//     // print nickname
//     char pokemon_nickname[11];
//     pksav_text_from_gen2(save->pokemon_party->nicknames[pokemon_index], pokemon_nickname, 10);
//     printf("Pokemon nickname: %s\n", pokemon_nickname);
// }
// void printParty(pksav_gen2_save_t *save)
// {
//     for (int i = 0; i < 6; i++)
//     {
//         printPartyPokemonAtIndex(save, i);
//     }
// }
// void swapPartyPokemonAtIndices(pksav_gen2_save_t *save, int pokemon_index1, int pokemon_index2)
// {
// swap nickname
// char tmp_nickname1[10];
// char tmp_nickname2[10];
// pksav_text_from_gen2(save->pokemon_party->nicknames[pokemon_index1], tmp_nickname1, 10);
// pksav_text_from_gen2(save->pokemon_party->nicknames[pokemon_index2], tmp_nickname2, 10);
// pksav_text_to_gen2(tmp_nickname2, save->pokemon_party->nicknames[pokemon_index1], 10);
// pksav_text_to_gen2(tmp_nickname1, save->pokemon_party->nicknames[pokemon_index2], 10);

// swap party
// pksav_gen2_party_pokemon_t tmp_pokemon = save->pokemon_party->party[pokemon_index1];
// save->pokemon_party->party[pokemon_index1] = save->pokemon_party->party[pokemon_index2];
// save->pokemon_party->party[pokemon_index2] = tmp_pokemon;

// // swap species
// uint8_t tmp_species = save->pokemon_party->species[pokemon_index1];
// save->pokemon_party->species[pokemon_index1] = save->pokemon_party->species[pokemon_index2];
// save->pokemon_party->species[pokemon_index2] = tmp_species;

// swap otnames
// char tmp_otname1[8];
// char tmp_otname2[8];
// pksav_text_from_gen2(save->pokemon_party->otnames[pokemon_index1], tmp_otname1, 7);
// pksav_text_from_gen2(save->pokemon_party->otnames[pokemon_index2], tmp_otname2, 7);
// pksav_text_to_gen2(tmp_otname2, save->pokemon_party->otnames[pokemon_index1], 7);
// pksav_text_to_gen2(tmp_otname1, save->pokemon_party->otnames[pokemon_index2], 7);
// }

int main()
{
    struct pksav_gen2_save save = loadSaveFromFile("../saves/crystal.gbc.sav");
    printTrainerData(&save);

    // if (err != PKSAV_ERROR_NONE)
    // {
    //     printf("Error loading save: %s\n", pksav_strerror(err));
    //     return 1;
    // }
    // printTrainerData(&save);

    // printParty(&save);

    // swapPartyPokemonAtIndices(&save, 0, 1);

    // printParty(&save);

    // Update 1st party pokemon name
    // changePartyPokemonNicknameAtIndex(&save, 0, "GYARADOS");

    // saveToFile(&save, "../rom/pk-crystal.sav");
    // if (err != PKSAV_ERROR_NONE)
    // {
    //     printf("Error saving save: %s\n", pksav_strerror(err));
    //     return 1;
    // }

    return 0;
}
