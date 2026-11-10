#include <stdio.h>
#include <random>
#include <string>

int main()
{
	using RUB = int;

	struct Wallet {
		RUB money = 0;
		RUB investment = 0;
	};
	struct Wallet my_wallet;
	struct mood {
		int happiness = 50;
		int stress = 50;
	};
	struct Time {
		int year = 2026;
		int month = 9;
	};
	struct month_spend {
		RUB taxes;
		RUB groceries;
		RUB car_maintenance;

	};
	struct Person {
		int age;
		int birth_month;
		Wallet wallet1;
		int mood;
		int stress;
		int health;
		RUB month_spend;
	};
	struct Job {
		std::string title;      
		RUB salary;  
		int moodboost;       
		int stress;
	};

	struct Person Alice;

	Job driver{"водитель", 120000, +10, 60};
	Job teacher{ "учитель", 80000, 0, 60 };
	Job Unemployed{ "Пособие по безработице", 10000, -10, 60 };
	Job Twich_streamer{ "Стримерша", 200000, -20, 60 };
	Job millionare{ "Миллионер", 10000000, +100, 60 };



	struct Time my_time;
	for (my_time.year < 2028;) {
		printf("%d ______ \n", my_time.year);
		for (my_time.month; my_time.month > 12;) {
			printf("%d \n", my_time.month);
			my_time.month = my_time.month + 1;
			my_wallet.money = my_wallet.money + my_wallet.salary - my_wallet.month_spend;
			printf("%d", my_wallet.money); printf("  RUB  \n");
		}
		printf("%d \n", my_time.month);
		my_time.year = my_time.year + 1;
		my_time.month = 1;
	};
	printf("%d \n", my_wallet.money);
	printf("%d \n", my_wallet.investment);
	printf("%d", my_time.year);
}
