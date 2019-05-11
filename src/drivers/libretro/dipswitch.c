#include <stdint.h>
#include <string.h>

#include "../../fceu.h"
#include "../../fceu-types.h"
#include "../../vsuni.h"
#include "../../git.h"
#include "../../driver.h"
#include "../../cart.h"

#include "dipswitch.h"

extern CartInfo iNESCart;

/* VSUNI */

typedef struct {
   const char *name;
   int        value;
} SETTING;

typedef struct {
   const char *name;
   int        settings_size;
   SETTING    settings[8];
} DIPSWITCH;

typedef struct {
   int        gameid;
   const char *romname;
   int        mask;
   int        dipswitch_size;
   DIPSWITCH  dipswitch_core_options[5];
} VSUNI_GAME;

static struct retro_variable dips_battlecity[] = {
   { "fceumm_dipswitch_battlecity_credits", "Credits for 2 Players; 2|1" },
   { "fceumm_dipswitch_battlecity_lives", "Lives; 3|5" },
   { "fceumm_dipswitch_battlecity_demosounds", "Demo Sounds; enabled|disabled" },
   { NULL, NULL }
};

static struct retro_variable dips_castlevania[] = {
   { "fceumm_dipswitch_castlevania_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_castlevania_lives", "Lives; 3|2" },
   { "fceumm_dipswitch_castlevania_bonus", "Bonus; 100k|200k|300k|400k" },
   { "fceumm_dipswitch_castlevania_difficulty", "Difficulty; Normal|Hard" },
   { NULL, NULL }
};

static struct retro_variable dips_clucluland[] = {
   { "fceumm_dipswitch_clucluland_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|2 Coins 1 Credit|3 Coins 1 Credit|4 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_clucluland_lives", "Lives; 3|4|5|2" },
   { NULL, NULL }
};

static struct retro_variable dips_drmario[] = {
   { "fceumm_dipswitch_drmario_droprateincrease", "Drop Rate Increase After; 7 Pills|8 Pills|9 Pills|10 Pills" },
   { "fceumm_dipswitch_drmario_viruslevel", "Virus Level; 1|3|5|7" },
   { "fceumm_dipswitch_drmario_dropspeedup", "Drop Speed Up; Slow|Medium|Fast|Fastest" },
   { "fceumm_dipswitch_drmario_freeplay", "Free Play; disabled|enabled" },
   { "fceumm_dipswitch_drmario_demosounds", "Demo Sounds; enabled|disabled" },
   { NULL, NULL }
};

static struct retro_variable dips_duckhunt[] = {
   { "fceumm_dipswitch_duckhunt_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|2 Coins 1 Credit|3 Coins 1 Credit|4 Coins 1 Credit|5 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_duckhunt_difficulty", "Difficulty; Normal|Easy|Hard|Medium" },
   { "fceumm_dipswitch_duckhunt_misspergame", "Miss per game; 5|3" },
   { "fceumm_dipswitch_duckhunt_bonus", "Bonus Life; 30000|50000|80000|100000" },
   { NULL, NULL }
};

static struct retro_variable dips_excitebike[] = {
   { "fceumm_dipswitch_excitebike_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|2 Coins 1 Credit|3 Coins 1 Credit|4 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_excitebike_bonusbike", "Bonus Bike; 100k and Every 50k|100k Only|Every 100k|None" },
   { "fceumm_dipswitch_excitebike_1stqualifyingtime", "1st Qualifying Time; Easy|Hard" },
   { "fceumm_dipswitch_excitebike_2ndqualifyingtime", "2st Qualifying Time; Easy|Hard" },
   { NULL, NULL }
};

static struct retro_variable dips_freedomforce[] = {
   { "fceumm_dipswitch_freedomforce_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|2 Coins 1 Credit|3 Coins 1 Credit|4 Coins 1 Credit|5 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_freedomforce_healthaward", "Health Awarded At; 10k 50k|10k 50k Every 50k|50k 60k|20k 60k Every 60k" },
   { "fceumm_dipswitch_freedomforce_damage", "Difficulty (Damage); Normal|Easy|Hard|Hardest" },
   { "fceumm_dipswitch_freedomforce_enemy", "Difficulty (Enemy); Normal|Hard" },
   { NULL, NULL }
};

static struct retro_variable dips_goonies[] = {
   { "fceumm_dipswitch_goonies_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_goonies_lives", "Lives; 4|3" },
   { "fceumm_dipswitch_goonies_bonus", "Bonus; 40000|50000|60000|70000" },
   { "fceumm_dipswitch_goonies_timer", "Timer; Slow|Fast" },
   { "fceumm_dipswitch_goonies_demosounds", "Demo Sounds; enabled|disabled" },
   { NULL, NULL }
};

static struct retro_variable dips_gradius[] = {
   { "fceumm_dipswitch_gradius_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_gradius_lives", "Lives; 3|4" },
   { "fceumm_dipswitch_gradius_bonus", "Bonus; 100k|200k|300k|400k" },
   { "fceumm_dipswitch_gradius_difficulty", "Difficulty; Normal|Hard" },
   { "fceumm_dipswitch_gradius_demosounds", "Demo Sounds; enabled|disabled" },
   { NULL, NULL }
};

