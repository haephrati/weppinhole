#include "stdafx.h"
#include "AirDump.h"
#include "aircrack-ptw-lib.h"

apinfo *aplst = NULL;
int PTW_DEFAULTWEIGHT[1] = { 256 };
int PTW_DEFAULTBF[PTW_KEYHSBYTES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

FILE* InitializeFile( unsigned char bssid[6] )
{
	char filename[256];
	_snprintf( filename, sizeof(filename), "%02x-%02x-%02x_%02x-%02x-%02x.ivs", bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5] );
	FILE *fp = fopen( filename, "wb+" );
	if( fp == NULL )
		return NULL;

	struct ivs2_filehdr fivs2;

	fivs2.version = IVS2_VERSION;

	if( fwrite( IVS2_MAGIC, 1, 4, fp ) != (size_t) 4 )
	{
		perror( "fwrite(IVs file MAGIC) failed" );
		return NULL;
	}

	if( fwrite( &fivs2, 1, sizeof(struct ivs2_filehdr), fp ) != (size_t) sizeof(struct ivs2_filehdr) )
	{
		perror( "fwrite(IVs file header) failed" );
		return NULL;
	}

	return fp;
}

apinfo *getap( unsigned char bssid[6] )
{
	apinfo *ap_cur = aplst;
	while( ap_cur )
	{
		if( memcmp( ap_cur->bssid, bssid, 6 ) == 0 )
			break;
		ap_cur = ap_cur->next;
	}

	if( ap_cur == NULL )
	{
		apinfo * ap = (apinfo*)malloc( sizeof(apinfo) );
		memset( ap, 0, sizeof(apinfo) );
		ap->bcn = 0;
		ap->pkt = 0;
		ap->nb_data = 0;
		ap->power = 0;
		memcpy( ap->bssid, bssid, 6 );
		ap->keylen = 13;

		ap->security = 0;
		ap->essid_stored = 0;
		ap->power = 0;
		ap->ssid_length = 0;
		ap->preamble = 0;
		ap->crack_result = false;
		ap->ivs = InitializeFile( bssid );
		ap->uiv_root = uniqueiv_init();

		ap->next = aplst;

		ap->ptw_clean = PTW_newattackstate();
		if (!ap->ptw_clean) 
		{
			perror("PTW_newattackstate()");
			free(ap);
			return NULL;
		}

		ap->ptw_vague = PTW_newattackstate();
		if (!ap->ptw_vague) 
		{
			perror("PTW_newattackstate()");
			free(ap_cur);
			return NULL;
		}

		ap->nb_ivs = 0;
		ap->nb_ivs_clean = 0;
		ap->nb_ivs_vague = 0;

		if( ap->ivs == NULL )
		{
			free( ap );
			return NULL;
		}

		aplst = ap;
		return ap;
	}

	return ap_cur;
}

int is_arp(unsigned char *wh, int len)
{
	int arpsize = 8 + 8 + 10*2;

	if(wh) {}
	/* remove non BROADCAST frames? could be anything, but
	* chances are good that we got an arp response tho.   */

	if (len == arpsize || len == 54)
		return 1;

	return 0;
}

int is_qos_arp_tkip(unsigned char *wh, int len)
{
	unsigned char *packet = wh;
	int qosarpsize = (24 + 2) + 8 + (8 + (8 + 10*2)) + 8 + 4; //82 in total

	if((packet[1] & 3) == 1) //to ds
	{
		if (len == qosarpsize) //always wireless
			return 1;
	}

	if((packet[1] & 3) == 2) //from ds
	{
		if (len == qosarpsize || len == qosarpsize + 18) //wireless or padded wired
			return 1;
	}

	return 0;
}

int is_spantree(unsigned char *wh)
{
	if ( memcmp( wh +  4, SPANTREE, 6 ) == 0 ||
		memcmp( wh + 16, SPANTREE, 6 ) == 0 )
		return 1;

	return 0;
}

int is_cdp_vtp(unsigned char *wh)
{
	if ( memcmp( wh +  4, CDP_VTP, 6 ) == 0 ||
		memcmp( wh + 16, CDP_VTP, 6 ) == 0 )
		return 1;

	return 0;
}

void *get_da(unsigned char *wh)
{
	if (wh[1] & IEEE80211_FC1_DIR_FROMDS)
		return wh + 4;
	else
		return wh + 4 + 6*2;
}

