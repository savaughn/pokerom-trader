#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "common.h"

// Global pointers for resolved_path and absolute_path
extern char *resolved_path;
extern char *absolute_path;

enum E_LANG
{
    EN,
    FR,
    DE,
    IT,
    ES,
    JA,
    KO,
    ZH,
    ZH_TW
};

static enum E_LANG LANG = EN;

// ENGLISH STRINGS
enum string_keys {
    ABOUT_TEXT1,
    ABOUT_TEXT2,
    ABOUT_TEXT3,
    ABOUT_TEXT4,
    ABOUT_TEXT5,
    ABOUT_TEXT6,
    LEGAL_BTN_TEXT,
    BACK_BTN_TEXT,
    FILE_EDIT_TEXT1,
    CLEAR_INPUT_BTN_TEXT,
    SAVE_BTN_TEXT,
    SETTINGS_BTN_TEXT,
    SETTINGS_CHANGE_DIR_BTN_TEXT,
    SETTINGS_DISABLE_RANDOM_DVS_BTN_TEXT,
    BTN_ON_TEXT,
    BTN_OFF_TEXT,
    ITEM_REQUIRED_FOR_EVOLUTION_TEXT,
    ABOUT_POKEROM_TRADER_TEXT,
    MAIN_MENU_TEXT,
    MAIN_MENU_TRADE_TEXT,
    MAIN_MENU_BILLS_PC_TEXT,
    MAIN_MENU_EVOLVE_TEXT,
    MAIN_MENU_QUIT_TEXT,
    FILE_SELECT_TEXT1,
    NO_CROSS_GEN_TEXT,
    NO_SAVE_FILES_FOUND_TEXT,
    SAVE_FOLDER_DOESNT_EXIST_TEXT,
    TRADE_BTN_TEXT,
    LANGUAGE_TEXT
};

static const char* english_strings[] = {
    [ABOUT_TEXT1] = "Pokerom Trader is a tool for trading Pokemon between two save files",
    [ABOUT_TEXT2] = "written by github.com/savaughn 2023",
    [ABOUT_TEXT3] = "Pokerom Trader is open source and licensed under the MIT license",
    [ABOUT_TEXT4] = "Pokerom Trader uses the following libraries:",
    [ABOUT_TEXT5] = "raylib - https://www.raylib.com/",
    [ABOUT_TEXT6] = "pksav - https://github.com/ncorgan/pksav",
    [LEGAL_BTN_TEXT] = "Legal >",
    [BACK_BTN_TEXT] = "< Back",
    [FILE_EDIT_TEXT1] = "Specify folder name containing saves",
    [CLEAR_INPUT_BTN_TEXT] = "Clear input",
    [SAVE_BTN_TEXT] = "Save!",
    [SETTINGS_BTN_TEXT] = "Settings",
    [SETTINGS_CHANGE_DIR_BTN_TEXT] = "Change Save Directory",
    [SETTINGS_DISABLE_RANDOM_DVS_BTN_TEXT] = "Disable random DVs on trade",
    [BTN_ON_TEXT] = "ON",
    [BTN_OFF_TEXT] = "OFF",
    [ITEM_REQUIRED_FOR_EVOLUTION_TEXT] = "Item required for evolution",
    [ABOUT_POKEROM_TRADER_TEXT] = "About Pokerom Trader",
    [MAIN_MENU_TEXT] = "Main Menu",
    [MAIN_MENU_TRADE_TEXT] = "Trade Pokemon",
    [MAIN_MENU_BILLS_PC_TEXT] = "Bill's PC",
    [MAIN_MENU_EVOLVE_TEXT] = "Evolve Pokemon",
    [MAIN_MENU_QUIT_TEXT] = "Quit",
    [FILE_SELECT_TEXT1] = "Select two save files of the same generation to trade between",
    [NO_CROSS_GEN_TEXT] = "Cross-gen trades are not yet supported",
    [NO_SAVE_FILES_FOUND_TEXT] = "No save files found in save folder",
    [SAVE_FOLDER_DOESNT_EXIST_TEXT] = "Save folder doesn't exist!",
    [TRADE_BTN_TEXT] = "Trade >",
    [LANGUAGE_TEXT] = "Language"
};

