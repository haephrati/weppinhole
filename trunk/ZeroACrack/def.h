#include "aircrack-ptw-lib.h"

#define KEYLIMIT (1000000)
#define BSSIDLEN (6)
#define ESSIDLEN (64)

#define	STD_OPN		0x0001
#define	STD_WEP		0x0002
#define	STD_WPA		0x0004
#define	STD_WPA2	0x0008

struct network
{
	u_char essid[ESSIDLEN];
	u_char bssid[BSSIDLEN];
	uint8_t keyindex;

	PTW_attackstate * state;

	uint32_t bcn;
	uint32_t security;
	uint32_t channel;

	bool rcrack;
	bool sprofile;
	char key[64];
	struct network * next;
};