void *get_sa(unsigned char *wh)
{
	if (wh[1] & IEEE80211_FC1_DIR_FROMDS)
		return wh + 4 + 6*2;
	else
		return wh + 4 + 6;
}

/* weight is used for guesswork in PTW.  Can be null if known_clear is not for
* PTW, but just for getting known clear-text.
*/
int known_clear(unsigned char *clear, int *clen, int *weight, unsigned char *wh, int len)
{
	unsigned char *ptr = clear;
	int num;

	if(is_arp(wh, len)) /*arp*/
	{
		len = sizeof(S_LLC_SNAP_ARP) - 1;
		memcpy(ptr, S_LLC_SNAP_ARP, len);
		ptr += len;

		/* arp hdr */
		len = 6;
		memcpy(ptr, "\x00\x01\x08\x00\x06\x04", len);
		ptr += len;

		/* type of arp */
		len = 2;
		if (memcmp(get_da(wh), "\xff\xff\xff\xff\xff\xff", 6) == 0)
			memcpy(ptr, "\x00\x01", len);
		else
			memcpy(ptr, "\x00\x02", len);
		ptr += len;

		/* src mac */
		len = 6;
		memcpy(ptr, get_sa(wh), len);
		ptr += len;

		len = ptr - ((unsigned char*)clear);
		*clen = len;
		if (weight)
			weight[0] = 256;
		return 1;

	}
	else if(is_spantree(wh)) /*spantree*/
	{
		len = sizeof(S_LLC_SNAP_SPANTREE) - 1;
		memcpy(ptr, S_LLC_SNAP_SPANTREE, len);
		ptr += len;

		len = ptr - ((unsigned char*)clear);
		*clen = len;
		if (weight)
			weight[0] = 256;
		return 1;
	}
	else if(is_cdp_vtp(wh)) /*spantree*/
	{
		len = sizeof(S_LLC_SNAP_CDP) - 1;
		memcpy(ptr, S_LLC_SNAP_CDP, len);
		ptr += len;

		len = ptr - ((unsigned char*)clear);
		*clen = len;
		if (weight)
			weight[0] = 256;
		return 1;
	}
	else /* IP */
	{
		unsigned short iplen = htons(len - 8);

		//                printf("Assuming IP %d\n", len);

		len = sizeof(S_LLC_SNAP_IP) - 1;
		memcpy(ptr, S_LLC_SNAP_IP, len);
		ptr += len;
#if 1
		//version=4; header_length=20; services=0
		len = 2;
		memcpy(ptr, "\x45\x00", len);
		ptr += len;

		//ip total length
		memcpy(ptr, &iplen, len);
		ptr += len;

		/* no guesswork */
		if (!weight) {
			*clen = ptr - ((unsigned char*)clear);
			return 1;
		}
#if 1
		/* setting IP ID 0 is ok, as we
		* bruteforce it later
		*/
		//ID=0
		len=2;
		memcpy(ptr, "\x00\x00", len);
		ptr += len;

		//ip flags=don't fragment
		len=2;
		memcpy(ptr, "\x40\x00", len);
		ptr += len;
#endif
#endif
		len = ptr - ((unsigned char*)clear);
		*clen = len;

		memcpy(clear+32, clear, len);
		memcpy(clear+32+14, "\x00\x00", 2); //ip flags=none

		num=2;
		//assert(weight);
		weight[0] = 220;
		weight[1] = 36;

		return num;
	}
	*clen=0;
	return 1;
}

