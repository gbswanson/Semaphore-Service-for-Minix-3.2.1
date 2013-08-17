#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

#define NUM_GRADS 6
#define NUM_UGRADS 2
#define SHM_SIZE 1024



int main (void) 
{
	int table_id, room_id, grad_write, ugrad_write;
	char table_sem[50];
	char room_sem[50];
	char num[50];
	char grad_speak[50];
	char ugrad_speak[50];
	char * cwd;
	char grad[100];
	char ugrad[100];
	char * check2;
	
	cwd = malloc (100 * sizeof(char));
	check2 = getcwd(cwd, 100);
	if(!check2)
		{
			printf("Error obtaining cwd\n");
			return 0;
		}
	strcpy(grad, cwd);
	strcpy(ugrad, cwd);
	strncat(grad, "/grad", 6);
	strncat(ugrad, "/ugrad", 7);
	
	table_id = sem_init(2) - 1;
	room_id = sem_init(1) - 1;
	grad_write = sem_init(1) -1;
	ugrad_write = sem_init(1) - 1;

	int i, check;
	
	snprintf(table_sem, 50, "%d", table_id);
	snprintf(room_sem, 50, "%d", room_id);
	snprintf(grad_speak, 50, "%d", grad_write);
	snprintf(ugrad_speak, 50, "%d", ugrad_write);
	
	key_t key_ugrad;
	int shmid_ugrad;
	char *data_ugrad;
		
		/* make the key: */
	if ((key_ugrad = ftok("ugrad.c", 'R')) == -1) {
		perror("ftok");
		exit(1);
	}

	/* connect to (and possibly create) the segment: */
	if ((shmid_ugrad = shmget(key_ugrad, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
		perror("shmget");
		exit(1);
	}

	/* attach to the segment to get a pointer to it: */
	data_ugrad = shmat(shmid_ugrad, (void *)0, 0);
	if (data_ugrad == (char *)(-1)) {
		perror("shmat");
		exit(1);
	}
	
	key_t key_grad;
	int shmid_grad;
	char *data_grad;
		
		/* make the key: */
	if ((key_grad = ftok("grad.c", 'R')) == -1) {
		perror("ftok");
		exit(1);
	}

	/* connect to (and possibly create) the segment: */
	if ((shmid_grad = shmget(key_grad, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
		perror("shmget");
		exit(1);
	}

	/* attach to the segment to get a pointer to it: */
	data_grad = shmat(shmid_grad, (void *)0, 0);
	if (data_grad == (char *)(-1)) {
		perror("shmat");
		exit(1);
	}
	
	sprintf(data_ugrad, "%d", 0);
	sprintf(data_grad, "%d", 0);	

	for (i = 0;  i < NUM_GRADS; i++)
	{
		snprintf(num, 50, "%d", i);
		pid_t pid = fork();
		if (pid == 0)
		{
			check = execl(grad, "grad", table_sem, room_sem, num, grad_speak, (char *) 0);	
			if (check < 0)
				printf("error\n");
		}	
		else if (pid < 0)
		{
			printf("Could not fork grad\n");
			return 0;
		}
	}
	for (i = 0;  i < NUM_UGRADS; i++)
	{
		snprintf(num, 50, "%d", i);
		pid_t pid = fork();
		if (pid == 0)
		{
			check = execl(ugrad, "ugrad", table_sem, room_sem, num, ugrad_speak, (char *) 0);	
			if (check < 0)
				printf("error\n");
		}	
		else if (pid < 0)
		{
			printf("Could not fork grad\n");
			return 0;
		}
	} 
		
}
