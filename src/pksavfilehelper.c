#include "pksavfilehelper.h"

/**
 * @brief detects the save file generation of a save file
 * @param path the path to the save file
 * @param save_generation_type a pointer to a SaveGenerationType to store the save generation type
 * @return an enum pksav_error
 */
enum pksav_error detect_savefile_generation(const char *path, SaveGenerationType *save_generation_type)
{
    enum pksav_error err = PKSAV_ERROR_NONE;

    enum pksav_gen1_save_type gen1_save_type = PKSAV_GEN1_SAVE_TYPE_NONE;
    enum pksav_gen2_save_type gen2_save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
    err = pksav_gen1_get_file_save_type(path, &gen1_save_type);

    if (gen1_save_type == PKSAV_GEN1_SAVE_TYPE_NONE)
    {
        pksav_gen2_get_file_save_type(path, &gen2_save_type);
        if (gen2_save_type == PKSAV_GEN2_SAVE_TYPE_NONE)
        {
            SAVE_FILE_ERROR = PKSAV_ERROR_INVALID_SAVE;
            *save_generation_type = SAVE_GENERATION_CORRUPTED;
            return 0;
        }
        else
        {
            *save_generation_type = SAVE_GENERATION_2;
        }
    }
    else
    {
        *save_generation_type = SAVE_GENERATION_1;
    }

    return err;
}
/**
 * @brief loads a save file from a path in the buffer
 * @param path the path to the save file
 * @return a PokemonSave struct
 */
void load_savefile_from_path(const char *path, PokemonSave *pkmn_save)
{
    enum pksav_error err = PKSAV_ERROR_NONE;
    // PokemonSave pkmn_save;
    pkmn_save->save_generation_type = SAVE_GENERATION_NONE;

    err = detect_savefile_generation(path, &pkmn_save->save_generation_type);
    if (err != PKSAV_ERROR_NONE)
    {
        error_handler(err, "Error detecting save file generation");
    }

    switch (pkmn_save->save_generation_type)
    {
    case SAVE_GENERATION_1:
    {
        enum pksav_gen1_save_type gen1_save_type;
        err = pksav_gen1_get_file_save_type(path, &gen1_save_type);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error getting save type");
        }
        struct pksav_gen1_save save;
        err = pksav_gen1_load_save_from_file(path, &save);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error loading save");
        }
        pkmn_save->save.gen1_save = save;
        break;
    }
    case SAVE_GENERATION_2:
    {
        enum pksav_gen2_save_type gen2_save_type;
        err = pksav_gen2_get_file_save_type(path, &gen2_save_type);
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
        pkmn_save->save.gen2_save = save;
        break;
    }
    default:
        break;
    }
}
/**
 * @brief saves the save buffer to a path
 * @param pkmn_save a pointer to a PokemonSave struct save buffer
 * @param path the path to the save file
 */
pksavhelper_error save_savefile_to_path(PokemonSave *pkmn_save, char *path)
{
    enum pksav_error err = PKSAV_ERROR_NONE;
    if (pkmn_save->save_generation_type == SAVE_GENERATION_1)
    {
        err = pksav_gen1_save_save(path, &pkmn_save->save.gen1_save);
    }
    else
    {
        err = pksav_gen2_save_save(path, &pkmn_save->save.gen2_save);
    }

    if (err != PKSAV_ERROR_NONE)
    {
        error_handler(err, "Error saving save");
    }
    else
    {
        printf("Saved to %s\n", path);
    }

    if (err != PKSAV_ERROR_NONE)
    {
        return error_update_save;
    }
    return error_none;
}

void load_display_files(const struct save_file_data *save_file_data, PokemonSave *pkmn_saves, uint8_t *num_saves)
{
    int allocated_saves = 0;
    long save_file_size = 0;
    // Load save files once
    for (int i = 0; i < save_file_data->num_saves; i++)
    {
        // if not initialized
        if (pkmn_saves[i].save_generation_type == SAVE_GENERATION_NONE)
        {
            load_savefile_from_path(save_file_data->saves_file_path[i], &pkmn_saves[i]);
            if (pkmn_saves[i].save_generation_type != SAVE_GENERATION_NONE)
            {
                allocated_saves++;
                save_file_size += pkmn_saves[i].save_generation_type == SAVE_GENERATION_1 ? sizeof(struct pksav_gen1_save) : sizeof(struct pksav_gen2_save);
            }
            else
            {
                puts("PokeromTrader: Not a pokemon save file");
            }
        }
    }
    if (allocated_saves > 0)
    {
        printf("PokeromTrader: Allocated memory (%ld bytes) for %d save files\n", save_file_size, allocated_saves);
        *num_saves = allocated_saves;
    }
}

void free_pkmn_saves(PokemonSave *pkmn_saves, uint8_t *save_file_count)
{
    if (*save_file_count == 0)
    {
        return;
    }
    uint8_t count = 0;
    printf("PokeromTrader: %u save files had been allocated\n", *save_file_count);
    for (int i = 0; i < *save_file_count; i++)
    {
        switch (pkmn_saves[i].save_generation_type)
        {
        case SAVE_GENERATION_1:
        {
            pksav_gen1_free_save(&pkmn_saves[i].save.gen1_save);
            count++;
            break;
        }
        case SAVE_GENERATION_2:
        {
            pksav_gen2_free_save(&pkmn_saves[i].save.gen2_save);
            count++;
            break;
        }
        default:
            puts("PokeromTrader: No save file to free");
            break;
        }
    }

    // reset save files to uninitialized
    for (int i = 0; i < MAX_FILE_PATH_COUNT; i++)
    {
        pkmn_saves[i].save_generation_type = SAVE_GENERATION_NONE;
    }

    *save_file_count -= count;

    printf("%d save files deallocated\n", count);

    if (*save_file_count != 0)
    {
        printf("PokeromTrader: %d save files were not deallocated\n", *save_file_count);
    }
}
