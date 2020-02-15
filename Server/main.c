
// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#define PORT 4848
struct tSensor
{
  uint8_t address[8];
  int16_t celsium;
};
static const uint8_t tSensorSize = 10;
void saveSensor(struct tSensor *sensor)
{
    char sensorName[16];
    for(int i=0;i<8;i++)
        sprintf(sensorName+2*i,"%02X",sensor->address[i]);
    FILE *fp;
    if ((fp = fopen(sensorName, "a"))==NULL)
    {
        printf("Cannot open file.\n");
        return;
    }
    char string[64];
    time_t now = time(NULL);
    sprintf(string,"%d - %d\n", now, sensor->celsium);
    printf(string);
    //fwrite(string, sizeof(x[0]), sizeof(x)/sizeof(x[0]), fp);
    fputs(string, fp);
    fclose(fp);
}
void parceSensors(uint8_t *buf, uint8_t size)
{
    if(size < tSensorSize + 2)
        return;

    if(!(buf[0] == 'R' && buf[1] == 'T'))
        return;
    buf+= 2;
    size-= 2;

    uint8_t number = size / tSensorSize;
    while(number)
    {
        struct tSensor sensor;
        for(int i=0;i<8;i++)
            sensor.address[i] = *buf++;
        sensor.celsium = *buf++;
        sensor.celsium += (*buf++) <<8;
        saveSensor(&sensor);
        number --;
    }
}
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    uint8_t buffer[1024] = {0};
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 48484
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 48484
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        valread = 0;
        uint8_t pos = 0;
        while(1)
        {
            valread = read( new_socket , buffer+pos, 1024);
            if(valread == 0)
                break;
            pos+= valread;
        }
        parceSensors(buffer, pos);
        printf("input msg\n");
        close(new_socket);
        fflush(stdout);
        usleep(50000);
    }
    return 0;
}
