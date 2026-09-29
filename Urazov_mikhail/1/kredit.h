#ifndef KREDIT
#define KREDIT
#include "types.h"

#define IPOTEKA_PERVONACHALNY_VZNOS_PROTSENT 20
#define IPOTEKA_BAZOVAYA_STAVKA 22.0
#define IPOTEKA_BAZOVAYA_SUMMA 8000000
#define IPOTEKA_SHAG_SUMMY 2000000
#define IPOTEKA_SHTRAF_PROTSENT 3
#define IPOTEKA_IZYATIE_MESYATSEV 6
#define IPOTEKA_IZYATIE_SKIDKA_PROTSENT 20
#define IPOTEKA_STRAHOVKA_PROTSENT 0.4
#define IPOTEKA_REFINANS_MIN_VYIGRYSH 1.5
#define IPOTEKA_REFINANS_KOMISSIYA_PROTSENT 1
#define IPOTEKA_REFINANS_MIN_MESYATSEV 36
#define IPOTEKA_DOSROCHNO_REZERV_PLATEZHEY 6
#define IPOTEKA_DOSROCHNO_DOLYA_PROTSENT 80
#define IPOTEKA_DOP_LET 10

#define VYCHET_LIMIT_POKUPKI 2000000
#define VYCHET_LIMIT_PROTSENTOV 3000000

#define MONTHS_IN_YEAR 12

double kredit_rynochnaya_stavka(World* w);

int kredit_annuitetny_platezh(int summa, double stavka, int mesyatsy);

int kredit_annuitetny_platezh_dlya(Ipoteka* m);

int kredit_mesyatsev_do_pogasheniya(int ostatok_dolga, double stavka, int platezh);

int kredit_dohod_v_mesyats(Person* p);

int kredit_dolg(Person* p);

void kredit_vydat(Person* p, World* w);

#endif
