#ifndef PKMNSTATS_H
#define PKMNSTATS_H

#include <pksav/gen1/type.h>
typedef unsigned char uint8_t;

#define GEN2_FRIENDSHIP_BASE (uint8_t)70

// Gen 1 species index doesn't match pokedex
#define SI_RHYDON (uint8_t)1
#define SI_KANGASKHAN (uint8_t)2
#define SI_NIDORAN_M (uint8_t)3
#define SI_CLEFAIRY (uint8_t)4
#define SI_SPEAROW (uint8_t)5
#define SI_VOLTORB (uint8_t)6
#define SI_NIDOKING (uint8_t)7
#define SI_SLOWBRO (uint8_t)8
#define SI_IVYSAUR (uint8_t)9
#define SI_EXEGGUTOR (uint8_t)10
#define SI_LICKITUNG (uint8_t)11
#define SI_EXEGGCUTE (uint8_t)12
#define SI_GRIMER (uint8_t)13
#define SI_GENGAR (uint8_t)14
#define SI_NIDORAN_F (uint8_t)15
#define SI_NIDOQUEEN (uint8_t)16
#define SI_CUBONE (uint8_t)17
#define SI_RHYHORN (uint8_t)18
#define SI_LAPRAS (uint8_t)19
#define SI_ARCANINE (uint8_t)20
#define SI_MEW (uint8_t)21
#define SI_GYARADOS (uint8_t)22
#define SI_SHELLDER (uint8_t)23
#define SI_TENTACOOL (uint8_t)24
#define SI_GASTLY (uint8_t)25
#define SI_SCYTHER (uint8_t)26
#define SI_STARYU (uint8_t)27
#define SI_BLASTOISE (uint8_t)28
#define SI_PINSIR (uint8_t)29
#define SI_TANGELA (uint8_t)30
#define SI_GROWLITHE (uint8_t)33
#define SI_ONIX (uint8_t)34
#define SI_FEAROW (uint8_t)35
#define SI_PIDGEY (uint8_t)36
#define SI_SLOWPOKE (uint8_t)37
#define SI_KADABRA (uint8_t)38
#define SI_GRAVELER (uint8_t)39
#define SI_CHANSEY (uint8_t)40
#define SI_MACHOKE (uint8_t)41
#define SI_MRMIME (uint8_t)42
#define SI_HITMONLEE (uint8_t)43
#define SI_HITMONCHAN (uint8_t)44
#define SI_ARBOK (uint8_t)45
#define SI_PARASECT (uint8_t)46
#define SI_PSYDUCK (uint8_t)47
#define SI_DROWZEE (uint8_t)48
#define SI_GOLEM (uint8_t)49
#define SI_MAGMAR (uint8_t)51
#define SI_ELECTABUZZ (uint8_t)53
#define SI_MAGNETON (uint8_t)54
#define SI_KOFFING (uint8_t)55
#define SI_MISSINGNO5 (uint8_t)56
#define SI_MANKEY (uint8_t)57
#define SI_SEEL (uint8_t)58
#define SI_DIGLETT (uint8_t)59
#define SI_TAUROS (uint8_t)60
#define SI_MISSINGNO6 (uint8_t)61
#define SI_MISSINGNO7 (uint8_t)62
#define SI_MISSINGNO8 (uint8_t)63
#define SI_FARFETCHD (uint8_t)64
#define SI_VENONAT (uint8_t)65
#define SI_DRAGONITE (uint8_t)66
#define SI_DODUO (uint8_t)70
#define SI_POLIWAG (uint8_t)71
#define SI_JYNX (uint8_t)72
#define SI_MOLTRES (uint8_t)73
#define SI_ARTICUNO (uint8_t)74
#define SI_ZAPDOS (uint8_t)75
#define SI_DITTO (uint8_t)76
#define SI_MEOWTH (uint8_t)77
#define SI_KRABBY (uint8_t)78
#define SI_VULPIX (uint8_t)82
#define SI_NINETALES (uint8_t)83
#define SI_PIKACHU (uint8_t)84
#define SI_RAICHU (uint8_t)85
#define SI_DRATINI (uint8_t)88
#define SI_DRAGONAIR (uint8_t)89
#define SI_KABUTO (uint8_t)90
#define SI_KABUTOPS (uint8_t)91
#define SI_HORSEA (uint8_t)92
#define SI_SEADRA (uint8_t)93
#define SI_SANDSHREW (uint8_t)96
#define SI_SANDSLASH (uint8_t)97
#define SI_OMANYTE (uint8_t)98
#define SI_OMASTAR (uint8_t)99
#define SI_JIGGLYPUFF (uint8_t)100
#define SI_WIGGLYTUFF (uint8_t)101
#define SI_EEVEE (uint8_t)102
#define SI_FLAREON (uint8_t)103
#define SI_JOLTEON (uint8_t)104
#define SI_VAPOREON (uint8_t)105
#define SI_MACHOP (uint8_t)106
#define SI_ZUBAT (uint8_t)107
#define SI_EKANS (uint8_t)108
#define SI_PARAS (uint8_t)109
#define SI_POLIWHIRL (uint8_t)110
#define SI_POLIWRATH (uint8_t)111
#define SI_WEEDLE (uint8_t)112
#define SI_KAKUNA (uint8_t)113
#define SI_BEEDRILL (uint8_t)114
#define SI_DODRIO (uint8_t)116
#define SI_PRIMEAPE (uint8_t)117
#define SI_DUGTRIO (uint8_t)118
#define SI_VENOMOTH (uint8_t)119
#define SI_DEWGONG (uint8_t)120
#define SI_CATERPIE (uint8_t)123
#define SI_METAPOD (uint8_t)124
#define SI_BUTTERFREE (uint8_t)125
#define SI_MACHAMP (uint8_t)126
#define SI_MISSINGNO22 (uint8_t)127
#define SI_GOLDUCK (uint8_t)128
#define SI_HYPNO (uint8_t)129
#define SI_GOLBAT (uint8_t)130
#define SI_MEWTWO (uint8_t)131
#define SI_SNORLAX (uint8_t)132
#define SI_MAGIKARP (uint8_t)133
#define SI_MUK (uint8_t)136
#define SI_KINGLER (uint8_t)138
#define SI_CLOYSTER (uint8_t)139
#define SI_MISSINGNO26 (uint8_t)140
#define SI_ELECTRODE (uint8_t)141
#define SI_CLEFABLE (uint8_t)142
#define SI_WEEZING (uint8_t)143
#define SI_PERSIAN (uint8_t)144
#define SI_MAROWAK (uint8_t)145
#define SI_HAUNTER (uint8_t)147
#define SI_ABRA (uint8_t)148
#define SI_ALAKAZAM (uint8_t)149
#define SI_PIDGEOTTO (uint8_t)150
#define SI_PIDGEOT (uint8_t)151
#define SI_STARMIE (uint8_t)152
#define SI_BULBASAUR (uint8_t)153
#define SI_VENUSAUR (uint8_t)154
#define SI_TENTACRUEL (uint8_t)155
#define SI_MISSINGNO28 (uint8_t)156
#define SI_GOLDEEN (uint8_t)157
#define SI_SEAKING (uint8_t)158
#define SI_PONYTA (uint8_t)163
#define SI_RAPIDASH (uint8_t)164
#define SI_RATTATA (uint8_t)165
#define SI_RATICATE (uint8_t)166
#define SI_NIDORINO (uint8_t)167
#define SI_NIDORINA (uint8_t)168
#define SI_GEODUDE (uint8_t)169
#define SI_PORYGON (uint8_t)170
#define SI_AERODACTYL (uint8_t)171
#define SI_MAGNEMITE (uint8_t)173
#define SI_CHARMANDER (uint8_t)176
#define SI_SQUIRTLE (uint8_t)177
#define SI_CHARMELEON (uint8_t)178
#define SI_WARTORTLE (uint8_t)179
#define SI_CHARIZARD (uint8_t)180
#define SI_ODDISH (uint8_t)184
#define SI_GLOOM (uint8_t)185
#define SI_VILEPLUME (uint8_t)186
#define SI_BELLSPROUT (uint8_t)187
#define SI_WEEPINBELL (uint8_t)188
#define SI_VICTREEBEL (uint8_t)189