static const char* german_strings[] = {
    [ABOUT_TEXT1] = "Pokerom Trader ist ein Werkzeug zum Handeln von Pokemon zwischen zwei Speicherdateien",
    [ABOUT_TEXT2] = "geschrieben von github.com/savaughn 2023",
    [ABOUT_TEXT3] = "Pokerom Trader ist Open Source und unter der MIT-Lizenz lizenziert",
    [ABOUT_TEXT4] = "Pokerom Trader verwendet folgende Bibliotheken:",
    [ABOUT_TEXT5] = "raylib - https://www.raylib.com/",
    [ABOUT_TEXT6] = "pksav - https://github.com/ncorgan/pksav",
    [LEGAL_BTN_TEXT] = "Rechtliches >",
    [BACK_BTN_TEXT] = "< Zurück",
    [FILE_EDIT_TEXT1] = "Ordnername mit gespeicherten Daten angeben",
    [CLEAR_INPUT_BTN_TEXT] = "Eingabe löschen",
    [SAVE_BTN_TEXT] = "Speichern!",
    [SETTINGS_BTN_TEXT] = "Einstellungen",
    [SETTINGS_CHANGE_DIR_BTN_TEXT] = "Speicherverzeichnis ändern",
    [SETTINGS_DISABLE_RANDOM_DVS_BTN_TEXT] = "Zufällige DVs deaktivieren",
    [BTN_ON_TEXT] = "AN",
    [BTN_OFF_TEXT] = "AUS",
    [ITEM_REQUIRED_FOR_EVOLUTION_TEXT] = "Gegenstand erforderlich",
    [ABOUT_POKEROM_TRADER_TEXT] = "Über Pokerom Trader",
    [MAIN_MENU_TEXT] = "Hauptmenü",
    [MAIN_MENU_TRADE_TEXT] = "Pokemon handeln",
    [MAIN_MENU_BILLS_PC_TEXT] = "Bill's PC",
    [MAIN_MENU_EVOLVE_TEXT] = "Pokemon entwickeln",
    [MAIN_MENU_QUIT_TEXT] = "Beenden",
    [FILE_SELECT_TEXT1] = "Wählen Sie zwei Speicherdateien derselben Generation aus, um zwischen ihnen zu handeln",
    [NO_CROSS_GEN_TEXT] = "Kreisgen-Handel wird derzeit nicht unterstützt",
    [NO_SAVE_FILES_FOUND_TEXT] = "Keine Speicherdateien im Speicherverzeichnis gefunden",
    [SAVE_FOLDER_DOESNT_EXIST_TEXT] = "Speicherverzeichnis existiert nicht!",
    [TRADE_BTN_TEXT] = "Handel >",
    [LANGUAGE_TEXT] = "Sprache"
};

static const char* french_strings[] = {
    [ABOUT_TEXT1] = "Pokerom Trader est un outil pour échanger des Pokémon entre deux fichiers de sauvegarde",
    [ABOUT_TEXT2] = "écrit par github.com/savaughn 2023",
    [ABOUT_TEXT3] = "Pokerom Trader est open source et sous licence MIT",
    [ABOUT_TEXT4] = "Pokerom Trader utilise les bibliothèques suivantes:",
    [ABOUT_TEXT5] = "raylib - https://www.raylib.com/",
    [ABOUT_TEXT6] = "pksav - https://github.com/ncorgan/pksav",
    [LEGAL_BTN_TEXT] = "Légal >",
    [BACK_BTN_TEXT] = "< Retour",
    [FILE_EDIT_TEXT1] = "Spécifiez le nom du dossier contenant les sauvegardes",
    [CLEAR_INPUT_BTN_TEXT] = "Effacer l'entrée",
    [SAVE_BTN_TEXT] = "Sauvegarder!",
    [SETTINGS_BTN_TEXT] = "Paramètres",
    [SETTINGS_CHANGE_DIR_BTN_TEXT] = "Changer le répertoire de sauvegarde",
    [SETTINGS_DISABLE_RANDOM_DVS_BTN_TEXT] = "Désactiver les DV aléatoires lors de l'échange",
    [BTN_ON_TEXT] = "ON",
    [BTN_OFF_TEXT] = "OFF",
    [ITEM_REQUIRED_FOR_EVOLUTION_TEXT] = "Objet requis pour l'évolution",
    [ABOUT_POKEROM_TRADER_TEXT] = "À propos de Pokerom Trader",
    [MAIN_MENU_TEXT] = "Menu principal",
    [MAIN_MENU_TRADE_TEXT] = "Échanger des Pokémon",
    [MAIN_MENU_BILLS_PC_TEXT] = "PC de Bill",
    [MAIN_MENU_EVOLVE_TEXT] = "Évoluer des Pokémon",
    [MAIN_MENU_QUIT_TEXT] = "Quitter",
    [FILE_SELECT_TEXT1] = "Sélectionnez deux fichiers de sauvegarde de la même génération pour échanger entre eux",
    [NO_CROSS_GEN_TEXT] = "Les échanges entre générations ne sont pas encore pris en charge",
    [NO_SAVE_FILES_FOUND_TEXT] = "Aucun fichier de sauvegarde trouvé dans le dossier de sauvegarde",
    [SAVE_FOLDER_DOESNT_EXIST_TEXT] = "Le dossier de sauvegarde n'existe pas!",
    [TRADE_BTN_TEXT] = "Échange >",
    [LANGUAGE_TEXT] = "Langue"
};

