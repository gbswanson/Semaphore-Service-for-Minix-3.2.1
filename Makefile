all:
	gcc -o grad grad.c
	gcc -o ugrad ugrad.c
	gcc -o main main.c
	
clean:
	rm grad
	rm ugrad
	rm main