// Gen 2 all pokemon index matches pokedex
#define BULBASAUR (uint8_t)1
#define IVYSAUR (uint8_t)2
#define VENUSAUR (uint8_t)3
#define CHARMANDER (uint8_t)4
#define CHARMELEON (uint8_t)5
#define CHARIZARD (uint8_t)6
#define SQUIRTLE (uint8_t)7
#define WARTORTLE (uint8_t)8
#define BLASTOISE (uint8_t)9
#define CATERPIE (uint8_t)10
#define METAPOD (uint8_t)11
#define BUTTERFREE (uint8_t)12
#define WEEDLE (uint8_t)13
#define KAKUNA (uint8_t)14
#define BEEDRILL (uint8_t)15
#define PIDGEY (uint8_t)16
#define PIDGEOTTO (uint8_t)17
#define PIDGEOT (uint8_t)18
#define RATTATA (uint8_t)19
#define RATICATE (uint8_t)20
#define SPEAROW (uint8_t)21
#define FEAROW (uint8_t)22
#define EKANS (uint8_t)23
#define ARBOK (uint8_t)24
#define PIKACHU (uint8_t)25
#define RAICHU (uint8_t)26
#define SANDSHREW (uint8_t)27
#define SANDSLASH (uint8_t)28
#define NIDORAN_F (uint8_t)29
#define NIDORINA (uint8_t)30
#define NIDOQUEEN (uint8_t)31
#define NIDORAN_M (uint8_t)32
#define NIDORINO (uint8_t)33
#define NIDOKING (uint8_t)34
#define CLEFAIRY (uint8_t)35
#define CLEFABLE (uint8_t)36
#define VULPIX (uint8_t)37
#define NINETALES (uint8_t)38
#define JIGGLYPUFF (uint8_t)39
#define WIGGLYTUFF (uint8_t)40
#define ZUBAT (uint8_t)41
#define GOLBAT (uint8_t)42
#define ODDISH (uint8_t)43
#define GLOOM (uint8_t)44
#define VILEPLUME (uint8_t)45
#define PARAS (uint8_t)46
#define PARASECT (uint8_t)47
#define VENONAT (uint8_t)48
#define VENOMOTH (uint8_t)49
#define DIGLETT (uint8_t)50
#define DUGTRIO (uint8_t)51
#define MEOWTH (uint8_t)52
#define PERSIAN (uint8_t)53
#define PSYDUCK (uint8_t)54
#define GOLDUCK (uint8_t)55
#define MANKEY (uint8_t)56
#define PRIMEAPE (uint8_t)57
#define GROWLITHE (uint8_t)58
#define ARCANINE (uint8_t)59
#define POLIWAG (uint8_t)60
#define POLIWHIRL (uint8_t)61
#define POLIWRATH (uint8_t)62
#define ABRA (uint8_t)63
#define KADABRA (uint8_t)64
#define ALAKAZAM (uint8_t)65
#define MACHOP (uint8_t)66
#define MACHOKE (uint8_t)67
#define MACHAMP (uint8_t)68
#define BELLSPROUT (uint8_t)69
#define WEEPINBELL (uint8_t)70
#define VICTREEBEL (uint8_t)71
#define TENTACOOL (uint8_t)72
#define TENTACRUEL (uint8_t)73
#define GEODUDE (uint8_t)74
#define GRAVELER (uint8_t)75
#define GOLEM (uint8_t)76
#define PONYTA (uint8_t)77
#define RAPIDASH (uint8_t)78
#define SLOWPOKE (uint8_t)79
#define SLOWBRO (uint8_t)80
#define MAGNEMITE (uint8_t)81
#define MAGNETON (uint8_t)82
#define FARFETCHD (uint8_t)83
#define DODUO (uint8_t)84
#define DODRIO (uint8_t)85
#define SEEL (uint8_t)86
#define DEWGONG (uint8_t)87
#define GRIMER (uint8_t)88
#define MUK (uint8_t)89
#define SHELLDER (uint8_t)90
#define CLOYSTER (uint8_t)91
#define GASTLY (uint8_t)92
#define HAUNTER (uint8_t)93
#define GENGAR (uint8_t)94
#define ONIX (uint8_t)95
#define DROWZEE (uint8_t)96
#define HYPNO (uint8_t)97
#define KRABBY (uint8_t)98
#define KINGLER (uint8_t)99
#define VOLTORB (uint8_t)100
#define ELECTRODE (uint8_t)101
#define EXEGGCUTE (uint8_t)102
#define EXEGGUTOR (uint8_t)103
#define CUBONE (uint8_t)104
#define MAROWAK (uint8_t)105
#define HITMONLEE (uint8_t)106
#define HITMONCHAN (uint8_t)107
#define LICKITUNG (uint8_t)108
#define KOFFING (uint8_t)109
#define WEEZING (uint8_t)110
#define RHYHORN (uint8_t)111
#define RHYDON (uint8_t)112
#define CHANSEY (uint8_t)113
#define TANGELA (uint8_t)114
#define KANGASKHAN (uint8_t)115
#define HORSEA (uint8_t)116
#define SEADRA (uint8_t)117
#define GOLDEEN (uint8_t)118
#define SEAKING (uint8_t)119
#define STARYU (uint8_t)120
#define STARMIE (uint8_t)121
#define MRMIME (uint8_t)122
#define SCYTHER (uint8_t)123
#define JYNX (uint8_t)124
#define ELECTABUZZ (uint8_t)125
#define MAGMAR (uint8_t)126
#define PINSIR (uint8_t)127
#define TAUROS (uint8_t)128
#define MAGIKARP (uint8_t)129
#define GYARADOS (uint8_t)130
#define LAPRAS (uint8_t)131
#define DITTO (uint8_t)132
#define EEVEE (uint8_t)133
#define VAPOREON (uint8_t)134
#define JOLTEON (uint8_t)135
#define FLAREON (uint8_t)136
#define PORYGON (uint8_t)137
#define OMANYTE (uint8_t)138
#define OMASTAR (uint8_t)139
#define KABUTO (uint8_t)140
#define KABUTOPS (uint8_t)141
#define AERODACTYL (uint8_t)142
#define SNORLAX (uint8_t)143
#define ARTICUNO (uint8_t)144
#define ZAPDOS (uint8_t)145
#define MOLTRES (uint8_t)146
#define DRATINI (uint8_t)147
#define DRAGONAIR (uint8_t)148
#define DRAGONITE (uint8_t)149
#define MEWTWO (uint8_t)150
#define MEW (uint8_t)151
#define CHIKORITA (uint8_t)152
#define BAYLEEF (uint8_t)153
#define MEGANIUM (uint8_t)154
#define CYNDAQUIL (uint8_t)155
#define QUILAVA (uint8_t)156
#define TYPHLOSION (uint8_t)157
#define TOTODILE (uint8_t)158
#define CROCONAW (uint8_t)159
#define FERALIGATR (uint8_t)160
#define SENTRET (uint8_t)161
#define FURRET (uint8_t)162
#define HOOTHOOT (uint8_t)163
#define NOCTOWL (uint8_t)164
#define LEDYBA (uint8_t)165
#define LEDIAN (uint8_t)166
#define SPINARAK (uint8_t)167
#define ARIADOS (uint8_t)168
#define CROBAT (uint8_t)169
#define CHINCHOU (uint8_t)170
#define LANTURN (uint8_t)171
#define PICHU (uint8_t)172
#define CLEFFA (uint8_t)173
#define IGGLYBUFF (uint8_t)174
#define TOGEPI (uint8_t)175
#define TOGETIC (uint8_t)176
#define NATU (uint8_t)177
#define XATU (uint8_t)178
#define MAREEP (uint8_t)179
#define FLAAFFY (uint8_t)180
#define AMPHAROS (uint8_t)181
#define BELLOSSOM (uint8_t)182
#define MARILL (uint8_t)183
#define AZUMARILL (uint8_t)184
#define SUDOWOODO (uint8_t)185
#define POLITOED (uint8_t)186
#define HOPPIP (uint8_t)187
#define SKIPLOOM (uint8_t)188
#define JUMPLUFF (uint8_t)189
#define AIPOM (uint8_t)190
#define SUNKERN (uint8_t)191
#define SUNFLORA (uint8_t)192
#define YANMA (uint8_t)193
#define WOOPER (uint8_t)194
#define QUAGSIRE (uint8_t)195
#define ESPEON (uint8_t)196
#define UMBREON (uint8_t)197
#define MURKROW (uint8_t)198
#define SLOWKING (uint8_t)199
#define MISDREAVUS (uint8_t)200
#define UNOWN (uint8_t)201
#define WOBBUFFET (uint8_t)202
#define GIRAFARIG (uint8_t)203
#define PINECO (uint8_t)204
#define FORRETRESS (uint8_t)205
#define DUNSPARCE (uint8_t)206
#define GLIGAR (uint8_t)207
#define STEELIX (uint8_t)208
#define SNUBBULL (uint8_t)209
#define GRANBULL (uint8_t)210
#define QWILFISH (uint8_t)211
#define SCIZOR (uint8_t)212
#define SHUCKLE (uint8_t)213
#define HERACROSS (uint8_t)214
#define SNEASEL (uint8_t)215
#define TEDDIURSA (uint8_t)216
#define URSARING (uint8_t)217
#define SLUGMA (uint8_t)218
#define MAGCARGO (uint8_t)219
#define SWINUB (uint8_t)220
#define PILOSWINE (uint8_t)221
#define CORSOLA (uint8_t)222
#define REMORAID (uint8_t)223
#define OCTILLERY (uint8_t)224
#define DELIBIRD (uint8_t)225
#define MANTINE (uint8_t)226
#define SKARMORY (uint8_t)227
#define HOUNDOUR (uint8_t)228
#define HOUNDOOM (uint8_t)229
#define KINGDRA (uint8_t)230
#define PHANPY (uint8_t)231
#define DONPHAN (uint8_t)232
#define PORYGON2 (uint8_t)233
#define STANTLER (uint8_t)234
#define SMEARGLE (uint8_t)235
#define TYROGUE (uint8_t)236
#define HITMONTOP (uint8_t)237
#define SMOOCHUM (uint8_t)238
#define ELEKID (uint8_t)239
#define MAGBY (uint8_t)240
#define MILTANK (uint8_t)241
#define BLISSEY (uint8_t)242
#define RAIKOU (uint8_t)243
#define ENTEI (uint8_t)244
#define SUICUNE (uint8_t)245
#define LARVITAR (uint8_t)246
#define PUPITAR (uint8_t)247
#define TYRANITAR (uint8_t)248
#define LUGIA (uint8_t)249
#define HO_OH (uint8_t)250
#define CELEBI (uint8_t)251

static const uint8_t species_gen1_to_gen2[SI_VICTREEBEL + 1] = {
    [SI_BULBASAUR] = BULBASAUR,
    [SI_IVYSAUR] = IVYSAUR,
    [SI_VENUSAUR] = VENUSAUR,
    [SI_CHARMANDER] = CHARMANDER,
    [SI_CHARMELEON] = CHARMELEON,
    [SI_CHARIZARD] = CHARIZARD,
    [SI_SQUIRTLE] = SQUIRTLE,
    [SI_WARTORTLE] = WARTORTLE,
    [SI_BLASTOISE] = BLASTOISE,
    [SI_CATERPIE] = CATERPIE,
    [SI_METAPOD] = METAPOD,
    [SI_BUTTERFREE] = BUTTERFREE,
    [SI_WEEDLE] = WEEDLE,
    [SI_KAKUNA] = KAKUNA,
    [SI_BEEDRILL] = BEEDRILL,
    [SI_PIDGEY] = PIDGEY,
    [SI_PIDGEOTTO] = PIDGEOTTO,
    [SI_PIDGEOT] = PIDGEOT,
    [SI_RATTATA] = RATTATA,
    [SI_RATICATE] = RATICATE,
    [SI_SPEAROW] = SPEAROW,
    [SI_FEAROW] = FEAROW,
    [SI_EKANS] = EKANS,
    [SI_ARBOK] = ARBOK,
    [SI_PIKACHU] = PIKACHU,
    [SI_RAICHU] = RAICHU,
    [SI_SANDSHREW] = SANDSHREW,
    [SI_SANDSLASH] = SANDSLASH,
    [SI_NIDORAN_F] = NIDORAN_F,
    [SI_NIDORINA] = NIDORINA,
    [SI_NIDOQUEEN] = NIDOQUEEN,
    [SI_NIDORAN_M] = NIDORAN_M,
    [SI_NIDORINO] = NIDORINO,
    [SI_NIDOKING] = NIDOKING,
    [SI_CLEFAIRY] = CLEFAIRY,
    [SI_CLEFABLE] = CLEFABLE,
    [SI_VULPIX] = VULPIX,
    [SI_NINETALES] = NINETALES,
    [SI_JIGGLYPUFF] = JIGGLYPUFF,
    [SI_WIGGLYTUFF] = WIGGLYTUFF,
    [SI_ZUBAT] = ZUBAT,
    [SI_GOLBAT] = GOLBAT,
    [SI_ODDISH] = ODDISH,
    [SI_GLOOM] = GLOOM,
    [SI_VILEPLUME] = VILEPLUME,
    [SI_PARAS] = PARAS,
    [SI_PARASECT] = PARASECT,
    [SI_VENONAT] = VENONAT,
    [SI_VENOMOTH] = VENOMOTH,
    [SI_DIGLETT] = DIGLETT,
    [SI_DUGTRIO] = DUGTRIO,
    [SI_MEOWTH] = MEOWTH,
    [SI_PERSIAN] = PERSIAN,
    [SI_PSYDUCK] = PSYDUCK,
    [SI_GOLDUCK] = GOLDUCK,
    [SI_MANKEY] = MANKEY,
    [SI_PRIMEAPE] = PRIMEAPE,
    [SI_GROWLITHE] = GROWLITHE,
    [SI_ARCANINE] = ARCANINE,
    [SI_POLIWAG] = POLIWAG,
    [SI_POLIWHIRL] = POLIWHIRL,
    [SI_POLIWRATH] = POLIWRATH,
    [SI_ABRA] = ABRA,
    [SI_KADABRA] = KADABRA,
    [SI_ALAKAZAM] = ALAKAZAM,
    [SI_MACHOP] = MACHOP,
    [SI_MACHOKE] = MACHOKE,
    [SI_MACHAMP] = MACHAMP,
    [SI_BELLSPROUT] = BELLSPROUT,
    [SI_WEEPINBELL] = WEEPINBELL,
    [SI_VICTREEBEL] = VICTREEBEL,
    [SI_TENTACOOL] = TENTACOOL,
    [SI_TENTACRUEL] = TENTACRUEL,
    [SI_GEODUDE] = GEODUDE,
    [SI_GRAVELER] = GRAVELER,
    [SI_GOLEM] = GOLEM,
    [SI_PONYTA] = PONYTA,
    [SI_RAPIDASH] = RAPIDASH,
    [SI_SLOWPOKE] = SLOWPOKE,
    [SI_SLOWBRO] = SLOWBRO,
    [SI_MAGNEMITE] = MAGNEMITE,
    [SI_MAGNETON] = MAGNETON,
    [SI_FARFETCHD] = FARFETCHD,
    [SI_DODUO] = DODUO,
    [SI_DODRIO] = DODRIO,
    [SI_SEEL] = SEEL,
    [SI_DEWGONG] = DEWGONG,
    [SI_GRIMER] = GRIMER,
    [SI_MUK] = MUK,
    [SI_SHELLDER] = SHELLDER,
    [SI_CLOYSTER] = CLOYSTER,
    [SI_GASTLY] = GASTLY,
    [SI_HAUNTER] = HAUNTER,
    [SI_GENGAR] = GENGAR,
    [SI_ONIX] = ONIX,
    [SI_DROWZEE] = DROWZEE,
    [SI_HYPNO] = HYPNO,
    [SI_KRABBY] = KRABBY,
    [SI_KINGLER] = KINGLER,
    [SI_VOLTORB] = VOLTORB,
    [SI_ELECTRODE] = ELECTRODE,
    [SI_EXEGGCUTE] = EXEGGCUTE,
    [SI_EXEGGUTOR] = EXEGGUTOR,
    [SI_CUBONE] = CUBONE,
    [SI_MAROWAK] = MAROWAK,
    [SI_HITMONLEE] = HITMONLEE,
    [SI_HITMONCHAN] = HITMONCHAN,
    [SI_LICKITUNG] = LICKITUNG,
    [SI_KOFFING] = KOFFING,
    [SI_WEEZING] = WEEZING,
    [SI_RHYHORN] = RHYHORN,
    [SI_RHYDON] = RHYDON,
    [SI_CHANSEY] = CHANSEY,
    [SI_TANGELA] = TANGELA,
    [SI_KANGASKHAN] = KANGASKHAN,
    [SI_HORSEA] = HORSEA,
    [SI_SEADRA] = SEADRA,
    [SI_GOLDEEN] = GOLDEEN,
    [SI_SEAKING] = SEAKING,
    [SI_STARYU] = STARYU,
    [SI_STARMIE] = STARMIE,
    [SI_MRMIME] = MRMIME,
    [SI_SCYTHER] = SCYTHER,
    [SI_JYNX] = JYNX,
    [SI_ELECTABUZZ] = ELECTABUZZ,
    [SI_MAGMAR] = MAGMAR,
    [SI_PINSIR] = PINSIR,
    [SI_TAUROS] = TAUROS,
    [SI_MAGIKARP] = MAGIKARP,
    [SI_GYARADOS] = GYARADOS,
    [SI_LAPRAS] = LAPRAS,
    [SI_DITTO] = DITTO,
    [SI_EEVEE] = EEVEE,
    [SI_VAPOREON] = VAPOREON,
    [SI_JOLTEON] = JOLTEON,
    [SI_FLAREON] = FLAREON,
    [SI_PORYGON] = PORYGON,
    [SI_OMANYTE] = OMANYTE,
    [SI_OMASTAR] = OMASTAR,
    [SI_KABUTO] = KABUTO,
    [SI_KABUTOPS] = KABUTOPS,
    [SI_AERODACTYL] = AERODACTYL,
    [SI_SNORLAX] = SNORLAX,
    [SI_ARTICUNO] = ARTICUNO,
    [SI_ZAPDOS] = ZAPDOS,
    [SI_MOLTRES] = MOLTRES,
    [SI_DRATINI] = DRATINI,
    [SI_DRAGONAIR] = DRAGONAIR,
    [SI_DRAGONITE] = DRAGONITE,
    [SI_MEWTWO] = MEWTWO,
    [SI_MEW] = MEW
};

