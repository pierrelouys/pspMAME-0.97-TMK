/*****************************************************************************
*
*	Yamaha YM2151 driver (version 2.150 final beta)
*
******************************************************************************/

//typedef INT16 FMSAMPLE;
//typedef INT32 FMSAMPLE_MIX;

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define FREQ_SH 		16	/* 16.16 fixed point (frequency calculations) */
#define EG_SH			16	/* 16.16 fixed point (envelope generator timing) */
#define LFO_SH			10	/* 22.10 fixed point (LFO calculations) 	  */
#define TIMER_SH		16	/* 16.16 fixed point (timers calculations)	  */

#define FREQ_MASK		((1<<FREQ_SH)-1)

#define ENV_BITS		10
#define ENV_LEN 		(1<<ENV_BITS)
#define ENV_STEP		(128.0/ENV_LEN)

#define MAX_ATT_INDEX	(ENV_LEN-1) /* 1023 */
#define MIN_ATT_INDEX	(0) 		/* 0 */

#define SIN_BITS		10
#define SIN_LEN 		(1<<SIN_BITS)
#define SIN_MASK		(SIN_LEN-1)

#define TL_RES_LEN		(256) /* 8 bits addressing (real chip) */


#define FINAL_SH	(0)
#define MAXOUT		(+32767)
#define MINOUT		(-32768)


/*	TL_TAB_LEN is calculated as:
*	13 - sinus amplitude bits	  (Y axis)
*	2  - sinus sign bit 		  (Y axis)
*	TL_RES_LEN - sinus resolution (X axis)
*/
#define TL_TAB_LEN (13*2*TL_RES_LEN)
static INT32 tl_tab[TL_TAB_LEN*2];

#define ENV_QUIET		(TL_TAB_LEN>>3)

/* sin waveform table in 'decibel' scale */
static UINT32 sin_tab[SIN_LEN];


/* translate from D1L to volume index (16 D1L levels) */
static UINT32 d1l_tab[16];


#define RATE_STEPS (8)
static const UINT8 eg_inc[19*RATE_STEPS]={

/*cycle:0 1  2 3  4 5  6 7*/

/* 0 */ 0,1, 0,1, 0,1, 0,1, /* rates 00..11 0 (increment by 0 or 1) */
/* 1 */ 0,1, 0,1, 1,1, 0,1, /* rates 00..11 1 */
/* 2 */ 0,1, 1,1, 0,1, 1,1, /* rates 00..11 2 */
/* 3 */ 0,1, 1,1, 1,1, 1,1, /* rates 00..11 3 */

/* 4 */ 1,1, 1,1, 1,1, 1,1, /* rate 12 0 (increment by 1) */
/* 5 */ 1,1, 1,2, 1,1, 1,2, /* rate 12 1 */
/* 6 */ 1,2, 1,2, 1,2, 1,2, /* rate 12 2 */
/* 7 */ 1,2, 2,2, 1,2, 2,2, /* rate 12 3 */

/* 8 */ 2,2, 2,2, 2,2, 2,2, /* rate 13 0 (increment by 2) */
/* 9 */ 2,2, 2,4, 2,2, 2,4, /* rate 13 1 */
/*10 */ 2,4, 2,4, 2,4, 2,4, /* rate 13 2 */
/*11 */ 2,4, 4,4, 2,4, 4,4, /* rate 13 3 */

/*12 */ 4,4, 4,4, 4,4, 4,4, /* rate 14 0 (increment by 4) */
/*13 */ 4,4, 4,8, 4,4, 4,8, /* rate 14 1 */
/*14 */ 4,8, 4,8, 4,8, 4,8, /* rate 14 2 */
/*15 */ 4,8, 8,8, 4,8, 8,8, /* rate 14 3 */

/*16 */ 8,8, 8,8, 8,8, 8,8, /* rates 15 0, 15 1, 15 2, 15 3 (increment by 8) */
/*17 */ 16,16,16,16,16,16,16,16, /* rates 15 2, 15 3 for attack */
/*18 */ 0,0, 0,0, 0,0, 0,0, /* infinity rates for attack and decay(s) */
};


#define O(a) (a*RATE_STEPS)

/*note that there is no O(17) in this table - it's directly in the code */
static const UINT8 PSP_ALIGN_DATA eg_rate_select[32+64+32]={	/* Envelope Generator rates (32 + 64 rates + 32 RKS) */
/* 32 dummy (infinite time) rates */
O(18),O(18),O(18),O(18),O(18),O(18),O(18),O(18),
O(18),O(18),O(18),O(18),O(18),O(18),O(18),O(18),
O(18),O(18),O(18),O(18),O(18),O(18),O(18),O(18),
O(18),O(18),O(18),O(18),O(18),O(18),O(18),O(18),

/* rates 00-11 */
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),

/* rate 12 */
O( 4),O( 5),O( 6),O( 7),

/* rate 13 */
O( 8),O( 9),O(10),O(11),

/* rate 14 */
O(12),O(13),O(14),O(15),

/* rate 15 */
O(16),O(16),O(16),O(16),

/* 32 dummy rates (same as 15 3) */
O(16),O(16),O(16),O(16),O(16),O(16),O(16),O(16),
O(16),O(16),O(16),O(16),O(16),O(16),O(16),O(16),
O(16),O(16),O(16),O(16),O(16),O(16),O(16),O(16),
O(16),O(16),O(16),O(16),O(16),O(16),O(16),O(16)

};
#undef O

/*rate	0,	  1,	2,	 3,   4,   5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15*/
/*shift 11,   10,	9,	 8,   7,   6,  5,  4,  3,  2, 1,  0,  0,  0,  0,  0 */
/*mask	2047, 1023, 511, 255, 127, 63, 31, 15, 7,  3, 1,  0,  0,  0,  0,  0 */

#define O(a) (a*1)
static const UINT8 PSP_ALIGN_DATA eg_rate_shift[32+64+32]={ /* Envelope Generator counter shifts (32 + 64 rates + 32 RKS) */
/* 32 infinite time rates */
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),


/* rates 00-11 */
O(11),O(11),O(11),O(11),
O(10),O(10),O(10),O(10),
O( 9),O( 9),O( 9),O( 9),
O( 8),O( 8),O( 8),O( 8),
O( 7),O( 7),O( 7),O( 7),
O( 6),O( 6),O( 6),O( 6),
O( 5),O( 5),O( 5),O( 5),
O( 4),O( 4),O( 4),O( 4),
O( 3),O( 3),O( 3),O( 3),
O( 2),O( 2),O( 2),O( 2),
O( 1),O( 1),O( 1),O( 1),
O( 0),O( 0),O( 0),O( 0),

/* rate 12 */
O( 0),O( 0),O( 0),O( 0),

/* rate 13 */
O( 0),O( 0),O( 0),O( 0),

/* rate 14 */
O( 0),O( 0),O( 0),O( 0),

/* rate 15 */
O( 0),O( 0),O( 0),O( 0),

/* 32 dummy rates (same as 15 3) */
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0)

};
#undef O


/*	DT1 defines offset in Hertz from base note
*	This table is converted while initialization...
*	Detune table shown in YM2151 User's Manual is wrong (verified on the real chip)
*/

static const UINT8 PSP_ALIGN_DATA dt1_tab[4*32] = { /* 4*32 DT1 values */
/* DT1=0 */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

/* DT1=1 */
  0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
  2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 8, 8, 8, 8,

/* DT1=2 */
  1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5,
  5, 6, 6, 7, 8, 8, 9,10,11,12,13,14,16,16,16,16,

/* DT1=3 */
  2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7,
  8, 8, 9,10,11,12,13,14,16,17,19,20,22,22,22,22
};

static const UINT16 PSP_ALIGN_DATA phaseinc_rom[768]={
1299,1300,1301,1302,1303,1304,1305,1306,1308,1309,1310,1311,1313,1314,1315,1316,
1318,1319,1320,1321,1322,1323,1324,1325,1327,1328,1329,1330,1332,1333,1334,1335,
1337,1338,1339,1340,1341,1342,1343,1344,1346,1347,1348,1349,1351,1352,1353,1354,
1356,1357,1358,1359,1361,1362,1363,1364,1366,1367,1368,1369,1371,1372,1373,1374,
1376,1377,1378,1379,1381,1382,1383,1384,1386,1387,1388,1389,1391,1392,1393,1394,
1396,1397,1398,1399,1401,1402,1403,1404,1406,1407,1408,1409,1411,1412,1413,1414,
1416,1417,1418,1419,1421,1422,1423,1424,1426,1427,1429,1430,1431,1432,1434,1435,
1437,1438,1439,1440,1442,1443,1444,1445,1447,1448,1449,1450,1452,1453,1454,1455,
1458,1459,1460,1461,1463,1464,1465,1466,1468,1469,1471,1472,1473,1474,1476,1477,
1479,1480,1481,1482,1484,1485,1486,1487,1489,1490,1492,1493,1494,1495,1497,1498,
1501,1502,1503,1504,1506,1507,1509,1510,1512,1513,1514,1515,1517,1518,1520,1521,
1523,1524,1525,1526,1528,1529,1531,1532,1534,1535,1536,1537,1539,1540,1542,1543,
1545,1546,1547,1548,1550,1551,1553,1554,1556,1557,1558,1559,1561,1562,1564,1565,
1567,1568,1569,1570,1572,1573,1575,1576,1578,1579,1580,1581,1583,1584,1586,1587,
1590,1591,1592,1593,1595,1596,1598,1599,1601,1602,1604,1605,1607,1608,1609,1610,
1613,1614,1615,1616,1618,1619,1621,1622,1624,1625,1627,1628,1630,1631,1632,1633,
1637,1638,1639,1640,1642,1643,1645,1646,1648,1649,1651,1652,1654,1655,1656,1657,
1660,1661,1663,1664,1666,1667,1669,1670,1672,1673,1675,1676,1678,1679,1681,1682,
1685,1686,1688,1689,1691,1692,1694,1695,1697,1698,1700,1701,1703,1704,1706,1707,
1709,1710,1712,1713,1715,1716,1718,1719,1721,1722,1724,1725,1727,1728,1730,1731,
1734,1735,1737,1738,1740,1741,1743,1744,1746,1748,1749,1751,1752,1754,1755,1757,
1759,1760,1762,1763,1765,1766,1768,1769,1771,1773,1774,1776,1777,1779,1780,1782,
1785,1786,1788,1789,1791,1793,1794,1796,1798,1799,1801,1802,1804,1806,1807,1809,
1811,1812,1814,1815,1817,1819,1820,1822,1824,1825,1827,1828,1830,1832,1833,1835,
1837,1838,1840,1841,1843,1845,1846,1848,1850,1851,1853,1854,1856,1858,1859,1861,
1864,1865,1867,1868,1870,1872,1873,1875,1877,1879,1880,1882,1884,1885,1887,1888,
1891,1892,1894,1895,1897,1899,1900,1902,1904,1906,1907,1909,1911,1912,1914,1915,
1918,1919,1921,1923,1925,1926,1928,1930,1932,1933,1935,1937,1939,1940,1942,1944,
1946,1947,1949,1951,1953,1954,1956,1958,1960,1961,1963,1965,1967,1968,1970,1972,
1975,1976,1978,1980,1982,1983,1985,1987,1989,1990,1992,1994,1996,1997,1999,2001,
2003,2004,2006,2008,2010,2011,2013,2015,2017,2019,2021,2022,2024,2026,2028,2029,
2032,2033,2035,2037,2039,2041,2043,2044,2047,2048,2050,2052,2054,2056,2058,2059,
2062,2063,2065,2067,2069,2071,2073,2074,2077,2078,2080,2082,2084,2086,2088,2089,
2092,2093,2095,2097,2099,2101,2103,2104,2107,2108,2110,2112,2114,2116,2118,2119,
2122,2123,2125,2127,2129,2131,2133,2134,2137,2139,2141,2142,2145,2146,2148,2150,
2153,2154,2156,2158,2160,2162,2164,2165,2168,2170,2172,2173,2176,2177,2179,2181,
2185,2186,2188,2190,2192,2194,2196,2197,2200,2202,2204,2205,2208,2209,2211,2213,
2216,2218,2220,2222,2223,2226,2227,2230,2232,2234,2236,2238,2239,2242,2243,2246,
2249,2251,2253,2255,2256,2259,2260,2263,2265,2267,2269,2271,2272,2275,2276,2279,
2281,2283,2285,2287,2288,2291,2292,2295,2297,2299,2301,2303,2304,2307,2308,2311,
2315,2317,2319,2321,2322,2325,2326,2329,2331,2333,2335,2337,2338,2341,2342,2345,
2348,2350,2352,2354,2355,2358,2359,2362,2364,2366,2368,2370,2371,2374,2375,2378,
2382,2384,2386,2388,2389,2392,2393,2396,2398,2400,2402,2404,2407,2410,2411,2414,
2417,2419,2421,2423,2424,2427,2428,2431,2433,2435,2437,2439,2442,2445,2446,2449,
2452,2454,2456,2458,2459,2462,2463,2466,2468,2470,2472,2474,2477,2480,2481,2484,
2488,2490,2492,2494,2495,2498,2499,2502,2504,2506,2508,2510,2513,2516,2517,2520,
2524,2526,2528,2530,2531,2534,2535,2538,2540,2542,2544,2546,2549,2552,2553,2556,
2561,2563,2565,2567,2568,2571,2572,2575,2577,2579,2581,2583,2586,2589,2590,2593
};