static const char* italian_strings[] = {
    [ABOUT_TEXT1] = "Pokerom Trader è uno strumento per lo scambio di Pokémon tra due file di salvataggio",
    [ABOUT_TEXT2] = "scritto da github.com/savaughn nel 2023",
    [ABOUT_TEXT3] = "Pokerom Trader è open source e con licenza MIT",
    [ABOUT_TEXT4] = "Pokerom Trader utilizza le seguenti librerie:",
    [ABOUT_TEXT5] = "raylib - https://www.raylib.com/",
    [ABOUT_TEXT6] = "pksav - https://github.com/ncorgan/pksav",
    [LEGAL_BTN_TEXT] = "Legale >",
    [BACK_BTN_TEXT] = "< Indietro",
    [FILE_EDIT_TEXT1] = "Specificare il nome della cartella contenente i salvataggi",
    [CLEAR_INPUT_BTN_TEXT] = "Cancella input",
    [SAVE_BTN_TEXT] = "Salva!",
    [SETTINGS_BTN_TEXT] = "Impostazioni",
    [SETTINGS_CHANGE_DIR_BTN_TEXT] = "Cambia Cartella Salvataggi",
    [SETTINGS_DISABLE_RANDOM_DVS_BTN_TEXT] = "Disabilita DV casuali durante lo scambio",
    [BTN_ON_TEXT] = "ACCESO",
    [BTN_OFF_TEXT] = "SPENTO",
    [ITEM_REQUIRED_FOR_EVOLUTION_TEXT] = "Oggetto richiesto per l'evoluzione",
    [ABOUT_POKEROM_TRADER_TEXT] = "Informazioni su Pokerom Trader",
    [MAIN_MENU_TEXT] = "Menu Principale",
    [MAIN_MENU_TRADE_TEXT] = "Scambia Pokémon",
    [MAIN_MENU_BILLS_PC_TEXT] = "PC di Bill",
    [MAIN_MENU_EVOLVE_TEXT] = "Evolvi Pokémon",
    [MAIN_MENU_QUIT_TEXT] = "Esci",
    [FILE_SELECT_TEXT1] = "Seleziona due file di salvataggio della stessa generazione per lo scambio",
    [NO_CROSS_GEN_TEXT] = "Gli scambi tra generazioni diverse non sono ancora supportati",
    [NO_SAVE_FILES_FOUND_TEXT] = "Nessun file di salvataggio trovato nella cartella di salvataggio",
    [SAVE_FOLDER_DOESNT_EXIST_TEXT] = "La cartella di salvataggio non esiste!",
    [TRADE_BTN_TEXT] = "Scambia >",
    [LANGUAGE_TEXT] = "Lingua"
};

