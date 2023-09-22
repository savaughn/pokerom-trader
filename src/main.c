#include <pksav.h>
#include <stdio.h>

pksav_gen2_save_t loadSaveFromFile(char *path, pksav_error_t err)
{
    bool is_gen2 = false;
    err = pksav_file_is_gen2_save(path, true, &is_gen2);
    if (!is_gen2) {
        printf("Save is not a Gen 2 save file!\n");
    }
    if (err != PKSAV_ERROR_NONE)
    {
        printf("Error checking save: %s\n", pksav_strerror(err));
    }
    
    pksav_gen2_save_t save;
    err = pksav_gen2_save_load(path, &save);
    if (err != PKSAV_ERROR_NONE)
    {
        printf("Error loading save: %s\n", pksav_strerror(err));
    }
    return save;
}

void printTrainerData(pksav_gen2_save_t *save)
{
    // Trainer name
    char player_name[8];
    pksav_text_from_gen2(save->trainer_name, player_name, 7);
    printf("Player name: %s\n", player_name);

    // Trainer ID
    uint16_t trainer_id = pksav_bigendian16(*save->trainer_id);
    printf("Player ID: %u\n", trainer_id);

    // Trainer Money
    uint32_t money = 0;
    pksav_from_bcd(save->money, 3, &money);
    printf("Player money: %u\n", money);

    // Time played
    pksav_gen2_time_t timePlayed = *save->time_played;
    printf("Time played: %d:%2d:%2d\n", timePlayed.hours, timePlayed.minutes, timePlayed.seconds);
}

void changePartyPokemonNicknameAtIndex(pksav_gen2_save_t *save, int pokemon_index, char *new_name)
{
    char pokemon_name[11];
    pksav_text_to_gen2(new_name, save->pokemon_party->nicknames[pokemon_index], 10);
    pksav_text_from_gen2(save->pokemon_party->nicknames[pokemon_index], pokemon_name, 10);
    printf("Pokemon new name: %s\n", pokemon_name);
}

void saveToFile(pksav_gen2_save_t *save, char *path, pksav_error_t err)
{
    pksav_gen2_save_save(path, save);
    pksav_gen2_save_free(save);
}

int main()
{
    pksav_error_t err = PKSAV_ERROR_NONE;
    pksav_gen2_save_t save = loadSaveFromFile("../rom/pk-crystal.sav", err);
    if (err != PKSAV_ERROR_NONE)
    {
        printf("Error loading save: %s\n", pksav_strerror(err));
        return 1;
    }
    printTrainerData(&save);

    // Update 1st party pokemon name
    changePartyPokemonNicknameAtIndex(&save, 0, "Pikachu");

    saveToFile(&save, "../rom/pk-crystal.sav", err);
    if (err != PKSAV_ERROR_NONE)
    {
        printf("Error saving save: %s\n", pksav_strerror(err));
        return 1;
    }

    return 0;
}
