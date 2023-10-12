#include "pksavfilehelper.h"

enum pksav_error detectSaveFileGeneration(const char *path, SaveGenerationType *save_generation_type)
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

PokemonSave loadSaveFromFile(const char *path)
{
    enum pksav_error err = PKSAV_ERROR_NONE;
    PokemonSave pokemon_save;
    SaveGenerationType save_generation_type = SAVE_GENERATION_NONE;

    err = detectSaveFileGeneration(path, &save_generation_type);
    if (err != PKSAV_ERROR_NONE)
    {
        error_handler(err, "Error detecting save file generation");
    }

    pokemon_save.save_generation_type = save_generation_type;

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
        pokemon_save.save.gen1_save = save;
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
        pokemon_save.save.gen2_save = save;
        break;
    }
    default:
        error_handler(PKSAV_ERROR_INVALID_SAVE, "Invalid save generation");
        break;
    }
    return pokemon_save;
}
void saveToFile(PokemonSave *pokemon_save, char *path)
{
    enum pksav_error err = PKSAV_ERROR_NONE;
    if (pokemon_save->save_generation_type == SAVE_GENERATION_1)
    {
        err = pksav_gen1_save_save(path, &pokemon_save->save.gen1_save);
    }
    else
    {
        err = pksav_gen2_save_save(path, &pokemon_save->save.gen2_save);
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