/*
	Noise LFO waveform.

	Here are just 256 samples out of much longer data.

	It does NOT repeat every 256 samples on real chip and I wasnt able to find
	the point where it repeats (even in strings as long as 131072 samples).

	I only put it here because its better than nothing and perhaps
	someone might be able to figure out the real algorithm.


	Note that (due to the way the LFO output is calculated) it is quite
	possible that two values: 0x80 and 0x00 might be wrong in this table.
	To be exact:
		some 0x80 could be 0x81 as well as some 0x00 could be 0x01.
*/

static const UINT8 PSP_ALIGN_DATA lfo_noise_waveform[256] = {
0xff,0xee,0xd3,0x80,0x58,0xda,0x7f,0x94,0x9e,0xe3,0xfa,0x00,0x4d,0xfa,0xff,0x6a,
0x7a,0xde,0x49,0xf6,0x00,0x33,0xbb,0x63,0x91,0x60,0x51,0xff,0x00,0xd8,0x7f,0xde,
0xdc,0x73,0x21,0x85,0xb2,0x9c,0x5d,0x24,0xcd,0x91,0x9e,0x76,0x7f,0x20,0xfb,0xf3,
0x00,0xa6,0x3e,0x42,0x27,0x69,0xae,0x33,0x45,0x44,0x11,0x41,0x72,0x73,0xdf,0xa2,

0x32,0xbd,0x7e,0xa8,0x13,0xeb,0xd3,0x15,0xdd,0xfb,0xc9,0x9d,0x61,0x2f,0xbe,0x9d,
0x23,0x65,0x51,0x6a,0x84,0xf9,0xc9,0xd7,0x23,0xbf,0x65,0x19,0xdc,0x03,0xf3,0x24,
0x33,0xb6,0x1e,0x57,0x5c,0xac,0x25,0x89,0x4d,0xc5,0x9c,0x99,0x15,0x07,0xcf,0xba,
0xc5,0x9b,0x15,0x4d,0x8d,0x2a,0x1e,0x1f,0xea,0x2b,0x2f,0x64,0xa9,0x50,0x3d,0xab,

0x50,0x77,0xe9,0xc0,0xac,0x6d,0x3f,0xca,0xcf,0x71,0x7d,0x80,0xa6,0xfd,0xff,0xb5,
0xbd,0x6f,0x24,0x7b,0x00,0x99,0x5d,0xb1,0x48,0xb0,0x28,0x7f,0x80,0xec,0xbf,0x6f,
0x6e,0x39,0x90,0x42,0xd9,0x4e,0x2e,0x12,0x66,0xc8,0xcf,0x3b,0x3f,0x10,0x7d,0x79,
0x00,0xd3,0x1f,0x21,0x93,0x34,0xd7,0x19,0x22,0xa2,0x08,0x20,0xb9,0xb9,0xef,0x51,

0x99,0xde,0xbf,0xd4,0x09,0x75,0xe9,0x8a,0xee,0xfd,0xe4,0x4e,0x30,0x17,0xdf,0xce,
0x11,0xb2,0x28,0x35,0xc2,0x7c,0x64,0xeb,0x91,0x5f,0x32,0x0c,0x6e,0x00,0xf9,0x92,
0x19,0xdb,0x8f,0xab,0xae,0xd6,0x12,0xc4,0x26,0x62,0xce,0xcc,0x0a,0x03,0xe7,0xdd,
0xe2,0x4d,0x8a,0xa6,0x46,0x95,0x0f,0x8f,0xf5,0x15,0x97,0x32,0xd4,0x28,0x1e,0x55
};



/* struct describing a single operator */
typedef struct
{
//	INT32 *connect; 		/* operator output 'direction' */

//	UINT32 phase;			/* accumulated operator phase */
	UINT32 freq;			/* operator frequency count */
//16
	UINT8/*32*/ ar; 		/* attack rate	*/
	UINT8/*32*/ d1r;		/* decay rate	*/
	UINT8/*32*/ d2r;		/* sustain rate */
	UINT8/*32*/ rr; 		/* release rate */
//
	UINT8/*32*/ mul;		/* frequency count multiply */
	UINT8/*32*/ ks; 		/* key scale	*/
	UINT8/*32*/ state;		/* Envelope state: 4-attack(AR) 3-decay(D1R) 2-sustain(D2R) 1-release(RR) 0-off */

	INT8/*32*/ dt1; 		/* current DT1 (detune 1 phase inc/decrement) value */
//24:16+8
	UINT32 dt1_i;			/* DT1 index * 32 */
//32:16+8+8
	UINT16/*32*/ tl;		/* Total attenuation Level */
	INT16/*32*/ volume; 	/* current envelope attenuation level */
	UINT16/*32*/ d1l;		/* envelope switches to sustain state after reaching this level */
	UINT16/*32*/ dt2;		/* current DT2 (detune 2) value */
//40:32+8
	UINT8	eg_sh_ar;		/* (attack state) */
	UINT8	eg_sel_ar;		/* (attack state) */
	UINT8	eg_sh_d1r;		/* (decay state) */
	UINT8	eg_sel_d1r; 	/* (decay state) */

	UINT8	eg_sh_d2r;		/* (sustain state) */
	UINT8	eg_sel_d2r; 	/* (sustain state) */
	UINT8	eg_sh_rr;		/* (release state) */
	UINT8	eg_sel_rr;		/* (release state) */
//48:32+8+8
	UINT16/*32*/ AMmask;	/* LFO Amplitude Modulation enable mask */
	UINT8/*32*/ key;		/* 0=last key was KEY OFF, 1=last key was KEY ON */
	UINT8 align_dummy111;	/* dummy for .align */
//[52]
} FM_SLOT;
typedef struct{
	/* channel specific data; note: each operator number 0 contains channel specific data */
	FM_SLOT SLOT[4];				/* the 4 operators per 1 channel. */
	UINT32 slot0_phase; 			/* accumulated operator phase */
	UINT32 slot1_phase; 			/* accumulated operator phase */
	UINT32 slot2_phase; 			/* accumulated operator phase */
	UINT32 slot3_phase; 			/* accumulated operator phase */

	UINT32 fb_shift;				/* feedback shift value for operators 0 in each channel */

//	INT32 *connect1;				/* SLOT1 output pointer */
//	INT32 *connect3;				/* SLOT3 output pointer */
//	INT32 *connect2;				/* SLOT2 output pointer */
//	UINT32 *connect4;				/* SLOT4 output pointer */
//	INT32 *chanout_chan_ptr;		/* SLOT4 output pointer */
//static INT32 PSP_ALIGN_DATA chanout_chan;
///
	UINT32 pms; 					/* channel PMS */
//.align
	UINT32 kc;						/* channel KC (copied to all operators) */
	UINT32 kc_i;					/* just for speedup */

	/* only M1 (operator 0) is filled with this data: */
//	INT32 *mem_connect; 			/* where to put the delayed sample (MEM) */
	INT32 mem_value;				/* delayed sample (MEM) value */
	/* channel specific data; note: each operator number 0 contains channel specific data */
	INT32 fb_out_curr;				/* operator feedback value (used only by operators 0) */
	INT32 fb_out_prev;				/* previous feedback value (used only by operators 0) */

	UINT8	ALGO_pan;//connect[8];	/* channels connections */
	UINT8/*32*/ ams;				/* channel AMS */
	/* end of channel specific data */
//	UINT8/*32*/ pan;//[8];			/* channels output masks (0xffffffff = enable) */
	UINT8 align_dummy222;			/* dummy for .align */
	UINT8 align_dummy333;			/* dummy for .align */
} OPM_CH;

typedef struct
{
	OPM_CH	opm_ch[8];				/* 2151 had 8 channels. */
//	FM_OPM oper[32];				/* the 32 operators */

	UINT32 eg_cnt;					/* global envelope generator counter */
	UINT32 eg_timer;				/* global envelope generator counter works at frequency = chipclock/64/3 */
	UINT32 eg_timer_add_length; 	/* step of eg_timer */
	UINT32 eg_timer_overflow;		/* envelope generator timer overlfows every 3 samples (on real chip) */

	UINT32 lfo_phase;				/* accumulated LFO phase (0 to 255) */
	UINT32 lfo_timer;				/* LFO timer						*/
	UINT32 lfo_timer_add_length;	/* step of lfo_timer				*/
	UINT32 lfo_overflow;			/* LFO generates new output when lfo_timer reaches this value */
	UINT32 lfo_counter; 			/* LFO phase increment counter		*/
	UINT32 lfo_counter_add; 		/* step of lfo_counter				*/
	UINT8  lfo_wsel;				/* LFO waveform (0-saw, 1-square, 2-triangle, 3-random noise) */
	UINT8  amd; 					/* LFO Amplitude Modulation Depth	*/
	INT8   pmd; 					/* LFO Phase Modulation Depth		*/
	UINT32 lfa; 					/* LFO current AM output			*/
	INT32  lfp; 					/* LFO current PM output			*/

	UINT8 test; 					/* TEST register */
	UINT8 ct;						/* output control pins (bit1-CT2, bit0-CT1) */

	UINT32 noise;					/* noise enable/period register (bit 7 - noise enable, bits 4-0 - noise period */
	UINT32 noise_rng;				/* 17 bit noise shift register */
	UINT32 noise_p; 				/* current noise 'phase'*/
	UINT32 noise_f; 				/* current noise period */

	UINT32 csm_req; 				/* CSM	KEY ON / KEY OFF sequence request */

	UINT32 irq_enable;				/* IRQ enable for timer B (bit 3) and timer A (bit 2); bit 7 - CSM mode (keyon to all slots, everytime timer A overflows) */
	UINT32 status;					/* chip status (BUSY, IRQ Flags) */

	/* ASG 980324 -- added for tracking timers */
	mame_timer	*timer_A;
	mame_timer	*timer_B;
	float timer_A_time[1024];		/* timer A times */
	float timer_B_time[256];		/* timer B times */
	UINT32 timer_A_index;			/* timer A index */
	UINT32 timer_B_index;			/* timer B index */
	UINT32 timer_A_index_old;		/* timer A previous index */
	UINT32 timer_B_index_old;		/* timer B previous index */

	/*	Frequency-deltas to get the closest frequency possible.
	*	There are 11 octaves because of DT2 (max 950 cents over base frequency)
	*	and LFO phase modulation (max 800 cents below AND over base frequency)
	*	Summary:   octave  explanation
	*			   0	   note code - LFO PM
	*			   1	   note code
	*			   2	   note code
	*			   3	   note code
	*			   4	   note code
	*			   5	   note code
	*			   6	   note code
	*			   7	   note code
	*			   8	   note code
	*			   9	   note code + DT2 + LFO PM
	*			   10	   note code + DT2 + LFO PM
	*/
	UINT32 freq[11*768];			/* 11 octaves, 768 'cents' per octave */

	/*	Frequency deltas for DT1. These deltas alter operator frequency
	*	after it has been taken from frequency-deltas table.
	*/
	INT32 dt1_freq[8*32];		/* 8 DT1 levels, 32 KC values */

	UINT32 noise_tab[32];			/* 17bit Noise Generator periods */

	FM_IRQHANDLER_2151 irqhandler;		/* IRQ function handler */

	UINT32 clock;					/* chip clock in Hz (passed from 2151intf.c) */
	UINT32 sampfreq;				/* sampling frequency in Hz (passed from 2151intf.c) */

} ym2151_t;

/* these variables stay here for speedup purposes only */
static ym2151_t PSP_ALIGN_DATA YM2151;
static ym2151_t *ym2151 = &YM2151;

//static INT32 PSP_ALIGN_DATA chanout[8];



#define EG_ATT			4
#define EG_DEC			3
#define EG_SUS			2
#define EG_REL			1
#define EG_OFF			0

#define FM_KEYON(op, key_set)								\
{															\
	if (!(op)->key) 										\
	{														\
		(op)->phase = 0;			/* clear phase */		\
		(op)->state = EG_ATT;		/* KEY ON = attack */	\
	}														\
	(op)->key |= key_set;									\
}

#define FM_KEYOFF(op, key_clr)								\
{															\
	if ((op)->key)											\
	{														\
		(op)->key &= key_clr;								\
		if (!(op)->key) 									\
		{													\
			if ((op)->state>EG_REL) 						\
				(op)->state = EG_REL;/* KEY OFF = release */\
		}													\
	}														\
}



