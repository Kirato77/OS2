// constants.h
#ifndef CONSTANTS_H
#define CONSTANTE_H

#include <semaphore.h>

#define PROBABILITY_STAND 0.1  // 10% de chance d'aller au stand
#define PROBABILITY_OUT 0.01// 1% de chance d'abandonner la scéance

sem_t sharedMemorySemaphore;

#endif