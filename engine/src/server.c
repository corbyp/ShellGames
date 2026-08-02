#include "server.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_LEN 4096

int fd;
struct pollfd pfd;
struct sockaddr_in client_addr;
socklen_t client_len;
struct sockaddr_in server_addr;

void print_packet(Packet packet) {
  printf("x: %d\ny: %d\nicon: %c\n", packet.x, packet.y, packet.icon);
}

int start_server(void) {
  int ret, n;
  char buf[100];

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Error while creating socket");
    return -1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(1337);

  if (bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Error while binding");
    return -1;
  }

  client_len = sizeof(client_addr);
  n = recvfrom(fd, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&client_addr,
               &client_len);
  buf[n] = 0;
  printf("Received: %s\n", buf);
  sendto(fd, "Hello, Client!\n", 16, 0, (struct sockaddr *)&client_addr,
         client_len);

  if ((ret = fcntl(fd, F_GETFL, 0)) == -1) {
    perror("fcntl F_GETFL failed");
    return -1;
  }

  if (fcntl(fd, F_SETFL, ret | O_NONBLOCK) == -1) {
    perror("fcntl F_SETFL failed");
    return -1;
  }

  pfd.fd = fd;
  pfd.events = POLLIN;

  return 0;
}

int start_client(void) {
  int ret, n;
  char buf[100];

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Error while creating socket");
    return -1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  // server_addr.sin_addr.s_addr = inet_addr("192.168.178.10");
  server_addr.sin_port = htons(1337);

  if ((ret = connect(fd, (struct sockaddr *)&server_addr,
                     sizeof(server_addr))) < 0) {
    perror("Error while connecting to server");
    return -1;
  }

  send(fd, "Hello, Server!\n", 16, 0);
  n = recv(fd, buf, sizeof(buf) - 1, 0);
  buf[n] = 0;
  printf("Received: %s\n", buf);

  if ((ret = fcntl(fd, F_GETFL, 0)) == -1) {
    perror("fcntl F_GETFL failed");
    return -1;
  }

  if (fcntl(fd, F_SETFL, ret | O_NONBLOCK) == -1) {
    perror("fcntl F_SETFL failed");
    return -1;
  }

  pfd.fd = fd;
  pfd.events = POLLIN;

  return 0;
}

int poll_server(Packet *packet) {
  pfd.revents = 0;

  int result = poll(&pfd, 1, 0);

  if (result > 0) {
    if (pfd.revents & POLLIN) {
      char buffer[12];

      ssize_t bytes_received =
          recvfrom(fd, buffer, sizeof(buffer), 0,
                   (struct sockaddr *)&client_addr, &client_len);
      if (bytes_received  == sizeof(Packet)) {
        memcpy(packet, buffer, sizeof(Packet));
      }
    }
  } else if (result < 0) {
    perror("poll error");
    return -1;
  }

  return 0;
}

int poll_client(Packet *packet) {
  pfd.revents = 0;

  int result = poll(&pfd, 1, 0);

  if (result > 0) {
    if (pfd.revents & POLLIN) {
      char buffer[12];

      ssize_t bytes_received = recv(fd, buffer, sizeof(buffer), 0);
      if (bytes_received == sizeof(Packet)) {
        memcpy(packet, buffer, sizeof(Packet));
      }
    }
  } else if (result < 0) {
    perror("poll error");
    return -1;
  }

  return 0;
}

int push_server(Packet packet) {
  char buffer[12];
  memcpy(buffer, &packet, sizeof(Packet));

  sendto(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr,
         sizeof(client_addr));

  return 0;
}

int push_client(Packet packet) {
  char buffer[12];
  memcpy(buffer, &packet, sizeof(Packet));

  send(fd, buffer, sizeof(buffer), 0);

  return 0;
}

int stop_comms(void) {
  if (close(fd) < 0) {
    perror("Error while closing fd");
    return -1;
  }

  return 0;
}