static void init_tables(void)
{
	INT32 i, x, n;
	double o, m;

	for (x = 0; x < TL_RES_LEN; x++)
	{
		m = (1 << 16) / pow(2, (x + 1) * (ENV_STEP / 4.0) / 8.0);
		m = floor(m);

		/* we never reach (1<<16) here due to the (x+1) */
		/* result fits within 16 bits at maximum */

		n = (int)m; 	/* 16 bits here */
		n >>= 4;		/* 12 bits here */
		if (n & 1)		/* round to closest */
			n = (n >> 1) + 1;
		else
			n = n >> 1;
						/* 11 bits here (rounded) */
		n <<= 2;		/* 13 bits here (as in real chip) */
		tl_tab[x * 2 + 0] = n;
		tl_tab[x * 2 + 1] = -tl_tab[x * 2 + 0];

		for (i = 1; i < 13; i++)
		{
			tl_tab[x*2+0 + i*2*TL_RES_LEN] =  tl_tab[x*2+0] >> i;
			tl_tab[x*2+1 + i*2*TL_RES_LEN] = -tl_tab[x*2+0 + i*2*TL_RES_LEN];
		}
	}

	for (i = 0; i < SIN_LEN; i++)
	{
		/* non-standard sinus */
		m = sin(((i * 2) + 1) * PI / SIN_LEN); /* verified on the real chip */

		/* we never reach zero here due to ((i*2)+1) */

		if (m > 0.0)
			o = 8*log(1.0/m)/log(2);	/* convert to 'decibels' */
		else
			o = 8*log(-1.0/m)/log(2);	/* convert to 'decibels' */

		o = o / (ENV_STEP/4);

		n = (int)(2.0*o);
		if (n & 1)						/* round to closest */
			n = (n >> 1) + 1;
		else
			n = n >> 1;

		sin_tab[i] = n * 2 + (m >= 0.0 ? 0 : 1);
	}

	/* calculate d1l_tab table */
	for (i=0; i<16; i++)
	{
		m = (i!=15 ? i : i+16) * (4.0/ENV_STEP);   /* every 3 'dB' except for all bits = 1 = 45+48 'dB' */
		d1l_tab[i] = m;
	}
}


static void init_chip_tables(void)
{
	int i,j;
	double mult,pom,phaseinc,Hz;
	double scaler;

	scaler = ((double)ym2151->clock / 64.0) / ((double)ym2151->sampfreq);

	/* this loop calculates Hertz values for notes from c-0 to b-7 */
	/* including 64 'cents' (100/64 that is 1.5625 of real cent) per note */
	/* i*100/64/1200 is equal to i/768 */

	/* real chip works with 10 bits fixed point values (10.10) */
	mult = (1<<(FREQ_SH-10)); /* -10 because phaseinc_rom table values are already in 10.10 format */

	for (i = 0; i < 768; i++)
	{
		/* 3.4375 Hz is note A; C# is 4 semitones higher */
	//	Hz = 1000;

		phaseinc = phaseinc_rom[i]; /* real chip phase increment */
		phaseinc *= scaler; 		/* adjust */


		/* octave 2 - reference octave */
		ym2151->freq[768+2*768+i] = ((int)(phaseinc*mult)) & 0xffffffc0; /* adjust to X.10 fixed point */
		/* octave 0 and octave 1 */
		for (j=0; j<2; j++)
		{
			ym2151->freq[768 + j*768 + i] = (ym2151->freq[768+2*768+i] >> (2-j)) & 0xffffffc0; /* adjust to X.10 fixed point */
		}
		/* octave 3 to 7 */
		for (j=3; j<8; j++)
		{
			ym2151->freq[768 + j*768 + i] = ym2151->freq[768+2*768+i] << (j-2);
		}
	}

	/* octave -1 (all equal to: oct 0, _KC_00_, _KF_00_) */
	for (i=0; i<768; i++)
	{
		ym2151->freq[0*768 + i] = ym2151->freq[1*768+0];
	}

	/* octave 8 and 9 (all equal to: oct 7, _KC_14_, _KF_63_) */
	for (j=8; j<10; j++)
	{
		for (i=0; i<768; i++)
		{
			ym2151->freq[768+ j*768 + i] = ym2151->freq[768 + 8*768 -1];
		}
	}

	mult = (1<<FREQ_SH);
	for (j=0; j<4; j++)
	{
		for (i=0; i<32; i++)
		{
			Hz = ((double)dt1_tab[j*32+i] * ((double)ym2151->clock/64.0)) / (double)(1<<20);

			/*calculate phase increment*/
			phaseinc = (Hz*SIN_LEN) / (double)ym2151->sampfreq;

			/*positive and negative values*/
			ym2151->dt1_freq[(j+0)*32 + i] = phaseinc * mult;
			ym2151->dt1_freq[(j+4)*32 + i] = -ym2151->dt1_freq[(j+0)*32 + i];
		}
	}


	/* calculate timers' deltas */
	/* User's Manual pages 15,16  */
//	mult = (1<<TIMER_SH);
	for (i=0; i<1024; i++)
	{
		/* ASG 980324: changed to compute both tim_A_tab and timer_A_time */
		pom= (64.0	*  (1024.0-i) / (double)ym2151->clock);
		ym2151->timer_A_time[i] = (float)pom;
	}
	for (i=0; i<256; i++)
	{
		/* ASG 980324: changed to compute both tim_B_tab and timer_B_time */
		pom= (1024.0 * (256.0-i)  / (double)ym2151->clock);
		ym2151->timer_B_time[i] = (float)pom;
	}

	/* calculate noise periods table */
	scaler = ((double)ym2151->clock / 64.0) / ((double)ym2151->sampfreq);
	for (i=0; i<32; i++)
	{
		j = (i!=31 ? i : 30);				/* rate 30 and 31 are the same */
		j = 32-j;
		j = (65536.0 / (double)(j*32.0));	/* number of samples per one shift of the shift register */
		/*ym2151->noise_tab[i] = j * 64;*/	/* number of chip clock cycles per one shift */
		ym2151->noise_tab[i] = j * 64 * scaler;
		/*logerror("noise_tab[%02x]=%08x\n", i, ym2151->noise_tab[i]);*/
	}
}

//#define FM_KEYON(op, key_set)
//{
//	if (!(op)->key)
//	{
//		(op)->phase = 0;			/* clear phase */
//		(op)->state = EG_ATT;		/* KEY ON = attack */
//	}
//	(op)->key |= key_set;
//}

//static void envelope_KONKOFF(FM_OPM *op, int v)



void timer_callback_2151_a(void *param)
{
	timer_adjust_ptr(ym2151->timer_A, ym2151->timer_A_time[ym2151->timer_A_index], NULL, 0);
	ym2151->timer_A_index_old = ym2151->timer_A_index;

	if (ym2151->irq_enable & 0x04)
	{
//		int oldstate = ym2151->status & 3;

		ym2151->status |= 1;
//		if (!oldstate) (*ym2151->irqhandler)(1);
	}
	if (ym2151->irq_enable & 0x80)
		ym2151->csm_req = 2;		/* request KEY ON / KEY OFF sequence */
}

void timer_callback_2151_b(void *param)
{
	timer_adjust_ptr(ym2151->timer_B, ym2151->timer_B_time[ym2151->timer_B_index], NULL, 0);
	ym2151->timer_B_index_old = ym2151->timer_B_index;

	if (ym2151->irq_enable & 0x08)
	{
//		int oldstate = ym2151->status & 3;

		ym2151->status |= 2;
//		if (!oldstate) (*ym2151->irqhandler)(1);
	}
}







