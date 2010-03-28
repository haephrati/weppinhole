#pragma once
#define SUCCESS  0
#define FAILURE  1
#define RESTART  2

#define KEYLIMIT 1000000
#define FFACT 2
//BSSID const. length of 6 bytes; can be together with all the other types
#define IVS2_BSSID	0x0001

//ESSID var. length; alone, or with BSSID
#define IVS2_ESSID	0x0002

//wpa structure, const. length; alone, or with BSSID
#define IVS2_WPA	0x0004

//IV+IDX+KEYSTREAM, var. length; alone or with BSSID
#define IVS2_XOR	0x0008

/* [IV+IDX][i][l][XOR_1]..[XOR_i][weight]                                                        *
* holds i possible keystreams for the same IV with a length of l for each keystream (l max 32)  *
* and an array "int weight[16]" at the end                                                      */
#define IVS2_PTW        0x0010

//unencrypted packet
#define IVS2_CLR        0x0020

#define	STD_OPN		0x0001
#define	STD_WEP		0x0002
#define	STD_WPA		0x0004
#define	STD_WPA2	0x0008

#define	ENC_WEP		0x0010
#define	ENC_TKIP	0x0020
#define	ENC_WRAP	0x0040
#define	ENC_CCMP	0x0080
#define ENC_WEP40	0x1000
#define	ENC_WEP104	0x0100

#define	AUTH_OPN	0x0200
#define	AUTH_PSK	0x0400
#define	AUTH_MGT	0x0800

#define STD_QOS         0x2000

#define IVS2_MAGIC              "\xAE\x78\xD1\xFF"
#define IVS2_VERSION             1

#define S_LLC_SNAP      "\xAA\xAA\x03\x00\x00\x00"
#define S_LLC_SNAP_ARP  (S_LLC_SNAP "\x08\x06")
#define S_LLC_SNAP_IP   (S_LLC_SNAP "\x08\x00")
#define S_LLC_SNAP_SPANTREE   "\x42\x42\x03\x00\x00\x00\x00\x00"
#define S_LLC_SNAP_CDP  "\xAA\xAA\x03\x00\x00\x0C\x20"
#define IEEE80211_FC1_DIR_FROMDS                0x02    /* AP ->STA */

#define TYPE_ARP    0
#define TYPE_IP     1

#define NULL_MAC  (unsigned char*)"\x00\x00\x00\x00\x00\x00"
#define BROADCAST (unsigned char*)"\xFF\xFF\xFF\xFF\xFF\xFF"
#define SPANTREE  (unsigned char*)"\x01\x80\xC2\x00\x00\x00"
#define CDP_VTP   (unsigned char*)"\x01\x00\x0C\xCC\xCC\xCC"
#include "aircrack-ptw-lib.h"

const unsigned char llcnull[4] = {0, 0, 0, 0};

struct ivs2_filehdr
{
	unsigned short version;
};

struct ivs2_pkthdr
{
	unsigned short  flags;
	unsigned short  len;
};

struct global
{
	char prev_bssid[6];
};

struct apinfo
{
	struct apinfo *next;

	unsigned char key[64];
	int power;
	int bcn;
	int pkt;
	int nb_data;
	unsigned int security;
	int preamble;
	int keylen;

	unsigned char bssid[6];
	char essid[256];

	char channel;
	char max_speed;
	short ssid_length;
	short essid_stored;
	
	unsigned char **uiv_root;	/* unique iv root structure */
								/* if wep-encrypted network */

	PTW_attackstate *ptw_clean;
	PTW_attackstate *ptw_vague;
	int nb_ivs;
	int nb_ivs_clean;
	int nb_ivs_vague;
	bool crack_result;
	CRITICAL_SECTION lock;
	FILE *ivs;
};

extern apinfo *aplst;
apinfo *getap( char bssid[6] );
int crack_wep_ptw( apinfo *ap_cur );
int dump_add_packet( global &G, unsigned char *h80211, int caplen );