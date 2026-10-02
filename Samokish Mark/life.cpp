#include <stdio.h>
#include <random>
#include <string>
//test
int main()
{
	using RUB = int;
	using USD = int;
	struct Wallet {
		int money = 0;
		int investment = 0;
	};
	struct Wallet my_wallet;
	struct Time {
		int year = 2026;
		int month = 9;
	};
	struct Time my_time;
	for (my_time.month; my_time.year < 2036;) {
		printf("%d ______ \n", my_time.year);
		for (my_time.month; my_time.month < 12;) {
			printf("%d \n", my_time.month);
			my_time.month = my_time.month + 1;
			my_wallet.money = my_wallet.money + 1000;
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
