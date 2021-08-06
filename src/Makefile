main: main.o cJSON.o iccelog.o iccemqttclient.o icceweatherclient.o 
	gcc  main.o cJSON.o iccelog.o iccemqttclient.o icceweatherclient.o  -o main -lrt -lpaho-mqtt3c -lpaho-mqtt3a

main.o: main.c 
	gcc -c main.c

cJSON.o: cJSON.c cJSON.h
	gcc -c cJSON.c cJSON.h

iccelog.o: iccelog.c iccelog.h
	gcc -c iccelog.c iccelog.h

iccemqttclient.o: iccemqttclient.c iccemqttclient.h
	gcc -c iccemqttclient.c iccemqttclient.h

icceweatherclient.o: icceweatherclient.c icceweatherclient.h
	gcc -c icceweatherclient.c icceweatherclient.h

clean:
	@echo "cleanning project"
	rm  -rf main *.o *.gch
	@echo "clean completed"