static const uint8_t species_gen2_to_gen1[MEW + 1] = {
    [BULBASAUR] = SI_BULBASAUR,
    [IVYSAUR] = SI_IVYSAUR,
    [VENUSAUR] = SI_VENUSAUR,
    [CHARMANDER] = SI_CHARMANDER,
    [CHARMELEON] = SI_CHARMELEON,
    [CHARIZARD] = SI_CHARIZARD,
    [SQUIRTLE] = SI_SQUIRTLE,
    [WARTORTLE] = SI_WARTORTLE,
    [BLASTOISE] = SI_BLASTOISE,
    [CATERPIE] = SI_CATERPIE,
    [METAPOD] = SI_METAPOD,
    [BUTTERFREE] = SI_BUTTERFREE,
    [WEEDLE] = SI_WEEDLE,
    [KAKUNA] = SI_KAKUNA,
    [BEEDRILL] = SI_BEEDRILL,
    [PIDGEY] = SI_PIDGEY,
    [PIDGEOTTO] = SI_PIDGEOTTO,
    [PIDGEOT] = SI_PIDGEOT,
    [RATTATA] = SI_RATTATA,
    [RATICATE] = SI_RATICATE,
    [SPEAROW] = SI_SPEAROW,
    [FEAROW] = SI_FEAROW,
    [EKANS] = SI_EKANS,
    [ARBOK] = SI_ARBOK,
    [PIKACHU] = SI_PIKACHU,
    [RAICHU] = SI_RAICHU,
    [SANDSHREW] = SI_SANDSHREW,
    [SANDSLASH] = SI_SANDSLASH,
    [NIDORAN_F] = SI_NIDORAN_F,
    [NIDORINA] = SI_NIDORINA,
    [NIDOQUEEN] = SI_NIDOQUEEN,
    [NIDORAN_M] = SI_NIDORAN_M,
    [NIDORINO] = SI_NIDORINO,
    [NIDOKING] = SI_NIDOKING,
    [CLEFAIRY] = SI_CLEFAIRY,
    [CLEFABLE] = SI_CLEFABLE,
    [VULPIX] = SI_VULPIX,
    [NINETALES] = SI_NINETALES,
    [JIGGLYPUFF] = SI_JIGGLYPUFF,
    [WIGGLYTUFF] = SI_WIGGLYTUFF,
    [ZUBAT] = SI_ZUBAT,
    [GOLBAT] = SI_GOLBAT,
    [ODDISH] = SI_ODDISH,
    [GLOOM] = SI_GLOOM,
    [VILEPLUME] = SI_VILEPLUME,
    [PARAS] = SI_PARAS,
    [PARASECT] = SI_PARASECT,
    [VENONAT] = SI_VENONAT,
    [VENOMOTH] = SI_VENOMOTH,
    [DIGLETT] = SI_DIGLETT,
    [DUGTRIO] = SI_DUGTRIO,
    [MEOWTH] = SI_MEOWTH,
    [PERSIAN] = SI_PERSIAN,
    [PSYDUCK] = SI_PSYDUCK,
    [GOLDUCK] = SI_GOLDUCK,
    [MANKEY] = SI_MANKEY,
    [PRIMEAPE] = SI_PRIMEAPE,
    [GROWLITHE] = SI_GROWLITHE,
    [ARCANINE] = SI_ARCANINE,
    [POLIWAG] = SI_POLIWAG,
    [POLIWHIRL] = SI_POLIWHIRL,
    [POLIWRATH] = SI_POLIWRATH,
    [ABRA] = SI_ABRA,
    [KADABRA] = SI_KADABRA,
    [ALAKAZAM] = SI_ALAKAZAM,
    [MACHOP] = SI_MACHOP,
    [MACHOKE] = SI_MACHOKE,
    [MACHAMP] = SI_MACHAMP,
    [BELLSPROUT] = SI_BELLSPROUT,
    [WEEPINBELL] = SI_WEEPINBELL,
    [VICTREEBEL] = SI_VICTREEBEL,
    [TENTACOOL] = SI_TENTACOOL,
    [TENTACRUEL] = SI_TENTACRUEL,
    [GEODUDE] = SI_GEODUDE,
    [GRAVELER] = SI_GRAVELER,
    [GOLEM] = SI_GOLEM,
    [PONYTA] = SI_PONYTA,
    [RAPIDASH] = SI_RAPIDASH,
    [SLOWPOKE] = SI_SLOWPOKE,
    [SLOWBRO] = SI_SLOWBRO,
    [MAGNEMITE] = SI_MAGNEMITE,
    [MAGNETON] = SI_MAGNETON,
    [FARFETCHD] = SI_FARFETCHD,
    [DODUO] = SI_DODUO,
    [DODRIO] = SI_DODRIO,
    [SEEL] = SI_SEEL,
    [DEWGONG] = SI_DEWGONG,
    [GRIMER] = SI_GRIMER,
    [MUK] = SI_MUK,
    [SHELLDER] = SI_SHELLDER,
    [CLOYSTER] = SI_CLOYSTER,
    [GASTLY] = SI_GASTLY,
    [HAUNTER] = SI_HAUNTER,
    [GENGAR] = SI_GENGAR,
    [ONIX] = SI_ONIX,
    [DROWZEE] = SI_DROWZEE,
    [HYPNO] = SI_HYPNO,
    [KRABBY] = SI_KRABBY,
    [KINGLER] = SI_KINGLER,
    [VOLTORB] = SI_VOLTORB,
    [ELECTRODE] = SI_ELECTRODE,
    [EXEGGCUTE] = SI_EXEGGCUTE,
    [EXEGGUTOR] = SI_EXEGGUTOR,
    [CUBONE] = SI_CUBONE,
    [MAROWAK] = SI_MAROWAK,
    [HITMONLEE] = SI_HITMONLEE,
    [HITMONCHAN] = SI_HITMONCHAN,
    [LICKITUNG] = SI_LICKITUNG,
    [KOFFING] = SI_KOFFING,
    [WEEZING] = SI_WEEZING,
    [RHYHORN] = SI_RHYHORN,
    [RHYDON] = SI_RHYDON,
    [CHANSEY] = SI_CHANSEY,
    [TANGELA] = SI_TANGELA,
    [KANGASKHAN] = SI_KANGASKHAN,
    [HORSEA] = SI_HORSEA,
    [SEADRA] = SI_SEADRA,
    [GOLDEEN] = SI_GOLDEEN,
    [SEAKING] = SI_SEAKING,
    [STARYU] = SI_STARYU,
    [STARMIE] = SI_STARMIE,
    [MRMIME] = SI_MRMIME,
    [SCYTHER] = SI_SCYTHER,
    [JYNX] = SI_JYNX,
    [ELECTABUZZ] = SI_ELECTABUZZ,
    [MAGMAR] = SI_MAGMAR,
    [PINSIR] = SI_PINSIR,
    [TAUROS] = SI_TAUROS,
    [MAGIKARP] = SI_MAGIKARP,
    [GYARADOS] = SI_GYARADOS,
    [LAPRAS] = SI_LAPRAS,
    [DITTO] = SI_DITTO,
    [EEVEE] = SI_EEVEE,
    [VAPOREON] = SI_VAPOREON,
    [JOLTEON] = SI_JOLTEON,
    [FLAREON] = SI_FLAREON,
    [PORYGON] = SI_PORYGON,
    [OMANYTE] = SI_OMANYTE,
    [OMASTAR] = SI_OMASTAR,
    [KABUTO] = SI_KABUTO,
    [KABUTOPS] = SI_KABUTOPS,
    [AERODACTYL] = SI_AERODACTYL,
    [SNORLAX] = SI_SNORLAX,
    [ARTICUNO] = SI_ARTICUNO,
    [ZAPDOS] = SI_ZAPDOS,
    [MOLTRES] = SI_MOLTRES,
    [DRATINI] = SI_DRATINI,
    [DRAGONAIR] = SI_DRAGONAIR,
    [DRAGONITE] = SI_DRAGONITE,
    [MEWTWO] = SI_MEWTWO,
    [MEW] = SI_MEW
};

// Items required for evolution
#define METAL_COAT (uint8_t)143   // 0x8F
#define KING_ROCK (uint8_t)82     // 0x52
#define DRAGON_SCALE (uint8_t)151 // 0x97
#define UPGRADE (uint8_t)172      // 0xAC

#define NUM_GEN1_EVOLUTION_PAIRS (uint8_t)4
#define NUM_GEN2_EVOLUTION_PAIRS (uint8_t)6
#define MAX_SPECIES_INDEX (uint8_t)255

// Pokémon Base Stats
struct pkmn_base_stats
{
    uint8_t max_hp;
    uint8_t atk;
    uint8_t def;
    uint8_t spd;
    uint8_t spcl;
    uint8_t spatk;
    uint8_t spdef;
    uint8_t catch_rate;
    uint8_t types[2];
};

