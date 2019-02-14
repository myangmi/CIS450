all: multiply filter sharpen

multiply: clock.c multiply.c
	gcc -g -o multiply multiply.c clock.c

filter: clock.c bmp.c filter.c
	gcc -g -o filter filter.c bmp.c clock.c

sharpen: clock.c bmp.c sharpen.c
	gcc -g -o sharpen sharpen.c bmp.c clock.c

tests: multiplyTest filterTest sharpenTest

multiplyTest: multiply
	./multiply 700

filterTest: flowers.bmp filter
	./filter filter.txt flowers.bmp flowersBASE.bmp flowersOPT.bmp
	diff flowersBASE.bmp flowersOPT.bmp

sharpenTest: datsun.bmp sharpen
	./sharpen datsun.bmp datsunBASE.bmp datsunOPT.bmp
	diff datsunBASE.bmp datsunOPT.bmp

clean:
	rm -f multiply filter sharpen
	rm -f datsunBASE.bmp datsunOPT.bmp
	rm -f flowersBASE.bmp flowersOPT.bmp