/* write a register on YM2151 chip number 'n' */
void YM2151WriteReg(int r, int v)
{
//	FM_OPM *op = &ym2151->oper[((r & 7) << 2) + ((r & 0x18) >> 3)];
	OPM_CH	*chan= &ym2151->opm_ch[(r&0x07)];
	FM_SLOT *op = &chan->SLOT[ ((r&0x18)>>3) ];

	/* adjust bus to 8 bits */
	r &= 0xff;
	v &= 0xff;

	switch (r & 0xe0)
	{
	case 0x00:
		switch(r)
		{
		case 0x01:	/* LFO reset(bit 1), Test Register (other bits) */
			ym2151->test = v;
			if (v&2) ym2151->lfo_phase = 0;
			break;

		case 0x08:
//			envelope_KONKOFF(&ym2151->oper[(v & 7) << 2], v);
		{
			OPM_CH	*chan_aaa= &ym2151->opm_ch[(v&0x07)];
		//	PSG = chip; /* PSG is used in FM_KEYON macro */
//			envelope_KONKOFF(&chan_aaa->SLOT[0], v );
//INLINE void envelope_KONKOFF(FM_SLOT * op, int v)
{
	if (v & 0x08)
	{
	//	FM_KEYON (op+0, 1)
{
	if (!chan_aaa->SLOT[0].key)
	{
		chan_aaa->slot0_phase = 0;			/* clear phase */
		chan_aaa->SLOT[0].state = EG_ATT;		/* KEY ON = attack */
	}
	chan_aaa->SLOT[0].key |= /*key_set*/1;
}
	}	else{		FM_KEYOFF(&chan_aaa->SLOT[0],~1)} /* M1 */
	if (v & 0x20)
	{
	//	FM_KEYON (op+1, 1)
{
	if (!chan_aaa->SLOT[1].key)
	{
		chan_aaa->slot1_phase = 0;			/* clear phase */
		chan_aaa->SLOT[1].state = EG_ATT;		/* KEY ON = attack */
	}
	chan_aaa->SLOT[1].key |= /*key_set*/1;
}
	}	else{		FM_KEYOFF(&chan_aaa->SLOT[1],~1)} /* M2 */
	if (v & 0x10)
	{
	//	FM_KEYON (op+2, 1)
{
	if (!chan_aaa->SLOT[2].key)
	{
		chan_aaa->slot2_phase = 0;			/* clear phase */
		chan_aaa->SLOT[2].state = EG_ATT;		/* KEY ON = attack */
	}
	chan_aaa->SLOT[2].key |= /*key_set*/1;
}
	}	else{		FM_KEYOFF(&chan_aaa->SLOT[2],~1)} /* C1 */
	if (v & 0x40)
	{
	//	FM_KEYON (op+3, 1)
{
	if (!chan_aaa->SLOT[3].key)
	{
		chan_aaa->slot3_phase = 0;			/* clear phase */
		chan_aaa->SLOT[3].state = EG_ATT;		/* KEY ON = attack */
	}
	chan_aaa->SLOT[3].key |= /*key_set*/1;
}
	}	else{		FM_KEYOFF(&chan_aaa->SLOT[3],~1)} /* C2 */
}

		}
			break;

		case 0x0f:	/* noise mode enable, noise period */
			ym2151->noise = v;
			ym2151->noise_f = ym2151->noise_tab[v & 0x1f];
			break;

		case 0x10:	/* timer A hi */
			ym2151->timer_A_index = (ym2151->timer_A_index & 0x003) | (v << 2);
			break;

		case 0x11:	/* timer A low */
			ym2151->timer_A_index = (ym2151->timer_A_index & 0x3fc) | (v & 3);
			break;

		case 0x12:	/* timer B */
			ym2151->timer_B_index = v;
			break;

		case 0x14:	/* CSM, irq flag reset, irq enable, timer start/stop */

			ym2151->irq_enable = v; /* bit 3-timer B, bit 2-timer A, bit 7 - CSM */

			if (v & 0x20)	/* reset timer B irq flag */
			{
//				int oldstate = ym2151->status & 3;

				ym2151->status &= 0xfd;
//				if (oldstate == 2) (*ym2151->irqhandler)(0);
			}

			if (v & 0x10)	/* reset timer A irq flag */
			{
//				int oldstate = ym2151->status & 3;

				ym2151->status &= 0xfe;
//				if (oldstate == 1) (*ym2151->irqhandler)(0);
			}

			if (v & 0x02)	/* load and start timer B */
			{
				/* ASG 980324: added a real timer */
				/* start timer _only_ if it wasn't already started (it will reload time value next round) */
				if (!timer_enable(ym2151->timer_B, 1))
				{
					timer_adjust_ptr(ym2151->timer_B, ym2151->timer_B_time[ym2151->timer_B_index], NULL, 0);
					ym2151->timer_B_index_old = ym2151->timer_B_index;
				}
			}
			else	/* stop timer B */
			{
				/* ASG 980324: added a real timer */
				timer_enable(ym2151->timer_B, 0);
			}

			if (v & 0x01)	/* load and start timer A */
			{
				if (!timer_enable(ym2151->timer_A, 1))
				{
					timer_adjust_ptr(ym2151->timer_A, ym2151->timer_A_time[ym2151->timer_A_index], NULL, 0);
					ym2151->timer_A_index_old = ym2151->timer_A_index;
				}
			}
			else	/* stop timer A */
			{
				/* ASG 980324: added a real timer */
				timer_enable(ym2151->timer_A, 0);
			}
			break;

		case 0x18:	/* LFO frequency */
			ym2151->lfo_overflow	= (1 << ((15 - (v >> 4)) + 3)) << LFO_SH;
			ym2151->lfo_counter_add = 0x10 + (v & 0x0f);
			break;

		case 0x19:	/* PMD (bit 7==1) or AMD (bit 7==0) */
			if (v & 0x80)
				ym2151->pmd = v & 0x7f;
			else
				ym2151->amd = v & 0x7f;
			break;

		case 0x1b:	/* CT2, CT1, LFO waveform */
			ym2151->ct = v >> 6;
			ym2151->lfo_wsel = v & 3;
			break;

		default:
//			logerror("YM2151 Write %02x to undocumented register #%02x\n",v,r);
			break;
		}
		break;

	case 0x20:
//		op = &ym2151->oper[(r & 7) << 2];
		op = &chan->SLOT[ 0 ];
		switch (r & 0x18)
		{
		case 0x00:	/* RL enable, Feedback, Connection */
//			op->fb_shift = ((v >> 3) & 7) ? ((v >> 3) & 7) + 6 : 0;
			chan->fb_shift = ((v >> 3) & 7) ? ((v >> 3) & 7) + 6 : 0;
#if 0
//			ym2151->pan[(r&7)*2   ] = (v & 0x40) ? ~0 : 0;
//			ym2151->pan[(r&7)*2 +1] = (v & 0x80) ? ~0 : 0;
			chan->pan[0/*mame_L*/] = (v & 0x40) ? ~0 : 0;
			chan->pan[1/*mame_R*/] = (v & 0x80) ? ~0 : 0;
#else
//			ym2151->pan[r & 7] = (v & 0xc0) ? ~0 : 0;
//			chan->pan = (v & 0xc0) ? ~0 : 0;
//			chan->pan = (v & 0xc0) ?  1 : 0;
#endif
//			ym2151->connect[r & 7] = v & 7;
//			chan->ALGO = v & 7;
			chan->ALGO_pan = v;

//			set_connect(op, r & 7, v & 7);
//			set_connect(chan, r&7);
//			set_connect(chan/*, r&7*/);
			break;

		case 0x08:	/* Key Code */
			v &= 0x7f;
//			if (v != op->kc)
			if (v != chan->kc)
			{
				UINT32 kc_channel;

				kc_channel = (v - (v>>2)) << 6;
				kc_channel += 768;
				kc_channel |= (chan->kc_i & 63);

				chan->kc = v;
				chan->kc_i = kc_channel;

				//refresh_EG( op , chan->kc);
				//INLINE void refresh_EG(FM_SLOT * op, UINT32 kc)
				{
					UINT8 jj;
					for (jj=0; jj<4; jj++)
					{
						chan->SLOT[jj].dt1 = ym2151->dt1_freq[chan->SLOT[jj].dt1_i + /*kc*/(v>>2)];
						chan->SLOT[jj].freq = (
							(ym2151->freq[kc_channel + chan->SLOT[jj].dt2]
							+ chan->SLOT[jj].dt1) * chan->SLOT[jj].mul) >> 1;

						/* vvv = 32 + 2*RATE + RKS = max 126 */
						UINT32 vvv;
						vvv = ((UINT32)v) >> chan->SLOT[jj].ks;
						if ((chan->SLOT[jj].ar+vvv) < 32+62)
						{
							chan->SLOT[jj].eg_sh_ar  = eg_rate_shift [chan->SLOT[jj].ar  + vvv];
							chan->SLOT[jj].eg_sel_ar = eg_rate_select[chan->SLOT[jj].ar  + vvv];
						}
						else
						{
							chan->SLOT[jj].eg_sh_ar  = 0;
							chan->SLOT[jj].eg_sel_ar = 17*RATE_STEPS;
						}
						chan->SLOT[jj].eg_sh_d1r = eg_rate_shift [chan->SLOT[jj].d1r + vvv];
						chan->SLOT[jj].eg_sel_d1r= eg_rate_select[chan->SLOT[jj].d1r + vvv];
						chan->SLOT[jj].eg_sh_d2r = eg_rate_shift [chan->SLOT[jj].d2r + vvv];
						chan->SLOT[jj].eg_sel_d2r= eg_rate_select[chan->SLOT[jj].d2r + vvv];
						chan->SLOT[jj].eg_sh_rr  = eg_rate_shift [chan->SLOT[jj].rr  + vvv];
						chan->SLOT[jj].eg_sel_rr = eg_rate_select[chan->SLOT[jj].rr  + vvv];
					}
				}
			}
			break;

		case 0x10:	/* Key Fraction */
			v >>= 2;
//			if (v !=  (op->kc_i & 63))
			if (v !=  (chan->kc_i & 63))
			{
				UINT32 kc_channel;

				kc_channel = v;
//				kc_channel |= (op->kc_i & ~63);
				kc_channel |= (chan->kc_i & ~63);

//				(op+0)->kc_i = kc_channel;
//				(op+1)->kc_i = kc_channel;
//				(op+2)->kc_i = kc_channel;
//				(op+3)->kc_i = kc_channel;
				chan->kc_i = kc_channel;

				(op+0)->freq = ((ym2151->freq[kc_channel + (op+0)->dt2] + (op+0)->dt1) * (op+0)->mul) >> 1;
				(op+1)->freq = ((ym2151->freq[kc_channel + (op+1)->dt2] + (op+1)->dt1) * (op+1)->mul) >> 1;
				(op+2)->freq = ((ym2151->freq[kc_channel + (op+2)->dt2] + (op+2)->dt1) * (op+2)->mul) >> 1;
				(op+3)->freq = ((ym2151->freq[kc_channel + (op+3)->dt2] + (op+3)->dt1) * (op+3)->mul) >> 1;
			}
			break;

		case 0x18:	/* PMS, AMS */
//			op->pms = (v>>4) & 7;
			chan->pms = (v>>4) & 7;
//			op->ams = (v & 3);
			chan->ams = (v & 3);
			break;
		}
		break;

	case 0x40:		/* DT1, MUL */
		{
			UINT32 olddt1_i = op->dt1_i;
			UINT32 oldmul = op->mul;

			op->dt1_i = (v&0x70)<<1;
			op->mul   = (v&0x0f) ? (v&0x0f)<<1: 1;

//			if (olddt1_i != op->dt1_i)
//				op->dt1 = ym2151->dt1_freq[op->dt1_i + (op->kc>>2)];
			if (olddt1_i != op->dt1_i)
				op->dt1 = ym2151->dt1_freq[op->dt1_i + (chan->kc>>2)];

//			if ((olddt1_i != op->dt1_i) || (oldmul != op->mul))
//				op->freq = ((ym2151->freq[op->kc_i + op->dt2] + op->dt1) * op->mul) >> 1;
			if ((olddt1_i != op->dt1_i) || (oldmul != op->mul))
				op->freq = ((ym2151->freq[chan->kc_i + op->dt2] + op->dt1) * op->mul) >> 1;
		}
		break;

	case 0x60:		/* TL */
		op->tl = (v&0x7f)<<(ENV_BITS-7); /* 7bit TL */
		break;

	case 0x80:		/* KS, AR */
		{
			UINT32 oldks = op->ks;
			UINT32 oldar = op->ar;

			op->ks = 5-(v>>6);
			op->ar = (v&0x1f) ? 32 + ((v&0x1f)<<1) : 0;

			if ((op->ar != oldar) || (op->ks != oldks))
			{
//				if ((op->ar + (op->kc>>op->ks)) < 32+62)
				if ((op->ar + (chan->kc>>op->ks)) < 32+62)
				{
//					op->eg_sh_ar  = eg_rate_shift [op->ar  + (op->kc>>op->ks)];
//					op->eg_sel_ar = eg_rate_select[op->ar  + (op->kc>>op->ks)];
					op->eg_sh_ar  = eg_rate_shift [op->ar  + (chan->kc>>op->ks)];
					op->eg_sel_ar = eg_rate_select[op->ar  + (chan->kc>>op->ks)];
				}
				else
				{
					op->eg_sh_ar  = 0;
					op->eg_sel_ar = 17*RATE_STEPS;
				}
			}

			if (op->ks != oldks)
			{
//				op->eg_sh_d1r = eg_rate_shift [op->d1r + (op->kc>>op->ks)];
//				op->eg_sel_d1r= eg_rate_select[op->d1r + (op->kc>>op->ks)];
//				op->eg_sh_d2r = eg_rate_shift [op->d2r + (op->kc>>op->ks)];
//				op->eg_sel_d2r= eg_rate_select[op->d2r + (op->kc>>op->ks)];
//				op->eg_sh_rr  = eg_rate_shift [op->rr  + (op->kc>>op->ks)];
//				op->eg_sel_rr = eg_rate_select[op->rr  + (op->kc>>op->ks)];

				op->eg_sh_d1r = eg_rate_shift [op->d1r + (chan->kc>>op->ks)];
				op->eg_sel_d1r= eg_rate_select[op->d1r + (chan->kc>>op->ks)];
				op->eg_sh_d2r = eg_rate_shift [op->d2r + (chan->kc>>op->ks)];
				op->eg_sel_d2r= eg_rate_select[op->d2r + (chan->kc>>op->ks)];
				op->eg_sh_rr  = eg_rate_shift [op->rr  + (chan->kc>>op->ks)];
				op->eg_sel_rr = eg_rate_select[op->rr  + (chan->kc>>op->ks)];
			}
		}
		break;

	case 0xa0:		/* LFO AM enable, D1R */
		op->AMmask = (v&0x80) ? ~0 : 0;
		op->d1r    = (v&0x1f) ? 32 + ((v&0x1f)<<1) : 0;
//		op->eg_sh_d1r = eg_rate_shift [op->d1r + (op->kc>>op->ks)];
//		op->eg_sel_d1r= eg_rate_select[op->d1r + (op->kc>>op->ks)];
		op->eg_sh_d1r = eg_rate_shift [op->d1r + (chan->kc>>op->ks)];
		op->eg_sel_d1r= eg_rate_select[op->d1r + (chan->kc>>op->ks)];
		break;

	case 0xc0:		/* DT2, D2R */
		{
/*	DT2 defines offset in cents from base note
*
*	This table defines offset in frequency-deltas table.
*	User's Manual page 22
*
*	Values below were calculated using formula: value =  orig.val / 1.5625
*
*	DT2=0 DT2=1 DT2=2 DT2=3
*	0	  600	781   950
*/
static const UINT32/*16*//*32*/ /*PSP_ALIGN_DATA*/ dt2_tab[4] = { 0, 384, 500, 608 };
			UINT16/*32*/ olddt2 = op->dt2;
			op->dt2 = dt2_tab[v>>6];
//			if (op->dt2 != olddt2)
//				op->freq = ((ym2151->freq[op->kc_i + op->dt2] + op->dt1) * op->mul) >> 1;
			if (op->dt2 != olddt2)
				op->freq = ((ym2151->freq[chan->kc_i + op->dt2] + op->dt1) * op->mul) >> 1;
		}
		op->d2r = (v&0x1f) ? 32 + ((v&0x1f)<<1) : 0;
//		op->eg_sh_d2r = eg_rate_shift [op->d2r + (op->kc>>op->ks)];
//		op->eg_sel_d2r= eg_rate_select[op->d2r + (op->kc>>op->ks)];
		op->eg_sh_d2r = eg_rate_shift [op->d2r + (chan->kc>>op->ks)];
		op->eg_sel_d2r= eg_rate_select[op->d2r + (chan->kc>>op->ks)];
		break;

	case 0xe0:		/* D1L, RR */
		op->d1l = d1l_tab[v>>4];
		op->rr	= 34 + ((v&0x0f)<<2);
//		op->eg_sh_rr  = eg_rate_shift [op->rr  + (op->kc>>op->ks)];
//		op->eg_sel_rr = eg_rate_select[op->rr  + (op->kc>>op->ks)];
		op->eg_sh_rr  = eg_rate_shift [op->rr  + (chan->kc>>op->ks)];
		op->eg_sel_rr = eg_rate_select[op->rr  + (chan->kc>>op->ks)];
		break;
	}
}


int YM2151ReadStatus(void)
{
	return ym2151->status;
}


#define VOLUME_CALC0 (CH->SLOT[0].tl + ((UINT32)(CH->SLOT[0].volume)) + (AM & (CH->SLOT[0].AMmask)))
#define VOLUME_CALC1 (CH->SLOT[1].tl + ((UINT32)(CH->SLOT[1].volume)) + (AM & (CH->SLOT[1].AMmask)))
#define VOLUME_CALC2 (CH->SLOT[2].tl + ((UINT32)(CH->SLOT[2].volume)) + (AM & (CH->SLOT[2].AMmask)))
#define VOLUME_CALC3 (CH->SLOT[3].tl + ((UINT32)(CH->SLOT[3].volume)) + (AM & (CH->SLOT[3].AMmask)))
#define VOLUME_CALC7 (CH->SLOT[3].tl + ((UINT32)(CH->SLOT[3].volume))								)


//INLINE INT32 op_calc(FM_OPM *OP, UINT32 env, INT32 pm)
INLINE INT32 op_calc(UINT32 phase, UINT32 env, INT32 pm)
{
//	UINT32 p = (env << 3) + sin_tab[(((INT32)((OP->phase & ~FREQ_MASK) + pm)) >> FREQ_SH) & SIN_MASK];
	UINT32 p = (env << 3) + sin_tab[(((INT32)((phase & ~FREQ_MASK) + pm)) >> FREQ_SH) & SIN_MASK];

//	if (p >= TL_TAB_LEN)	return 0;
	return tl_tab[p];
}


#define PAN_R 0x80
#define PAN_L 0x40