static const char* spanish_strings[] = {
    [ABOUT_TEXT1] = "Pokerom Trader es una herramienta para intercambiar Pokémon entre dos archivos de guardado",
    [ABOUT_TEXT2] = "escrito por github.com/savaughn en 2023",
    [ABOUT_TEXT3] = "Pokerom Trader es de código abierto y está licenciado bajo la licencia MIT",
    [ABOUT_TEXT4] = "Pokerom Trader utiliza las siguientes bibliotecas:",
    [ABOUT_TEXT5] = "raylib - https://www.raylib.com/",
    [ABOUT_TEXT6] = "pksav - https://github.com/ncorgan/pksav",
    [LEGAL_BTN_TEXT] = "Legal >",
    [BACK_BTN_TEXT] = "< Atrás",
    [FILE_EDIT_TEXT1] = "Especificar el nombre de la carpeta que contiene las partidas",
    [CLEAR_INPUT_BTN_TEXT] = "Borrar entrada",
    [SAVE_BTN_TEXT] = "Guardar",
    [SETTINGS_BTN_TEXT] = "Configuración",
    [SETTINGS_CHANGE_DIR_BTN_TEXT] = "Cambiar Directorio de Guardado",
    [SETTINGS_DISABLE_RANDOM_DVS_BTN_TEXT] = "Desactivar DVs aleatorios en el intercambio",
    [BTN_ON_TEXT] = "ENCENDIDO",
    [BTN_OFF_TEXT] = "APAGADO",
    [ITEM_REQUIRED_FOR_EVOLUTION_TEXT] = "Objeto requerido para la evolución",
    [ABOUT_POKEROM_TRADER_TEXT] = "Acerca de Pokerom Trader",
    [MAIN_MENU_TEXT] = "Menú Principal",
    [MAIN_MENU_TRADE_TEXT] = "Intercambiar Pokémon",
    [MAIN_MENU_BILLS_PC_TEXT] = "PC de Bill",
    [MAIN_MENU_EVOLVE_TEXT] = "Evolucionar Pokémon",
    [MAIN_MENU_QUIT_TEXT] = "Salir",
    [FILE_SELECT_TEXT1] = "Selecciona dos archivos de guardado de la misma generación para intercambiar entre ellos",
    [NO_CROSS_GEN_TEXT] = "Los intercambios entre generaciones diferentes aún no son compatibles",
    [NO_SAVE_FILES_FOUND_TEXT] = "No se encontraron archivos de guardado en la carpeta de guardado",
    [SAVE_FOLDER_DOESNT_EXIST_TEXT] = "¡La carpeta de guardado no existe!",
    [TRADE_BTN_TEXT] = "Intercambiar >",
    [LANGUAGE_TEXT] = "Idioma"
};

static const char* japanese_strings[] = {
    [ABOUT_TEXT1] = "Pokerom Traderは、2つのセーブファイル間でポケモンを交換するためのツールです",
    [ABOUT_TEXT2] = "github.com/savaughnによって2023年に制作されました",
    [ABOUT_TEXT3] = "Pokerom TraderはオープンソースでMITライセンスの下で提供されています",
    [ABOUT_TEXT4] = "Pokerom Traderは以下のライブラリを使用しています:",
    [ABOUT_TEXT5] = "raylib - https://www.raylib.com/",
    [ABOUT_TEXT6] = "pksav - https://github.com/ncorgan/pksav",
    [LEGAL_BTN_TEXT] = "法的情報 >",
    [BACK_BTN_TEXT] = "< 戻る",
    [FILE_EDIT_TEXT1] = "セーブファイルが格納されているフォルダの名前を指定してください",
    [CLEAR_INPUT_BTN_TEXT] = "クリア",
    [SAVE_BTN_TEXT] = "保存",
    [SETTINGS_BTN_TEXT] = "設定",
    [SETTINGS_CHANGE_DIR_BTN_TEXT] = "セーブフォルダの変更",
    [SETTINGS_DISABLE_RANDOM_DVS_BTN_TEXT] = "ランダムDVの無効化",
    [BTN_ON_TEXT] = "ON",
    [BTN_OFF_TEXT] = "OFF",
    [ITEM_REQUIRED_FOR_EVOLUTION_TEXT] = "進化に必要なアイテム",
    [ABOUT_POKEROM_TRADER_TEXT] = "Pokerom Traderについて",
    [MAIN_MENU_TEXT] = "メインメニュー",
    [MAIN_MENU_TRADE_TEXT] = "ポケモンの交換",
    [MAIN_MENU_BILLS_PC_TEXT] = "ビルのPC",
    [MAIN_MENU_EVOLVE_TEXT] = "ポケモンの進化",
    [MAIN_MENU_QUIT_TEXT] = "終了",
    [FILE_SELECT_TEXT1] = "同じ世代の2つのセーブファイルを選択して交換してください",
    [NO_CROSS_GEN_TEXT] = "異なる世代間の交換はまだサポートされていません",
    [NO_SAVE_FILES_FOUND_TEXT] = "セーブフォルダ内でセーブファイルが見つかりませんでした",
    [SAVE_FOLDER_DOESNT_EXIST_TEXT] = "セーブフォルダは存在しません！",
    [TRADE_BTN_TEXT] = "交換 >",
    [LANGUAGE_TEXT] = "言語"
};

