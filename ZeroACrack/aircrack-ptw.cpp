#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "def.h"
network * networktable = NULL;

const uint8_t beginpacket[] = {0xAA, 0xAA, 0x03, 0x00, 0x00, 0x00, 0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x02 };

const uint8_t broadcast[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//
//void printKey(uint8_t * key, int keylen) 
//{
//        int i;
//        printf("Found key with len %02d: ", keylen);
//        for (i = 0; i < keylen; i++) 
//		{
//                printf("%02X ", key[i]);
//        }
//        printf("\n");
//}

void dumpacket( unsigned char *packet, int len )
{
	int dstoffset;
	uint8_t key[PTW_KEYHSBYTES];
	uint8_t buf[PTW_n];
	uint8_t iv[3];
	uint8_t keystream[16];
	uint8_t bssid[BSSIDLEN];
	int i,z;
	network *network_ptr;

	switch( packet[1] & 3 )
	{
	case  0: memcpy( bssid, packet + 16, 6 ); break;  //Adhoc
	case  1: memcpy( bssid, packet +  4, 6 ); break;  //ToDS
	case  2: memcpy( bssid, packet + 10, 6 ); break;  //FromDS
	case  3: memcpy( bssid, packet + 10, 6 ); break;  //WDS -> Transmitter taken as BSSID
	}

	network_ptr = networktable;
	while( network_ptr ) 
	{
		if ( memcmp(network_ptr->bssid, bssid, BSSIDLEN) == 0 ) 
		{
			break;
		}
		network_ptr = network_ptr->next;
	}

	if( network_ptr == NULL ) 
	{
		// Allocate a new table
		//printf("allocating a new table\n");
		//printf("bssid = %02X:%02X:%02X:%02X:%02X:%02X  keyindex=%01d\n", packet[bssoffset+0], packet[bssoffset+1], packet[bssoffset+2], packet[bssoffset+3], packet[bssoffset+4], packet[bssoffset+5], packet[27]);
		network *tmp = (network*) malloc(sizeof(network));
		tmp->state = PTW_newattackstate();
		if(tmp->state == NULL) 
		{
			// printf("could not allocate state\n");
			exit(-1);
		}
		memcpy( tmp->bssid, bssid, BSSIDLEN);
		tmp->keyindex = -1;
		tmp->bcn = 0;
		tmp->security = 0;
		tmp->channel = 0;
		tmp->sprofile = false;
		tmp->rcrack = false;
		memcpy( tmp->bssid, bssid, BSSIDLEN );

		tmp->next = networktable;
		networktable = network_ptr = tmp;
	}

	if( network_ptr->rcrack == true )
		return;

	if( packet[0] == 0x80 || packet[0] == 0x50 )
	{
		if( ( packet[34] & 0x10 ) >> 4 ) 
			network_ptr->security |= STD_WEP;
		else 
			network_ptr->security |= STD_OPN;

		unsigned char *p = packet + 36;

		while( p < packet + len )
		{
			if( p + 2 + p[1] > packet + len )
				break;

			switch( p[0] )
			{
			case 0x00:
				memcpy( network_ptr->essid, p + 2, p[1] );
				network_ptr->essid[p[1]] = 0;
				break;
			case 0x03:
				network_ptr->channel = p[2];
				break;
			case 0x30:
				if( p[2] == 1 )
				{

				}
				break;
			case 0xdd:
				break;
			}

			p += 2 + p[1];
		}
	}

	if( packet[0] == 0x08 ) 
	{
		if( network_ptr->keyindex == 0xff )
		{
			network_ptr->keyindex = packet[27]&0xc0;
		}

		if( network_ptr->keyindex == (packet[27]&0xc0) )
		{
			// ARP
			// Find the right state

			if (( packet[1] & 0x03) == 0x01) 
			{
				dstoffset = 10;
			} 
			else
			{
				dstoffset = 4;
			}

			++network_ptr->bcn;

			z = ( ( packet[1] & 3 ) != 3 ) ? 24 : 30;

			/* Check if 802.11e (QoS) */
			if( (packet[0] & 0x80) == 0x80) z+=2;

			if( (packet[1] & 0x40) != 0x40 )
			{
				network_ptr->security |= STD_OPN;
				network_ptr->rcrack = true;
				network_ptr->key[0] = 0;
				return;
			}
			else
			{
				if((packet[z+3] & 0x20) == 0x20)
				{
					network_ptr->security |= STD_WPA;
				}
				else
				{
					network_ptr->security |= STD_WEP;
				}
			}

			// Find IV
			for (i = 0; i < PTW_IVBYTES; i++) 
			{
				iv[i] = packet[24+i];
			}

			for (i = 0; i < PTW_KSBYTES; i++) 
			{
				keystream[i] = packet[28+i] ^ beginpacket[i];
			}

			if (memcmp(broadcast,&packet[dstoffset], 6) == 0) 
			{
				// it is a request
				// printf("found request\n");
				keystream[PTW_KSBYTES-1] ^= 0x03;
			}

			for (i = 0; i < PTW_KSBYTES; i++) 
			{
				buf[i] = keystream[i] ^ packet[28+i];
			}
			PTW_addsession(network_ptr->state, iv, keystream);

			//////////////////////////////////////////////////////////////////////////
			// crack;
			if( network_ptr->bcn%5000 == 0 )
			{
				if(PTW_computeKey(network_ptr->state, key, 13, KEYLIMIT) == 1) 
				{
					network_ptr->rcrack = true;
					strncpy( network_ptr->key, (char*)key, _countof(network_ptr->key) );
				}

				if(PTW_computeKey(network_ptr->state, key, 5, KEYLIMIT/10) == 1) 
				{
					network_ptr->rcrack = true;
					strncpy( network_ptr->key, (char*)key, _countof(network_ptr->key) );
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}

	//for ( k = 0; k < numstates; k++) 
	//{
	//	printf("stats for bssid %02X:%02X:%02X:%02X:%02X:%02X  keyindex=%01d packets=%d\n",
	//		networktable[k].bssid[0],
	//		networktable[k].bssid[1],
	//		networktable[k].bssid[2],
	//		networktable[k].bssid[3],
	//		networktable[k].bssid[4],
	//		networktable[k].bssid[5],
	//		networktable[k].keyindex,
	//		networktable[k].state->packets_collected);

	//	if(PTW_computeKey(networktable[k].state, key, 13, KEYLIMIT) == 1) 
	//	{
	//		// printKey(key, 13);
	//	}

	//	if(PTW_computeKey(networktable[k].state, key, 5, KEYLIMIT/10) == 1) 
	//	{
	//		// printKey(key, 5);
	//	}
	//}
}

void freenetwork( network* network_ptr )
{
	PTW_freeattackstate( network_ptr->state );
	free( network_ptr );
}