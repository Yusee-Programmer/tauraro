// ==========================================
// RANDOM MODULE - Enhanced Pure C Implementation
// ==========================================
// Provides: Random number generation functions
// Platform: Cross-platform

#ifndef TAURARO_RANDOM_MODULE_H
#define TAURARO_RANDOM_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

static unsigned long g_random_seed = 0;

// Linear congruential generator
static inline unsigned long tau_lcg_next(unsigned long *seed) {
    *seed = (*seed * 1103515245 + 12345) & 0x7fffffff;
    return *seed;
}

// Seed the random number generator
static inline TauValue tauraro_random_seed(TauValue seed_val) {
    if (seed_val.type == 0) {
        g_random_seed = seed_val.value.i;
    } else {
        g_random_seed = (unsigned long)time(NULL);
    }
    srand(g_random_seed);
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// random.random() - Return random float in [0.0, 1.0)
static inline TauValue tauraro_random_random(void) {
    double r = (double)rand() / (double)RAND_MAX;
    return (TauValue){.type = 1, .value.f = r, .refcount = 1, .next = NULL};
}

// random.randint(a, b) - Return random int in [a, b]
static inline TauValue tauraro_random_randint(TauValue a_val, TauValue b_val) {
    long long a = a_val.type == 0 ? a_val.value.i : 0;
    long long b = b_val.type == 0 ? b_val.value.i : 100;
    
    if (a > b) {
        long long tmp = a;
        a = b;
        b = tmp;
    }
    
    long long range = b - a + 1;
    long long r = a + (rand() % range);
    return (TauValue){.type = 0, .value.i = r, .refcount = 1, .next = NULL};
}

// random.uniform(a, b) - Return random float in [a, b]
static inline TauValue tauraro_random_uniform(TauValue a_val, TauValue b_val) {
    double a = a_val.type == 1 ? a_val.value.f : a_val.value.i;
    double b = b_val.type == 1 ? b_val.value.f : b_val.value.i;
    
    if (a > b) {
        double tmp = a;
        a = b;
        b = tmp;
    }
    
    double r = a + (((double)rand() / RAND_MAX) * (b - a));
    return (TauValue){.type = 1, .value.f = r, .refcount = 1, .next = NULL};
}

// random.choice(list) - Return random element from list
static inline TauValue tauraro_random_choice(TauValue items) {
    if (items.type != 4 || !items.value.list || items.value.list->size == 0) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }
    
    TauList* list = items.value.list;
    int idx = rand() % list->size;
    return list->items[idx];
}

// random.shuffle(list) - Shuffle list in place
static inline TauValue tauraro_random_shuffle(TauValue items) {
    if (items.type != 4 || !items.value.list) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }
    
    TauList* list = items.value.list;
    if (list->size < 2) return items;
    
    // Fisher-Yates shuffle
    for (int i = list->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        TauValue tmp = list->items[i];
        list->items[i] = list->items[j];
        list->items[j] = tmp;
    }
    
    return items;
}

// random.sample(population, k) - Return k random unique items
static inline TauValue tauraro_random_sample(TauValue population, TauValue k_val) {
    if (population.type != 4 || !population.value.list) {
        TauList* result = (TauList*)malloc(sizeof(TauList));
        result->size = 0;
        result->capacity = 0;
        result->items = NULL;
        return (TauValue){.type = 4, .value.list = result, .refcount = 1, .next = NULL};
    }
    
    int k = k_val.type == 0 ? k_val.value.i : 1;
    TauList* pop = population.value.list;
    
    if (k > pop->size) k = pop->size;
    if (k < 0) k = 0;
    
    TauList* result = (TauList*)malloc(sizeof(TauList));
    result->size = k;
    result->capacity = k * 2;
    result->items = (TauValue*)malloc(sizeof(TauValue) * result->capacity);
    
    // Random sampling without replacement (simplified)
    int* selected = (int*)malloc(sizeof(int) * k);
    for (int i = 0; i < k; i++) {
        int idx;
        int valid = 0;
        while (!valid) {
            idx = rand() % pop->size;
            valid = 1;
            for (int j = 0; j < i; j++) {
                if (selected[j] == idx) {
                    valid = 0;
                    break;
                }
            }
        }
        selected[i] = idx;
        result->items[i] = pop->items[idx];
    }
    
    free(selected);
    return (TauValue){.type = 4, .value.list = result, .refcount = 1, .next = NULL};
}

// random.gauss(mu, sigma) - Gaussian/Normal distribution
static inline TauValue tauraro_random_gauss(TauValue mu_val, TauValue sigma_val) {
    double mu = mu_val.type == 1 ? mu_val.value.f : mu_val.value.i;
    double sigma = sigma_val.type == 1 ? sigma_val.value.f : sigma_val.value.i;
    
    // Box-Muller transform
    double u1 = ((double)rand() + 1.0) / (RAND_MAX + 2.0);
    double u2 = ((double)rand() + 1.0) / (RAND_MAX + 2.0);
    
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    double result = mu + sigma * z;
    
    return (TauValue){.type = 1, .value.f = result, .refcount = 1, .next = NULL};
}

// random.expovariate(lambd) - Exponential distribution
static inline TauValue tauraro_random_expovariate(TauValue lambd_val) {
    double lambd = lambd_val.type == 1 ? lambd_val.value.f : lambd_val.value.i;
    
    if (lambd <= 0) return (TauValue){.type = 1, .value.f = 0, .refcount = 1, .next = NULL};
    
    double u = ((double)rand() + 1.0) / (RAND_MAX + 2.0);
    double result = -log(u) / lambd;
    
    return (TauValue){.type = 1, .value.f = result, .refcount = 1, .next = NULL};
}

// random.betavariate(alpha, beta) - Beta distribution
static inline TauValue tauraro_random_betavariate(TauValue alpha_val, TauValue beta_val) {
    double alpha = alpha_val.type == 1 ? alpha_val.value.f : alpha_val.value.i;
    double beta = beta_val.type == 1 ? beta_val.value.f : beta_val.value.i;
    
    // Simplified beta sampling
    double y = (alpha > 0) ? -log(((double)rand() + 1) / (RAND_MAX + 2)) / alpha : 0;
    double z = (beta > 0) ? -log(((double)rand() + 1) / (RAND_MAX + 2)) / beta : 0;
    
    double result = y / (y + z);
    return (TauValue){.type = 1, .value.f = result, .refcount = 1, .next = NULL};
}

#endif // TAURARO_RANDOM_MODULE_H