static const char* korean_strings[] = {
    [ABOUT_TEXT1] = "Pokerom Trader는 두 개의 세이브 파일 간에 포켓몬을 교환하는 도구입니다",
    [ABOUT_TEXT2] = "github.com/savaughn에 의해 2023년에 작성되었습니다",
    [ABOUT_TEXT3] = "Pokerom Trader는 오픈 소스이며 MIT 라이선스 하에서 제공됩니다",
    [ABOUT_TEXT4] = "Pokerom Trader는 다음 라이브러리를 사용합니다:",
    [ABOUT_TEXT5] = "raylib - https://www.raylib.com/",
    [ABOUT_TEXT6] = "pksav - https://github.com/ncorgan/pksav",
    [LEGAL_BTN_TEXT] = "법적 정보 >",
    [BACK_BTN_TEXT] = "< 뒤로",
    [FILE_EDIT_TEXT1] = "세이브 파일이 저장된 폴더 이름을 지정하십시오",
    [CLEAR_INPUT_BTN_TEXT] = "입력 지우기",
    [SAVE_BTN_TEXT] = "저장",
    [SETTINGS_BTN_TEXT] = "설정",
    [SETTINGS_CHANGE_DIR_BTN_TEXT] = "세이브 폴더 변경",
    [SETTINGS_DISABLE_RANDOM_DVS_BTN_TEXT] = "교환시 랜덤 DV 비활성화",
    [BTN_ON_TEXT] = "켜짐",
    [BTN_OFF_TEXT] = "꺼짐",
    [ITEM_REQUIRED_FOR_EVOLUTION_TEXT] = "진화를 위한 아이템",
    [ABOUT_POKEROM_TRADER_TEXT] = "Pokerom Trader 정보",
    [MAIN_MENU_TEXT] = "메인 메뉴",
    [MAIN_MENU_TRADE_TEXT] = "포켓몬 교환",
    [MAIN_MENU_BILLS_PC_TEXT] = "빌의 PC",
    [MAIN_MENU_EVOLVE_TEXT] = "포켓몬 진화",
    [MAIN_MENU_QUIT_TEXT] = "종료",
    [FILE_SELECT_TEXT1] = "동일한 세대의 두 개의 세이브 파일을 선택하여 교환하세요",
    [NO_CROSS_GEN_TEXT] = "다른 세대 간 교환이 아직 지원되지 않습니다",
    [NO_SAVE_FILES_FOUND_TEXT] = "세이브 폴더에서 세이브 파일을 찾을 수 없습니다",
    [SAVE_FOLDER_DOESNT_EXIST_TEXT] = "세이브 폴더가 존재하지 않습니다!",
    [TRADE_BTN_TEXT] = "교환 >",
    [LANGUAGE_TEXT] = "언어"
};