static void chan_calc_ALGO0(OPM_CH	*CH,s16 *mixing_buffer, int length)
{
int ii;
	UINT32 env;
//
	UINT32 AM = 0;
	if (CH->ams)
		AM = ym2151->lfa << (CH->ams-1);
///////////
INT32 CH_fb_out_prev=CH->fb_out_prev;
INT32 CH_fb_out_curr=CH->fb_out_curr;
INT32 CH_mem_value=CH->mem_value;
///////////
	for (ii = 0; ii < length; ii++)
	{
//ALGO0:
{
// INT32 m2/*,*//*c1,*//*c2*/;				/* Phase Modulation input for operators 2,3,4 */

		/* M1---C1---MEM---M2---C2---OUT */
//		CH->connect1 = &c1;
//		CH->connect2 = &mem;
//		CH->connect3 = &c2;
//		CH->mem_connect = &m2;

/*	m2 =*/ /*c1 =*/ /*c2 =*/

///*	*CH->mem_connect*/m2 = CH_mem_value;	/* restore delayed sample (MEM) value to m2 or c2 */

	env = VOLUME_CALC0;
	{
		INT32 out = CH_fb_out_prev + CH_fb_out_curr;
		CH_fb_out_prev = CH_fb_out_curr;

	//	if (!CH->connect1)	/* M1 */
	//		/* algorithm 5 */
	//		mem = c1 = c2 = CH_fb_out_prev;
	//	else
			/* other algorithms */
///*			*CH->connect1*/c1 = CH_fb_out_prev;

		CH_fb_out_curr = 0;
		if (env < ENV_QUIET)
		{
			if (!CH->fb_shift)
				out = 0;
			CH_fb_out_curr = op_calc(CH->slot0_phase, env, out << CH->fb_shift);
		}
	}
	INT32 c2;
	c2=0;
	env = VOLUME_CALC1; /* M2 */	/* SLOT 3 */
	if (env < ENV_QUIET)
/*		*CH->connect3*/c2 += op_calc(CH->slot1_phase, env, (/*m2*/CH_mem_value<<15));

	INT32 mem;					/* one sample delay memory */
	mem = 0;
	env = VOLUME_CALC2; /* C1 */	/* SLOT 2 */
	if (env < ENV_QUIET)
/*		*CH->connect2*/mem += op_calc(CH->slot2_phase, env, (/*c1*/CH_fb_out_prev<<15));

	env = VOLUME_CALC3; /* C2 */
	if (env < ENV_QUIET){
		/*if((PAN_L|PAN_R)&(CH->ALGO_pan))*/
		{
			mixing_buffer[ii] += (INT32)op_calc(CH->slot3_phase, env, (c2<<15));
		}
	}
	/* M1 */
	CH_mem_value = mem;
}

//goto my_quit;
//my_quit:
#if 1
//static void advance_phase(void)
{
		/* phase generator */
		{
		//	UINT8 PMS_OFF_MODE;
		//	PMS_OFF_MODE=1;
//			if (op->pms)	/* only when phase modulation from LFO is enabled for this channel */
			if (CH->pms)	/* only when phase modulation from LFO is enabled for this channel */
			{
				INT32 mod_ind = ym2151->lfp;		/* -128...+127 (8bits signed) */
				if (CH->pms < 6){	mod_ind >>= (6 - CH->pms);}
				else			{	mod_ind <<= (CH->pms - 5);}

				if (mod_ind)
				{
//					UINT32 kc_channel = op->kc_i + mod_ind;
					UINT32 kc_channel = CH->kc_i + mod_ind;
					CH->slot0_phase += ((ym2151->freq[kc_channel + CH->SLOT[0].dt2] + CH->SLOT[0].dt1) * CH->SLOT[0].mul) >> 1;
					CH->slot1_phase += ((ym2151->freq[kc_channel + CH->SLOT[1].dt2] + CH->SLOT[1].dt1) * CH->SLOT[1].mul) >> 1;
					CH->slot2_phase += ((ym2151->freq[kc_channel + CH->SLOT[2].dt2] + CH->SLOT[2].dt1) * CH->SLOT[2].mul) >> 1;
					CH->slot3_phase += ((ym2151->freq[kc_channel + CH->SLOT[3].dt2] + CH->SLOT[3].dt1) * CH->SLOT[3].mul) >> 1;
				//	PMS_OFF_MODE=0;
					goto ALGO0_PMS_OFF_MODE;
				}
			//	else{}		/* phase modulation from LFO is equal to zero */
			}
		//	else{}			/* phase modulation from LFO is disabled */
		//	if(PMS_OFF_MODE)
			{
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
			}
		ALGO0_PMS_OFF_MODE:
		;
		} //while (i);
}
#else
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
#endif
;
	}
///////////
CH->fb_out_prev=CH_fb_out_prev;
CH->fb_out_curr=CH_fb_out_curr;
CH->mem_value=CH_mem_value;
///////////
}
/////////////////////////////////
static void chan_calc_ALGO1(OPM_CH	*CH,s16 *mixing_buffer, int length)
{
int ii;
	UINT32 env;

//
	UINT32 AM = 0;
	if (CH->ams)
		AM = ym2151->lfa << (CH->ams-1);
///////////
INT32 CH_fb_out_prev=CH->fb_out_prev;
INT32 CH_fb_out_curr=CH->fb_out_curr;
INT32 CH_mem_value=CH->mem_value;
///////////
	for (ii = 0; ii < length; ii++)
	{

//
//ALGO1:
{
//INT32 /*m2,*/c1/*,c2*/;				/* Phase Modulation input for operators 2,3,4 */
 INT32 mem; 				/* one sample delay memory */

		/* M1------+-MEM---M2---C2---OUT */
		/*		C1-+					 */
//		CH->connect1 = &mem;
//		CH->connect2 = &mem;
//		CH->connect3 = &c2;
//		CH->mem_connect = &m2;


//	/*m2 =*/ c1 = /*c2 =*/ /*mem =*/ 0;

///*	*CH->mem_connect*/m2 = CH_mem_value;	/* restore delayed sample (MEM) value to m2 or c2 */

	env = VOLUME_CALC0;
	{
		INT32 out = CH_fb_out_prev + CH_fb_out_curr;
		CH_fb_out_prev = CH_fb_out_curr;

	//	if (!CH->connect1)	/* M1 */
	//		/* algorithm 5 */
	//		mem = c1 = c2 = CH_fb_out_prev;
	//	else
			/* other algorithms */
/*			*CH->connect1*/mem = CH_fb_out_prev;

		CH_fb_out_curr = 0;
		if (env < ENV_QUIET)
		{
			if (!CH->fb_shift)
				out = 0;
			CH_fb_out_curr = op_calc(CH->slot0_phase, env, out << CH->fb_shift);
		}
	}
	INT32 c2;
	c2=0;
	env = VOLUME_CALC1; /* M2 */	/* SLOT 3 */
	if (env < ENV_QUIET)
/*		*CH->connect3*/c2 += op_calc(CH->slot1_phase, env, (/*m2*/CH_mem_value<<15));

	env = VOLUME_CALC2; /* C1 */	/* SLOT 2 */
	if (env < ENV_QUIET)
/*		*CH->connect2*/mem += op_calc(CH->slot2_phase, env, (/*c1*/0<<15));

	env = VOLUME_CALC3; /* C2 */
	if (env < ENV_QUIET)
	{
		/*if((PAN_L|PAN_R)&(CH->ALGO_pan))*/
		{
			mixing_buffer[ii] += (INT32)op_calc(CH->slot3_phase, env, (c2<<15));
		}
	}
	/* M1 */
	CH_mem_value = mem;
}

//goto my_quit;
//my_quit:
#if 1
//static void advance_phase(void)
{
		/* phase generator */
		{
		//	UINT8 PMS_OFF_MODE;
		//	PMS_OFF_MODE=1;
//			if (op->pms)	/* only when phase modulation from LFO is enabled for this channel */
			if (CH->pms)	/* only when phase modulation from LFO is enabled for this channel */
			{
				INT32 mod_ind = ym2151->lfp;		/* -128...+127 (8bits signed) */
				if (CH->pms < 6){	mod_ind >>= (6 - CH->pms);}
				else			{	mod_ind <<= (CH->pms - 5);}

				if (mod_ind)
				{
//					UINT32 kc_channel = op->kc_i + mod_ind;
					UINT32 kc_channel = CH->kc_i + mod_ind;
					CH->slot0_phase += ((ym2151->freq[kc_channel + CH->SLOT[0].dt2] + CH->SLOT[0].dt1) * CH->SLOT[0].mul) >> 1;
					CH->slot1_phase += ((ym2151->freq[kc_channel + CH->SLOT[1].dt2] + CH->SLOT[1].dt1) * CH->SLOT[1].mul) >> 1;
					CH->slot2_phase += ((ym2151->freq[kc_channel + CH->SLOT[2].dt2] + CH->SLOT[2].dt1) * CH->SLOT[2].mul) >> 1;
					CH->slot3_phase += ((ym2151->freq[kc_channel + CH->SLOT[3].dt2] + CH->SLOT[3].dt1) * CH->SLOT[3].mul) >> 1;
				//	PMS_OFF_MODE=0;
					goto ALGO1_PMS_OFF_MODE;
				}
			//	else{}		/* phase modulation from LFO is equal to zero */
			}
		//	else{}			/* phase modulation from LFO is disabled */
		//	if(PMS_OFF_MODE)
			{
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
			}
		ALGO1_PMS_OFF_MODE:
		;
		} //while (i);
}
#else
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
#endif
;
	}
///////////
CH->fb_out_prev=CH_fb_out_prev;
CH->fb_out_curr=CH_fb_out_curr;
CH->mem_value=CH_mem_value;
///////////
}
/////////////////////////////////
static void chan_calc_ALGO2(OPM_CH	*CH,s16 *mixing_buffer, int length)
{
int ii;
	UINT32 env;

//
	UINT32 AM = 0;
	if (CH->ams)
		AM = ym2151->lfa << (CH->ams-1);
///////////
INT32 CH_fb_out_prev=CH->fb_out_prev;
INT32 CH_fb_out_curr=CH->fb_out_curr;
INT32 CH_mem_value=CH->mem_value;
///////////
	for (ii = 0; ii < length; ii++)
	{

//
//ALGO2:
{
//INT32 /*m2,*/c1/*,c2*/;				/* Phase Modulation input for operators 2,3,4 */
 INT32 mem; 				/* one sample delay memory */

		/* M1-----------------+-C2---OUT */
		/*		C1---MEM---M2-+ 		 */
//		CH->connect1 = &c2;
//		CH->connect2 = &mem;
//		CH->connect3 = &c2;
//		CH->mem_connect = &m2;

	/*m2 =*/ /*c1 =*/ /*c2 =*/ mem = 0;

///*	*CH->mem_connect*/m2 = CH_mem_value;	/* restore delayed sample (MEM) value to m2 or c2 */
	INT32 c2;
	env = VOLUME_CALC0;
	{
		INT32 out = CH_fb_out_prev + CH_fb_out_curr;
		CH_fb_out_prev = CH_fb_out_curr;

	//	if (!CH->connect1)	/* M1 */
	//		/* algorithm 5 */
	//		mem = c1 = c2 = CH_fb_out_prev;
	//	else
			/* other algorithms */

/*			*CH->connect1*/c2 = CH_fb_out_prev;

		CH_fb_out_curr = 0;
		if (env < ENV_QUIET)
		{
			if (!CH->fb_shift)
				out = 0;
			CH_fb_out_curr = op_calc(CH->slot0_phase, env, out << CH->fb_shift);
		}
	}

	env = VOLUME_CALC1; /* M2 */	/* SLOT 3 */
	if (env < ENV_QUIET)
/*		*CH->connect3*/c2 += op_calc(CH->slot1_phase, env, (/*m2*/CH_mem_value<<15));

	env = VOLUME_CALC2; /* C1 */	/* SLOT 2 */
	if (env < ENV_QUIET)
/*		*CH->connect2*/mem += op_calc(CH->slot2_phase, env, (/*c1*/0<<15));

	env = VOLUME_CALC3; /* C2 */
	if (env < ENV_QUIET)
	{
		/*if((PAN_L|PAN_R)&(CH->ALGO_pan))*/
		{
			mixing_buffer[ii] += (INT32)op_calc(CH->slot3_phase, env, (c2<<15));
		}
	}
	/* M1 */
	CH_mem_value = mem;
}
//goto my_quit;
//my_quit:
#if 1
//static void advance_phase(void)
{
		/* phase generator */
		{
		//	UINT8 PMS_OFF_MODE;
		//	PMS_OFF_MODE=1;
//			if (op->pms)	/* only when phase modulation from LFO is enabled for this channel */
			if (CH->pms)	/* only when phase modulation from LFO is enabled for this channel */
			{
				INT32 mod_ind = ym2151->lfp;		/* -128...+127 (8bits signed) */
				if (CH->pms < 6){	mod_ind >>= (6 - CH->pms);}
				else			{	mod_ind <<= (CH->pms - 5);}

				if (mod_ind)
				{
//					UINT32 kc_channel = op->kc_i + mod_ind;
					UINT32 kc_channel = CH->kc_i + mod_ind;
					CH->slot0_phase += ((ym2151->freq[kc_channel + CH->SLOT[0].dt2] + CH->SLOT[0].dt1) * CH->SLOT[0].mul) >> 1;
					CH->slot1_phase += ((ym2151->freq[kc_channel + CH->SLOT[1].dt2] + CH->SLOT[1].dt1) * CH->SLOT[1].mul) >> 1;
					CH->slot2_phase += ((ym2151->freq[kc_channel + CH->SLOT[2].dt2] + CH->SLOT[2].dt1) * CH->SLOT[2].mul) >> 1;
					CH->slot3_phase += ((ym2151->freq[kc_channel + CH->SLOT[3].dt2] + CH->SLOT[3].dt1) * CH->SLOT[3].mul) >> 1;
				//	PMS_OFF_MODE=0;
					goto ALGO2_PMS_OFF_MODE;
				}
			//	else{}		/* phase modulation from LFO is equal to zero */
			}
		//	else{}			/* phase modulation from LFO is disabled */
		//	if(PMS_OFF_MODE)
			{
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
			}
		ALGO2_PMS_OFF_MODE:
		;
		} //while (i);
}
#else
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
#endif
;
	}
///////////
CH->fb_out_prev=CH_fb_out_prev;
CH->fb_out_curr=CH_fb_out_curr;
CH->mem_value=CH_mem_value;
///////////
}
/////////////////////////////////
static void chan_calc_ALGO3(OPM_CH	*CH,s16 *mixing_buffer, int length)
{
int ii;
	UINT32 env;

//
	UINT32 AM = 0;
	if (CH->ams)
		AM = ym2151->lfa << (CH->ams-1);
///////////
INT32 CH_fb_out_prev=CH->fb_out_prev;
INT32 CH_fb_out_curr=CH->fb_out_curr;
INT32 CH_mem_value=CH->mem_value;
///////////
	for (ii = 0; ii < length; ii++)
	{
//ALGO3:
{
//INT32 m2/*,c1*//*,c2*/;				/* Phase Modulation input for operators 2,3,4 */
 INT32 mem; 				/* one sample delay memory */

		/* M1---C1---MEM------+-C2---OUT */
		/*				   M2-+ 		 */
//		CH->connect1 = &c1;
//		CH->connect2 = &mem;
//		CH->connect3 = &c2;
//		CH->mem_connect = &c2;

	/*m2 =*/ /*c1 =*/ /*c2 =*/ mem = 0;
INT32 c1;
	env = VOLUME_CALC0;
	{
		INT32 out = CH_fb_out_prev + CH_fb_out_curr;
		CH_fb_out_prev = CH_fb_out_curr;

	//	if (!CH->connect1)	/* M1 */
	//		/* algorithm 5 */
	//		mem = c1 = c2 = CH_fb_out_prev;
	//	else
			/* other algorithms */
/*			*CH->connect1*/c1 = CH_fb_out_prev;

		CH_fb_out_curr = 0;
		if (env < ENV_QUIET)
		{
			if (!CH->fb_shift)
				out = 0;
			CH_fb_out_curr = op_calc(CH->slot0_phase, env, out << CH->fb_shift);
		}
	}

	INT32 c2;
/*	*CH->mem_connect*/c2 = CH_mem_value;	/* restore delayed sample (MEM) value to m2 or c2 */

	env = VOLUME_CALC1; /* M2 */	/* SLOT 3 */
	if (env < ENV_QUIET)
/*		*CH->connect3*/c2 += op_calc(CH->slot1_phase, env, (/*m2*/0<<15));

	env = VOLUME_CALC2; /* C1 */	/* SLOT 2 */
	if (env < ENV_QUIET)
/*		*CH->connect2*/mem += op_calc(CH->slot2_phase, env, (c1<<15));

	env = VOLUME_CALC3; /* C2 */
	if (env < ENV_QUIET)
	{
		/*if((PAN_L|PAN_R)&(CH->ALGO_pan))*/
		{
			mixing_buffer[ii] += (INT32)op_calc(CH->slot3_phase, env, (c2<<15));
		}
	}
	/* M1 */
	CH_mem_value = mem;

}
//goto my_quit;
//my_quit:
#if 1
//static void advance_phase(void)
{
		/* phase generator */
		{
		//	UINT8 PMS_OFF_MODE;
		//	PMS_OFF_MODE=1;
//			if (op->pms)	/* only when phase modulation from LFO is enabled for this channel */
			if (CH->pms)	/* only when phase modulation from LFO is enabled for this channel */
			{
				INT32 mod_ind = ym2151->lfp;		/* -128...+127 (8bits signed) */
				if (CH->pms < 6){	mod_ind >>= (6 - CH->pms);}
				else			{	mod_ind <<= (CH->pms - 5);}

				if (mod_ind)
				{
//					UINT32 kc_channel = op->kc_i + mod_ind;
					UINT32 kc_channel = CH->kc_i + mod_ind;
					CH->slot0_phase += ((ym2151->freq[kc_channel + CH->SLOT[0].dt2] + CH->SLOT[0].dt1) * CH->SLOT[0].mul) >> 1;
					CH->slot1_phase += ((ym2151->freq[kc_channel + CH->SLOT[1].dt2] + CH->SLOT[1].dt1) * CH->SLOT[1].mul) >> 1;
					CH->slot2_phase += ((ym2151->freq[kc_channel + CH->SLOT[2].dt2] + CH->SLOT[2].dt1) * CH->SLOT[2].mul) >> 1;
					CH->slot3_phase += ((ym2151->freq[kc_channel + CH->SLOT[3].dt2] + CH->SLOT[3].dt1) * CH->SLOT[3].mul) >> 1;
				//	PMS_OFF_MODE=0;
					goto ALGO3_PMS_OFF_MODE;
				}
			//	else{}		/* phase modulation from LFO is equal to zero */
			}
		//	else{}			/* phase modulation from LFO is disabled */
		//	if(PMS_OFF_MODE)
			{
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
			}
		ALGO3_PMS_OFF_MODE:
		;
		} //while (i);
}
#else
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
#endif
;
	}
///////////
CH->fb_out_prev=CH_fb_out_prev;
CH->fb_out_curr=CH_fb_out_curr;
CH->mem_value=CH_mem_value;
///////////
}
/////////////////////////////////
static void chan_calc_ALGO4(OPM_CH	*CH,s16 *mixing_buffer, int length)
{
int ii;
	UINT32 env;
//
	UINT32 AM = 0;
	if (CH->ams)
		AM = ym2151->lfa << (CH->ams-1);
///////////
INT32 CH_fb_out_prev=CH->fb_out_prev;
INT32 CH_fb_out_curr=CH->fb_out_curr;
//INT32 CH_mem_value=CH->mem_value;
///////////
	for (ii = 0; ii < length; ii++)
	{
//
//ALGO4:
{
//INT32 m2/*,c1*//*,c2*/;				/* Phase Modulation input for operators 2,3,4 */
//INT32 mem;					/* one sample delay memory */

		/* M1---C1-+-OUT */
		/* M2---C2-+	 */
		/* MEM: not used */
//		CH->connect1 = &c1;
//		CH->connect2 = &chanout_chan;
//		CH->connect3 = &c2;
//		CH->mem_connect = &mem; /* store it anywhere where it will not be used */

//	m2 = /*c1 =*/ /*c2 =*/ /*mem =*/ 0;

///*	*CH->mem_connect*/mem = CH_mem_value;	/* restore delayed sample (MEM) value to m2 or c2 */
INT32 c1;
	env = VOLUME_CALC0;
	{
		INT32 out = CH_fb_out_prev + CH_fb_out_curr;
		CH_fb_out_prev = CH_fb_out_curr;

	//	if (!CH->connect1)	/* M1 */
	//		/* algorithm 5 */
	//		mem = c1 = c2 = CH_fb_out_prev;
	//	else
			/* other algorithms */
/*			*CH->connect1*/c1 = CH_fb_out_prev;

		CH_fb_out_curr = 0;
		if (env < ENV_QUIET)
		{
			if (!CH->fb_shift)
				out = 0;
			CH_fb_out_curr = op_calc(CH->slot0_phase, env, out << CH->fb_shift);
		}
	}

	INT32 c2;
	c2=0;
	env = VOLUME_CALC1; /* M2 */	/* SLOT 3 */
	if (env < ENV_QUIET)
/*		*CH->connect3*/c2 += op_calc(CH->slot1_phase, env, (/*m2*/0<<15));

	env = VOLUME_CALC2; /* C1 */	/* SLOT 2 */
	if (env < ENV_QUIET)
	{
		/*if((PAN_L|PAN_R)&(CH->ALGO_pan))*/
		{
/*		*CH->connect2*/mixing_buffer[ii] += (INT32)op_calc(CH->slot2_phase, env, (c1<<15));
		}
	}

	env = VOLUME_CALC3; /* C2 */
	if (env < ENV_QUIET)
	{
		/*if((PAN_L|PAN_R)&(CH->ALGO_pan))*/
		{
			mixing_buffer[ii] += (INT32)op_calc(CH->slot3_phase, env, (c2<<15));
		}
	}
	/* M1 */
//	CH_mem_value = mem;

}
//goto my_quit;
//my_quit:
#if 1
//static void advance_phase(void)
{
		/* phase generator */
		{
		//	UINT8 PMS_OFF_MODE;
		//	PMS_OFF_MODE=1;
//			if (op->pms)	/* only when phase modulation from LFO is enabled for this channel */
			if (CH->pms)	/* only when phase modulation from LFO is enabled for this channel */
			{
				INT32 mod_ind = ym2151->lfp;		/* -128...+127 (8bits signed) */
				if (CH->pms < 6){	mod_ind >>= (6 - CH->pms);}
				else			{	mod_ind <<= (CH->pms - 5);}

				if (mod_ind)
				{
//					UINT32 kc_channel = op->kc_i + mod_ind;
					UINT32 kc_channel = CH->kc_i + mod_ind;
					CH->slot0_phase += ((ym2151->freq[kc_channel + CH->SLOT[0].dt2] + CH->SLOT[0].dt1) * CH->SLOT[0].mul) >> 1;
					CH->slot1_phase += ((ym2151->freq[kc_channel + CH->SLOT[1].dt2] + CH->SLOT[1].dt1) * CH->SLOT[1].mul) >> 1;
					CH->slot2_phase += ((ym2151->freq[kc_channel + CH->SLOT[2].dt2] + CH->SLOT[2].dt1) * CH->SLOT[2].mul) >> 1;
					CH->slot3_phase += ((ym2151->freq[kc_channel + CH->SLOT[3].dt2] + CH->SLOT[3].dt1) * CH->SLOT[3].mul) >> 1;
				//	PMS_OFF_MODE=0;
					goto ALGO4_PMS_OFF_MODE;
				}
			//	else{}		/* phase modulation from LFO is equal to zero */
			}
		//	else{}			/* phase modulation from LFO is disabled */
		//	if(PMS_OFF_MODE)
			{
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
			}
		ALGO4_PMS_OFF_MODE:
		;
		} //while (i);
}
#else
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
#endif
;
	}
///////////
CH->fb_out_prev=CH_fb_out_prev;
CH->fb_out_curr=CH_fb_out_curr;
//CH->mem_value=CH_mem_value;
///////////
}
/////////////////////////////////
static void chan_calc_ALGO5(OPM_CH	*CH,s16 *mixing_buffer, int length)
{
int ii;
	UINT32 env;

//
	UINT32 AM = 0;
	if (CH->ams)
		AM = ym2151->lfa << (CH->ams-1);
///////////
INT32 CH_fb_out_prev=CH->fb_out_prev;
INT32 CH_fb_out_curr=CH->fb_out_curr;
INT32 CH_mem_value=CH->mem_value;
///////////
	for (ii = 0; ii < length; ii++)
	{
//ALGO5:
{
//INT32 /*m2,*/c1,c2;				/* Phase Modulation input for operators 2,3,4 */

		/*	  +----C1----+	   */
		/* M1-+-MEM---M2-+-OUT */
		/*	  +----C2----+	   */
//		CH->connect1 = 0;	/* special mark */
//		CH->connect2 = &chanout_chan;
//		CH->connect3 = &chanout_chan;
//		CH->mem_connect = &m2;

	INT32 chanout_chan;
	chanout_chan = 0;
//	/*m2 =*/ c1 = c2 = mem = 0;

///*	*CH->mem_connect*/m2 = CH_mem_value;	/* restore delayed sample (MEM) value to m2 or c2 */
//INT32 mem;					/* one sample delay memory */
INT32 my_prev;
	env = VOLUME_CALC0;
	{
		INT32 out;
		out = CH_fb_out_prev + CH_fb_out_curr;

my_prev=		CH_fb_out_prev = CH_fb_out_curr;

	//	if (!CH->connect1)	/* M1 */
			/* algorithm 5 */
//			mem = /*c1 = c2 =*/ CH_fb_out_prev;
	//	else
	//		/* other algorithms */
	//		*CH->connect1 = CH_fb_out_prev;

		CH_fb_out_curr = 0;
		if (env < ENV_QUIET)
		{
			if (!CH->fb_shift)
				out = 0;
			CH_fb_out_curr = op_calc(CH->slot0_phase, env, out << CH->fb_shift);
		}
	}

	env = VOLUME_CALC1; /* M2 */	/* SLOT 3 */
	if (env < ENV_QUIET)
/*		*CH->connect3*/chanout_chan += op_calc(CH->slot1_phase, env, (/*m2*/CH_mem_value<<15));

	env = VOLUME_CALC2; /* C1 */	/* SLOT 2 */
	if (env < ENV_QUIET)
/*		*CH->connect2*/chanout_chan += op_calc(CH->slot2_phase, env, (/*c1*//*mem*/my_prev<<15));

	env = VOLUME_CALC3; /* C2 */
	if (env < ENV_QUIET){
		(chanout_chan) += op_calc(CH->slot3_phase, env, (/*c2*//*mem*/my_prev<<15));
	}
	/* M1 */
	CH_mem_value = /*mem*/my_prev;
	/*if((PAN_L|PAN_R)&(CH->ALGO_pan))*/ {	mixing_buffer[ii] += (chanout_chan);}
}
//goto my_quit;
//my_quit:
#if 1
//static void advance_phase(void)
{
		/* phase generator */
		{
		//	UINT8 PMS_OFF_MODE;
		//	PMS_OFF_MODE=1;
//			if (op->pms)	/* only when phase modulation from LFO is enabled for this channel */
			if (CH->pms)	/* only when phase modulation from LFO is enabled for this channel */
			{
				INT32 mod_ind = ym2151->lfp;		/* -128...+127 (8bits signed) */
				if (CH->pms < 6){	mod_ind >>= (6 - CH->pms);}
				else			{	mod_ind <<= (CH->pms - 5);}

				if (mod_ind)
				{
//					UINT32 kc_channel = op->kc_i + mod_ind;
					UINT32 kc_channel = CH->kc_i + mod_ind;
					CH->slot0_phase += ((ym2151->freq[kc_channel + CH->SLOT[0].dt2] + CH->SLOT[0].dt1) * CH->SLOT[0].mul) >> 1;
					CH->slot1_phase += ((ym2151->freq[kc_channel + CH->SLOT[1].dt2] + CH->SLOT[1].dt1) * CH->SLOT[1].mul) >> 1;
					CH->slot2_phase += ((ym2151->freq[kc_channel + CH->SLOT[2].dt2] + CH->SLOT[2].dt1) * CH->SLOT[2].mul) >> 1;
					CH->slot3_phase += ((ym2151->freq[kc_channel + CH->SLOT[3].dt2] + CH->SLOT[3].dt1) * CH->SLOT[3].mul) >> 1;
				//	PMS_OFF_MODE=0;
					goto ALGO5_PMS_OFF_MODE;
				}
			//	else{}		/* phase modulation from LFO is equal to zero */
			}
		//	else{}			/* phase modulation from LFO is disabled */
		//	if(PMS_OFF_MODE)
			{
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
			}
		ALGO5_PMS_OFF_MODE:
		;
		} //while (i);
}
#else
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
#endif
;
	}
///////////
CH->fb_out_prev=CH_fb_out_prev;
CH->fb_out_curr=CH_fb_out_curr;
CH->mem_value=CH_mem_value;
///////////
}
/////////////////////////////////
static void chan_calc_ALGO6(OPM_CH	*CH,s16 *mixing_buffer, int length)
{
int ii;
	UINT32 env;

//
	UINT32 AM = 0;
	if (CH->ams)
		AM = ym2151->lfa << (CH->ams-1);
///////////
INT32 CH_fb_out_prev=CH->fb_out_prev;
INT32 CH_fb_out_curr=CH->fb_out_curr;
//INT32 CH_mem_value=CH->mem_value;
///////////
	for (ii = 0; ii < length; ii++)
	{
//ALGO6:
{
//INT32 m2/*,c1*//*,c2*/;				/* Phase Modulation input for operators 2,3,4 */
//INT32 mem;					/* one sample delay memory */

		/* M1---C1-+	 */
		/*		M2-+-OUT */
		/*		C2-+	 */
		/* MEM: not used */
//		CH->connect1 = &c1;
//		CH->connect2 = &chanout_chan;
//		CH->connect3 = &chanout_chan;
//		CH->mem_connect = &mem; /* store it anywhere where it will not be used */
	INT32 chanout_chan;
	chanout_chan = 0;

//	m2 = /*c1 =*/ /*c2 =*/ /*mem =*/ 0;

///*	*CH->mem_connect*/mem = CH_mem_value;	/* restore delayed sample (MEM) value to m2 or c2 */
INT32 c1;
	env = VOLUME_CALC0;
	{
		INT32 out = CH_fb_out_prev + CH_fb_out_curr;
		CH_fb_out_prev = CH_fb_out_curr;

	//	if (!CH->connect1)	/* M1 */
	//		/* algorithm 5 */
	//		mem = c1 = c2 = CH_fb_out_prev;
	//	else
			/* other algorithms */
/*			*CH->connect1*/c1 = CH_fb_out_prev;

		CH_fb_out_curr = 0;
		if (env < ENV_QUIET)
		{
			if (!CH->fb_shift)
				out = 0;
			CH_fb_out_curr = op_calc(CH->slot0_phase, env, out << CH->fb_shift);
		}
	}

	env = VOLUME_CALC1; /* M2 */	/* SLOT 3 */
	if (env < ENV_QUIET)
/*		*CH->connect3*/chanout_chan += op_calc(CH->slot1_phase, env, (/*m2*/0<<15));

	env = VOLUME_CALC2; /* C1 */	/* SLOT 2 */
	if (env < ENV_QUIET)
/*		*CH->connect2*/chanout_chan += op_calc(CH->slot2_phase, env, (c1<<15));

	env = VOLUME_CALC3; /* C2 */
	if (env < ENV_QUIET){
		(chanout_chan) += op_calc(CH->slot3_phase, env, (/*c2*/0<<15));
	}
	/* M1 */
//	CH_mem_value = mem;
	/*if((PAN_L|PAN_R)&(CH->ALGO_pan))*/ {	mixing_buffer[ii] += (chanout_chan);}
}
//goto my_quit;
//my_quit:
#if 1
//static void advance_phase(void)
{
		/* phase generator */
		{
		//	UINT8 PMS_OFF_MODE;
		//	PMS_OFF_MODE=1;
//			if (op->pms)	/* only when phase modulation from LFO is enabled for this channel */
			if (CH->pms)	/* only when phase modulation from LFO is enabled for this channel */
			{
				INT32 mod_ind = ym2151->lfp;		/* -128...+127 (8bits signed) */
				if (CH->pms < 6){	mod_ind >>= (6 - CH->pms);}
				else			{	mod_ind <<= (CH->pms - 5);}

				if (mod_ind)
				{
//					UINT32 kc_channel = op->kc_i + mod_ind;
					UINT32 kc_channel = CH->kc_i + mod_ind;
					CH->slot0_phase += ((ym2151->freq[kc_channel + CH->SLOT[0].dt2] + CH->SLOT[0].dt1) * CH->SLOT[0].mul) >> 1;
					CH->slot1_phase += ((ym2151->freq[kc_channel + CH->SLOT[1].dt2] + CH->SLOT[1].dt1) * CH->SLOT[1].mul) >> 1;
					CH->slot2_phase += ((ym2151->freq[kc_channel + CH->SLOT[2].dt2] + CH->SLOT[2].dt1) * CH->SLOT[2].mul) >> 1;
					CH->slot3_phase += ((ym2151->freq[kc_channel + CH->SLOT[3].dt2] + CH->SLOT[3].dt1) * CH->SLOT[3].mul) >> 1;
				//	PMS_OFF_MODE=0;
					goto ALGO6_PMS_OFF_MODE;
				}
			//	else{}		/* phase modulation from LFO is equal to zero */
			}
		//	else{}			/* phase modulation from LFO is disabled */
		//	if(PMS_OFF_MODE)
			{
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
			}
		ALGO6_PMS_OFF_MODE:
		;
		} //while (i);
}
#else
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
#endif
;
	}
///////////
CH->fb_out_prev=CH_fb_out_prev;
CH->fb_out_curr=CH_fb_out_curr;
//CH->mem_value=CH_mem_value;
///////////
}
/////////////////////////////////