// Pokémon Base Stats (Gen2 species index is pokedex entry id)
static const struct pkmn_base_stats pkmn_base_stats_gen2[MAX_SPECIES_INDEX + 1] = {
    [BULBASAUR] = {.max_hp = 45, .atk = 49, .def = 49, .spd = 45, .spcl = 65},
    [IVYSAUR] = {.max_hp = 60, .atk = 62, .def = 63, .spd = 60, .spcl = 80},
    [VENUSAUR] = {.max_hp = 80, .atk = 82, .def = 83, .spd = 80, .spcl = 100},
    [CHARMANDER] = {.max_hp = 39, .atk = 52, .def = 43, .spd = 65, .spcl = 60},
    [CHARMELEON] = {.max_hp = 58, .atk = 64, .def = 58, .spd = 80, .spcl = 65},
    [CHARIZARD] = {.max_hp = 78, .atk = 84, .def = 78, .spd = 100, .spcl = 85},
    [SQUIRTLE] = {.max_hp = 44, .atk = 48, .def = 65, .spd = 43, .spcl = 50},
    [WARTORTLE] = {.max_hp = 59, .atk = 63, .def = 80, .spd = 58, .spcl = 65},
    [BLASTOISE] = {.max_hp = 79, .atk = 83, .def = 100, .spd = 78, .spcl = 85},
    [CATERPIE] = {.max_hp = 45, .atk = 30, .def = 35, .spd = 45, .spcl = 20},
    [METAPOD] = {.max_hp = 50, .atk = 20, .def = 55, .spd = 30, .spcl = 25},
    [BUTTERFREE] = {.max_hp = 60, .atk = 45, .def = 50, .spd = 70, .spcl = 80},
    [WEEDLE] = {.max_hp = 40, .atk = 35, .def = 30, .spd = 50, .spcl = 20},
    [KAKUNA] = {.max_hp = 45, .atk = 25, .def = 50, .spd = 35, .spcl = 25},
    [BEEDRILL] = {.max_hp = 65, .atk = 80, .def = 40, .spd = 75, .spcl = 45},
    [PIDGEY] = {.max_hp = 40, .atk = 45, .def = 40, .spd = 56, .spcl = 35},
    [PIDGEOTTO] = {.max_hp = 63, .atk = 60, .def = 55, .spd = 71, .spcl = 50},
    [PIDGEOT] = {.max_hp = 83, .atk = 80, .def = 75, .spd = 91, .spcl = 70},
    [RATTATA] = {.max_hp = 30, .atk = 56, .def = 35, .spd = 72, .spcl = 25},
    [RATICATE] = {.max_hp = 55, .atk = 81, .def = 60, .spd = 97, .spcl = 50},
    [SPEAROW] = {.max_hp = 40, .atk = 60, .def = 30, .spd = 70, .spcl = 31},
    [FEAROW] = {.max_hp = 65, .atk = 90, .def = 65, .spd = 100, .spcl = 61},
    [EKANS] = {.max_hp = 35, .atk = 60, .def = 44, .spd = 55, .spcl = 40},
    [ARBOK] = {.max_hp = 60, .atk = 85, .def = 69, .spd = 80, .spcl = 65},
    [PIKACHU] = {.max_hp = 35, .atk = 55, .def = 40, .spd = 90, .spcl = 50},
    [RAICHU] = {.max_hp = 60, .atk = 90, .def = 55, .spd = 100, .spcl = 90},
    [SANDSHREW] = {.max_hp = 50, .atk = 75, .def = 85, .spd = 40, .spcl = 30},
    [SANDSLASH] = {.max_hp = 75, .atk = 100, .def = 110, .spd = 65, .spcl = 55},
    [NIDORAN_F] = {.max_hp = 55, .atk = 47, .def = 52, .spd = 41, .spcl = 40},
    [NIDORINA] = {.max_hp = 70, .atk = 62, .def = 67, .spd = 56, .spcl = 55},
    [NIDOQUEEN] = {.max_hp = 90, .atk = 82, .def = 87, .spd = 76, .spcl = 75},
    [NIDORAN_M] = {.max_hp = 46, .atk = 57, .def = 40, .spd = 50, .spcl = 40},
    [NIDORINO] = {.max_hp = 61, .atk = 72, .def = 57, .spd = 65, .spcl = 55},
    [NIDOKING] = {.max_hp = 81, .atk = 92, .def = 77, .spd = 85, .spcl = 75},
    [CLEFAIRY] = {.max_hp = 70, .atk = 45, .def = 48, .spd = 35, .spcl = 60},
    [CLEFABLE] = {.max_hp = 95, .atk = 70, .def = 73, .spd = 60, .spcl = 85},
    [VULPIX] = {.max_hp = 38, .atk = 41, .def = 40, .spd = 65, .spcl = 65},
    [NINETALES] = {.max_hp = 73, .atk = 76, .def = 75, .spd = 100, .spcl = 100},
    [JIGGLYPUFF] = {.max_hp = 115, .atk = 45, .def = 20, .spd = 20, .spcl = 25},
    [WIGGLYTUFF] = {.max_hp = 140, .atk = 70, .def = 45, .spd = 45, .spcl = 50},
    [ZUBAT] = {.max_hp = 40, .atk = 45, .def = 35, .spd = 55, .spcl = 40},
    [GOLBAT] = {.max_hp = 75, .atk = 80, .def = 70, .spd = 90, .spcl = 75},
    [ODDISH] = {.max_hp = 45, .atk = 50, .def = 55, .spd = 30, .spcl = 75},
    [GLOOM] = {.max_hp = 60, .atk = 65, .def = 70, .spd = 40, .spcl = 85},
    [VILEPLUME] = {.max_hp = 75, .atk = 80, .def = 85, .spd = 50, .spcl = 100},
    [PARAS] = {.max_hp = 35, .atk = 70, .def = 55, .spd = 25, .spcl = 55},
    [PARASECT] = {.max_hp = 60, .atk = 95, .def = 80, .spd = 30, .spcl = 80},
    [VENONAT] = {.max_hp = 60, .atk = 55, .def = 50, .spd = 45, .spcl = 40},
    [VENOMOTH] = {.max_hp = 70, .atk = 65, .def = 60, .spd = 90, .spcl = 90},
    [DIGLETT] = {.max_hp = 10, .atk = 55, .def = 25, .spd = 95, .spcl = 45},
    [DUGTRIO] = {.max_hp = 35, .atk = 80, .def = 50, .spd = 120, .spcl = 70},
    [MEOWTH] = {.max_hp = 40, .atk = 45, .def = 35, .spd = 90, .spcl = 40},
    [PERSIAN] = {.max_hp = 65, .atk = 70, .def = 60, .spd = 115, .spcl = 65},
    [PSYDUCK] = {.max_hp = 50, .atk = 52, .def = 48, .spd = 55, .spcl = 50},
    [GOLDUCK] = {.max_hp = 80, .atk = 82, .def = 78, .spd = 85, .spcl = 80},
    [MANKEY] = {.max_hp = 40, .atk = 80, .def = 35, .spd = 70, .spcl = 35},
    [PRIMEAPE] = {.max_hp = 65, .atk = 105, .def = 60, .spd = 95, .spcl = 60},
    [GROWLITHE] = {.max_hp = 55, .atk = 70, .def = 45, .spd = 60, .spcl = 50},
    [ARCANINE] = {.max_hp = 90, .atk = 110, .def = 80, .spd = 95, .spcl = 80},
    [POLIWAG] = {.max_hp = 40, .atk = 50, .def = 40, .spd = 90, .spcl = 40},
    [POLIWHIRL] = {.max_hp = 65, .atk = 65, .def = 65, .spd = 90, .spcl = 50},
    [POLIWRATH] = {.max_hp = 90, .atk = 85, .def = 95, .spd = 70, .spcl = 70},
    [ABRA] = {.max_hp = 25, .atk = 20, .def = 15, .spd = 90, .spcl = 105},
    [KADABRA] = {.max_hp = 40, .atk = 35, .def = 30, .spd = 105, .spcl = 120},
    [ALAKAZAM] = {.max_hp = 55, .atk = 50, .def = 45, .spd = 120, .spcl = 135},
    [MACHOP] = {.max_hp = 70, .atk = 80, .def = 50, .spd = 35, .spcl = 35},
    [MACHOKE] = {.max_hp = 80, .atk = 100, .def = 70, .spd = 45, .spcl = 50},
    [MACHAMP] = {.max_hp = 90, .atk = 130, .def = 80, .spd = 55, .spcl = 65},
    [BELLSPROUT] = {.max_hp = 50, .atk = 75, .def = 35, .spd = 40, .spcl = 70},
    [WEEPINBELL] = {.max_hp = 65, .atk = 90, .def = 50, .spd = 55, .spcl = 85},
    [VICTREEBEL] = {.max_hp = 80, .atk = 105, .def = 65, .spd = 70, .spcl = 100},
    [TENTACOOL] = {.max_hp = 40, .atk = 40, .def = 35, .spd = 70, .spcl = 100},
    [TENTACRUEL] = {.max_hp = 80, .atk = 70, .def = 65, .spd = 100, .spcl = 120},
    [GEODUDE] = {.max_hp = 40, .atk = 80, .def = 100, .spd = 20, .spcl = 30},
    [GRAVELER] = {.max_hp = 55, .atk = 95, .def = 115, .spd = 35, .spcl = 45},
    [GOLEM] = {.max_hp = 80, .atk = 120, .def = 130, .spd = 45, .spcl = 55},
    [PONYTA] = {.max_hp = 50, .atk = 85, .def = 55, .spd = 90, .spcl = 65},
    [RAPIDASH] = {.max_hp = 65, .atk = 100, .def = 70, .spd = 105, .spcl = 80},
    [SLOWPOKE] = {.max_hp = 90, .atk = 65, .def = 65, .spd = 15, .spcl = 40},
    [SLOWBRO] = {.max_hp = 95, .atk = 75, .def = 110, .spd = 30, .spcl = 80},
    [MAGNEMITE] = {.max_hp = 25, .atk = 35, .def = 70, .spd = 45, .spcl = 95},
    [MAGNETON] = {.max_hp = 50, .atk = 60, .def = 95, .spd = 70, .spcl = 120},
    [FARFETCHD] = {.max_hp = 52, .atk = 65, .def = 55, .spd = 60, .spcl = 58},
    [DODUO] = {.max_hp = 35, .atk = 85, .def = 45, .spd = 75, .spcl = 35},
    [DODRIO] = {.max_hp = 60, .atk = 110, .def = 70, .spd = 100, .spcl = 60},
    [SEEL] = {.max_hp = 65, .atk = 45, .def = 55, .spd = 45, .spcl = 70},
    [DEWGONG] = {.max_hp = 90, .atk = 70, .def = 80, .spd = 70, .spcl = 95},
    [GRIMER] = {.max_hp = 80, .atk = 80, .def = 50, .spd = 25, .spcl = 40},
    [MUK] = {.max_hp = 105, .atk = 105, .def = 75, .spd = 50, .spcl = 65},
    [SHELLDER] = {.max_hp = 30, .atk = 65, .def = 100, .spd = 40, .spcl = 45},
    [CLOYSTER] = {.max_hp = 50, .atk = 95, .def = 180, .spd = 70, .spcl = 85},
    [GASTLY] = {.max_hp = 30, .atk = 35, .def = 30, .spd = 80, .spcl = 100},
    [HAUNTER] = {.max_hp = 45, .atk = 50, .def = 45, .spd = 95, .spcl = 115},
    [GENGAR] = {.max_hp = 60, .atk = 65, .def = 60, .spd = 110, .spcl = 130},
    [ONIX] = {.max_hp = 35, .atk = 45, .def = 160, .spd = 70, .spcl = 30},
    [DROWZEE] = {.max_hp = 60, .atk = 48, .def = 45, .spd = 42, .spcl = 90},
    [HYPNO] = {.max_hp = 85, .atk = 73, .def = 70, .spd = 67, .spcl = 115},
    [KRABBY] = {.max_hp = 30, .atk = 105, .def = 90, .spd = 50, .spcl = 25},
    [KINGLER] = {.max_hp = 55, .atk = 130, .def = 115, .spd = 75, .spcl = 50},
    [VOLTORB] = {.max_hp = 40, .atk = 30, .def = 50, .spd = 100, .spcl = 55},
    [ELECTRODE] = {.max_hp = 60, .atk = 50, .def = 70, .spd = 140, .spcl = 80},
    [EXEGGCUTE] = {.max_hp = 60, .atk = 40, .def = 80, .spd = 40, .spcl = 60},
    [EXEGGUTOR] = {.max_hp = 95, .atk = 85, .def = 110, .spd = 45, .spcl = 85},
    [CUBONE] = {.max_hp = 50, .atk = 50, .def = 95, .spd = 35, .spcl = 40},
    [MAROWAK] = {.max_hp = 60, .atk = 80, .def = 110, .spd = 45, .spcl = 50},
    [HITMONLEE] = {.max_hp = 50, .atk = 120, .def = 53, .spd = 87, .spcl = 35},
    [HITMONCHAN] = {.max_hp = 50, .atk = 105, .def = 79, .spd = 76, .spcl = 35},
    [LICKITUNG] = {.max_hp = 90, .atk = 55, .def = 75, .spd = 30, .spcl = 60},
    [KOFFING] = {.max_hp = 40, .atk = 65, .def = 95, .spd = 35, .spcl = 60},
    [WEEZING] = {.max_hp = 65, .atk = 90, .def = 120, .spd = 60, .spcl = 85},
    [RHYHORN] = {.max_hp = 80, .atk = 85, .def = 95, .spd = 25, .spcl = 30},
    [RHYDON] = {.max_hp = 105, .atk = 130, .def = 120, .spd = 45, .spcl = 45},
    [CHANSEY] = {.max_hp = 250, .atk = 5, .def = 5, .spd = 50, .spcl = 105},
    [TANGELA] = {.max_hp = 65, .atk = 55, .def = 115, .spd = 60, .spcl = 100},
    [KANGASKHAN] = {.max_hp = 105, .atk = 95, .def = 80, .spd = 90, .spcl = 40},
    [HORSEA] = {.max_hp = 30, .atk = 40, .def = 70, .spd = 60, .spcl = 70},
    [SEADRA] = {.max_hp = 55, .atk = 65, .def = 95, .spd = 85, .spcl = 95},
    [GOLDEEN] = {.max_hp = 45, .atk = 67, .def = 60, .spd = 63, .spcl = 50},
    [SEAKING] = {.max_hp = 80, .atk = 92, .def = 65, .spd = 68, .spcl = 80},
    [STARYU] = {.max_hp = 30, .atk = 45, .def = 55, .spd = 85, .spcl = 70},
    [STARMIE] = {.max_hp = 60, .atk = 75, .def = 85, .spd = 115, .spcl = 100},
    [MRMIME] = {.max_hp = 40, .atk = 45, .def = 65, .spd = 90, .spcl = 100},
    [SCYTHER] = {.max_hp = 70, .atk = 110, .def = 80, .spd = 105, .spcl = 55},
    [JYNX] = {.max_hp = 65, .atk = 50, .def = 35, .spd = 95, .spcl = 95},
    [ELECTABUZZ] = {.max_hp = 65, .atk = 83, .def = 57, .spd = 105, .spcl = 85},
    [MAGMAR] = {.max_hp = 65, .atk = 95, .def = 57, .spd = 93, .spcl = 85},
    [PINSIR] = {.max_hp = 65, .atk = 125, .def = 100, .spd = 85, .spcl = 55},
    [TAUROS] = {.max_hp = 75, .atk = 100, .def = 95, .spd = 110, .spcl = 70},
    [MAGIKARP] = {.max_hp = 20, .atk = 10, .def = 55, .spd = 80, .spcl = 20},
    [GYARADOS] = {.max_hp = 95, .atk = 125, .def = 79, .spd = 81, .spcl = 100},
    [LAPRAS] = {.max_hp = 130, .atk = 85, .def = 80, .spd = 60, .spcl = 95},
    [DITTO] = {.max_hp = 48, .atk = 48, .def = 48, .spd = 48, .spcl = 48},
    [EEVEE] = {.max_hp = 55, .atk = 55, .def = 50, .spd = 55, .spcl = 65},
    [VAPOREON] = {.max_hp = 130, .atk = 65, .def = 60, .spd = 65, .spcl = 110},
    [JOLTEON] = {.max_hp = 65, .atk = 65, .def = 60, .spd = 130, .spcl = 110},
    [FLAREON] = {.max_hp = 65, .atk = 130, .def = 60, .spd = 65, .spcl = 110},
    [PORYGON] = {.max_hp = 65, .atk = 60, .def = 70, .spd = 40, .spcl = 75},
    [OMANYTE] = {.max_hp = 35, .atk = 40, .def = 100, .spd = 35, .spcl = 90},
    [OMASTAR] = {.max_hp = 70, .atk = 60, .def = 125, .spd = 55, .spcl = 115},
    [KABUTO] = {.max_hp = 30, .atk = 80, .def = 90, .spd = 55, .spcl = 45},
    [KABUTOPS] = {.max_hp = 60, .atk = 115, .def = 105, .spd = 80, .spcl = 70},
    [AERODACTYL] = {.max_hp = 80, .atk = 105, .def = 65, .spd = 130, .spcl = 60},
    [SNORLAX] = {.max_hp = 160, .atk = 110, .def = 65, .spd = 30, .spcl = 65},
    [ARTICUNO] = {.max_hp = 90, .atk = 85, .def = 100, .spd = 85, .spcl = 95},
    [ZAPDOS] = {.max_hp = 90, .atk = 90, .def = 85, .spd = 100, .spcl = 125},
    [MOLTRES] = {.max_hp = 90, .atk = 100, .def = 90, .spd = 90, .spcl = 125},
    [DRATINI] = {.max_hp = 41, .atk = 64, .def = 45, .spd = 50, .spcl = 50},
    [DRAGONAIR] = {.max_hp = 61, .atk = 84, .def = 65, .spd = 70, .spcl = 70},
    [DRAGONITE] = {.max_hp = 91, .atk = 134, .def = 95, .spd = 80, .spcl = 100},
    [MEWTWO] = {.max_hp = 106, .atk = 110, .def = 90, .spd = 130, .spcl = 154},
    [MEW] = {.max_hp = 100, .atk = 100, .def = 100, .spd = 100, .spcl = 100},
    [CHIKORITA] = {.max_hp = 45, .atk = 49, .def = 65, .spd = 45, .spatk = 49, .spdef = 65},
    [BAYLEEF] = {.max_hp = 60, .atk = 62, .def = 80, .spd = 60, .spatk = 63, .spdef = 80},
    [MEGANIUM] = {.max_hp = 80, .atk = 82, .def = 100, .spd = 80, .spatk = 83, .spdef = 100},
    [CYNDAQUIL] = {.max_hp = 39, .atk = 52, .def = 43, .spd = 65, .spatk = 60, .spdef = 50},
    [QUILAVA] = {.max_hp = 58, .atk = 64, .def = 58, .spd = 80, .spatk = 80, .spdef = 65},
    [TYPHLOSION] = {.max_hp = 78, .atk = 84, .def = 78, .spd = 100, .spatk = 109, .spdef = 85},
    [TOTODILE] = {.max_hp = 50, .atk = 65, .def = 64, .spd = 43, .spatk = 44, .spdef = 48},
    [CROCONAW] = {.max_hp = 65, .atk = 80, .def = 80, .spd = 58, .spatk = 59, .spdef = 63},
    [FERALIGATR] = {.max_hp = 85, .atk = 105, .def = 100, .spd = 78, .spatk = 79, .spdef = 83},
    [SENTRET] = {.max_hp = 35, .atk = 46, .def = 34, .spd = 20, .spatk = 35, .spdef = 45},
    [FURRET] = {.max_hp = 85, .atk = 76, .def = 64, .spd = 90, .spatk = 45, .spdef = 55},
    [HOOTHOOT] = {.max_hp = 60, .atk = 30, .def = 30, .spd = 50, .spatk = 36, .spdef = 56},
    [NOCTOWL] = {.max_hp = 100, .atk = 50, .def = 50, .spd = 70, .spatk = 86, .spdef = 96},
    [LEDYBA] = {.max_hp = 40, .atk = 20, .def = 30, .spd = 55, .spatk = 40, .spdef = 80},
    [LEDIAN] = {.max_hp = 55, .atk = 35, .def = 50, .spd = 85, .spatk = 55, .spdef = 110},
    [SPINARAK] = {.max_hp = 40, .atk = 60, .def = 40, .spd = 30, .spatk = 40, .spdef = 40},
    [ARIADOS] = {.max_hp = 70, .atk = 90, .def = 70, .spd = 40, .spatk = 60, .spdef = 60},
    [CROBAT] = {.max_hp = 85, .atk = 90, .def = 80, .spd = 130, .spatk = 70, .spdef = 80},
    [CHINCHOU] = {.max_hp = 75, .atk = 38, .def = 38, .spd = 67, .spatk = 56, .spdef = 56},
    [LANTURN] = {.max_hp = 125, .atk = 58, .def = 58, .spd = 67, .spatk = 76, .spdef = 76},
    [PICHU] = {.max_hp = 20, .atk = 40, .def = 15, .spd = 60, .spatk = 35, .spdef = 35},
    [CLEFFA] = {.max_hp = 50, .atk = 25, .def = 28, .spd = 15, .spatk = 45, .spdef = 55},
    [IGGLYBUFF] = {.max_hp = 90, .atk = 30, .def = 15, .spd = 15, .spatk = 40, .spdef = 20},
    [TOGEPI] = {.max_hp = 35, .atk = 20, .def = 65, .spd = 20, .spatk = 40, .spdef = 65},
    [TOGETIC] = {.max_hp = 55, .atk = 40, .def = 85, .spd = 40, .spatk = 80, .spdef = 105},
    [NATU] = {.max_hp = 40, .atk = 50, .def = 45, .spd = 70, .spatk = 70, .spdef = 45},
    [XATU] = {.max_hp = 65, .atk = 75, .def = 70, .spd = 95, .spatk = 95, .spdef = 70},
    [MAREEP] = {.max_hp = 55, .atk = 40, .def = 40, .spd = 35, .spatk = 65, .spdef = 45},
    [FLAAFFY] = {.max_hp = 70, .atk = 55, .def = 55, .spd = 45, .spatk = 80, .spdef = 60},
    [AMPHAROS] = {.max_hp = 90, .atk = 75, .def = 85, .spd = 55, .spatk = 115, .spdef = 90},
    [BELLOSSOM] = {.max_hp = 75, .atk = 80, .def = 95, .spd = 50, .spatk = 90, .spdef = 100},
    [MARILL] = {.max_hp = 70, .atk = 20, .def = 50, .spd = 40, .spatk = 20, .spdef = 50},
    [AZUMARILL] = {.max_hp = 100, .atk = 50, .def = 80, .spd = 50, .spatk = 50, .spdef = 80},
    [SUDOWOODO] = {.max_hp = 70, .atk = 100, .def = 115, .spd = 30, .spatk = 30, .spdef = 65},
    [POLITOED] = {.max_hp = 90, .atk = 75, .def = 75, .spd = 70, .spatk = 90, .spdef = 100},
    [HOPPIP] = {.max_hp = 35, .atk = 35, .def = 40, .spd = 50, .spatk = 35, .spdef = 55},
    [SKIPLOOM] = {.max_hp = 55, .atk = 45, .def = 50, .spd = 80, .spatk = 45, .spdef = 65},
    [JUMPLUFF] = {.max_hp = 75, .atk = 55, .def = 70, .spd = 110, .spatk = 55, .spdef = 85},
    [AIPOM] = {.max_hp = 55, .atk = 70, .def = 55, .spd = 85, .spatk = 40, .spdef = 55},
    [SUNKERN] = {.max_hp = 30, .atk = 30, .def = 30, .spd = 30, .spatk = 30, .spdef = 30},
    [SUNFLORA] = {.max_hp = 75, .atk = 75, .def = 55, .spd = 30, .spatk = 105, .spdef = 85},
    [YANMA] = {.max_hp = 65, .atk = 65, .def = 45, .spd = 95, .spatk = 45, .spdef = 75},
    [WOOPER] = {.max_hp = 55, .atk = 45, .def = 45, .spd = 15, .spatk = 25, .spdef = 25},
    [QUAGSIRE] = {.max_hp = 95, .atk = 85, .def = 85, .spd = 35, .spatk = 65, .spdef = 65},
    [ESPEON] = {.max_hp = 65, .atk = 65, .def = 60, .spd = 110, .spatk = 130, .spdef = 95},
    [UMBREON] = {.max_hp = 95, .atk = 65, .def = 110, .spd = 65, .spatk = 60, .spdef = 130},
    [MURKROW] = {.max_hp = 60, .atk = 85, .def = 42, .spd = 91, .spatk = 85, .spdef = 42},
    [SLOWKING] = {.max_hp = 95, .atk = 75, .def = 80, .spd = 30, .spatk = 100, .spdef = 110},
    [MISDREAVUS] = {.max_hp = 60, .atk = 60, .def = 60, .spd = 85, .spatk = 85, .spdef = 85},
    [UNOWN] = {.max_hp = 48, .atk = 72, .def = 48, .spd = 48, .spatk = 72, .spdef = 48},
    [WOBBUFFET] = {.max_hp = 190, .atk = 33, .def = 58, .spd = 33, .spatk = 33, .spdef = 58},
    [GIRAFARIG] = {.max_hp = 70, .atk = 80, .def = 65, .spd = 85, .spatk = 90, .spdef = 65},
    [PINECO] = {.max_hp = 50, .atk = 65, .def = 90, .spd = 15, .spatk = 35, .spdef = 35},
    [FORRETRESS] = {.max_hp = 75, .atk = 90, .def = 140, .spd = 40, .spatk = 60, .spdef = 60},
    [DUNSPARCE] = {.max_hp = 100, .atk = 70, .def = 70, .spd = 45, .spatk = 65, .spdef = 65},
    [GLIGAR] = {.max_hp = 65, .atk = 75, .def = 105, .spd = 85, .spatk = 35, .spdef = 65},
    [STEELIX] = {.max_hp = 75, .atk = 85, .def = 200, .spd = 30, .spatk = 55, .spdef = 65},
    [SNUBBULL] = {.max_hp = 60, .atk = 80, .def = 50, .spd = 30, .spatk = 40, .spdef = 40},
    [GRANBULL] = {.max_hp = 90, .atk = 120, .def = 75, .spd = 45, .spatk = 60, .spdef = 60},
    [QWILFISH] = {.max_hp = 65, .atk = 95, .def = 75, .spd = 85, .spatk = 55, .spdef = 55},
    [SCIZOR] = {.max_hp = 70, .atk = 130, .def = 100, .spd = 55, .spatk = 80, .spdef = 65},
    [SHUCKLE] = {.max_hp = 20, .atk = 10, .def = 230, .spd = 5, .spatk = 10, .spdef = 230},
    [HERACROSS] = {.max_hp = 80, .atk = 125, .def = 75, .spd = 85, .spatk = 40, .spdef = 95},
    [SNEASEL] = {.max_hp = 55, .atk = 95, .def = 55, .spd = 115, .spatk = 35, .spdef = 75},
    [TEDDIURSA] = {.max_hp = 60, .atk = 80, .def = 50, .spd = 40, .spatk = 50, .spdef = 50},
    [URSARING] = {.max_hp = 90, .atk = 130, .def = 75, .spd = 55, .spatk = 75, .spdef = 75},
    [SLUGMA] = {.max_hp = 40, .atk = 40, .def = 40, .spd = 20, .spatk = 70, .spdef = 40},
    [MAGCARGO] = {.max_hp = 50, .atk = 50, .def = 120, .spd = 30, .spatk = 80, .spdef = 80},
    [SWINUB] = {.max_hp = 50, .atk = 50, .def = 40, .spd = 50, .spatk = 30, .spdef = 30},
    [PILOSWINE] = {.max_hp = 100, .atk = 100, .def = 80, .spd = 60, .spatk = 60, .spdef = 60},
    [CORSOLA] = {.max_hp = 65, .atk = 55, .def = 95, .spd = 35, .spatk = 65, .spdef = 95},
    [REMORAID] = {.max_hp = 35, .atk = 65, .def = 35, .spd = 65, .spatk = 65, .spdef = 35},
    [OCTILLERY] = {.max_hp = 75, .atk = 105, .def = 75, .spd = 45, .spatk = 105, .spdef = 75},
    [DELIBIRD] = {.max_hp = 45, .atk = 55, .def = 45, .spd = 75, .spatk = 65, .spdef = 45},
    [MANTINE] = {.max_hp = 65, .atk = 40, .def = 70, .spd = 70, .spatk = 80, .spdef = 140},
    [SKARMORY] = {.max_hp = 65, .atk = 80, .def = 140, .spd = 70, .spatk = 40, .spdef = 70},
    [HOUNDOUR] = {.max_hp = 45, .atk = 60, .def = 30, .spd = 65, .spatk = 80, .spdef = 50},
    [HOUNDOOM] = {.max_hp = 75, .atk = 90, .def = 50, .spd = 95, .spatk = 110, .spdef = 80},
    [KINGDRA] = {.max_hp = 75, .atk = 95, .def = 95, .spd = 85, .spatk = 95, .spdef = 95},
    [PHANPY] = {.max_hp = 90, .atk = 60, .def = 60, .spd = 40, .spatk = 40, .spdef = 40},
    [DONPHAN] = {.max_hp = 90, .atk = 120, .def = 120, .spd = 50, .spatk = 60, .spdef = 60},
    [PORYGON2] = {.max_hp = 85, .atk = 80, .def = 90, .spd = 60, .spatk = 105, .spdef = 95},
    [STANTLER] = {.max_hp = 73, .atk = 95, .def = 62, .spd = 85, .spatk = 85, .spdef = 65},
    [SMEARGLE] = {.max_hp = 55, .atk = 20, .def = 35, .spd = 75, .spatk = 20, .spdef = 45},
    [TYROGUE] = {.max_hp = 35, .atk = 35, .def = 35, .spd = 35, .spatk = 35, .spdef = 35},
    [HITMONTOP] = {.max_hp = 50, .atk = 95, .def = 95, .spd = 70, .spatk = 35, .spdef = 110},
    [SMOOCHUM] = {.max_hp = 45, .atk = 30, .def = 15, .spd = 65, .spatk = 85, .spdef = 65},
    [ELEKID] = {.max_hp = 45, .atk = 63, .def = 37, .spd = 95, .spatk = 65, .spdef = 55},
    [MAGBY] = {.max_hp = 45, .atk = 75, .def = 37, .spd = 83, .spatk = 70, .spdef = 55},
    [MILTANK] = {.max_hp = 95, .atk = 80, .def = 105, .spd = 100, .spatk = 40, .spdef = 70},
    [BLISSEY] = {.max_hp = 255, .atk = 10, .def = 10, .spd = 55, .spatk = 75, .spdef = 135},
    [RAIKOU] = {.max_hp = 90, .atk = 85, .def = 75, .spd = 115, .spatk = 115, .spdef = 100},
    [ENTEI] = {.max_hp = 115, .atk = 115, .def = 85, .spd = 100, .spatk = 90, .spdef = 75},
    [SUICUNE] = {.max_hp = 100, .atk = 75, .def = 115, .spd = 85, .spatk = 90, .spdef = 115},
    [LARVITAR] = {.max_hp = 50, .atk = 64, .def = 50, .spd = 41, .spatk = 45, .spdef = 50},
    [PUPITAR] = {.max_hp = 70, .atk = 84, .def = 70, .spd = 51, .spatk = 65, .spdef = 70},
    [TYRANITAR] = {.max_hp = 100, .atk = 134, .def = 110, .spd = 61, .spatk = 95, .spdef = 100},
    [LUGIA] = {.max_hp = 106, .atk = 90, .def = 130, .spd = 110, .spatk = 90, .spdef = 154},
    [HO_OH] = {.max_hp = 106, .atk = 130, .def = 90, .spd = 110, .spatk = 110, .spdef = 154},
    [CELEBI] = {.max_hp = 100, .atk = 100, .def = 100, .spd = 100, .spatk = 100, .spdef = 100},
};

