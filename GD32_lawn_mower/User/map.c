#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "map.h"
#include "gd32f4xx_it.h"
#include "systick.h"


#define PI                      3.1415926
#define EARTH_RADIUS            6378.137        //µØÇò½üËÆ°ë¾¶

uint8_t GPS_COMMA=',';
uint8_t GPS_STOP='*';

MAP_POINT_t map_point[4] = {0};

double radian(double d);
double get_distance(double lat1, double lng1, double lat2, double lng2);

double radian(double d)
{
    return d * PI / 180.0; 
}

double change_ddmm_to_dd(const char *ddmm, char *dd)
{
    if (NULL == ddmm || NULL == dd) {
        return -1;
    }
    int lenSrc = strlen(ddmm)+1;
    int lenMm = 0;
    int flag = 1;
		
    memcpy(dd,ddmm,lenSrc);
		
    char *pcMm;
    double dMm;
    int iMm;
		
    pcMm = strstr(dd,".");
		
    if(pcMm == NULL) {
        pcMm = dd+strlen(dd)-2;
        iMm = atoi(pcMm);
		    dMm = iMm /60.0;
    }
		else {	
		    if (pcMm - dd > 2) {
            pcMm = pcMm - 2;
        }
		    else {
            pcMm = dd;
            flag = 0;
        }

        dMm = atof(pcMm);
        dMm /= 60.0;
    }

    sprintf(pcMm,"%lf",dMm);
		
    if(flag) {
        strcpy(pcMm,pcMm+1);
    }

    pcMm = strstr(dd,".");
    lenMm = strlen(pcMm);
		
    if(lenMm > (6+2)) {
        memset(pcMm+6+2,0,lenMm-6-2);
    }

    return 1;
}

double get_distance(double lat1, double lng1, double lat2, double lng2)
{
    double radLat1 = radian(lat1);
    double radLat2 = radian(lat2);
    double a = radLat1 - radLat2;
    double b = radian(lng1) - radian(lng2);
    double dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2) )));
    dst = dst * EARTH_RADIUS;
    dst= round(dst * 10000) / 10000;
    return dst;
}

uint8_t gps_data_pick(point_number_t point_num, uint8_t *data, uint8_t data_len)
{
		uint8_t i=0;
			uint8_t j=0;
				uint8_t k = 0;
	  uint8_t comma_flag = 0;
    MAP_POINT_t *p;
	  p = &map_point[point_num];

    if(data_len != 0) {
		    for(i = 0; i < data_len; i++) {
				    if(data[i] == GPS_COMMA) {
						    comma_flag++;
						}
						if(comma_flag == 2) {
						    if(data[i+1] != GPS_COMMA) {
							      p->lng_ddmm[j] = data[i+1];
					          j++;
								}
						}
						if(comma_flag == 4) {
						    if(data[i+1] != GPS_COMMA) {
						        p->lat_dddmm[k] = data[i+1];
						        k++;
                }
						}
						if(comma_flag == 5) {
						    if((j > 0)&&(k > 0)) {
								    return 0;
								}else {
								    return 1;
								}
						} 
				}
		}

}
