#include <stdio.h>
#include <stdlib.h>

struct _retire_info {
  int months;            //Number of months applicable to
  double contribution;   //How many dollars are contributed per month
  double rate_of_return; //Rate of return
};
typedef struct _retire_info retire_info;

//Function for recalculating the current savings for a given number of months
//First, print current savings
//Recalculate savings with interest
//Recalculate savings with contribution
//Continue for given number of months
//Finally, return the new value of current_savings
double savings_calc (double current_savings, retire_info info, int current_month) {
  for (int i = 0; i < info.months; i++) {
    printf("Age %3d month %2d you have $%.2f\n",((current_month + i) / 12), ((current_month + i) % 12), current_savings);
    current_savings *= (1 + info.rate_of_return);
    current_savings += info.contribution;
  }
  
  return current_savings;
  
}

void retirement (int startAge,         //in months
		 double initial,       //initial savings in dollars
		 retire_info working,  //info about working
		 retire_info retired){ //info about being retired

  double current_savings = initial;
  int current_time = startAge;
  //Calculate working time
  current_savings = savings_calc(current_savings, working, current_time);

  //Calculate retired time
  current_time += working.months;
  current_savings = savings_calc(current_savings, retired, current_time);
  
}

int main (void) {
  retire_info working;
  working.months = 489;
  working.contribution = 1000;
  working.rate_of_return = (0.045/12);

  retire_info retired;
  retired.months = 384;
  retired.contribution = -4000;
  retired.rate_of_return = (0.01/12);
  
  retirement(327, 21345, working, retired);
  
  return EXIT_SUCCESS;
  
}
