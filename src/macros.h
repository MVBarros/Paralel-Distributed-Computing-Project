#ifndef MACROS_H
#define MACROS_H

#define BLOCK_LOW(ID,P,N) ((ID)*(N)/(P))
#define BLOCK_HIGH(ID,P,N) (BLOCK_LOW((ID)+1,(P),(N)) - 1)
#define BLOCK_SIZE(ID,P,N) (BLOCK_HIGH((ID),(P),(N)) - BLOCK_LOW((ID),(P),(N)) + 1)
#define BLOCK_OWNER(INDEX,P,N) (((P)*((INDEX)+1)-1)/(N))

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

#define LEFT_PARTITION_SIZE(N) ((N) % 2 ? ((N) - 1) / 2 : (N) / 2)
#define RIGHT_PARTITION_SIZE(N) ((N) % 2 ? ((N) + 1) / 2 : (N) / 2)

#define IS_POWER_OF_TWO(N) ((N) & ((N) - 1)) == 0)

#define MPI_TAG_DUMP_TREE 90
#endif