static struct retro_variable dips_gumshoe[] = {
   { "fceumm_dipswitch_gumshoe_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_gumshoe_difficulty", "Difficulty; Normal|Easy|Hard|Hardest" },
   { "fceumm_dipswitch_gumshoe_lives", "Lives; 3|5" },
   { "fceumm_dipswitch_gumshoe_bullets", "Bullets per Balloon; 3|2" },
   { "fceumm_dipswitch_gumshoe_bonus", "Bonus Man Awarded; At 50000|None" },
   { NULL, NULL }
};

static struct retro_variable dips_hogansalley[] = {
   { "fceumm_dipswitch_hogansalley_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_hogansalley_difficulty", "Difficulty; Normal|Easy|Hard|Hardest" },
   { "fceumm_dipswitch_hogansalley_misspergame", "Misses Per Game; 5|3" },
   { "fceumm_dipswitch_hogansalley_bonuslife", "Bonus Life; 30000|50000|80000|100000" },
   { NULL, NULL }
};

static struct retro_variable dips_iceclimber[] = {
   { "fceumm_dipswitch_iceclimber_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_iceclimber_lives", "Lives; 3|4|5|7" },
   { "fceumm_dipswitch_iceclimber_difficulty", "Difficulty; Easy|Hard" },
   { "fceumm_dipswitch_iceclimber_time", "Time before bear appears; Long|Short" },
   { NULL, NULL }
};

static struct retro_variable dips_jajamaru[] = {
   { "fceumm_dipswitch_jajamaru_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_jajamaru_lives", "Lives; 3|4|5" },
   { "fceumm_dipswitch_jajamaru_demosounds", "Demo Sounds; enabled|disabled" },
   { NULL, NULL }
};

static struct retro_variable dips_mightybj[] = {
   { "fceumm_dipswitch_mightybj_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit" },
   { "fceumm_dipswitch_mightybj_lives", "Lives; 3|4|5|2" },
   { NULL, NULL }
};

static struct retro_variable dips_pinball[] = {
   { "fceumm_dipswitch_pinball_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_pinball_sidedrainwalls", "Side Drain Walls; High|Low" },
   { "fceumm_dipswitch_pinball_bonus", "Bonus Life; 50000|70000" },
   { "fceumm_dipswitch_pinball_balls", "Balls; 3|4|5|2" },
   { "fceumm_dipswitch_pinball_ballspeed", "Ball Speed; Slow|Fast" },
   { NULL, NULL }
};

