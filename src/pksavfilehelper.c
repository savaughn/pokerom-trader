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
        // TODO: Error check
        *save_generation_type = SAVE_GENERATION_2;
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
PokemonSave load_savefile_from_path(const char *path)
{
    enum pksav_error err = PKSAV_ERROR_NONE;
    PokemonSave pkmn_save;
    SaveGenerationType save_generation_type = SAVE_GENERATION_NONE;

    err = detect_savefile_generation(path, &save_generation_type);
    if (err != PKSAV_ERROR_NONE)
    {
        error_handler(err, "Error detecting save file generation");
    }

    pkmn_save.save_generation_type = save_generation_type;

    switch (save_generation_type)
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
        pkmn_save.save.gen1_save = save;
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
        pkmn_save.save.gen2_save = save;
        break;
    }
    default:
        error_handler(PKSAV_ERROR_INVALID_SAVE, "Invalid save generation");
        break;
    }
    return pkmn_save;
}
/**
 * @brief saves the save buffer to a path
 * @param pkmn_save a pointer to a PokemonSave struct save buffer
 * @param path the path to the save file
*/
void save_savefile_to_path(PokemonSave *pkmn_save, char *path)
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
}
