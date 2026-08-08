#pragma once

typedef struct Packet {
  int x;
  int y;
  char icon;
} Packet;

int start_server(void);
int start_client(char *server_ip);

int stop_comms(void);

int push_server(Packet packet);
int push_client(Packet packet);
int poll_server(Packet *packet);
int poll_client(Packet *packet);

void print_packet(Packet packet);