// Pokémon Base Stats for Species Index (Gen1 id)
static const struct pkmn_base_stats pkmn_base_stats_gen1[SI_VICTREEBEL + 1] = {
    [SI_BULBASAUR] = {.max_hp = 45, .atk = 49, .def = 49, .spd = 45, .spcl = 65, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_GRASS, PKSAV_GEN1_TYPE_POISON}},
    [SI_IVYSAUR] = {.max_hp = 60, .atk = 62, .def = 63, .spd = 60, .spcl = 80, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_GRASS, PKSAV_GEN1_TYPE_POISON}},
    [SI_VENUSAUR] = {.max_hp = 80, .atk = 82, .def = 83, .spd = 80, .spcl = 100, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_GRASS, PKSAV_GEN1_TYPE_POISON}},
    [SI_CHARMANDER] = {.max_hp = 39, .atk = 52, .def = 43, .spd = 65, .spcl = 50, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_FIRE, PKSAV_GEN1_TYPE_FIRE}},
    [SI_CHARMELEON] = {.max_hp = 58, .atk = 64, .def = 58, .spd = 80, .spcl = 65, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_FIRE, PKSAV_GEN1_TYPE_FIRE}},
    [SI_CHARIZARD] = {.max_hp = 78, .atk = 84, .def = 78, .spd = 100, .spcl = 85, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_FIRE, PKSAV_GEN1_TYPE_FLYING}},
    [SI_SQUIRTLE] = {.max_hp = 44, .atk = 48, .def = 65, .spd = 43, .spcl = 50, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_WARTORTLE] = {.max_hp = 59, .atk = 63, .def = 80, .spd = 58, .spcl = 65, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_BLASTOISE] = {.max_hp = 79, .atk = 83, .def = 100, .spd = 78, .spcl = 85, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_CATERPIE] = {.max_hp = 45, .atk = 30, .def = 35, .spd = 45, .spcl = 20, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_BUG, PKSAV_GEN1_TYPE_BUG}},
    [SI_METAPOD] = {.max_hp = 50, .atk = 20, .def = 55, .spd = 30, .spcl = 25, .catch_rate = 120, .types = {PKSAV_GEN1_TYPE_BUG, PKSAV_GEN1_TYPE_BUG}},
    [SI_BUTTERFREE] = {.max_hp = 60, .atk = 45, .def = 50, .spd = 70, .spcl = 80, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_BUG, PKSAV_GEN1_TYPE_FLYING}},
    [SI_WEEDLE] = {.max_hp = 40, .atk = 35, .def = 30, .spd = 50, .spcl = 20, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_BUG, PKSAV_GEN1_TYPE_POISON}},
    [SI_KAKUNA] = {.max_hp = 45, .atk = 25, .def = 50, .spd = 35, .spcl = 25, .catch_rate = 120, .types = {PKSAV_GEN1_TYPE_BUG, PKSAV_GEN1_TYPE_POISON}},
    [SI_BEEDRILL] = {.max_hp = 65, .atk = 80, .def = 40, .spd = 75, .spcl = 45, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_BUG, PKSAV_GEN1_TYPE_POISON}},
    [SI_PIDGEY] = {.max_hp = 40, .atk = 45, .def = 40, .spd = 56, .spcl = 35, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_FLYING}},
    [SI_PIDGEOTTO] = {.max_hp = 63, .atk = 60, .def = 55, .spd = 71, .spcl = 50, .catch_rate = 120, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_FLYING}},
    [SI_PIDGEOT] = {.max_hp = 83, .atk = 80, .def = 75, .spd = 101, .spcl = 70, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_FLYING}},
    [SI_RATTATA] = {.max_hp = 30, .atk = 56, .def = 35, .spd = 72, .spcl = 25, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_RATICATE] = {.max_hp = 55, .atk = 81, .def = 60, .spd = 97, .spcl = 50, .catch_rate = 127, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_SPEAROW] = {.max_hp = 40, .atk = 60, .def = 30, .spd = 70, .spcl = 31, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_FLYING}},
    [SI_FEAROW] = {.max_hp = 65, .atk = 90, .def = 65, .spd = 100, .spcl = 61, .catch_rate = 90, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_FLYING}},
    [SI_EKANS] = {.max_hp = 35, .atk = 60, .def = 44, .spd = 55, .spcl = 40, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_POISON}},
    [SI_ARBOK] = {.max_hp = 60, .atk = 85, .def = 69, .spd = 80, .spcl = 65, .catch_rate = 90, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_POISON}},
    [SI_PIKACHU] = {.max_hp = 35, .atk = 55, .def = 30, .spd = 90, .spcl = 50, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_ELECTRIC, PKSAV_GEN1_TYPE_ELECTRIC}},
    [SI_RAICHU] = {.max_hp = 60, .atk = 90, .def = 55, .spd = 100, .spcl = 90, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_ELECTRIC, PKSAV_GEN1_TYPE_ELECTRIC}},
    [SI_SANDSHREW] = {.max_hp = 50, .atk = 75, .def = 85, .spd = 40, .spcl = 30, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_GROUND, PKSAV_GEN1_TYPE_GROUND}},
    [SI_SANDSLASH] = {.max_hp = 75, .atk = 100, .def = 110, .spd = 65, .spcl = 55, .catch_rate = 90, .types = {PKSAV_GEN1_TYPE_GROUND, PKSAV_GEN1_TYPE_GROUND}},
    [SI_NIDORAN_F] = {.max_hp = 55, .atk = 47, .def = 52, .spd = 41, .spcl = 40, .catch_rate = 235, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_POISON}},
    [SI_NIDORINA] = {.max_hp = 70, .atk = 62, .def = 67, .spd = 56, .spcl = 55, .catch_rate = 120, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_POISON}},
    [SI_NIDOQUEEN] = {.max_hp = 90, .atk = 82, .def = 87, .spd = 76, .spcl = 75, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_GROUND}},
    [SI_NIDORAN_M] = {.max_hp = 46, .atk = 57, .def = 40, .spd = 50, .spcl = 40, .catch_rate = 235, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_POISON}},
    [SI_NIDORINO] = {.max_hp = 61, .atk = 72, .def = 57, .spd = 65, .spcl = 55, .catch_rate = 120, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_POISON}},
    [SI_NIDOKING] = {.max_hp = 81, .atk = 92, .def = 77, .spd = 85, .spcl = 75, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_GROUND}},
    [SI_CLEFAIRY] = {.max_hp = 70, .atk = 45, .def = 48, .spd = 35, .spcl = 60, .catch_rate = 150, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_CLEFABLE] = {.max_hp = 95, .atk = 70, .def = 73, .spd = 60, .spcl = 85, .catch_rate = 25, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_VULPIX] = {.max_hp = 38, .atk = 41, .def = 40, .spd = 65, .spcl = 65, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_FIRE, PKSAV_GEN1_TYPE_FIRE}},
    [SI_NINETALES] = {.max_hp = 73, .atk = 76, .def = 75, .spd = 100, .spcl = 100, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_FIRE, PKSAV_GEN1_TYPE_FIRE}},
    [SI_JIGGLYPUFF] = {.max_hp = 115, .atk = 45, .def = 20, .spd = 20, .spcl = 25, .catch_rate = 170, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_WIGGLYTUFF] = {.max_hp = 140, .atk = 70, .def = 45, .spd = 45, .spcl = 50, .catch_rate = 50, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_ZUBAT] = {.max_hp = 40, .atk = 45, .def = 35, .spd = 55, .spcl = 40, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_FLYING}},
    [SI_GOLBAT] = {.max_hp = 75, .atk = 80, .def = 70, .spd = 90, .spcl = 75, .catch_rate = 90, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_FLYING}},
    [SI_ODDISH] = {.max_hp = 45, .atk = 50, .def = 55, .spd = 30, .spcl = 75, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_GRASS, PKSAV_GEN1_TYPE_POISON}},
    [SI_GLOOM] = {.max_hp = 60, .atk = 65, .def = 70, .spd = 40, .spcl = 85, .catch_rate = 120, .types = {PKSAV_GEN1_TYPE_GRASS, PKSAV_GEN1_TYPE_POISON}},
    [SI_VILEPLUME] = {.max_hp = 75, .atk = 80, .def = 85, .spd = 50, .spcl = 100, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_GRASS, PKSAV_GEN1_TYPE_POISON}},
    [SI_PARAS] = {.max_hp = 35, .atk = 70, .def = 55, .spd = 25, .spcl = 55, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_BUG, PKSAV_GEN1_TYPE_GRASS}},
    [SI_PARASECT] = {.max_hp = 60, .atk = 95, .def = 80, .spd = 30, .spcl = 80, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_BUG, PKSAV_GEN1_TYPE_GRASS}},
    [SI_VENONAT] = {.max_hp = 60, .atk = 55, .def = 50, .spd = 45, .spcl = 40, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_BUG, PKSAV_GEN1_TYPE_POISON}},
    [SI_VENOMOTH] = {.max_hp = 70, .atk = 65, .def = 60, .spd = 90, .spcl = 90, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_BUG, PKSAV_GEN1_TYPE_POISON}},
    [SI_DIGLETT] = {.max_hp = 10, .atk = 55, .def = 25, .spd = 95, .spcl = 45, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_GROUND, PKSAV_GEN1_TYPE_GROUND}},
    [SI_DUGTRIO] = {.max_hp = 35, .atk = 80, .def = 50, .spd = 120, .spcl = 70, .catch_rate = 50, .types = {PKSAV_GEN1_TYPE_GROUND, PKSAV_GEN1_TYPE_GROUND}},
    [SI_MEOWTH] = {.max_hp = 40, .atk = 45, .def = 35, .spd = 90, .spcl = 40, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_PERSIAN] = {.max_hp = 65, .atk = 70, .def = 60, .spd = 115, .spcl = 65, .catch_rate = 90, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_PSYDUCK] = {.max_hp = 50, .atk = 52, .def = 48, .spd = 55, .spcl = 50, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_GOLDUCK] = {.max_hp = 80, .atk = 82, .def = 78, .spd = 85, .spcl = 80, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_MANKEY] = {.max_hp = 40, .atk = 80, .def = 35, .spd = 70, .spcl = 35, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_FIGHTING, PKSAV_GEN1_TYPE_FIGHTING}},
    [SI_PRIMEAPE] = {.max_hp = 65, .atk = 105, .def = 60, .spd = 95, .spcl = 60, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_FIGHTING, PKSAV_GEN1_TYPE_FIGHTING}},
    [SI_GROWLITHE] = {.max_hp = 55, .atk = 70, .def = 45, .spd = 60, .spcl = 50, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_FIRE, PKSAV_GEN1_TYPE_FIRE}},
    [SI_ARCANINE] = {.max_hp = 90, .atk = 110, .def = 80, .spd = 95, .spcl = 80, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_FIRE, PKSAV_GEN1_TYPE_FIRE}},
    [SI_POLIWAG] = {.max_hp = 40, .atk = 50, .def = 40, .spd = 90, .spcl = 40, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_POLIWHIRL] = {.max_hp = 65, .atk = 65, .def = 65, .spd = 90, .spcl = 50, .catch_rate = 120, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_POLIWRATH] = {.max_hp = 90, .atk = 85, .def = 95, .spd = 70, .spcl = 70, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_FIGHTING}},
    [SI_ABRA] = {.max_hp = 25, .atk = 20, .def = 15, .spd = 90, .spcl = 105, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_PSYCHIC, PKSAV_GEN1_TYPE_PSYCHIC}},
    [SI_KADABRA] = {.max_hp = 40, .atk = 35, .def = 30, .spd = 105, .spcl = 120, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_PSYCHIC, PKSAV_GEN1_TYPE_PSYCHIC}},
    [SI_ALAKAZAM] = {.max_hp = 55, .atk = 50, .def = 45, .spd = 120, .spcl = 135, .catch_rate = 50, .types = {PKSAV_GEN1_TYPE_PSYCHIC, PKSAV_GEN1_TYPE_PSYCHIC}},
    [SI_MACHOP] = {.max_hp = 70, .atk = 80, .def = 50, .spd = 35, .spcl = 35, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_FIGHTING, PKSAV_GEN1_TYPE_FIGHTING}},
    [SI_MACHOKE] = {.max_hp = 80, .atk = 100, .def = 70, .spd = 45, .spcl = 50, .catch_rate = 90, .types = {PKSAV_GEN1_TYPE_FIGHTING, PKSAV_GEN1_TYPE_FIGHTING}},
    [SI_MACHAMP] = {.max_hp = 90, .atk = 130, .def = 80, .spd = 55, .spcl = 65, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_FIGHTING, PKSAV_GEN1_TYPE_FIGHTING}},
    [SI_BELLSPROUT] = {.max_hp = 50, .atk = 75, .def = 35, .spd = 40, .spcl = 70, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_GRASS, PKSAV_GEN1_TYPE_POISON}},
    [SI_WEEPINBELL] = {.max_hp = 65, .atk = 90, .def = 50, .spd = 55, .spcl = 85, .catch_rate = 120, .types = {PKSAV_GEN1_TYPE_GRASS, PKSAV_GEN1_TYPE_POISON}},
    [SI_VICTREEBEL] = {.max_hp = 80, .atk = 105, .def = 65, .spd = 70, .spcl = 100, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_GRASS, PKSAV_GEN1_TYPE_POISON}},
    [SI_TENTACOOL] = {.max_hp = 40, .atk = 40, .def = 35, .spd = 70, .spcl = 100, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_POISON}},
    [SI_TENTACRUEL] = {.max_hp = 80, .atk = 70, .def = 65, .spd = 100, .spcl = 120, .catch_rate = 60, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_POISON}},
    [SI_GEODUDE] = {.max_hp = 40, .atk = 80, .def = 100, .spd = 20, .spcl = 30, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_ROCK, PKSAV_GEN1_TYPE_GROUND}},
    [SI_GRAVELER] = {.max_hp = 55, .atk = 95, .def = 115, .spd = 35, .spcl = 45, .catch_rate = 120, .types = {PKSAV_GEN1_TYPE_ROCK, PKSAV_GEN1_TYPE_GROUND}},
    [SI_GOLEM] = {.max_hp = 80, .atk = 110, .def = 130, .spd = 45, .spcl = 55, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_ROCK, PKSAV_GEN1_TYPE_GROUND}},
    [SI_PONYTA] = {.max_hp = 50, .atk = 85, .def = 55, .spd = 90, .spcl = 65, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_FIRE, PKSAV_GEN1_TYPE_FIRE}},
    [SI_RAPIDASH] = {.max_hp = 65, .atk = 100, .def = 70, .spd = 105, .spcl = 80, .catch_rate = 60, .types = {PKSAV_GEN1_TYPE_FIRE, PKSAV_GEN1_TYPE_FIRE}},
    [SI_SLOWPOKE] = {.max_hp = 90, .atk = 65, .def = 65, .spd = 15, .spcl = 40, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_PSYCHIC}},
    [SI_SLOWBRO] = {.max_hp = 95, .atk = 75, .def = 110, .spd = 30, .spcl = 80, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_PSYCHIC}},
    [SI_MAGNEMITE] = {.max_hp = 25, .atk = 35, .def = 70, .spd = 45, .spcl = 95, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_ELECTRIC, PKSAV_GEN1_TYPE_ELECTRIC}},
    [SI_MAGNETON] = {.max_hp = 50, .atk = 60, .def = 95, .spd = 70, .spcl = 120, .catch_rate = 60, .types = {PKSAV_GEN1_TYPE_ELECTRIC, PKSAV_GEN1_TYPE_ELECTRIC}},
    [SI_FARFETCHD] = {.max_hp = 52, .atk = 65, .def = 55, .spd = 60, .spcl = 58, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_FLYING}},
    [SI_DODUO] = {.max_hp = 35, .atk = 85, .def = 45, .spd = 75, .spcl = 35, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_FLYING}},
    [SI_DODRIO] = {.max_hp = 60, .atk = 110, .def = 70, .spd = 100, .spcl = 60, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_FLYING}},
    [SI_SEEL] = {.max_hp = 65, .atk = 45, .def = 55, .spd = 45, .spcl = 70, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_DEWGONG] = {.max_hp = 90, .atk = 70, .def = 80, .spd = 70, .spcl = 95, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_ICE}},
    [SI_GRIMER] = {.max_hp = 80, .atk = 80, .def = 50, .spd = 25, .spcl = 40, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_POISON}},
    [SI_MUK] = {.max_hp = 105, .atk = 105, .def = 75, .spd = 50, .spcl = 65, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_POISON}},
    [SI_SHELLDER] = {.max_hp = 30, .atk = 65, .def = 100, .spd = 40, .spcl = 45, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_CLOYSTER] = {.max_hp = 50, .atk = 95, .def = 180, .spd = 70, .spcl = 85, .catch_rate = 60, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_ICE}},
    [SI_GASTLY] = {.max_hp = 30, .atk = 35, .def = 30, .spd = 80, .spcl = 100, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_GHOST, PKSAV_GEN1_TYPE_POISON}},
    [SI_HAUNTER] = {.max_hp = 45, .atk = 50, .def = 45, .spd = 95, .spcl = 115, .catch_rate = 90, .types = {PKSAV_GEN1_TYPE_GHOST, PKSAV_GEN1_TYPE_POISON}},
    [SI_GENGAR] = {.max_hp = 60, .atk = 65, .def = 60, .spd = 110, .spcl = 130, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_GHOST, PKSAV_GEN1_TYPE_POISON}},
    [SI_ONIX] = {.max_hp = 35, .atk = 45, .def = 160, .spd = 70, .spcl = 30, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_ROCK, PKSAV_GEN1_TYPE_GROUND}},
    [SI_DROWZEE] = {.max_hp = 60, .atk = 48, .def = 45, .spd = 42, .spcl = 90, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_PSYCHIC, PKSAV_GEN1_TYPE_PSYCHIC}},
    [SI_HYPNO] = {.max_hp = 85, .atk = 73, .def = 70, .spd = 67, .spcl = 115, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_PSYCHIC, PKSAV_GEN1_TYPE_PSYCHIC}},
    [SI_KRABBY] = {.max_hp = 30, .atk = 105, .def = 90, .spd = 50, .spcl = 25, .catch_rate = 225, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_KINGLER] = {.max_hp = 55, .atk = 130, .def = 115, .spd = 75, .spcl = 50, .catch_rate = 60, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_VOLTORB] = {.max_hp = 40, .atk = 30, .def = 50, .spd = 100, .spcl = 55, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_ELECTRIC, PKSAV_GEN1_TYPE_ELECTRIC}},
    [SI_ELECTRODE] = {.max_hp = 60, .atk = 50, .def = 70, .spd = 140, .spcl = 80, .catch_rate = 60, .types = {PKSAV_GEN1_TYPE_ELECTRIC, PKSAV_GEN1_TYPE_ELECTRIC}},
    [SI_EXEGGCUTE] = {.max_hp = 60, .atk = 40, .def = 80, .spd = 40, .spcl = 60, .catch_rate = 90, .types = {PKSAV_GEN1_TYPE_GRASS, PKSAV_GEN1_TYPE_PSYCHIC}},
    [SI_EXEGGUTOR] = {.max_hp = 95, .atk = 95, .def = 85, .spd = 55, .spcl = 125, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_GRASS, PKSAV_GEN1_TYPE_PSYCHIC}},
    [SI_CUBONE] = {.max_hp = 50, .atk = 50, .def = 95, .spd = 35, .spcl = 40, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_GROUND, PKSAV_GEN1_TYPE_GROUND}},
    [SI_MAROWAK] = {.max_hp = 60, .atk = 80, .def = 110, .spd = 45, .spcl = 50, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_GROUND, PKSAV_GEN1_TYPE_GROUND}},
    [SI_HITMONLEE] = {.max_hp = 50, .atk = 120, .def = 53, .spd = 87, .spcl = 35, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_FIGHTING, PKSAV_GEN1_TYPE_FIGHTING}},
    [SI_HITMONCHAN] = {.max_hp = 50, .atk = 105, .def = 79, .spd = 76, .spcl = 35, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_FIGHTING, PKSAV_GEN1_TYPE_FIGHTING}},
    [SI_LICKITUNG] = {.max_hp = 90, .atk = 55, .def = 75, .spd = 30, .spcl = 60, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_KOFFING] = {.max_hp = 40, .atk = 65, .def = 95, .spd = 35, .spcl = 60, .catch_rate = 190, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_POISON}},
    [SI_WEEZING] = {.max_hp = 65, .atk = 90, .def = 120, .spd = 60, .spcl = 85, .catch_rate = 60, .types = {PKSAV_GEN1_TYPE_POISON, PKSAV_GEN1_TYPE_POISON}},
    [SI_RHYHORN] = {.max_hp = 80, .atk = 85, .def = 95, .spd = 25, .spcl = 30, .catch_rate = 120, .types = {PKSAV_GEN1_TYPE_GROUND, PKSAV_GEN1_TYPE_ROCK}},
    [SI_RHYDON] = {.max_hp = 105, .atk = 130, .def = 120, .spd = 45, .spcl = 45, .catch_rate = 60, .types = {PKSAV_GEN1_TYPE_GROUND, PKSAV_GEN1_TYPE_ROCK}},
    [SI_CHANSEY] = {.max_hp = 250, .atk = 5, .def = 5, .spd = 35, .spcl = 105, .catch_rate = 30, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_TANGELA] = {.max_hp = 65, .atk = 55, .def = 115, .spd = 60, .spcl = 100, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_GRASS, PKSAV_GEN1_TYPE_GRASS}},
    [SI_KANGASKHAN] = {.max_hp = 105, .atk = 95, .def = 80, .spd = 90, .spcl = 40, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_HORSEA] = {.max_hp = 30, .atk = 40, .def = 70, .spd = 60, .spcl = 70, .catch_rate = 225, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_SEADRA] = {.max_hp = 55, .atk = 65, .def = 95, .spd = 85, .spcl = 95, .catch_rate = 75, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_GOLDEEN] = {.max_hp = 45, .atk = 67, .def = 60, .spd = 63, .spcl = 50, .catch_rate = 225, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_SEAKING] = {.max_hp = 80, .atk = 92, .def = 65, .spd = 68, .spcl = 80, .catch_rate = 60, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_STARYU] = {.max_hp = 30, .atk = 45, .def = 55, .spd = 85, .spcl = 70, .catch_rate = 225, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_STARMIE] = {.max_hp = 60, .atk = 75, .def = 85, .spd = 115, .spcl = 100, .catch_rate = 60, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_PSYCHIC}},
    [SI_MRMIME] = {.max_hp = 40, .atk = 45, .def = 65, .spd = 90, .spcl = 100, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_PSYCHIC, PKSAV_GEN1_TYPE_PSYCHIC}},
    [SI_SCYTHER] = {.max_hp = 70, .atk = 110, .def = 80, .spd = 105, .spcl = 55, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_BUG, PKSAV_GEN1_TYPE_FLYING}},
    [SI_JYNX] = {.max_hp = 65, .atk = 50, .def = 35, .spd = 95, .spcl = 95, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_ICE, PKSAV_GEN1_TYPE_PSYCHIC}},
    [SI_ELECTABUZZ] = {.max_hp = 65, .atk = 83, .def = 57, .spd = 105, .spcl = 85, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_ELECTRIC, PKSAV_GEN1_TYPE_ELECTRIC}},
    [SI_MAGMAR] = {.max_hp = 65, .atk = 95, .def = 57, .spd = 93, .spcl = 85, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_FIRE, PKSAV_GEN1_TYPE_FIRE}},
    [SI_PINSIR] = {.max_hp = 65, .atk = 125, .def = 100, .spd = 85, .spcl = 55, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_BUG, PKSAV_GEN1_TYPE_BUG}},
    [SI_TAUROS] = {.max_hp = 75, .atk = 100, .def = 95, .spd = 110, .spcl = 70, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_MAGIKARP] = {.max_hp = 20, .atk = 10, .def = 55, .spd = 80, .spcl = 20, .catch_rate = 255, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_GYARADOS] = {.max_hp = 95, .atk = 125, .def = 79, .spd = 81, .spcl = 100, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_FLYING}},
    [SI_LAPRAS] = {.max_hp = 130, .atk = 85, .def = 80, .spd = 60, .spcl = 95, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_ICE}},
    [SI_DITTO] = {.max_hp = 48, .atk = 48, .def = 48, .spd = 48, .spcl = 48, .catch_rate = 35, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_EEVEE] = {.max_hp = 55, .atk = 55, .def = 50, .spd = 55, .spcl = 65, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_VAPOREON] = {.max_hp = 130, .atk = 65, .def = 60, .spd = 65, .spcl = 110, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_WATER, PKSAV_GEN1_TYPE_WATER}},
    [SI_JOLTEON] = {.max_hp = 65, .atk = 65, .def = 60, .spd = 130, .spcl = 110, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_ELECTRIC, PKSAV_GEN1_TYPE_ELECTRIC}},
    [SI_FLAREON] = {.max_hp = 65, .atk = 130, .def = 60, .spd = 65, .spcl = 110, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_FIRE, PKSAV_GEN1_TYPE_FIRE}},
    [SI_PORYGON] = {.max_hp = 65, .atk = 60, .def = 70, .spd = 40, .spcl = 75, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_OMANYTE] = {.max_hp = 35, .atk = 40, .def = 100, .spd = 35, .spcl = 90, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_ROCK, PKSAV_GEN1_TYPE_WATER}},
    [SI_OMASTAR] = {.max_hp = 70, .atk = 60, .def = 125, .spd = 55, .spcl = 115, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_ROCK, PKSAV_GEN1_TYPE_WATER}},
    [SI_KABUTO] = {.max_hp = 30, .atk = 80, .def = 90, .spd = 55, .spcl = 45, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_ROCK, PKSAV_GEN1_TYPE_WATER}},
    [SI_KABUTOPS] = {.max_hp = 60, .atk = 115, .def = 105, .spd = 80, .spcl = 70, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_ROCK, PKSAV_GEN1_TYPE_WATER}},
    [SI_AERODACTYL] = {.max_hp = 80, .atk = 105, .def = 65, .spd = 130, .spcl = 60, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_ROCK, PKSAV_GEN1_TYPE_FLYING}},
    [SI_SNORLAX] = {.max_hp = 160, .atk = 110, .def = 65, .spd = 30, .spcl = 65, .catch_rate = 25, .types = {PKSAV_GEN1_TYPE_NORMAL, PKSAV_GEN1_TYPE_NORMAL}},
    [SI_ARTICUNO] = {.max_hp = 90, .atk = 85, .def = 100, .spd = 85, .spcl = 95, .catch_rate = 3, .types = {PKSAV_GEN1_TYPE_ICE, PKSAV_GEN1_TYPE_FLYING}},
    [SI_ZAPDOS] = {.max_hp = 90, .atk = 90, .def = 85, .spd = 100, .spcl = 125, .catch_rate = 3, .types = {PKSAV_GEN1_TYPE_ELECTRIC, PKSAV_GEN1_TYPE_FLYING}},
    [SI_MOLTRES] = {.max_hp = 90, .atk = 100, .def = 90, .spd = 90, .spcl = 125, .catch_rate = 3, .types = {PKSAV_GEN1_TYPE_FIRE, PKSAV_GEN1_TYPE_FLYING}},
    [SI_DRATINI] = {.max_hp = 41, .atk = 64, .def = 45, .spd = 50, .spcl = 50, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_DRAGON, PKSAV_GEN1_TYPE_DRAGON}},
    [SI_DRAGONAIR] = {.max_hp = 61, .atk = 84, .def = 65, .spd = 70, .spcl = 70, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_DRAGON, PKSAV_GEN1_TYPE_DRAGON}},
    [SI_DRAGONITE] = {.max_hp = 91, .atk = 134, .def = 95, .spd = 80, .spcl = 100, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_DRAGON, PKSAV_GEN1_TYPE_FLYING}},
    [SI_MEWTWO] = {.max_hp = 106, .atk = 110, .def = 90, .spd = 130, .spcl = 154, .catch_rate = 3, .types = {PKSAV_GEN1_TYPE_PSYCHIC, PKSAV_GEN1_TYPE_PSYCHIC}},
    [SI_MEW] = {.max_hp = 100, .atk = 100, .def = 100, .spd = 100, .spcl = 100, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_PSYCHIC, PKSAV_GEN1_TYPE_PSYCHIC}},
};

#endif // PKMNSTATS_H