int crack_wep_ptw( apinfo *ap_cur )
{
	int (*all)[256];
	int i, j, len = 0;

	all = (int(*)[256])malloc(256*32*sizeof(int));
	if (all == NULL) 
	{
		return FAILURE;
	}

	//initial setup (complete keyspace)
	for (i = 0; i < 32; i++) 
	{
		for (j = 0; j < 256; j++) 
		{
			all[i][j] = 1;
		}
	}

	if(ap_cur->nb_ivs_clean > 99)
	{
		ap_cur->nb_ivs = ap_cur->nb_ivs_clean;
		//first try without bruteforcing, using only "clean" keystreams
		if(ap_cur->keylen != 13)
		{
			if(PTW_computeKey(ap_cur->ptw_clean, ap_cur->key, ap_cur->keylen, (KEYLIMIT*FFACT), PTW_DEFAULTBF, all, 0) == 1)
				len = ap_cur->keylen;
		}
		else
		{
			/* try 1000 40bit keys first, to find the key "instantly" and you don't need to wait for 104bit to fail */
			if(PTW_computeKey(ap_cur->ptw_clean, ap_cur->key, 5, 1000, PTW_DEFAULTBF, all, 0) == 1)
				len = 5;
			else if(PTW_computeKey(ap_cur->ptw_clean, ap_cur->key, 13, (KEYLIMIT*FFACT), PTW_DEFAULTBF, all, 0) == 1)
				len = 13;
			else if(PTW_computeKey(ap_cur->ptw_clean, ap_cur->key, 5, (KEYLIMIT*FFACT)/3, PTW_DEFAULTBF, all, 0) == 1)
				len = 5;
		}
	}

	if(!len)
	{
		ap_cur->nb_ivs = ap_cur->nb_ivs_vague;
		//in case its not found, try bruteforcing the id field and include "vague" keystreams
		PTW_DEFAULTBF[10]=1;
		PTW_DEFAULTBF[11]=1;
		//        PTW_DEFAULTBF[12]=1;

		if(ap_cur->keylen != 13)
		{
			if(PTW_computeKey(ap_cur->ptw_vague, ap_cur->key, ap_cur->keylen, (KEYLIMIT*FFACT), PTW_DEFAULTBF, all, 0) == 1)
				len = ap_cur->keylen;
		}
		else
		{
			/* try 1000 40bit keys first, to find the key "instantly" and you don't need to wait for 104bit to fail */
			if(PTW_computeKey(ap_cur->ptw_vague, ap_cur->key, 5, 1000, PTW_DEFAULTBF, all, 0) == 1)
				len = 5;
			else if(PTW_computeKey(ap_cur->ptw_vague, ap_cur->key, 13, (KEYLIMIT*FFACT), PTW_DEFAULTBF, all, 0) == 1)
				len = 13;
			else if(PTW_computeKey(ap_cur->ptw_vague, ap_cur->key, 5, (KEYLIMIT*FFACT)/10, PTW_DEFAULTBF, all, 0) == 1)
				len = 5;
		}
	}

	free( all );
	if (!len)
		return FAILURE;

	//puts( (const char*)ap_cur->key );
	return SUCCESS;
}

