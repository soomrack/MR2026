#include "vika.h"

extern Person vika;

void vika_drawing(const int year, int month) {
	if (vika.cash < 0 and vika.artist_reputation > 3) {
		std::cout << '\n' << month << "/" << year << " " << "vika find an art customer" << '\n';
		std::cout << "cash = " << vika.cash << " => ";
		float art_price = 0.01 * vika.artist_reputation * vika.drawing_furry;
		vika.cash += art_price;
		vika_bonus_mental(5);
		std::cout << " vika successfully sold the Art at a price of " << art_price << " => " << " cash = " << vika.cash << '\n';
	}
}


void unemployed_artist(const int year, int month) {
	if (year == 2027 and (month == 7 or month == 8)) {
		return;
	}

	if (!vika.in_hometown and vika.unemployed) {
		return;
	}

	if (vika.in_hometown and vika.unemployed and vika.artist_reputation > 3) {
		std::cout << '\n' << month << "/" << year << " " << "vika find an art customer" << '\n';
		std::cout << "cash = " << vika.cash << " => ";
		float art_price = 0.01 * vika.artist_reputation * vika.drawing_furry;
		vika.cash += art_price;
		std::cout << " ViKa successfully Sold the art at a price of " << art_price << " => " << " cash = " << vika.cash << '\n';
	}
}


void vika_social_media(const int year, int month) {
	if (year == 2028 and month == 8) {
		if (vika.on_vacation or vika.sick or vika.second_job) {
			vika.monthly_posts = 1;
		}
		else { 
			vika.monthly_posts = 5; 
		}
	}

	int new_tiktok_followers = 0;

	if (vika.monthly_posts > 0) {
		for (int i = 0; i < vika.monthly_posts; i++) {
			if (rand() % 100 < 30 + vika.artist_reputation / 5) {
				new_tiktok_followers += 1 + rand() % 4;
			}
		}
	}
	vika.tiktok_followers += new_tiktok_followers;

	if (vika.tiktok_followers > 300) {
		vika.artist_reputation = 4 + vika.tiktok_followers / 300;
	}

	if (vika.artist_reputation > 100) {
		vika.artist_reputation = 100;
	}

	if (year >= 2028) {
		std::cout << "	tt followers = " << vika.tiktok_followers << " reputation = " << vika.artist_reputation << " posts = "
			<< vika.monthly_posts << '\n';
	}

}