static void chan_calc_ALGO7(OPM_CH	*CH,s16 *mixing_buffer, int length)
{
int ii;
	UINT32 env;

//
	UINT32 AM = 0;
	if (CH->ams)
		AM = ym2151->lfa << (CH->ams-1);
///////////
INT32 CH_fb_out_prev=CH->fb_out_prev;
INT32 CH_fb_out_curr=CH->fb_out_curr;
//INT32 CH_mem_value=CH->mem_value;
///////////
	for (ii = 0; ii < length; ii++)
	{
//ALGO7:
{
//INT32 m2,c1,c2;				/* Phase Modulation input for operators 2,3,4 */
//INT32 mem;					/* one sample delay memory */

		/* M1-+ 	*/
		/* C1-+-OUT */
		/* M2-+ 	*/
		/* C2-+ 	*/
		/* MEM: not used*/
//		CH->connect1 = &chanout_chan;
//		CH->connect2 = &chanout_chan;
//		CH->connect3 = &chanout_chan;
//		CH->mem_connect = &mem; /* store it anywhere where it will not be used */

	INT32 chanout_chan;
	chanout_chan = 0;

//	m2 = c1 = c2 = /*mem =*/ 0;

///*	*CH->mem_connect*/mem = CH_mem_value;	/* restore delayed sample (MEM) value to m2 or c2 */

	env = VOLUME_CALC0;
	{
		INT32 out = CH_fb_out_prev + CH_fb_out_curr;
		CH_fb_out_prev = CH_fb_out_curr;

	//	if (!CH->connect1)	/* M1 */
	//		/* algorithm 5 */
	//		mem = c1 = c2 = CH_fb_out_prev;
	//	else
			/* other algorithms */
/*			*CH->connect1*/chanout_chan = CH_fb_out_prev;

		CH_fb_out_curr = 0;
		if (env < ENV_QUIET)
		{
			if (!CH->fb_shift)
				out = 0;
			CH_fb_out_curr = op_calc(CH->slot0_phase, env, out << CH->fb_shift);
		}
	}

	env = VOLUME_CALC1; /* M2 */	/* SLOT 3 */
	if (env < ENV_QUIET)
/*		*CH->connect3*/chanout_chan += op_calc(CH->slot1_phase, env, (/*m2*/0<<15));

	env = VOLUME_CALC2; /* C1 */	/* SLOT 2 */
	if (env < ENV_QUIET)
/*		*CH->connect2*/chanout_chan += op_calc(CH->slot2_phase, env, (/*c1*/0<<15));

	env = VOLUME_CALC3; /* C2 */
	if (env < ENV_QUIET){
		(chanout_chan) += op_calc(CH->slot3_phase, env, (/*c2*/0<<15));
	}
	/* M1 */
//	CH_mem_value = mem;
	/*if((PAN_L|PAN_R)&(CH->ALGO_pan))*/ {	mixing_buffer[ii] += (chanout_chan);}
}
//goto my_quit;
//my_quit:
#if 1
//static void advance_phase(void)
{
		/* phase generator */
		{
		//	UINT8 PMS_OFF_MODE;
		//	PMS_OFF_MODE=1;
//			if (op->pms)	/* only when phase modulation from LFO is enabled for this channel */
			if (CH->pms)	/* only when phase modulation from LFO is enabled for this channel */
			{
				INT32 mod_ind = ym2151->lfp;		/* -128...+127 (8bits signed) */
				if (CH->pms < 6){	mod_ind >>= (6 - CH->pms);}
				else			{	mod_ind <<= (CH->pms - 5);}

				if (mod_ind)
				{
//					UINT32 kc_channel = op->kc_i + mod_ind;
					UINT32 kc_channel = CH->kc_i + mod_ind;
					CH->slot0_phase += ((ym2151->freq[kc_channel + CH->SLOT[0].dt2] + CH->SLOT[0].dt1) * CH->SLOT[0].mul) >> 1;
					CH->slot1_phase += ((ym2151->freq[kc_channel + CH->SLOT[1].dt2] + CH->SLOT[1].dt1) * CH->SLOT[1].mul) >> 1;
					CH->slot2_phase += ((ym2151->freq[kc_channel + CH->SLOT[2].dt2] + CH->SLOT[2].dt1) * CH->SLOT[2].mul) >> 1;
					CH->slot3_phase += ((ym2151->freq[kc_channel + CH->SLOT[3].dt2] + CH->SLOT[3].dt1) * CH->SLOT[3].mul) >> 1;
				//	PMS_OFF_MODE=0;
					goto ALGO7_PMS_OFF_MODE;
				}
			//	else{}		/* phase modulation from LFO is equal to zero */
			}
		//	else{}			/* phase modulation from LFO is disabled */
		//	if(PMS_OFF_MODE)
			{
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
			}
		ALGO7_PMS_OFF_MODE:
		;
		} //while (i);
}
#else
				CH->slot0_phase += CH->SLOT[0].freq;
				CH->slot1_phase += CH->SLOT[1].freq;
				CH->slot2_phase += CH->SLOT[2].freq;
				CH->slot3_phase += CH->SLOT[3].freq;
#endif
;
	}
///////////
CH->fb_out_prev=CH_fb_out_prev;
CH->fb_out_curr=CH_fb_out_curr;
//CH->mem_value=CH_mem_value;
///////////
}
/////////////////////////////////



