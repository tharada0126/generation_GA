#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define POP_SIZE 100
#define CHROM_LEN 10
#define MAX_GENERATION 100
#define CROSSOVER_RATE 0.5

typedef struct _Individual
{
	int Chromsome[CHROM_LEN];
	int Fitness;
	int SumWeight;
} Individual;
Individual Population[POP_SIZE];
Individual Children[POP_SIZE];

#define CAPACITY_RATE 0.5
typedef struct _Item
{
	int value;
	int weight;
} Item;
Item items[CHROM_LEN];
int capacity;
int values[] = {
	97,
	53,
	50,
	73,
	31,
	100,
	50,
	73,
	47,
	32
};
int weights[] = {
	63,
	96,
	70,
	69,
	98,
	50,
	69,
	60,
	61,
	90,
};

void init_items(void)
{
	int i;
	int sum = 0;

	for(i = 0; i < CHROM_LEN; i++){
		items[i].value = values[i];
		items[i].weight = weights[i];
		sum += weights[i];
	}
	capacity = (int)(sum * CAPACITY_RATE);
}

/*
 * print one chromsome formated as
 * xxxxxxxxxx: fitness=yyy
 *
 * <arguments>
 * pop_i: the index of printed chromsome
 */
void print_chrom(int pop_i)
{
	int i;

	for(i = 0; i < CHROM_LEN; i++){
		printf("%d", Population[pop_i].Chromsome[i]);
	}
	printf(": fitness=%d, weight=%d\n", Population[pop_i].Fitness, Population[pop_i].SumWeight);
}

/*
 * print all chromsome formated as
 * xxxxxxxxxx: fitness=yyy
 */
void print_all(void)
{
	int i;

	for(i = 0; i < POP_SIZE; i++){
		print_chrom(i);
	}
}

/*
 * <summary>
 * fitness calculation for one-max problem
 *
 * <arguments>
 * pop_i: the index of calculated chromsome
 */
void calc_fitness(int pop_i)
{
	int sum_value = 0;
	int sum_weight = 0;
	int i;

	for(i = 0; i < CHROM_LEN; i++){
		if(Population[pop_i].Chromsome[i] == 1){
			sum_value += items[i].value;
			sum_weight += items[i].weight;
		}
	}
	
	if(sum_weight > capacity){
		sum_value /= 10;
	}
	Population[pop_i].Fitness = sum_value;
	Population[pop_i].SumWeight = sum_weight;
}

/*
 * calculate all chromsomes
 */
void calc_fit_all(void)
{
	int i;

	for(i = 0; i < POP_SIZE; i++){
		calc_fitness(i);
	}
}

/*
 * sorting population ordered by fitness
 */
void sort_population(void)
{
	int i, j;
	Individual tmp1, tmp2;

	for(i = 0; i < POP_SIZE - 1; i++){
		for(j = i; j < POP_SIZE; j++){
			tmp1 = Population[i];
			tmp2 = Population[j];
			if(tmp1.Fitness < tmp2.Fitness){
				Population[i] = tmp2;
				Population[j] = tmp1;
			}
		}
	}
}

/*
 * initialize all chromsome with random 0/1 bit
 */
void init_chrom(void)
{
	int i, j;

	for(i = 0; i < POP_SIZE; i++){
		for(j = 0; j < CHROM_LEN; j++){
			Population[i].Chromsome[j] = rand() % 2;
		}
		calc_fitness(i);
	}
}

/*
 * select index by tournament selection
 *
 * tournament selection selects from randomly choosen two chromsomes
 * depending on these fitnesses
 *
 * <return>
 * the index of selected chromsome
 */
int select_chrom(void)
{
	int i1, i2;
	int ret;
	
	i1 = rand() % POP_SIZE;
	do{
		i2 = rand() % POP_SIZE;
	} while(i1 == i2);

	if(Population[i1].Fitness >= Population[i2].Fitness){
		ret = i1;
	}
	else{
		ret = i2;
	}

	return ret;
}

/*
 * generate next generation in Children array
 */
void generate_children(void)
{
	int i;
	int c;
	
	// fill Children array with selected Individuals
	for(i = 0; i < POP_SIZE; i++){
		c = select_chrom();
		Children[i] = Population[c];
	}
}

/*
 * crossover for children depending on CROSSOVER_RATE
 */
void crossover(void)
{
	int i, j;
	int crossover_point;
	int tmp;

	for(i = 0; i < POP_SIZE - 1; i += 2){
		if((double)rand() / ((double)RAND_MAX + 1) < CROSSOVER_RATE){
			crossover_point = rand() % CHROM_LEN;
			for(j = crossover_point; j < CHROM_LEN; j++){
				tmp = Children[i].Chromsome[j];
				Children[i].Chromsome[j] = Children[i + 1].Chromsome[j];
				Children[i + 1].Chromsome[j] = tmp;
			}
		}
	}
}

/*
 * mutation
 */
void mutation(void)
{
	int i, j;

	for(i = 0; i < POP_SIZE; i++){
		for(j = 0; j < CHROM_LEN; j++){
			if((double)rand() / ((double)RAND_MAX + 1) < 1.0 / CHROM_LEN){
				if(Children[i].Chromsome[j] == 0){
					Children[i].Chromsome[j] = 1;
				}
				else{
					Children[i].Chromsome[j] = 0;
				}
			}
		}
	}
}

/*
 * swap Children and Population to next generation
 */
void swap_population(void)
{
	int i;

	for(i = 0; i < POP_SIZE; i++){
		Population[i] = Children[i];
	}
}

/*
 * 1. select two children
 * 2. crossover
 * 3. mutation
 */
void GA(void)
{
	int gen = 0;

	init_items();
	init_chrom();

	while(gen < MAX_GENERATION)
	{
		calc_fit_all();
		sort_population();
		printf("%d gen\r\n", gen);
		print_all();
		generate_children();
		crossover();
		mutation();
		swap_population();
		gen++;
	}
	calc_fit_all();
	sort_population();
	printf("%d gen\r\n", gen);
	print_all();
	printf("best solution\r\n");
	print_chrom(0);
}

void calc_best(void)
{
	int best[10];
	int cand[10];
	int best_fit = 0, best_weight;
	int sum_fit;
	int sum_weight;
	int i, j;
	int bit;
	int pattern = 1;
	
	init_items();
	for(i = 0; i < CHROM_LEN; i++){
		pattern *= 2;
	}

	for(i = 0; i < pattern; i++){
		bit = 1;
		sum_fit = 0;
		sum_weight = 0;
		for(j = 0; j < CHROM_LEN; j++){
			if(i & (bit << j)){
				cand[j] = 1;
				sum_fit += values[j];
				sum_weight += weights[j];
			}
			else{
				cand[j] = 0;
			}
		}
		if(sum_weight > capacity){
			sum_fit = 0;
		}
		if(sum_fit > best_fit){
			for(j = 0; j < CHROM_LEN; j++){
				best[j] = cand[j];
			}
			best_fit = sum_fit;
			best_weight = sum_weight;
		}
	}
	printf("best\r\n");
	for(i = 0; i < CHROM_LEN; i++){
		printf("%d", best[i]);
	}
	printf(": fitness = %d, weight = %d (capacity = %d)\r\n", best_fit, best_weight, capacity);
}

int main(int argc, char **argv)
{
	srand((unsigned int)time(NULL));
	GA();
	calc_best();
	return 0;
}
