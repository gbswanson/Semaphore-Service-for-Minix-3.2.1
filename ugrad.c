#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHM_SIZE 1024

int main (int argc, char ** argv)
{	
	key_t key;
	int shmid;
	int table_id, room_id, ugrads_eating, speak_id;
	char *data;

	srand(time(NULL));
	
		/* make the key: */
	if ((key = ftok("ugrad.c", 'R')) == -1) {
		perror("ftok");
		exit(1);
	}

	/* connect to (and possibly create) the segment: */
	if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
		perror("shmget");
		exit(1);
	}

	/* attach to the segment to get a pointer to it: */
	data = shmat(shmid, (void *)0, 0);
	if (data == (char *)(-1)) {
		perror("shmat");
		exit(1);
	}

	
	table_id = atoi(argv[1]);
	room_id = atoi(argv[2]);
	speak_id = atoi(argv[4]);

	
	ugrads_eating = 0;
	while (1)
		{
			sem_down(speak_id);
			if (atoi(data) == 0)
				{
					sprintf (data, "%d", 1);
					sem_down(room_id);	
					sem_up(speak_id);
					
					sem_down(table_id);
					int random_number = rand() % 4;
					printf("Ugrad %d eating\n", atoi(argv[3]));
					sleep(random_number);
					printf("Ugrad %d done eating\n", atoi(argv[3]));
					sem_up(table_id);
					
					sem_down(speak_id);
					ugrads_eating = atoi(data);
					sprintf(data, "%d", --ugrads_eating);
					if (atoi(data) == 0)
							sem_up(room_id);
					sem_up(speak_id);
					
					sem_down(room_id);
					sem_up(room_id);
				}
			else
				{	
					sprintf (data, "%d", 2);	
					sem_up(speak_id);
					
					sem_down(table_id);
					int random_number = rand() % 15;
					printf("Ugrad %d eating\n", atoi(argv[3]));
					sleep(random_number);
					printf("Ugrad %d done eating\n", atoi(argv[3]));
					sem_up(table_id);
					
					sem_down(speak_id);
					ugrads_eating = atoi(data);
					sprintf(data, "%d", --ugrads_eating);			
					if (atoi(data) == 0)
							sem_up(room_id);	
					sem_up(speak_id);
					
					sem_down(room_id);
					sem_up(room_id);
				}
				
		}
	return 0;		
}