static /*const*/ void (*chan_calc_at_algo[])(OPM_CH *CH,s16 *mixing_buffer, int length) =
{
chan_calc_ALGO0,
chan_calc_ALGO1,
chan_calc_ALGO2,
chan_calc_ALGO3,
chan_calc_ALGO4,
chan_calc_ALGO5,
chan_calc_ALGO6,
chan_calc_ALGO7,
};


/////////////////////////////////
#if 0
static void chan_7_noise_mix(OPM_CH *CH,s16 *mixing_buffer, int length)
{
int ii;
	UINT32 env;
//	UINT32 AM = 0;
//	if (CH->ams) AM = ym2151->lfa << (CH->ams-1);
	for (ii = 0; ii < length; ii++)
	{
		env = VOLUME_CALC7; /* C2 */
		{
			UINT32 noiseout;
			noiseout = 0;
			if (env < 0x3ff)
				noiseout = (env ^ 0x3ff) << 1;	/* range of the YM2151 noise output is -2044 to 2040 */
			mixing_buffer[ii]  += ((ym2151->noise_rng & 0x10000) ? noiseout: -noiseout); /* bit 16 -> output */
		}
	}
}
#else
static void chan_7_noise_mix(OPM_CH *CH,s16 *mixing_buffer, int length)
{
//	UINT32 AM = 0;
//	if (CH->ams) AM = ym2151->lfa << (CH->ams-1);

	const UINT32 env = VOLUME_CALC7; /* C2 */
	if (env < 0x3ff)
	{
		UINT16 ii;
		const UINT16 noiseout = (env ^ 0x3ff) << 1;	/* range of the YM2151 noise output is -2044 to 2040 */
		if (ym2151->noise_rng & 0x10000)
		{
			for (ii = 0; ii < length; ii++)
			{
				mixing_buffer[ii] += ( noiseout ); /* bit 16 -> output */
			}
		}else
		{
			for (ii = 0; ii < length; ii++)
			{
				mixing_buffer[ii] += ( -noiseout ); /* bit 16 -> output */
			}
		}
	}
//	else{
//		/* not add noise  */
//	}
}
#endif
/////////////////////////////////