int dump_add_packet( global &G, unsigned char *h80211, int caplen )
{
	int i, n, z, seq, dlen, offset, clen, o;
	int type, length, numuni=0, numauth=0;
	struct ivs2_pkthdr ivs2;
	unsigned char *p, *org_p;
	unsigned char bssid[6];
	unsigned char stmac[6];
	unsigned char clear[2048];
	int weight[16];
	int num_xor=0;

	/* skip packets smaller than a 802.11 header */

	if( caplen < 24 )
		goto write_packet;

	/* skip (uninteresting) control frames */

	if( ( h80211[0] & 0x0C ) == 0x04 )
		goto write_packet;

	/* if it's a LLC null packet, just forget it (may change in the future) */

	if ( caplen > 28)
		if ( memcmp(h80211 + 24, llcnull, 4) == 0)
			return ( 0 );

	/* grab the sequence number */
	seq = ((h80211[22]>>4)+(h80211[23]<<4));

	/* locate the access point's MAC address */

	switch( h80211[1] & 3 )
	{
	case  0: memcpy( bssid, h80211 + 16, 6 ); break;  //Adhoc
	case  1: memcpy( bssid, h80211 +  4, 6 ); break;  //ToDS
	case  2: memcpy( bssid, h80211 + 10, 6 ); break;  //FromDS
	case  3: memcpy( bssid, h80211 + 10, 6 ); break;  //WDS -> Transmitter taken as BSSID
	}

	/* update our chained list of access points */
	struct apinfo* ap_cur = getap( bssid );

	// 已经破解的则不再继续分析数据包。
	if( ap_cur->crack_result )
		return 0;

	/* locate the station MAC in the 802.11 header */

	switch( h80211[1] & 3 )
	{
	case  0:

		/* if management, check that SA != BSSID */

		if( memcmp( h80211 + 10, bssid, 6 ) == 0 )
			goto skip_station;

		memcpy( stmac, h80211 + 10, 6 );
		break;

	case  1:

		/* ToDS packet, must come from a client */

		memcpy( stmac, h80211 + 10, 6 );
		break;

	case  2:

		/* FromDS packet, reject broadcast MACs */

		if( h80211[4] != 0 ) goto skip_station;
		memcpy( stmac, h80211 +  4, 6 ); break;

	default: 
		goto skip_station;
	}


skip_station:

	ap_cur->pkt++;
	/* packet parsing: Beacon or Probe Response */

	if( h80211[0] == 0x80 || h80211[0] == 0x50 )
	{
		++ap_cur->bcn;
		if( !(ap_cur->security & (STD_OPN|STD_WEP|STD_WPA|STD_WPA2)) )
		{
			if( ( h80211[34] & 0x10 ) >> 4 ) 
				ap_cur->security |= STD_WEP|ENC_WEP;
			else 
				ap_cur->security |= STD_OPN;
		}

		ap_cur->preamble = ( h80211[34] & 0x20 ) >> 5;

		p = h80211 + 36;

		while( p < h80211 + caplen )
		{
			if( p + 2 + p[1] > h80211 + caplen )
				break;

			//only update the essid length if the new length is > the old one
			if( p[0] == 0x00 && (ap_cur->ssid_length < p[1]) ) ap_cur->ssid_length = p[1];

			if( p[0] == 0x00 && p[1] > 0 && p[2] != '\0' &&	( p[1] > 1 || p[2] != ' ' ) )
			{
				/* found a non-cloaked ESSID */

				//                n = ( p[1] > 32 ) ? 32 : p[1];
				n = p[1];

				memset( ap_cur->essid, 0, sizeof(ap_cur->essid) );
				memcpy( ap_cur->essid, p + 2, n );

				if( ap_cur->ivs != NULL && !ap_cur->essid_stored )
				{
					memset(&ivs2, '\x00', sizeof(struct ivs2_pkthdr));
					ivs2.flags |= IVS2_ESSID;
					ivs2.len += ap_cur->ssid_length;

					if( memcmp( G.prev_bssid, ap_cur->bssid, 6 ) != 0 )
					{
						ivs2.flags |= IVS2_BSSID;
						ivs2.len += 6;
						memcpy( G.prev_bssid, ap_cur->bssid,  6 );
					}

					/* write header */
					if( fwrite( &ivs2, 1, sizeof(struct ivs2_pkthdr), ap_cur->ivs )
						!= (size_t) sizeof(struct ivs2_pkthdr) )
					{
						perror( "fwrite(IV header) failed" );
						return( 1 );
					}

					/* write BSSID */
					if(ivs2.flags & IVS2_BSSID)
					{
						if( fwrite( ap_cur->bssid, 1, 6, ap_cur->ivs ) != (size_t) 6 )
						{
							perror( "fwrite(IV bssid) failed" );
							return( 1 );
						}
					}

					/* write essid */
					if( fwrite( ap_cur->essid, 1, ap_cur->ssid_length, ap_cur->ivs ) != (size_t) ap_cur->ssid_length )
					{
						perror( "fwrite(IV essid) failed" );
						return( 1 );
					}

					ap_cur->essid_stored = 1;
				}

				for( i = 0; i < n; i++ )
					if( ( ap_cur->essid[i] >   0 && ap_cur->essid[i] <  32 ) ||
						( ap_cur->essid[i] > 126 && ap_cur->essid[i] < 160 ) )
						ap_cur->essid[i] = '.';
			}

			/* get the maximum speed in Mb and the AP's channel */

			if( p[0] == 0x01 || p[0] == 0x32 )
			{
				if(ap_cur->max_speed < ( p[1 + p[1]] & 0x7F ) / 2)
					ap_cur->max_speed = ( p[1 + p[1]] & 0x7F ) / 2;
			}

			if( p[0] == 0x03 )
				ap_cur->channel = p[2];

			p += 2 + p[1];
		}
	}

	/* packet parsing: Beacon & Probe response */

	if( (h80211[0] == 0x80 || h80211[0] == 0x50) && caplen > 38)
	{
		p=h80211+36;         //ignore hdr + fixed params

		while( p < h80211 + caplen )
		{
			type = p[0];
			length = p[1];
			if(p+2+length > h80211 + caplen) {
				/*                printf("error parsing tags! %p vs. %p (tag: %i, length: %i,position: %i)\n", (p+2+length), (h80211+caplen), type, length, (p-h80211));
				exit(1);*/
				break;
			}

			if( (type == 0xDD && (length >= 8) && (memcmp(p+2, "\x00\x50\xF2\x01\x01\x00", 6) == 0)) || (type == 0x30) )
			{
				ap_cur->security &= ~(STD_WEP|ENC_WEP|STD_WPA);

				org_p = p;
				offset = 0;

				if(type == 0xDD)
				{
					//WPA defined in vendor specific tag -> WPA1 support
					ap_cur->security |= STD_WPA;
					offset = 4;
				}

				if(type == 0x30)
				{
					ap_cur->security |= STD_WPA2;
					offset = 0;
				}

				if(length < (18+offset))
				{
					p += length+2;
					continue;
				}

				if( p+9+offset > h80211+caplen )
					break;
				numuni  = p[8+offset] + (p[9+offset]<<8);

				if( p+ (11+offset) + 4*numuni > h80211+caplen)
					break;
				numauth = p[(10+offset) + 4*numuni] + (p[(11+offset) + 4*numuni]<<8);

				p += (10+offset);

				if(type != 0x30)
				{
					if( p + (4*numuni) + (2+4*numauth) > h80211+caplen)
						break;
				}
				else
				{
					if( p + (4*numuni) + (2+4*numauth) + 2 > h80211+caplen)
						break;
				}

				for(i=0; i<numuni; i++)
				{
					switch(p[i*4+3])
					{
					case 0x01:
						ap_cur->security |= ENC_WEP;
						break;
					case 0x02:
						ap_cur->security |= ENC_TKIP;
						break;
					case 0x03:
						ap_cur->security |= ENC_WRAP;
						break;
					case 0x04:
						ap_cur->security |= ENC_CCMP;
						break;
					case 0x05:
						ap_cur->security |= ENC_WEP104;
						break;
					default:
						break;
					}
				}

				p += 2+4*numuni;

				for(i=0; i<numauth; i++)
				{
					switch(p[i*4+3])
					{
					case 0x01:
						ap_cur->security |= AUTH_MGT;
						break;
					case 0x02:
						ap_cur->security |= AUTH_PSK;
						break;
					default:
						break;
					}
				}

				p += 2+4*numauth;

				if( type == 0x30 ) p += 2;

				p = org_p + length+2;
			}
			else if( (type == 0xDD && (length >= 8) && (memcmp(p+2, "\x00\x50\xF2\x02\x01\x01", 6) == 0)))
			{
				ap_cur->security |= STD_QOS;
				p += length+2;
			}
			else p += length+2;
		}
	}

	/* packet parsing: Authentication Response */

	if( h80211[0] == 0xB0 && caplen >= 30)
	{
		if( ap_cur->security & STD_WEP )
		{
			//successful step 2 or 4 (coming from the AP)
			if(memcmp(h80211+28, "\x00\x00", 2) == 0 &&
				(h80211[26] == 0x02 || h80211[26] == 0x04))
			{
				ap_cur->security &= ~(AUTH_OPN | AUTH_PSK | AUTH_MGT);
				if(h80211[24] == 0x00) ap_cur->security |= AUTH_OPN;
				if(h80211[24] == 0x01) ap_cur->security |= AUTH_PSK;
			}
		}
	}

	/* packet parsing: Association Request */

	if( h80211[0] == 0x00 && caplen > 28 )
	{
		p = h80211 + 28;

		while( p < h80211 + caplen )
		{
			if( p + 2 + p[1] > h80211 + caplen )
				break;

			if( p[0] == 0x00 && p[1] > 0 && p[2] != '\0' &&
				( p[1] > 1 || p[2] != ' ' ) )
			{
				/* found a non-cloaked ESSID */

				n = ( p[1] > 32 ) ? 32 : p[1];

				memset( ap_cur->essid, 0, 33 );
				memcpy( ap_cur->essid, p + 2, n );

				if( ap_cur->ivs != NULL && !ap_cur->essid_stored )
				{
					memset(&ivs2, '\x00', sizeof(struct ivs2_pkthdr));
					ivs2.flags |= IVS2_ESSID;
					ivs2.len += ap_cur->ssid_length;

					if( memcmp( G.prev_bssid, ap_cur->bssid, 6 ) != 0 )
					{
						ivs2.flags |= IVS2_BSSID;
						ivs2.len += 6;
						memcpy( G.prev_bssid, ap_cur->bssid,  6 );
					}

					/* write header */
					if( fwrite( &ivs2, 1, sizeof(struct ivs2_pkthdr), ap_cur->ivs )
						!= (size_t) sizeof(struct ivs2_pkthdr) )
					{
						perror( "fwrite(IV header) failed" );
						return( 1 );
					}

					/* write BSSID */
					if(ivs2.flags & IVS2_BSSID)
					{
						if( fwrite( ap_cur->bssid, 1, 6, ap_cur->ivs )
							!= (size_t) 6 )
						{
							perror( "fwrite(IV bssid) failed" );
							return( 1 );
						}
					}

					/* write essid */
					if( fwrite( ap_cur->essid, 1, ap_cur->ssid_length, ap_cur->ivs )
						!= (size_t) ap_cur->ssid_length )
					{
						perror( "fwrite(IV essid) failed" );
						return( 1 );
					}

					ap_cur->essid_stored = 1;
				}

				for( i = 0; i < n; i++ )
					if( ap_cur->essid[i] < 32 ||
						( ap_cur->essid[i] > 126 && ap_cur->essid[i] < 160 ) )
						ap_cur->essid[i] = '.';
			}

			p += 2 + p[1];
		}
		++ap_cur->bcn;
	}

	/* packet parsing: some data */

	if( ( h80211[0] & 0x0C ) == 0x08 )
	{
		/* update the channel if we didn't get any beacon */

		//if( ap_cur->channel == -1 )
		//{
		//	if(ri->ri_channel > 0 && ri->ri_channel < 167)
		//		ap_cur->channel = ri->ri_channel;
		//	else
		//		ap_cur->channel = G.channel[cardnum];
		//}

		/* check the SNAP header to see if data is encrypted */

		z = ( ( h80211[1] & 3 ) != 3 ) ? 24 : 30;

		/* Check if 802.11e (QoS) */
		if( (h80211[0] & 0x80) == 0x80) z+=2;

		//if(z==24)
		//{
		//	if(list_check_decloak(&(ap_cur->packets), caplen, h80211) != 0)
		//	{
		//		list_add_packet(&(ap_cur->packets), caplen, h80211);
		//	}
		//	else
		//	{
		//		ap_cur->is_decloak = 1;
		//		ap_cur->decloak_detect = 0;
		//		list_tail_free(&(ap_cur->packets));
		//		memset(G.message, '\x00', sizeof(G.message));
		//		snprintf( G.message, sizeof( G.message ) - 1,
		//			"][ Decloak: %02X:%02X:%02X:%02X:%02X:%02X ",
		//			ap_cur->bssid[0], ap_cur->bssid[1], ap_cur->bssid[2],
		//			ap_cur->bssid[3], ap_cur->bssid[4], ap_cur->bssid[5]);
		//	}
		//}

		if( z + 26 > caplen )
			goto write_packet;

		if(ap_cur->security == 0 || (ap_cur->security & STD_WEP) )
		{
			if( (h80211[1] & 0x40) != 0x40 )
			{
				ap_cur->security |= STD_OPN;
			}
			else
			{
				if((h80211[z+3] & 0x20) == 0x20)
				{
					ap_cur->security |= STD_WPA;
				}
				else
				{
					ap_cur->security |= STD_WEP;
					if( (h80211[z+3] & 0xC0) != 0x00)
					{
						ap_cur->security |= ENC_WEP40;
					}
					else
					{
						ap_cur->security &= ~ENC_WEP40;
						ap_cur->security |= ENC_WEP;
					}
				}
			}
		}

		if( z + 10 > caplen )
			goto write_packet;

		if( ap_cur->security & STD_WEP )
		{
			/* WEP: check if we've already seen this IV */

			if( ! uniqueiv_check( ap_cur->uiv_root, &h80211[z] ) )
			{
				/* first time seen IVs */

				if( ap_cur->ivs != NULL )
				{
					memset(&ivs2, '\x00', sizeof(struct ivs2_pkthdr));
					ivs2.flags = 0;
					ivs2.len = 0;

					/* datalen = caplen - (header+iv+ivs) */
					dlen = caplen -z -4 -4; //original data len
					if(dlen > 2048) dlen = 2048;
					//get cleartext + len + 4(iv+idx)
					num_xor = known_clear(clear, &clen, weight, h80211, dlen);
					if(num_xor == 1)
					{
						ivs2.flags |= IVS2_XOR;
						ivs2.len += clen + 4;
						/* reveal keystream (plain^encrypted) */
						for(n=0; n<(ivs2.len-4); n++)
						{
							clear[n] = (clear[n] ^ h80211[z+4+n]) & 0xFF;
						}
						//clear is now the keystream
					}
					else
					{
						//do it again to get it 2 bytes higher
						num_xor = known_clear(clear+2, &clen, weight, h80211, dlen);
						ivs2.flags |= IVS2_PTW;
						//len = 4(iv+idx) + 1(num of keystreams) + 1(len per keystream) + 32*num_xor + 16*sizeof(int)(weight[16])
						ivs2.len += 4 + 1 + 1 + 32*num_xor + 16*sizeof(int);
						clear[0] = num_xor;
						clear[1] = clen;
						/* reveal keystream (plain^encrypted) */
						for(o=0; o<num_xor; o++)
						{
							for(n=0; n<(ivs2.len-4); n++)
							{
								clear[2+n+o*32] = (clear[2+n+o*32] ^ h80211[z+4+n]) & 0xFF;
							}
						}
						memcpy(clear+4 + 1 + 1 + 32*num_xor, weight, 16*sizeof(int));
						//clear is now the keystream
					}

					if( memcmp( G.prev_bssid, ap_cur->bssid, 6 ) != 0 )
					{
						ivs2.flags |= IVS2_BSSID;
						ivs2.len += 6;
						memcpy( G.prev_bssid, ap_cur->bssid,  6 );
					}

					//////////////////////////////////////////////////////////////////////////
					// add by albert.xu
					if( (ivs2.flags & IVS2_ESSID) || (ivs2.flags & IVS2_BSSID) )
					{
					}
					else if( ivs2.flags & IVS2_XOR )
					{
						if( ivs2.len >= 16 )
						{
							if (PTW_addsession(ap_cur->ptw_clean, (h80211+z), clear, PTW_DEFAULTWEIGHT, 1))
								ap_cur->nb_ivs_clean++;

							if (PTW_addsession(ap_cur->ptw_vague, (h80211+z), clear, PTW_DEFAULTWEIGHT, 1))
								ap_cur->nb_ivs_vague++;
						}
					}
					else if( ivs2.flags & IVS2_PTW )
					{
						int clearsize;

						clearsize = ivs2.len;
						//int w[16];

						if (clear[1] >= 13 && clearsize >= (6 + clear[0]*32 + 16*(signed)sizeof(int)))
						{
							//memcpy(w, clear+(ivs2.len-4)-15*sizeof(int), 16*sizeof(int));

							if (PTW_addsession(ap_cur->ptw_vague, h80211+z, clear+2, weight, clear[0]))
								ap_cur->nb_ivs_vague++;
						}
					}
					//////////////////////////////////////////////////////////////////////////

					if( fwrite( &ivs2, 1, sizeof(struct ivs2_pkthdr), ap_cur->ivs )
						!= (size_t) sizeof(struct ivs2_pkthdr) )
					{
						perror( "fwrite(IV header) failed" );
						return( 1 );
					}

					if( ivs2.flags & IVS2_BSSID )
					{
						if( fwrite( ap_cur->bssid, 1, 6, ap_cur->ivs ) != (size_t) 6 )
						{
							perror( "fwrite(IV bssid) failed" );
							return( 1 );
						}
						ivs2.len -= 6;
					}

					if( fwrite( h80211+z, 1, 4, ap_cur->ivs ) != (size_t) 4 )
					{
						perror( "fwrite(IV iv+idx) failed" );
						return( 1 );
					}
					ivs2.len -= 4;

					if( fwrite( clear, 1, ivs2.len, ap_cur->ivs ) != (size_t) ivs2.len )
					{
						perror( "fwrite(IV keystream) failed" );
						return( 1 );
					}

					fflush( ap_cur->ivs );
				}

				uniqueiv_mark( ap_cur->uiv_root, &h80211[z] );
			}

		}
		ap_cur->nb_data++;

		//if( ap_cur->nb_ivs_vague && ap_cur->nb_ivs_vague%5000 == 0 )
		//{
		//	int ret = crack_wep_ptw(ap_cur);
		//	if( ret == FAILURE )
		//	{
		//		memset( &ap_cur->wep, 0, sizeof(ap_cur->wep) );
		//	}
		//	else if( ret == SUCCESS )
		//	{
		//		ap_cur->crack_result = true;
		//	}
		//}
	}
write_packet:
	return( 0 );
}