static struct retro_variable dips_platoon[] = {
   { "fceumm_dipswitch_platoon_demosounds", "Demo Sounds; enabled|disabled" },
   { "fceumm_dipswitch_platoon_difficulty", "Difficulty; Normal|Medium|Hard|Easy" },
   { "fceumm_dipswitch_platoon_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { NULL, NULL }
};

static struct retro_variable dips_rbibaseball[] = {
   { "fceumm_dipswitch_rbibaseball_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|3 Coins 1 Credit|2 Coins 1 Credit" },
   { "fceumm_dipswitch_rbibaseball_inn", "1p-inn 2p-inn time-min; 2 2 4|3 2 6|4 3 7|2 1 3" },
   { "fceumm_dipswitch_rbibaseball_demosounds", "Demo Sounds; enabled|disabled" },
   { NULL, NULL }
};

static struct retro_variable dips_slalom[] = {
   { "fceumm_dipswitch_slalom_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_slalom_freestylepts", "Freestyle Points; Hold Time|Left Right" },
   { "fceumm_dipswitch_slalom_difficulty", "Difficulty; Normal|Medium|Hard|Easy" },
   { "fceumm_dipswitch_slalom_allowcontinue", "Allow Continue; enabled|disabled" },
   { "fceumm_dipswitch_slalom_inverted_input", "Inverted Input; disabled|enabled" },
   { NULL, NULL }
};

static struct retro_variable dips_smb[] = {
   { "fceumm_dipswitch_smb_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_smb_lives", "Lives; 3|2" },
   { "fceumm_dipswitch_smb_bonuslife", "Bonus Life (coins); 100|150|200|250" },
   { "fceumm_dipswitch_smb_timer", "Timer; Slow|Fast" },
   { "fceumm_dipswitch_smb_continuelives", "Continue Lives; 3|4" },
   { NULL, NULL }
};

static struct retro_variable dips_soccer[] = {
   { "fceumm_dipswitch_soccer_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_soccer_pts_timer", "Points Timer; 1000 Pts|1200 Pts|600 Pts|800 Pts" },
   { "fceumm_dipswitch_soccer_difficulty", "Difficulty; Normal|Medium|Hard|Easy" },
   { NULL, NULL }
};

static struct retro_variable dips_starluster[] = {
   { "fceumm_dipswitch_starluster_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|3 Coins 1 Credit|2 Coins 1 Credit" },
   { NULL, NULL }
};

static struct retro_variable dips_superskykid[] = {
   { "fceumm_dipswitch_superskykid_difficulty", "Difficulty; Normal|Medium|Hard|Easy" },
   { "fceumm_dipswitch_superskykid_lives", "Lives; 3|2"},
   { "fceumm_dipswitch_superskykid_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|3 Coins 1 Credit|2 Coins 1 Credit" },
   { NULL, NULL }
};

static struct retro_variable dips_superxevious[] = {
   { "fceumm_dipswitch_superxevious_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|3 Coins 1 Credit|2 Coins 1 Credit" },
   { NULL, NULL }
};

static struct retro_variable dips_tetris[] = {
   { "fceumm_dipswitch_tetris_difficulty", "Difficulty; Normal|Medium|Hard|Easy" },
   { NULL, NULL }
};

static struct retro_variable dips_tkoboxing[] = {
   { "fceumm_dipswitch_tkoboxing_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|3 Coins 1 Credit|2 Coins 1 Credit" },
   { "fceumm_dipswitch_tkoboxing_difficulty", "Difficulty; Easy|Normal|Hard|Very Hard" },
   { NULL, NULL }
};

static struct retro_variable dips_topgun[] = {
   { "fceumm_dipswitch_topgun_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_topgun_livespercoin", "Live per Coin; 3-12 Max|2-9 Max" },
   { "fceumm_dipswitch_topgun_bonuslife", "Bonus Life; 30k and Every 50k|50k and Every 100k|100k and Every 150k|200k and Every 200k" },
   { "fceumm_dipswitch_topgun_difficulty", "Difficulty; Easy|Hard" },
   { "fceumm_dipswitch_topgun_demosounds", "Demo Sounds; enabled|disabled" },
   { NULL, NULL }
};

static struct retro_variable dips_machrider[] = {
   { "fceumm_dipswitch_machrider_coinage", "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play" },
   { "fceumm_dipswitch_machrider_time", "Time; 280 (Easy)|250|220|200 (Hard)" },
   { "fceumm_dipswitch_machrider_enemies", "Enemies; Less|More" },
   { NULL, NULL }
};

static VSUNI_GAME vsuni_games_list[] = {
   { VS_BATTLECITY, "VS. Battle City", 0x07, 3,
      {
         { "fceumm_dipswitch_battlecity_credits", 2,
            {
               { "2", 0x01 },
               { "1", 0x00 }
            }
         },
         { "fceumm_dipswitch_battlecity_lives", 2,
            {
               { "5", 0x02 },
               { "3", 0x00 }
            }
         },
         { "fceumm_dipswitch_battlecity_demosounds", 2,
            {
               { "disabled", 0x04 },
               { "enabled",  0x00 }
            }
         },
      }
   },
   { VS_CASTLEVANIA, "VS. Castlevania", 0x7f, 4,
      {
         { "fceumm_dipswitch_castlevania_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "2 Coins 1 Credit", 0x06 },
               { "3 Coins 1 Credit", 0x01 },
               { "4 Coins 1 Credit", 0x05 },
               { "5 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_castlevania_lives", 2,
            {
               { "2", 0x08 },
               { "3", 0x00 }
            }
         },
         { "fceumm_dipswitch_castlevania_bonus", 4,
            {
               { "100k", 0x00 },
               { "200k", 0x20 },
               { "300k", 0x10 },
               { "400k", 0x30 }
            }
         },
         { "fceumm_dipswitch_castlevania_difficulty", 2,
            {
               { "Hard",   0x40 },
               { "Normal", 0x00 }
            }
         }
      }
   },
   { VS_CLUCLULAND, "VS. Clu Clu Land", 0x67, 2,
      {
         { "fceumm_dipswitch_clucluland_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "1 Coin 4 Credits", 0x06 },
               { "2 Coins 1 Credit", 0x01 },
               { "3 Coins 1 Credit", 0x05 },
               { "4 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_clucluland_lives", 4,
            {
               { "2", 0x60 },
               { "3", 0x00 },
               { "4", 0x40 },
               { "5", 0x20 }
            }
         }
      }
   },
   { VS_DRMARIO, "VS. Dr. Mario", 0xff, 5,
      {
         { "fceumm_dipswitch_drmario_droprateincrease", 4,
            {
               { "7 Pills",  0x00 },
               { "8 Pills",  0x01 },
               { "9 Pills",  0x02 },
               { "10 Pills", 0x03 }
            },
         },
         { "fceumm_dipswitch_drmario_viruslevel", 4,
            {
               { "1", 0x00 },
               { "3", 0x04 },
               { "5", 0x08 },
               { "7", 0x0c }
            },
         },
         { "fceumm_dipswitch_drmario_dropspeedup", 4,
            {
               { "Slow",    0x00 },
               { "Medium",  0x10 },
               { "Fast",    0x20 },
               { "Fastest", 0x30 }
            },
         },
         { "fceumm_dipswitch_drmario_freeplay", 2,
            {
               { "disabled", 0x00 },
               { "enabled",  0x40 }
            },
         },
         { "fceumm_dipswitch_drmario_demosounds", 2,
            {
               { "disabled", 0x00 },
               { "enabled",  0x80 }
            }
         }
      }
   },
   { VS_DUCKHUNT, "VS. Duck Hunt", 0xff, 4,
      {
         { "fceumm_dipswitch_duckhunt_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "2 Coins 1 Credit", 0x06 },
               { "3 Coins 1 Credit", 0x01 },
               { "4 Coins 1 Credit", 0x05 },
               { "5 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_duckhunt_difficulty", 4,
            {
               { "Easy",   0x00 },
               { "Normal", 0x08 },
               { "Medium", 0x10 },
               { "Hard",   0x18 }
            }
         },
         { "fceumm_dipswitch_duckhunt_misspergame", 2,
            {
               { "3", 0x00 },
               { "5", 0x20 }
            }
         },
         { "fceumm_dipswitch_duckhunt_bonus", 4,
            {
               { "30000",  0x00 },
               { "50000",  0x40 },
               { "80000",  0x80 },
               { "100000", 0xc0 }
            }
         }
      }
   },
   { VS_EXITEBIKE, "VS. Excite Bike", 0x7f, 4,
      {
         { "fceumm_dipswitch_excitebike_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "1 Coin 4 Credits", 0x06 },
               { "2 Coins 1 Credit", 0x01 },
               { "3 Coins 1 Credit", 0x05 },
               { "4 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_excitebike_bonusbike", 4,
            {
               { "100k and Every 50k", 0x00 },
               { "100k Only",          0x08 },
               { "Every 100k",         0x10 },
               { "None",               0x18 }
            }
         },
         { "fceumm_dipswitch_excitebike_1stqualifyingtime", 2,
            {
               { "Easy", 0x00 },
               { "Hard", 0x20 }
            }
         },
         { "fceumm_dipswitch_excitebike_2ndqualifyingtime", 2,
            {
               { "Easy", 0x00 },
               { "Hard", 0x40 }
            }
         }
      }
   },
   { VS_FREEDOMFORCE, "VS. Freedom Forece", 0xff, 4,
      {
         { "fceumm_dipswitch_freedomforce_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "2 Coins 1 Credit", 0x06 },
               { "3 Coins 1 Credit", 0x01 },
               { "4 Coins 1 Credit", 0x05 },
               { "5 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_freedomforce_healthaward", 4,
            {
               { "10k 50k",           0x10 },
               { "20k 60k",           0x00 },
               { "20k 60k Every 60k", 0x08 },
               { "10k 50k Every 50k", 0x18 }
            }
         },
         { "fceumm_dipswitch_freedomforce_damage", 4,
            {
               { "Normal",  0x00 },
               { "Easy",    0x40 },
               { "Hard",    0x20 },
               { "Hardest", 0x60 }
            }
         },
         { "fceumm_dipswitch_freedomforce_enemy", 2,
            {
               { "Normal", 0x80 },
               { "Hard",   0x00 }
            }
         }
      }
   },
   { VS_GOONIES, "VS. Goonies", 0xff, 5,
      {
         { "fceumm_dipswitch_goonies_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "2 Coins 1 Credit", 0x06 },
               { "3 Coins 1 Credit", 0x01 },
               { "4 Coins 1 Credit", 0x05 },
               { "5 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_goonies_lives", 2,
            {
               { "3", 0x08 },
               { "4", 0x00 }
            }
         },
         { "fceumm_dipswitch_goonies_bonus", 4,
            {
               { "40000", 0x00 },
               { "50000", 0x20 },
               { "60000", 0x10 },
               { "70000", 0x30 }
            }
         },
         { "fceumm_dipswitch_goonies_timer", 2,
            {
               { "Slow", 0x00 },
               { "Fast", 0x40 }
            }
         },
         { "fceumm_dipswitch_goonies_demosounds", 2,
            {
               { "enabled",  0x80 },
               { "disabled", 0x00 }
            }
         },
      }
   },
   { VS_GRADIUS, "VS. Gradius", 0xff, 5,
      {
         { "fceumm_dipswitch_gradius_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "2 Coins 1 Credit", 0x06 },
               { "3 Coins 1 Credit", 0x01 },
               { "4 Coins 1 Credit", 0x05 },
               { "5 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_gradius_lives", 2,
            {
               { "3", 0x08 },
               { "4", 0x00 }
            }
         },
         { "fceumm_dipswitch_gradius_bonus", 4,
            {
               { "100k", 0x00 },
               { "200k", 0x20 },
               { "300k", 0x10 },
               { "400k", 0x30 }
            }
         },
         { "fceumm_dipswitch_gradius_difficulty", 2,
            {
               { "Hard",   0x40 },
               { "Normal", 0x00 }
            }
         },
         { "fceumm_dipswitch_gradius_demosounds", 2,
            {
               { "enabled",  0x80 },
               { "disabled", 0x00 }
            }
         }
      }
   },
   { VS_GUMSHOE, "VS. Gumshoe", 0xff, 5,
      {
         { "fceumm_dipswitch_gumshoe_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "2 Coins 1 Credit", 0x06 },
               { "3 Coins 1 Credit", 0x01 },
               { "4 Coins 1 Credit", 0x05 },
               { "5 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_gumshoe_difficulty", 4,
            {
               { "Normal",  0x08 },
               { "Easy",    0x00 },
               { "Hard",    0x10 },
               { "Hardest", 0x18 }
            }
         },
         { "fceumm_dipswitch_gumshoe_lives", 2,
            {
               { "3", 0x20 },
               { "5", 0x00 }
            }
         },
         { "fceumm_dipswitch_gumshoe_bullets", 2,
            {
               { "3", 0x00 },
               { "2", 0x40 }
            }
         },
         { "fceumm_dipswitch_gumshoe_bonus", 2,
            {
               { "At 50000", 0x00 },
               { "None",     0x80 }
            }
         }
      }
   },
   { VS_HOGANSALLEY, "VS. Hogan Alley", 0xff, 4,
      {
         { "fceumm_dipswitch_hogansalley_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "2 Coins 1 Credit", 0x06 },
               { "3 Coins 1 Credit", 0x01 },
               { "4 Coins 1 Credit", 0x05 },
               { "5 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_hogansalley_difficulty", 4,
            {
               { "Normal",  0x08 },
               { "Easy",    0x00 },
               { "Hard",    0x10 },
               { "Hardest", 0x18 }
            }
         },
         { "fceumm_dipswitch_hogansalley_misspergame", 2,
            {
               { "3", 0x00 },
               { "5", 0x20 }
            }
         },
         { "fceumm_dipswitch_hogansalley_bonuslife", 4,
            {
               { "30000",  0x00 },
               { "50000",  0x40 },
               { "80000",  0x80 },
               { "100000", 0xc0 }
            }
         }
      }
   },
   { VS_ICECLIMBER, "VS. Ice Climber", 0x7f, 4,
      {
         { "fceumm_dipswitch_iceclimber_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "1 Coin 4 Credits", 0x06 },
               { "2 Coins 1 Credit", 0x01 },
               { "3 Coins 1 Credit", 0x05 },
               { "4 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_iceclimber_lives", 4,
            {
               { "3", 0x00 },
               { "4", 0x10 },
               { "5", 0x08 },
               { "7", 0x18 }
            }
         },
         { "fceumm_dipswitch_iceclimber_difficulty", 2,
            {
               { "Easy", 0x00 },
               { "Hard", 0x20 }
            }
         },
         { "fceumm_dipswitch_iceclimber_time", 2,
            {
               { "Long",  0x00 },
               { "Short", 0x40 }
            }
         }
      }
   },
   { VS_JAJAMARU, "VS. Jajamaru", 0x9f, 3,
      {
         { "fceumm_dipswitch_jajamaru_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "1 Coin 4 Credits", 0x06 },
               { "2 Coins 1 Credit", 0x01 },
               { "3 Coins 1 Credit", 0x05 },
               { "4 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_jajamaru_lives", 3,
            {
               { "3", 0x00 },
               { "4", 0x10 },
               { "5", 0x08 }
            }
         },
         { "fceumm_dipswitch_jajamaru_demosounds", 2,
            {
               { "enabled",  0x80 },
               { "disabled", 0x00 }
            }
         }
      }
   },
   { VS_MIGHTYBOMBJACK, "VS. Mighty Bomb Jack", 0x1f, 2,
      {
         { "fceumm_dipswitch_mightybj_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "1 Coin 4 Credits", 0x06 },
               { "2 Coins 1 Credit", 0x01 },
               { "3 Coins 1 Credit", 0x05 },
               { "4 Coins 1 Credit", 0x03 },
               { "5 Coins 1 Credit", 0x07 }
            }
         },
         { "fceumm_dipswitch_mightybj_lives", 4,
            {
               { "2", 0x10 },
               { "3", 0x00 },
               { "4", 0x08 },
               { "5", 0x18 }
            }
         }
      }
   },
   { VS_PINBALL, "VS. Pinball", 0xff, 5,
      {
         { "fceumm_dipswitch_pinball_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x01 },
               { "1 Coin 2 Credits", 0x06 },
               { "1 Coin 3 Credits", 0x02 },
               { "1 Coin 4 Credits", 0x04 },
               { "2 Coins 1 Credit", 0x05 },
               { "3 Coins 1 Credit", 0x03 },
               { "4 Coins 1 Credit", 0x07 },
               { "Free Play",        0x00 }
            }
         },
         { "fceumm_dipswitch_pinball_sidedrainwalls", 2,
            {
               { "High", 0x00 },
               { "Low",  0x08 }
            }
         },
         { "fceumm_dipswitch_pinball_bonus", 2,
            {
               { "50000", 0x00 },
               { "70000", 0x10 }
            }
         },
         { "fceumm_dipswitch_pinball_balls", 4,
            {
               { "2", 0x60 },
               { "3", 0x00 },
               { "4", 0x40 },
               { "5", 0x20 }
            }
         },
         { "fceumm_dipswitch_pinball_ballspeed", 2,
            {
               { "Slow", 0x00 },
               { "Fast", 0x80 }
            }
         }
      }
   },
   { VS_PLATOON, "VS. Platoon", 0xfc, 3,
      {
         { "fceumm_dipswitch_platoon_demosounds", 2,
            {
               { "enabled",  0x00 },
               { "disabled", 0x04 }
            }
         },
         { "fceumm_dipswitch_platoon_difficulty", 4,
            {
               { "Easy",   0x00 },
               { "Normal", 0x08 },
               { "Medium", 0x10 },
               { "Hard",   0x18 }
            }
         },
         { "fceumm_dipswitch_platoon_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x20 },
               { "1 Coin 3 Credits", 0x40 },
               { "2 Coins 1 Credit", 0x60 },
               { "3 Coins 1 Credit", 0x80 },
               { "4 Coins 1 Credit", 0xa0 },
               { "5 Coins 1 Credit", 0xc0 },
               { "Free Play",        0xe0 }
            }
         }
      }
   },
   { VS_RBIBASEBALL, "VS. Atari R.B.I. Baseball", 0x1f, 3,
      {
         { "fceumm_dipswitch_rbibaseball_coinage", 4,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x01 },
               { "2 Coins 1 Credit", 0x02 },
               { "3 Coins 1 Credit", 0x03 }
            }
         },
         { "fceumm_dipswitch_rbibaseball_inn", 4,
            {
               { "2 1 3", 0x04 },
               { "2 2 4", 0x0c },
               { "3 2 6", 0x00 },
               { "4 3 7", 0x08 }
            }
         },
         { "fceumm_dipswitch_rbibaseball_demosounds", 2,
            {
               { "enabled",  0x00 },
               { "disabled", 0x10 }
            }
         }
      }
   },
   { VS_SLALOM, "VS. Slalom", 0xff, 5,
      {
         { "fceumm_dipswitch_slalom_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "2 Coins 1 Credit", 0x06 },
               { "3 Coins 1 Credit", 0x01 },
               { "4 Coins 1 Credit", 0x05 },
               { "5 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_slalom_freestylepts", 2,
            {
               { "Hold Time",  0x08 },
               { "Left Right", 0x00 }
            }
         },
         { "fceumm_dipswitch_slalom_difficulty", 4,
            {
               { "Easy",   0x00 },
               { "Normal", 0x10 },
               { "Medium", 0x20 },
               { "Hard",   0x30 }
            }
         },
         { "fceumm_dipswitch_slalom_allowcontinue", 2,
            {
               { "enabled",  0x00 },
               { "disabled", 040 }
            }
         },
         { "fceumm_dipswitch_slalom_inverted_input", 2,
            {
               { "enabled",  0x80 },
               { "disabled", 0x00 }
            }
         }
      }
   },
   { VS_SMB, "VS. Super Mario Bros.", 0xff, 5,
      {
         { "fceumm_dipswitch_smb_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x06 },
               { "1 Coin 3 Credits", 0x01 },
               { "1 Coin 4 Credits", 0x05 },
               { "1 Coin 5 Credits", 0x03 },
               { "2 Coins 1 Credit", 0x04 },
               { "3 Coins 1 Credit", 0x02 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_smb_lives", 2,
            {
               { "2", 0x08 },
               { "3", 0x00 }
            }
         },
         { "fceumm_dipswitch_smb_bonuslife", 4,
            {
               { "100", 0x00 },
               { "150", 0x20 },
               { "200", 0x10 },
               { "250", 0x30 }
            }
         },
         { "fceumm_dipswitch_smb_timer", 2,
            {
               { "Slow", 0x00 },
               { "Fast", 0x40 }
            }
         },
         { "fceumm_dipswitch_smb_continuelives", 2,
            {
               { "3", 0x80 },
               { "4", 0x00 }
            }
         }
      }
   },
   { VS_SOCCER, "VS. Soccer", 0x7f, 3,
      {
         { "fceumm_dipswitch_soccer_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "1 Coin 4 Credits", 0x06 },
               { "2 Coins 1 Credit", 0x01 },
               { "3 Coins 1 Credit", 0x05 },
               { "4 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_soccer_pts_timer", 4,
            {
               { "600 Pts",  0x00 },
               { "800 Pts",  0x10 },
               { "1000 Pts", 0x08 },
               { "1200 Pts", 0x18 }
            }
         },
         { "fceumm_dipswitch_soccer_difficulty", 4,
            {
               { "Easy",   0x00 },
               { "Normal", 0x40 },
               { "Medium", 0x20 },
               { "Hard",   0x60 }
            }
         }
      }
   },
   { VS_STARLUSTER, "VS. Star Luster", 0x03, 1,
      {
         { "fceumm_dipswitch_starluster_coinage", 4,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x02 },
               { "2 Coins 1 Credit", 0x01 },
               { "3 Coins 1 Credit", 0x03 }
            }
         }
      }
   },
   { VS_SUPERSKYKID, "VS. Super Sky Kid", 0x1f, 3,
      {
         { "fceumm_dipswitch_superskykid_difficulty", 4,
            {
               { "Easy",   0x00 },
               { "Normal", 0x01 },
               { "Medium", 0x02 },
               { "Hard",   0x03 }
            }
         },
         { "fceumm_dipswitch_superskykid_lives", 2,
            {
               { "2", 0x00 },
               { "3", 0x04 }
            }
         },
         { "fceumm_dipswitch_superskykid_coinage", 4,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x08 },
               { "2 Coins 1 Credit", 0x10 },
               { "3 Coins 1 Credit", 0x18 }
            }
         }
      }
   },
   { VS_SUPERXEVIOUS, "VS. Super Xevious", 0x30, 1,
      {
         { "fceumm_dipswitch_superxevious_coinage", 4,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x10 },
               { "2 Coins 1 Credit", 0x20 },
               { "3 Coins 1 Credit", 0x30 }
            }
         }
      }
   },
   { VS_TETRIS, "VS. Tetris", 0x0c, 1,
      {
         { "fceumm_dipswitch_tetris_difficulty", 4,
            {
               { "Easy",   0x00 },
               { "Normal", 0x04 },
               { "Medium", 0x08 },
               { "Hard",   0x0c }
            }
         }
      }
   },
   { VS_TKOBOXING, "VS. T.K.O. Boxing", 0x0f, 2,
      {
         { "fceumm_dipswitch_tkoboxing_coinage", 4,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x01 },
               { "2 Coins 1 Credit", 0x02 },
               { "3 Coins 1 Credit", 0x03 }
            }
         },
         { "fceumm_dipswitch_tkoboxing_difficulty", 4,
            {
               { "Easy",      0x00 },
               { "Normal",    0x04 },
               { "Hard",      0x08 },
               { "Very Hard", 0x0c }
            }
         }
      }
   },
   { VS_TOPGUN, "VS. Top Gun", 0xff, 5,
      {
         { "fceumm_dipswitch_topgun_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "2 Coins 1 Credit", 0x01 },
               { "3 Coins 1 Credit", 0x05 },
               { "4 Coins 1 Credit", 0x06 },
               { "5 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_topgun_livespercoin", 2,
            {
               { "3-12 Max", 0x00 },
               { "2-9 Max",  0x08 }
            }
         },
         { "fceumm_dipswitch_topgun_bonuslife", 4,
            {
               { "30k and Every 50k",   0x00 },
               { "50k and Every 100k",  0x20 },
               { "100k and Every 150k", 0x10 },
               { "200k and Every 200k", 0x30 }
            }
         },
         { "fceumm_dipswitch_topgun_difficulty", 2,
            {
               { "Easy", 0x00 },
               { "Hard", 0x40 }
            }
         },
         { "fceumm_dipswitch_topgun_demosounds", 2,
            {
               { "disabled", 0x00 },
               { "enabled",  0x80 }
            }
         }
      }
   },
   { VS_MACHRIDER, "VS. Mach Rider", 0x7f, 3,
      {
         { "fceumm_dipswitch_machrider_coinage", 8,
            {
               { "1 Coin 1 Credit",  0x00 },
               { "1 Coin 2 Credits", 0x04 },
               { "1 Coin 3 Credits", 0x02 },
               { "1 Coin 4 Credits", 0x06 },
               { "2 Coins 1 Credit", 0x01 },
               { "3 Coins 1 Credit", 0x05 },
               { "4 Coins 1 Credit", 0x03 },
               { "Free Play",        0x07 }
            }
         },
         { "fceumm_dipswitch_machrider_time", 4,
            {
               { "280 (Easy)", 0x00 },
               { "250",        0x10 },
               { "220",        0x08 },
               { "200 (Hard)", 0x18 }
            }
         },
         { "fceumm_dipswitch_machrider_enemies", 2,
            {
               { "Less", 0x00 },
               { "More", 0x40 }
            }
         }
      }
   },
   { 0 }
};

static VSUNI_GAME *vsgame = NULL;

static void get_vsuni_dipswitch(int *index, struct retro_variable vars[], int id)
{
#define APPEND_VARS(dips)                        \
   {                                             \
      while (dips[i].key != NULL)                \
      {                                          \
         vars[index_core].key = dips[i].key;     \
         vars[index_core].value = dips[i].value; \
         index_core++;                           \
         i++;                                    \
      }                                          \
      *index = index_core;                       \
   }

   int i = 0;
   int index_core = *index;

   if (id == 0)
      return;

   switch (id) {
   case VS_BATTLECITY:
      APPEND_VARS(dips_battlecity);
      break;
   case VS_CASTLEVANIA:
      APPEND_VARS(dips_castlevania);
      break;
   case VS_CLUCLULAND:
      APPEND_VARS(dips_clucluland);
      break;
   case VS_DRMARIO:
      APPEND_VARS(dips_drmario);
      break;
   case VS_DUCKHUNT:
      APPEND_VARS(dips_duckhunt);
      break;
   case VS_EXITEBIKE:
      APPEND_VARS(dips_excitebike);
      break;
   case VS_FREEDOMFORCE:
      APPEND_VARS(dips_freedomforce);
      break;
   case VS_GOONIES:
      APPEND_VARS(dips_goonies);
      break;
   case VS_GRADIUS:
      APPEND_VARS(dips_gradius);
      break;
   case VS_GUMSHOE:
      APPEND_VARS(dips_gumshoe);
      break;
   case VS_HOGANSALLEY:
      APPEND_VARS(dips_hogansalley);
      break;
   case VS_ICECLIMBER:
      APPEND_VARS(dips_iceclimber);
      break;
   case VS_JAJAMARU:
      APPEND_VARS(dips_jajamaru);
      break;
   /* case VS_LADIESGOLF: */
   /* case VS_STROKEANDMATCHGOLF: */
   case VS_MACHRIDER:
      APPEND_VARS(dips_machrider);
      break;
   case VS_MIGHTYBOMBJACK:
      APPEND_VARS(dips_mightybj);
      break;
   case VS_PINBALL:
      APPEND_VARS(dips_pinball);
      break;
   case VS_PLATOON:
      APPEND_VARS(dips_platoon);
      break;
   case VS_RBIBASEBALL:
      APPEND_VARS(dips_rbibaseball);
      break;
   case VS_SLALOM:
      APPEND_VARS(dips_slalom);
      break;
   case VS_SMB:
      APPEND_VARS(dips_smb);
      break;
   case VS_SOCCER:
      APPEND_VARS(dips_soccer);
      break;
   case VS_STARLUSTER:
      APPEND_VARS(dips_starluster);
      break;
   case VS_SUPERSKYKID:
      APPEND_VARS(dips_superskykid);
      break;
   case VS_SUPERXEVIOUS:
      APPEND_VARS(dips_superxevious);
      break;
   case VS_TETRIS:
      APPEND_VARS(dips_tetris);
      break;
   case VS_TKOBOXING:
      APPEND_VARS(dips_tkoboxing);
      break;
   case VS_TOPGUN:
      APPEND_VARS(dips_topgun);
      break;
   default:
      FCEU_printf("No dipsettings found. %d\n", id);
      break;
   }
#undef APPEND_VARS
}

void update_dipswitch_vsuni(void)
{
   struct retro_variable var = {0};
   int vsdip_new = 0x00;
   int idx_dips, idx_var;

   if (GameInfo->type != GIT_VSUNI)
      return;

   if (vsgame == NULL)
      return;

   vsdip_new = (int)FCEUI_VSUniGetDIPs();
   vsdip_new &= ~vsgame->mask;
   for (idx_dips = 0; idx_dips < vsgame->dipswitch_size; idx_dips++)
   {
      DIPSWITCH *core_option = &vsgame->dipswitch_core_options[idx_dips];
      var.key = core_option->name;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) == 0)
         continue;
      for (idx_var = 0; idx_var < core_option->settings_size; idx_var++)
      {
         SETTING *vs_settings = &core_option->settings[idx_var];
         if (strcmp(var.value, vs_settings->name) != 0)
            continue;
         vsdip_new |= vs_settings->value;
      }
   }

   if (FCEUI_VSUniGetDIPs() != (uint8)vsdip_new)
   {
      vsdip = (uint8)vsdip_new;
#ifdef DEBUG
      FCEU_printf("Dipswitch changed %d%d%d%d%d%d%d%d\n",
         (vsdip >> 0) & 1, (vsdip >> 1) & 1, (vsdip >> 2) & 1, (vsdip >> 3) & 1,
         (vsdip >> 4) & 1, (vsdip >> 5) & 1, (vsdip >> 6) & 1, (vsdip >> 7) & 1);
#endif
   }
}

/* Nintendo World Championship */
static struct retro_variable dipswitch_nwc[] = {
   { "fceumm_dipswitch_nwc_swa", "Dipswitch SW1 (18.7s); disabled|enabled" },
   { "fceumm_dipswitch_nwc_swb", "Dipswitch SW2 (37.5s); disabled|enabled" },
   { "fceumm_dipswitch_nwc_swc", "Dipswitch SW3 (1m 15s); enabled|disabled" },
   { "fceumm_dipswitch_nwc_swd", "Dipswitch SW4 (2m 30s); disabled|enabled" },
   { NULL, NULL }
};

static void update_dipswitch_nwc(void)
{
   struct retro_variable var = {0};
   int dips = 0x00;
   int index;

   if (iNESCart.mapper != 105)
      return;

   for (index = 0; index < 4; index++)
   {
      struct retro_variable *nwc = &dipswitch_nwc[index];
      var.key = nwc->key;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      {
         if (strcmp(var.value, "enabled") == 0)
            dips |= (1 << index);
      }
   }

   if (GameInfo->cspecial != dips)
   {
      GameInfo->cspecial = dips;
#ifdef DEBUG
      FCEU_printf("Dipswitch changed = %d%d%d%d\n", (dips >> 0) & 1,
         (dips >> 1) & 1, (dips >> 2) & 1, (dips >> 3) & 1);
#endif
   }

}

void set_dipswitch_variables(int *index, struct retro_variable vars[])
{
   int i, idx = *index;

   /* VSUNI Dipswitch */
   if (GameInfo->type == GIT_VSUNI)
   {
      VSUNI_GAME *vs = vsuni_games_list;
      while (vs->gameid)
      {
         if (vs->gameid == GameInfo->gameid)
         {
            /* update core options with dipswitch options */
            get_vsuni_dipswitch(&idx, &vars[0], vs->gameid);
            vsgame = vs;
            break;
         }
         vs++;
      }
   }

   /* Nintendo World Championship cart (Mapper 105)*/
   if (iNESCart.mapper == 105)
   {
      for (i = 0; i < 4; i++, idx++)
         vars[idx] = dipswitch_nwc[i];
   }

   *index = idx;
}

void update_dipswitch(void)
{
   if (iNESCart.mapper == 105) {
      update_dipswitch_nwc();
   }

   if (GameInfo->type == GIT_VSUNI) {
      update_dipswitch_vsuni();
   }
}
