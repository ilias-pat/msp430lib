#ifndef PIN_MAP_H_
#define PIN_MAP_H_

/* MSP430 GPIO pin definitions */
#define P1_0	0x0100
#define P1_1	0x0101
#define P1_2	0x0102
#define P1_3	0x0103
#define P1_4	0x0104
#define P1_5	0x0105
#define P1_6	0x0106
#define P1_7	0x0107
#define	P2_0	0x0200
#define	P2_1	0x0201
#define	P2_2	0x0202
#define	P2_3	0x0203
#define	P2_4	0x0204
#define	P2_5	0x0205
#define	P2_6	0x0206
#define	P2_7	0x0207
#define P3_0	0x0300
#define P3_1	0x0301
#define P3_2	0x0302
#define P3_3	0x0303
#define P3_4	0x0304
#define P3_5	0x0305
#define P3_6	0x0306
#define P3_7	0x0307
#define P4_0	0x0400
#define P4_1	0x0401
#define P4_2	0x0402
#define P4_3	0x0403
#define P4_4	0x0404
#define P4_5	0x0405
#define P4_6	0x0406
#define P4_7	0x0407
#define P5_0	0x0500
#define P5_1	0x0501
#define P5_2	0x0502
#define P5_3	0x0503
#define P5_3	0x0503
#define P5_4	0x0504
#define P5_5	0x0505
#define P5_6	0x0506
#define P5_7	0x0507
#define P6_0	0x0600
#define P6_1	0x0601
#define P6_2	0x0602
#define P6_3	0x0603
#define P6_4	0x0604
#define P6_5	0x0605
#define P6_6	0x0606
#define P6_7	0x0607

/** Maps pin to port number. */
#define mapPinToPort( pin )		( ( unsigned char )( pin >> 8 ) )

/** Maps pin to bit number. */
#define mapPinToBit( pin )		( ( unsigned char )( pin ) )

#endif