#if 0
static void chan7_calc(void)
{
	OPM_CH	*CH= &ym2151->opm_ch[7/*chan*/];
//	FM_OPM *op;
	FM_SLOT *op;
	UINT32 env;
	UINT32 AM = 0;

	INT32 chanout_chan;

	chanout_chan = 0;
	m2 = c1 = c2 = mem = 0;
//	op = &ym2151->oper[7 * 4];		/* M1 */
	op = &CH->SLOT[0];	/* M1 */

	*CH->mem_connect = CH_mem_value;	/* restore delayed sample (MEM) value to m2 or c2 */

//	if (op->ams)
//		AM = ym2151->lfa << (op->ams - 1);
	if (CH->ams)
		AM = ym2151->lfa << (CH->ams - 1);
	env = VOLUME_CALC0;
//	{
//		INT32 out = op->fb_out_prev + op->fb_out_curr;
//		op->fb_out_prev = op->fb_out_curr;
//
//		if (!CH->connect1)
//			/* algorithm 5 */
//			mem = c1 = c2 = op->fb_out_prev;
//		else
//			/* other algorithms */
//			*CH->connect1 = op->fb_out_prev;
//
//		op->fb_out_curr = 0;
//		if (env < ENV_QUIET)
//		{
//			if (!op->fb_shift)
//				out = 0;
//			op->fb_out_curr = op_calc((op)->phase, env, out << op->fb_shift);
//		}
//	}
	{
		INT32 out = CH_fb_out_prev + CH_fb_out_curr;
		CH_fb_out_prev = CH_fb_out_curr;

		if (!CH->connect1)
			/* algorithm 5 */
			mem = c1 = c2 = CH_fb_out_prev;
		else
			/* other algorithms */
			*CH->connect1 = CH_fb_out_prev;

		CH_fb_out_curr = 0;
		if (env < ENV_QUIET)
		{
			if (!CH->fb_shift)
				out = 0;
			CH_fb_out_curr = op_calc(CH->slot0_phase, env, out << CH->fb_shift);
		}
	}

	env = VOLUME_CALC1; /* M2 */	/* SLOT 3 */
	if (env < ENV_QUIET)
		*CH->connect3 += op_calc(CH->slot1_phase, env, (m2<<15));

	env = VOLUME_CALC2; /* C1 */	/* SLOT 2 */
	if (env < ENV_QUIET)
		*CH->connect2 += op_calc(CH->slot2_phase, env, (c1<<15));

	env = VOLUME_CALC3; /* C2 */
	if (ym2151->noise & 0x80)
	{
		UINT32 noiseout;

		noiseout = 0;
		if (env < 0x3ff)
			noiseout = (env ^ 0x3ff) << 1;	/* range of the YM2151 noise output is -2044 to 2040 */
		(chanout_chan) += ((ym2151->noise_rng & 0x10000) ? noiseout: -noiseout); /* bit 16 -> output */
	}
	else
	{
		if (env < ENV_QUIET)
			(chanout_chan) += op_calc(CH->slot3_phase, env, (c2<<15));
	}
	/* M1 */
	CH_mem_value = mem;









//#define PAN_R 0x80
//#define PAN_L 0x40

//	if(PAN_R&(CH->ALGO_pan)) {	outR += (chanout_chan);}
//	if(PAN_L&(CH->ALGO_pan)) {	outL += (chanout_chan);}
	/*if((PAN_L|PAN_R)&(CH->ALGO_pan))*/ {	out_MONO += (chanout_chan);}

}
#endif


static void advance_eg(void)
{
//	FM_OPM *op;
//	UINT32 i;


	while (ym2151->eg_timer >= ym2151->eg_timer_overflow)
	{
		int ccc;
		ym2151->eg_timer -= ym2151->eg_timer_overflow;
		ym2151->eg_cnt++;
		/* envelope generator */
//		op = &ym2151->oper[0];	/* CH 0 M1 */
//		i = 32;
//		do
//		{
		for(ccc=0;ccc<8;ccc++){
			int ddd;
			OPM_CH	*chan_eee= &ym2151->opm_ch[ccc];
			for(ddd=0;ddd<4;ddd++){
				FM_SLOT *op = &chan_eee->SLOT[ddd];
				switch (op->state)
				{
				case EG_ATT:	/* attack phase */
					if (!(ym2151->eg_cnt & ((1 << op->eg_sh_ar) - 1)))
					{
						op->volume += (~op->volume * (eg_inc[op->eg_sel_ar + ((ym2151->eg_cnt >> op->eg_sh_ar) & 7)])) >> 4;
						if (op->volume <= MIN_ATT_INDEX)
						{
							op->volume = MIN_ATT_INDEX;
							op->state = EG_DEC;
						}
					}
					break;
				case EG_DEC:	/* decay phase */
					if (!(ym2151->eg_cnt & ((1 << op->eg_sh_d1r) - 1)))
					{
						op->volume += eg_inc[op->eg_sel_d1r + ((ym2151->eg_cnt >> op->eg_sh_d1r) & 7)];
						if (op->volume >= op->d1l)
							op->state = EG_SUS;
					}
					break;
				case EG_SUS:	/* sustain phase */
					if (!(ym2151->eg_cnt & ((1 << op->eg_sh_d2r) - 1)))
					{
						op->volume += eg_inc[op->eg_sel_d2r + ((ym2151->eg_cnt >> op->eg_sh_d2r) & 7)];
						if (op->volume >= MAX_ATT_INDEX)
						{
							op->volume = MAX_ATT_INDEX;
							op->state = EG_OFF;
						}
					}
					break;
				case EG_REL:	/* release phase */
					if (!(ym2151->eg_cnt & ((1 << op->eg_sh_rr) - 1)))
					{
						op->volume += eg_inc[op->eg_sel_rr + ((ym2151->eg_cnt >> op->eg_sh_rr) & 7)];
						if (op->volume >= MAX_ATT_INDEX)
						{
							op->volume = MAX_ATT_INDEX;
							op->state = EG_OFF;
						}
					}
					break;
				}
			}
//			op++;
//			i--;
		}// while (i);
	}
}




static void advance_lfo(void)
{
//	FM_OPM *op;
	UINT32 i;
	int a,p;

	/* LFO */
//	if (ym2151->test & 2)
//		ym2151->lfo_phase = 0;
//	else
	{

		if (ym2151->lfo_timer >= ym2151->lfo_overflow)
		{
			ym2151->lfo_timer	-= ym2151->lfo_overflow;
			ym2151->lfo_counter += ym2151->lfo_counter_add;
			ym2151->lfo_phase	+= ym2151->lfo_counter >> 4;
			ym2151->lfo_phase	&= 255;
			ym2151->lfo_counter &= 15;
		}
	}

	i = ym2151->lfo_phase;
	/* calculate LFO AM and PM waveform value (all verified on real chip, except for noise algorithm which is impossible to analyse)*/
	switch (ym2151->lfo_wsel)
	{
	case 0:
		/* saw */
		/* AM: 255 down to 0 */
		/* PM: 0 to 127, -127 to 0 (at PMD=127: LFP = 0 to 126, -126 to 0) */
		a = 255 - i;
		p = (i < 128) ? i : i - 255;
		break;

	case 1:
		/* square */
		/* AM: 255, 0 */
		/* PM: 128,-128 (LFP = exactly +PMD, -PMD) */
		if (i < 128){
			a = 255;
			p = 128;
		}else{
			a = 0;
			p = -128;
		}
		break;

	case 2:
		/* triangle */
		/* AM: 255 down to 1 step -2; 0 up to 254 step +2 */
		/* PM: 0 to 126 step +2, 127 to 1 step -2, 0 to -126 step -2, -127 to -1 step +2*/
		a = (i < 128) ? 255 - (i << 1) : (i << 1) - 256;

		if (i < 64) 				/* i = 0..63 */
			p = i << 1; 			/* 0 to 126 step +2 */
		else if (i < 128)			/* i = 64..127 */
			p = 255 - (i << 1); 	/* 127 to 1 step -2 */
		else if (i < 192)			/* i = 128..191 */
			p = 256 - (i << 1); 	/* 0 to -126 step -2*/
		else						/* i = 192..255 */
			p = (i << 1) - 511; 	/*-127 to -1 step +2*/
		break;

	case 3:
	default:	/*keep the compiler happy*/
		/* random */
		/* the real algorithm is unknown !!!
			We just use a snapshot of data from real chip */

		/* AM: range 0 to 255	 */
		/* PM: range -128 to 127 */

		a = lfo_noise_waveform[i];
		p = a - 128;
		break;
	}
	ym2151->lfa = a * ym2151->amd >> 7;
	ym2151->lfp = p * ym2151->pmd >> 7;

	/*	The Noise Generator of the YM2151 is 17-bit shift register.
	*	Input to the bit16 is negated (bit0 XOR bit3) (EXNOR).
	*	Output of the register is negated (bit0 XOR bit3).
	*	Simply use bit16 as the noise output.
	*/
	ym2151->noise_p += ym2151->noise_f;
	i = (ym2151->noise_p >> 16);	/* number of events (shifts of the shift register) */
	ym2151->noise_p &= 0xffff;
	while (i)
	{
		UINT32 j;
		j = ((ym2151->noise_rng ^ (ym2151->noise_rng >> 3)) & 1) ^ 1;
		ym2151->noise_rng = (j << 16) | (ym2151->noise_rng >> 1);
		i--;
	}
}

/********  ***********/


void YM2151Reset(void)
{
	int i;

//	/* initialize hardware registers */
//	for (i = 0; i < 32; i++)
//	{
//		memset(&ym2151->oper[i], 0, sizeof(FM_OPM));
//		ym2151->oper[i].volume = MAX_ATT_INDEX;
//		ym2151->oper[i].kc_i = 768; /* min kc_i value */
//	}
	/* initialize hardware registers */
	for (i=0; i<8; i++){
	int j;
	OPM_CH	*chan_iii= &ym2151->opm_ch[i];
		for (j=0; j<4; j++)
		{
		FM_SLOT *op_iii = &chan_iii->SLOT[j];
			memset(op_iii, 0, sizeof(FM_SLOT));
			op_iii->volume = MAX_ATT_INDEX;
		}
		chan_iii->kc_i = 768; /* min kc_i value */
	}

	ym2151->eg_timer = 0;
	ym2151->eg_cnt	 = 0;

	ym2151->lfo_timer  = 0;
	ym2151->lfo_counter= 0;
	ym2151->lfo_phase  = 0;
	ym2151->lfo_wsel   = 0;
	ym2151->pmd = 0;
	ym2151->amd = 0;
	ym2151->lfa = 0;
	ym2151->lfp = 0;

	ym2151->test= 0;

	ym2151->irq_enable = 0;
	timer_enable(ym2151->timer_A, 0);
	timer_enable(ym2151->timer_B, 0);
	ym2151->timer_A_index = 0;
	ym2151->timer_B_index = 0;
	ym2151->timer_A_index_old = 0;
	ym2151->timer_B_index_old = 0;

	ym2151->noise	  = 0;
	ym2151->noise_rng = 0;
	ym2151->noise_p   = 0;
	ym2151->noise_f   = ym2151->noise_tab[0];

	ym2151->csm_req = 0;
	ym2151->status	  = 0;

	YM2151WriteReg(0x1b, 0);	/* only because of CT1, CT2 output pins */
	YM2151WriteReg(0x18, 0);	/* set LFO frequency */
	for (i = 0x20; i < 0x100; i++)	/* set the operators */
	{
		YM2151WriteReg(i, 0);
	}
}


static void YM2151Init(int clock, int rate, FM_IRQHANDLER_2151 IRQHandler)
{
	memset(ym2151, 0, sizeof(ym2151_t));

	ym2151->clock	   = clock;
	ym2151->sampfreq   = rate;
	ym2151->irqhandler = IRQHandler;

	init_tables();
	init_chip_tables();

//	ym2151->lfo_timer_add		 =	(1<<LFO_SH) * (clock/64.0) / ym2151->sampfreq;
	ym2151->lfo_timer_add_length = ((1<<LFO_SH) * (clock/64.0)*(sr_length) / rate);

//	ym2151->eg_timer_add		=  (1<<EG_SH)	* (clock/64.0) / ym2151->sampfreq;
	ym2151->eg_timer_add_length = ((1<<EG_SH)	* (clock/64.0)*(sr_length) / rate);
	ym2151->eg_timer_overflow = 3 * (1<<EG_SH);

	ym2151->timer_A = timer_alloc_ptr(timer_callback_2151_a);
	ym2151->timer_B = timer_alloc_ptr(timer_callback_2151_b);

	YM2151Reset();
}


/* -------chip inter face------- */


/*------------------------------------------------------
	Reset YM2151 emulation
 -----------------------------------------------------*/

static int lastreg;

READ8_HANDLER( YM2151_status_port_0_r )
{
	return YM2151ReadStatus();
}

WRITE8_HANDLER( YM2151_register_port_0_w )
{
	lastreg = data;
}

WRITE8_HANDLER( YM2151_data_port_0_w )
{
	YM2151WriteReg(lastreg, data);
}