static const char* chinese_strings[] = {
    [ABOUT_TEXT1] = "Pokerom Trader是用于在两个存档文件之间交换宝可梦的工具",
    [ABOUT_TEXT2] = "由github.com/savaughn于2023年编写",
    [ABOUT_TEXT3] = "Pokerom Trader是开源的，根据MIT许可证授权",
    [ABOUT_TEXT4] = "Pokerom Trader使用以下库：",
    [ABOUT_TEXT5] = "raylib - https://www.raylib.com/",
    [ABOUT_TEXT6] = "pksav - https://github.com/ncorgan/pksav",
    [LEGAL_BTN_TEXT] = "法律信息 >",
    [BACK_BTN_TEXT] = "< 返回",
    [FILE_EDIT_TEXT1] = "指定包含存档的文件夹名称",
    [CLEAR_INPUT_BTN_TEXT] = "清除输入",
    [SAVE_BTN_TEXT] = "保存！",
    [SETTINGS_BTN_TEXT] = "设置",
    [SETTINGS_CHANGE_DIR_BTN_TEXT] = "更改存档目录",
    [SETTINGS_DISABLE_RANDOM_DVS_BTN_TEXT] = "在交换时禁用随机努力值",
    [BTN_ON_TEXT] = "开",
    [BTN_OFF_TEXT] = "关",
    [ITEM_REQUIRED_FOR_EVOLUTION_TEXT] = "进化所需物品",
    [ABOUT_POKEROM_TRADER_TEXT] = "关于Pokerom Trader",
    [MAIN_MENU_TEXT] = "主菜单",
    [MAIN_MENU_TRADE_TEXT] = "交换宝可梦",
    [MAIN_MENU_BILLS_PC_TEXT] = "比尔的电脑",
    [MAIN_MENU_EVOLVE_TEXT] = "宝可梦进化",
    [MAIN_MENU_QUIT_TEXT] = "退出",
    [FILE_SELECT_TEXT1] = "选择两个相同世代的存档文件以进行交换",
    [NO_CROSS_GEN_TEXT] = "尚不支持世代间交换",
    [NO_SAVE_FILES_FOUND_TEXT] = "在存档文件夹中找不到存档文件",
    [SAVE_FOLDER_DOESNT_EXIST_TEXT] = "存档文件夹不存在！",
    [TRADE_BTN_TEXT] = "交换 >",
    [LANGUAGE_TEXT] = "语言"
};

static const char* chinese_tw_strings[] = {
    [ABOUT_TEXT1] = "Pokerom Trader是用於在兩個儲存檔案之間交換寶可夢的工具",
    [ABOUT_TEXT2] = "由github.com/savaughn於2023年編寫",
    [ABOUT_TEXT3] = "Pokerom Trader是開源的，根據MIT許可證授權",
    [ABOUT_TEXT4] = "Pokerom Trader使用以下庫：",
    [ABOUT_TEXT5] = "raylib - https://www.raylib.com/",
    [ABOUT_TEXT6] = "pksav - https://github.com/ncorgan/pksav",
    [LEGAL_BTN_TEXT] = "法律信息 >",
    [BACK_BTN_TEXT] = "< 返回",
    [FILE_EDIT_TEXT1] = "指定包含儲存檔案的資料夾名稱",
    [CLEAR_INPUT_BTN_TEXT] = "清除輸入",
    [SAVE_BTN_TEXT] = "保存！",
    [SETTINGS_BTN_TEXT] = "設定",
    [SETTINGS_CHANGE_DIR_BTN_TEXT] = "更改儲存檔案目錄",
    [SETTINGS_DISABLE_RANDOM_DVS_BTN_TEXT] = "在交換時禁用隨機努力值",
    [BTN_ON_TEXT] = "開",
    [BTN_OFF_TEXT] = "關",
    [ITEM_REQUIRED_FOR_EVOLUTION_TEXT] = "進化所需物品",
    [ABOUT_POKEROM_TRADER_TEXT] = "關於Pokerom Trader",
    [MAIN_MENU_TEXT] = "主選單",
    [MAIN_MENU_TRADE_TEXT] = "交換寶可夢",
    [MAIN_MENU_BILLS_PC_TEXT] = "比爾的電腦",
    [MAIN_MENU_EVOLVE_TEXT] = "寶可夢進化",
    [MAIN_MENU_QUIT_TEXT] = "退出",
    [FILE_SELECT_TEXT1] = "選擇兩個相同世代的儲存檔案進行交換",
    [NO_CROSS_GEN_TEXT] = "尚不支援世代間交換",
    [NO_SAVE_FILES_FOUND_TEXT] = "儲存檔案夾中找不到儲存檔案",
    [SAVE_FOLDER_DOESNT_EXIST_TEXT] = "儲存檔案夾不存在！",
    [TRADE_BTN_TEXT] = "交換 >",
    [LANGUAGE_TEXT] = "語言"
};


// Function to get save files in a directory
int get_save_files(struct SaveFileData *save_data);

// free global pointers
void free_filehelper_pointers(void);
int write_key_to_config(const char *key, const char *value);
char *read_key_from_config(const char *key);
const char* t(uint16_t key);
void set_language(uint8_t lang);
enum E_LANG get_language(void);

#endif /* FILEHELPER_H */
