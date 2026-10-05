#include "log.h"
#include "mortage.h"
#include "peter.h"
#include "time.h"
#include "world.h"
extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;

#include <algorithm>
#include <cmath>
#include <vector>

Mortage mortage_init(unsigned int room_count) {
  Mortage mortage;

  mortage.quad_meters = int_number_generator(36 * room_count, 45 * room_count);
  mortage.debt = world.cost_per_quad_meter * mortage.quad_meters;
  mortage.down_payment = 0.2 * mortage.debt;
  mortage.principal_amount = mortage.debt - mortage.down_payment;
  mortage.interest_rate = (world.key_rate + 4) / 100.0 / 12.0;
  mortage.month = 12 * 10;

  RUB K = mortage.principal_amount;
  double r = mortage.interest_rate;
  double t = std::pow(1.0 + r, mortage.month);

  mortage.payment = static_cast<RUB>(K * (r * t) / (t - 1));
  mortage.room_count = room_count;

  return mortage;
}

void checking_readiness() {
  if (peter.flat_roomcount == 0) {
    mortage_init(1);
    if (peter.cash > mortage.down_payment and
        0.7 * peter.month_income > mortage.payment) {
      peter.mortages.push_back(mortage_init(1));
    }
  }

  if (peter.flat_roomcount == 1 and peter.childs == 1) {
    mortage_init(2);
    if (peter.cash > mortage.down_payment and
        0.7 * peter.month_income > mortage.payment) {
      peter.mortages.push_back(mortage_init(2));
    }
  }

  if (peter.flat_roomcount == 2 and peter.childs == 2) {
    mortage_init(3);
    if (peter.cash > mortage.down_payment and
        0.7 * peter.month_income > mortage.payment) {
      peter.mortages.push_back(mortage_init(3));
    }
  }

  if (peter.flat_roomcount == 3) {
    mortage_init(1);
    if (peter.cash > mortage.down_payment and
        0.7 * peter.month_income > mortage.payment) {
      peter.mortages.push_back(mortage_init(1));
    }
  }
}
