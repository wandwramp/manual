#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define FLIP(a) (a = ( a=='0' ? '1' : '0'))

#define NUM_BLOCKS 204
#define PARITY 124

void Rx(char *data);
void Tx(char *data);

static struct  passwd   *pw_ent;
static time_t  sec;
static int rx_connected = 0;
static int tx_connected = 0;
static int fifo;
static int sample_no = -1;
static int matches_parity = 0;
static int no_check  = 0;
static char gen_ans[200];
static struct sample_t
   {
   int  err;
   char data[100];
   }sample[] = {
     /* WARNING  If you change the data or error types you must change the*/
     /* parity array to match.                                            */


/*  0       1       2       3       4       5       6       7       8*/
/*Set 1 No error*/
{0, "1100010111101000101110101011110001110111010101111010111010111010"}, /* 0*/
{0, "1010011010100101011110000101010101000010111000101010101000001010"},
{0, "0101100101011010100001111010101010111101000111010101010111110101"},
{0, "1010101010101010101010101010101010101010101010101010101010101010"},
{0, "0101010101010101010101010101010101010101010101010101010101010101"},
{0, "0011001100110011001100110011001100110011001100110011001100110011"},
{0, "1100110011001100110011001100110011001100110011001100110011001100"},
{0, "0000000011111111000000001111111100000000111111110000000011111111"},
{0, "1111111100000000111111110000000011111111000000001111111100000000"},
{0, "1111111111111111111111100000000000000001111111111010101011111000"},
{0, "0000000000000000000000000000000000000000000000000000000000000000"},
{0, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 2 For 1 bit errors*/
{2, "1010011010100101011110000101010101000010111000101010101000001010"}, /*12*/
{2, "1100010111101000101110101011110001110111010101111010111010111010"},
{2, "0101100101011010100001111010101010111101000111010101010111110101"},
{2, "1010101010101010101010101010101010101010101010101010101010101010"},
{2, "0101010101010101010101010101010101010101010101010101010101010101"},
{2, "0011001100110011001100110011001100110011001100110011001100110011"},
{2, "1100110011001100110011001100110011001100110011001100110011001100"},
{2, "0000000011111111000000001111111100000000111111110000000011111111"},
{2, "1111111100000000111111110000000011111111000000001111111100000000"},
{2, "1111111111111111111111100000000000000001111111111010101011111000"},
{2, "0000000000000000000000000000000000000000000000000000000000000000"},
{2, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 3 For two adjacent bit errors*/
{3, "1010011010100101011110000101010101000010111000101010101000001010"}, /*24*/
{3, "1100010111101000101110101011110001110111010101111010111010111010"},
{3, "0101100101011010100001111010101010111101000111010101010111110101"},
{3, "1010101010101010101010101010101010101010101010101010101010101010"},
{3, "0101010101010101010101010101010101010101010101010101010101010101"},
{3, "0011001100110011001100110011001100110011001100110011001100110011"},
{3, "1100110011001100110011001100110011001100110011001100110011001100"},
{3, "0000000011111111000000001111111100000000111111110000000011111111"},
{3, "1111111100000000111111110000000011111111000000001111111100000000"},
{3, "1111111111111111111111100000000000000001111111111010101011111000"},
{3, "0000000000000000000000000000000000000000000000000000000000000000"},
{3, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 4 No error*/
{0, "1010011010100101011110000101010101000010111000101010101000001010"}, /*36*/
{0, "1100010111101000101110101011110001110111010101111010111010111010"},
{0, "0101100101011010100001111010101010111101000111010101010111110101"},
{0, "1010101010101010101010101010101010101010101010101010101010101010"},
{0, "0101010101010101010101010101010101010101010101010101010101010101"},
{0, "0011001100110011001100110011001100110011001100110011001100110011"},
{0, "1100110011001100110011001100110011001100110011001100110011001100"},
{0, "0000000011111111000000001111111100000000111111110000000011111111"},
{0, "1111111100000000111111110000000011111111000000001111111100000000"},
{0, "1111111111111111111111100000000000000001111111111010101011111000"},
{0, "0000000000000000000000000000000000000000000000000000000000000000"},
{0, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 5 For errors in the same column sucessive chars*/
{4, "1010011010100101011110000101010101000010111000101010101000001010"}, /*48*/
{4, "1100010111101000101110101011110001110111010101111010111010111010"},
{4, "0101100101011010100001111010101010111101000111010101010111110101"},
{4, "1010101010101010101010101010101010101010101010101010101010101010"},
{4, "0101010101010101010101010101010101010101010101010101010101010101"},
{4, "0011001100110011001100110011001100110011001100110011001100110011"},
{4, "1100110011001100110011001100110011001100110011001100110011001100"},
{4, "0000000011111111000000001111111100000000111111110000000011111111"},
{4, "1111111100000000111111110000000011111111000000001111111100000000"},
{4, "1111111111111111111111100000000000000001111111111010101011111000"},
{4, "0000000000000000000000000000000000000000000000000000000000000000"},
{4, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 6 For errors in the same column sucessive chars 9 bit chars*/
{41, "1010011010100101011110000101010101000010111000101010101000001010"}, /*60*/
{41, "1100010111101000101110101011110001110111010101111010111010111010"},
{41, "0101100101011010100001111010101010111101000111010101010111110101"},
{41, "1010101010101010101010101010101010101010101010101010101010101010"},
{41, "0101010101010101010101010101010101010101010101010101010101010101"},
{41, "0011001100110011001100110011001100110011001100110011001100110011"},
{41, "1100110011001100110011001100110011001100110011001100110011001100"},
{41, "0000000011111111000000001111111100000000111111110000000011111111"},
{41, "1111111100000000111111110000000011111111000000001111111100000000"},
{41, "1111111111111111111111100000000000000001111111111010101011111000"},
{41, "0000000000000000000000000000000000000000000000000000000000000000"},
{41, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 7 For two errors in each of two characters, in the same columns*/
{5, "1010011010100101011110000101010101000010111000101010101000001010"}, /*72*/
{5, "1100010111101000101110101011110001110111010101111010111010111010"},
{5, "0101100101011010100001111010101010111101000111010101010111110101"},
{5, "1010101010101010101010101010101010101010101010101010101010101010"},
{5, "0101010101010101010101010101010101010101010101010101010101010101"},
{5, "0011001100110011001100110011001100110011001100110011001100110011"},
{5, "1100110011001100110011001100110011001100110011001100110011001100"},
{5, "0000000011111111000000001111111100000000111111110000000011111111"},
{5, "1111111100000000111111110000000011111111000000001111111100000000"},
{5, "1111111111111111111111100000000000000001111111111010101011111000"},
{5, "0000000000000000000000000000000000000000000000000000000000000000"},
{5, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 8 For two errors in each of two characters, in the same columns 9 bit*/
{51, "1010011010100101011110000101010101000010111000101010101000001010"}, /*84*/
{51, "1100010111101000101110101011110001110111010101111010111010111010"},
{51, "0101100101011010100001111010101010111101000111010101010111110101"},
{51, "1010101010101010101010101010101010101010101010101010101010101010"},
{51, "0101010101010101010101010101010101010101010101010101010101010101"},
{51, "0011001100110011001100110011001100110011001100110011001100110011"},
{51, "1100110011001100110011001100110011001100110011001100110011001100"},
{51, "0000000011111111000000001111111100000000111111110000000011111111"},
{51, "1111111100000000111111110000000011111111000000001111111100000000"},
{51, "1111111111111111111111100000000000000001111111111010101011111000"},
{51, "0000000000000000000000000000000000000000000000000000000000000000"},
{51, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 9 Add to one character subtract from the next*/
{6, "1010011010100101011110000101010101000010111000101010101000001010"}, /*96*/
{6, "1100010111101000101110101011110001110111010101111010111010111010"},
{6, "0101100101011010100001111010101010111101000111010101010111110101"},
{6, "1010101010101010101010101010101010101010101010101010101010101010"},
{6, "0101010101010101010101010101010101010101010101010101010101010101"},
{6, "0011001100110011001100110011001100110011001100110011001100110011"},
{6, "1100110011001100110011001100110011001100110011001100110011001100"},
{6, "0000000011111111000000001111111100000000111111110000000011111111"},
{6, "1111111100000000111111110000000011111111000000001111111100000000"},
{6, "1111111111111111111111100000000000000001111111111010101011111000"},
{6, "0000000000000000000000000000000000000000000000000000000000000000"},
{6, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 10 Add to one character subtract from the next 9 bit*/
{61, "1010011010100101011110000101010101000010111000101010101000001010"}, /*108*/
{61, "1100010111101000101110101011110001110111010101111010111010111010"},
{61, "0101100101011010100001111010101010111101000111010101010111110101"},
{61, "1010101010101010101010101010101010101010101010101010101010101010"},
{61, "0101010101010101010101010101010101010101010101010101010101010101"},
{61, "0011001100110011001100110011001100110011001100110011001100110011"},
{61, "1100110011001100110011001100110011001100110011001100110011001100"},
{61, "0000000011111111000000001111111100000000111111110000000011111111"},
{61, "1111111100000000111111110000000011111111000000001111111100000000"},
{61, "1111111111111111111111100000000000000001111111111010101011111000"},
{61, "0000000000000000000000000000000000000000000000000000000000000000"},
{61, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 11 All zeros*/
{7, "1010011010100101011110000101010101000010111000101010101000001010"}, /*120*/
{7, "1100010111101000101110101011110001110111010101111010111010111010"},
{7, "0101100101011010100001111010101010111101000111010101010111110101"},
{7, "1010101010101010101010101010101010101010101010101010101010101010"},
{7, "0101010101010101010101010101010101010101010101010101010101010101"},
{7, "0011001100110011001100110011001100110011001100110011001100110011"},
{7, "1100110011001100110011001100110011001100110011001100110011001100"},
{7, "0000000011111111000000001111111100000000111111110000000011111111"},
{7, "1111111100000000111111110000000011111111000000001111111100000000"},
{7, "1111111111111111111111100000000000000001111111111010101011111000"},
{7, "0000000000000000000000000000000000000000000000000000000000000000"},
{7, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 12 All ones*/
{8, "1010011010100101011110000101010101000010111000101010101000001010"}, /*132*/
{8, "1100010111101000101110101011110001110111010101111010111010111010"},
{8, "0101100101011010100001111010101010111101000111010101010111110101"},
{8, "1010101010101010101010101010101010101010101010101010101010101010"},
{8, "0101010101010101010101010101010101010101010101010101010101010101"},
{8, "0011001100110011001100110011001100110011001100110011001100110011"},
{8, "1100110011001100110011001100110011001100110011001100110011001100"},
{8, "0000000011111111000000001111111100000000111111110000000011111111"},
{8, "1111111100000000111111110000000011111111000000001111111100000000"},
{8, "1111111111111111111111100000000000000001111111111010101011111000"},
{8, "0000000000000000000000000000000000000000000000000000000000000000"},
{8, "1111111111111111111111111111111111111111111111111111111111111111"},
#define VERIFY2HERE 143
/*Set 13 Short reply*/
{9, "1010011010100101011110000101010101000010111000101010101000001010"}, /*144*/
{9, "1100010111101000101110101011110001110111010101111010111010111010"},
{9, "0101100101011010100001111010101010111101000111010101010111110101"},
{9, "1010101010101010101010101010101010101010101010101010101010101010"},
{9, "0101010101010101010101010101010101010101010101010101010101010101"},
{9, "0011001100110011001100110011001100110011001100110011001100110011"},
{9, "1100110011001100110011001100110011001100110011001100110011001100"},
{9, "0000000011111111000000001111111100000000111111110000000011111111"},
{9, "1111111100000000111111110000000011111111000000001111111100000000"},
{9, "1111111111111111111111100000000000000001111111111010101011111000"},
{9, "0000000000000000000000000000000000000000000000000000000000000000"},
{9, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 13 Short reply*/
{9, "1010011010100101011110000101010101000010111000101010101000001010"}, /*156*/
{9, "1100010111101000101110101011110001110111010101111010111010111010"},
{9, "0101100101011010100001111010101010111101000111010101010111110101"},
{9, "1010101010101010101010101010101010101010101010101010101010101010"},
{9, "0101010101010101010101010101010101010101010101010101010101010101"},
{9, "0011001100110011001100110011001100110011001100110011001100110011"},
{9, "1100110011001100110011001100110011001100110011001100110011001100"},
{9, "0000000011111111000000001111111100000000111111110000000011111111"},
{9, "1111111100000000111111110000000011111111000000001111111100000000"},
{9, "1111111111111111111111100000000000000001111111111010101011111000"},
{9, "0000000000000000000000000000000000000000000000000000000000000000"},
{9, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 14 Short reply*/
{91, "1010011010100101011110000101010101000010111000101010101000001010"}, /*168*/
{91, "1100010111101000101110101011110001110111010101111010111010111010"},
{91, "0101100101011010100001111010101010111101000111010101010111110101"},
{91, "1010101010101010101010101010101010101010101010101010101010101010"},
{91, "0101010101010101010101010101010101010101010101010101010101010101"},
{91, "0011001100110011001100110011001100110011001100110011001100110011"},
{91, "1100110011001100110011001100110011001100110011001100110011001100"},
{91, "0000000011111111000000001111111100000000111111110000000011111111"},
{91, "1111111100000000111111110000000011111111000000001111111100000000"},
{91, "1111111111111111111111100000000000000001111111111010101011111000"},
{91, "0000000000000000000000000000000000000000000000000000000000000000"},
{91, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 15 No reply*/
{10, "1010011010100101011110000101010101000010111000101010101000001010"}, /*180*/
{10, "1100010111101000101110101011110001110111010101111010111010111010"},
{10, "0101100101011010100001111010101010111101000111010101010111110101"},
{10, "1010101010101010101010101010101010101010101010101010101010101010"},
{10, "0101010101010101010101010101010101010101010101010101010101010101"},
{10, "0011001100110011001100110011001100110011001100110011001100110011"},
{10, "1100110011001100110011001100110011001100110011001100110011001100"},
{10, "0000000011111111000000001111111100000000111111110000000011111111"},
{10, "1111111100000000111111110000000011111111000000001111111100000000"},
{10, "1111111111111111111111100000000000000001111111111010101011111000"},
{10, "0000000000000000000000000000000000000000000000000000000000000000"},
{10, "1111111111111111111111111111111111111111111111111111111111111111"},
/*Set 16 No error to end with*/
{0, "1010011010100101011110000101010101000010111000101010101000001010"}, /*192*/
{0, "1100010111101000101110101011110001110111010101111010111010111010"},
{0, "0101100101011010100001111010101010111101000111010101010111110101"},
{0, "1010101010101010101010101010101010101010101010101010101010101010"},
{0, "0101010101010101010101010101010101010101010101010101010101010101"},
{0, "0011001100110011001100110011001100110011001100110011001100110011"},
{0, "1100110011001100110011001100110011001100110011001100110011001100"},
{0, "0000000011111111000000001111111100000000111111110000000011111111"},
{0, "1111111100000000111111110000000011111111000000001111111100000000"},
{0, "1111111111111111111111100000000000000001111111111010101011111000"},
{0, "0000000000000000000000000000000000000000000000000000000000000000"},
{0, "1111111111111111111111111111111111111111111111111111111111111111"}, /*203*/
{0, "" } };

/*The correct results for parity, 1= correct 0 - not correct*/
int parity[] = {
/*  0*/ 1, 
/*  1*/ 1, /*  2*/ 1, /*  3*/ 1, /*  4*/ 1, /*  5*/ 1,
/*  6*/ 1, /*  7*/ 1, /*  8*/ 1, /*  9*/ 1, /* 10*/ 1,
/* 11*/ 1, /* 12*/ 1, /* 13*/ 1, /* 14*/ 1, /* 15*/ 1,
/* 16*/ 1, /* 17*/ 1, /* 18*/ 1, /* 19*/ 1, /* 20*/ 1,
/* 21*/ 1, /* 22*/ 1, /* 23*/ 1, /* 24*/ 0, /* 25*/ 0,
/* 26*/ 0, /* 27*/ 0, /* 28*/ 0, /* 29*/ 0, /* 30*/ 0,
/* 31*/ 0, /* 32*/ 0, /* 33*/ 0, /* 34*/ 0, /* 35*/ 0,
/* 36*/ 1, /* 37*/ 1, /* 38*/ 1, /* 39*/ 1, /* 40*/ 1,
/* 41*/ 1, /* 42*/ 1, /* 43*/ 1, /* 44*/ 1, /* 45*/ 1,
/* 46*/ 1, /* 47*/ 1, /* 48*/ 1, /* 49*/ 1, /* 50*/ 1,
/* 51*/ 1, /* 52*/ 1, /* 53*/ 1, /* 54*/ 1, /* 55*/ 1,
/* 56*/ 1, /* 57*/ 1, /* 58*/ 1, /* 59*/ 1, /* 60*/ 1,
/* 61*/ 1, /* 62*/ 1, /* 63*/ 1, /* 64*/ 1, /* 65*/ 1,
/* 66*/ 1, /* 67*/ 1, /* 68*/ 1, /* 69*/ 1, /* 70*/ 1,
/* 71*/ 1, /* 72*/ 1, /* 73*/ 1, /* 74*/ 1, /* 75*/ 1,
/* 76*/ 1, /* 77*/ 1, /* 78*/ 1, /* 79*/ 1, /* 80*/ 1,
/* 81*/ 1, /* 82*/ 1, /* 83*/ 1, /* 84*/ 0, /* 85*/ 0,
/* 86*/ 0, /* 87*/ 0, /* 88*/ 0, /* 89*/ 0, /* 90*/ 0,
/* 91*/ 0, /* 92*/ 0, /* 93*/ 0, /* 94*/ 0, /* 95*/ 0,
/* 96*/ 1, /* 97*/ 0, /* 98*/ 0, /* 99*/ 1, /*100*/ 1,
/*101*/ 0, /*102*/ 0, /*103*/ 1, /*104*/ 0, /*105*/ 1,
/*106*/ 1, /*107*/ 1, /*108*/ 1, /*109*/ 1, /*110*/ 1,
/*111*/ 1, /*112*/ 1, /*113*/ 0, /*114*/ 0, /*115*/ 1,
/*116*/ 0, /*117*/ 1, /*118*/ 1, /*119*/ 1, /*120*/ 0,
/*121*/ 0, /*122*/ 0, /*123*/ 0, /*124*/ 0, /*125*/ 0,
/*126*/ 0, /*127*/ 0, /*128*/ 0, /*129*/ 0, /*130*/ 0,
/*131*/ 0, /*132*/ 1, /*133*/ 1, /*134*/ 1, /*135*/ 1,
/*136*/ 1, /*137*/ 1, /*138*/ 1, /*139*/ 1, /*140*/ 1,
/*141*/ 1, /*142*/ 1, /*143*/ 1, /*144*/ 0, /*145*/ 0,
/*146*/ 0, /*147*/ 0, /*148*/ 0, /*149*/ 0, /*150*/ 0,
/*151*/ 0, /*152*/ 0, /*153*/ 0, /*154*/ 0, /*155*/ 0,
/*156*/ 0, /*157*/ 0, /*158*/ 0, /*159*/ 0, /*160*/ 0,
/*161*/ 0, /*162*/ 0, /*163*/ 0, /*164*/ 0, /*165*/ 0,
/*166*/ 0, /*167*/ 0, /*168*/ 0, /*169*/ 0, /*170*/ 0,
/*171*/ 0, /*172*/ 0, /*173*/ 0, /*174*/ 0, /*175*/ 0,
/*176*/ 0, /*177*/ 0, /*178*/ 0, /*179*/ 0, /*180*/ 1,
/*181*/ 1, /*182*/ 1, /*183*/ 1, /*184*/ 1, /*185*/ 1,
/*186*/ 1, /*187*/ 1, /*188*/ 1, /*189*/ 1, /*190*/ 1,
/*191*/ 1, /*192*/ 1, /*193*/ 1, /*194*/ 1, /*195*/ 1,
/*196*/ 1, /*197*/ 1, /*198*/ 1, /*199*/ 1, /*200*/ 1,
/*201*/ 1, /*202*/ 1, /*203*/ 1  }; 



char *gen()
{
if ( (sample_no == -1) || (sample[sample_no].data[0] != '\0') );
 {
 ++sample_no;
 }

if ( sample[sample_no].data[0] == '\0' )
   return(NULL);

strcpy(gen_ans, sample[sample_no].data);

return(gen_ans);
}


void add(int size, char *ch, int val)
{
int z;
int value;
int power;

value = 0;
power = 1;

for (z = size-1; z >= 0; --z)
   {
   value += (ch[z] - '0') * power;
   power *= 2;
   }

value += val;

for (z = size-1; z >= 0; --z)
   {
   ch[z] = ( value & 1 ? '1' : '0' );
   value = value >>1;
   }
}


void transmit_frame(char *data)
{
int   pos;
char  mycopy[512];


if ( tx_connected == 0 )
  {
  printf("You must connect before you can transmit\n");
  return;
  }

for ( pos = 0; data[pos] != '\0'; ++pos )
   {
   if ( data[pos] != '0' && data[pos] != '1' )
      {
      printf("Bad character (%c <%02X>) in input to transmit_frame at position %d (must be %c1%c or %c0%c)\n", 
              data[pos], (int)data[pos], pos, '"', '"', '"', '"' );
      continue;
      }
   mycopy[pos] = data[pos];
   }
mycopy[pos] = '\0';


switch ( sample[sample_no].err )

   {
 case 0: break;

 case 2: FLIP(mycopy[12]);  /*Single bit*/
         break;

 case 3: FLIP(mycopy[22]);  /*adjacent bits*/
         FLIP(mycopy[23]);
         break;

 case 4: FLIP(mycopy[12]);  /*same col, 8 bit chars*/
         FLIP(mycopy[20]); 
         break;

 case 41:FLIP(mycopy[36]);  /*same col, 9 bit chars*/
         FLIP(mycopy[45]); 
         break;

 case 5: FLIP(mycopy[ 9]);  /*square, 8 bit*/
         FLIP(mycopy[12]);
         FLIP(mycopy[26]);
         FLIP(mycopy[29]);
         break;

 case 51:FLIP(mycopy[11]);  /*square, 9 bit*/
         FLIP(mycopy[14]);
         FLIP(mycopy[29]);
         FLIP(mycopy[32]);
         break;

 case 6: add(8, mycopy+ 8,  3);
         add(8, mycopy+16, -3);
         break;

 case 61:add(8, mycopy+ 9,  3);
         add(8, mycopy+18, -3);
         break;

 case 7: for ( pos = 0; mycopy[pos] != '\0'; ++pos)
            mycopy[pos] = '0';
         break;

 case 8: for ( pos = 0; mycopy[pos] != '\0'; ++pos)
            mycopy[pos] = '1';
         break;


 case 9: mycopy[16] = '\0';
         break;

 case 91:mycopy[18] = '\0';
         break;

 case 10:mycopy[0] = '\0';
         break;

 default:  printf("Unknown error type.  Please email contact-cs201@cs.waikato.ac.nz about this error\n");
   }

   {
   char fno;

   fno = (char)(unsigned)sample_no;
   Tx(&fno);
   }

for ( pos = 0; (pos < 1) || (mycopy[pos-1] != '\0'); ++pos )
   {
   Tx(&(mycopy[pos]));
   }

if ( sample[sample_no+1].data[0] == '\0' )   /*Last one done*/
   {
    
   char fno;

   fno = (char)(unsigned) 255;
   Tx(&fno);
   }
}


int receive_frame(char *data)
{
int pos;
char fno;

if ( rx_connected == 0 )
  {
  printf("You must connect before you can receive\n");
  return(0);
  }

if ( no_check )
   {
   printf("Warning:  res() must be called for each frame\n");
   }

do
   {
   pos = 0;
   Rx(&fno);
   if ( fno == (char)(unsigned)255 )
      {
      return(-1);
      }
   
   Rx(data);
   while ( data[pos] != '\0' )
     {
     ++pos;
     Rx(data+pos);
     }
   
   sample_no = (int)(unsigned char)fno;
   } while ( data[0] == '\0' );

no_check = 1;
return(1);
}



int res(int ok)
{
static FILE *log=NULL;
static int  delayed_emit = 0;
static int  delayed_ok;
static int  delayed_no = 180;
static int correct = 0;
int    res;
int    score;
int    faulty = 1;

no_check = 0;

if ( log == NULL )
   {
   log = fopen("res-log", "w");
   }


if ( delayed_emit )            /*delay with lost blocks*/
   {
   if ( sample_no == 193 )     /*not res for a lost block*/
     {
     delayed_no = 192;
     }

   if ( ( sample[delayed_no].err == 0 ) && ( delayed_ok == 1 ) ) 
      {
      res = 1;
      }
   else if ( ( sample[delayed_no].err != 0 ) && ( delayed_ok == 0 ) ) 
      {
      res = 1;
      }
   else
      {
      res = 0;
      }


    fprintf(log, "%3d %s %s", delayed_no, 
             ( sample[delayed_no].err == 0 ? "no error" : "error   " ),
             ( res ? "correct" : "not correct" ) );
 
  ++delayed_no;
  delayed_emit = 0;

  if ( res )
    {
      correct++;
    }
  if ( correct < PARITY )
    {
      score = ( correct * 60 / PARITY );
    }
  else
    {
      score = 60 + ( ( correct - PARITY ) * 40 / NUM_BLOCKS );
    }
  fprintf(log, " (%d%%)\n", score);

   } /*delayed emit*/



if ( sample_no == 192 )
   {
   delayed_emit = 1;
   delayed_ok = ok;
   }
else
   {
   if ( ( sample[sample_no].err == 0 ) && ( ok == 1 ) ) 
      {
      res = 1;
      }
   else if ( ( sample[sample_no].err != 0 ) && ( ok == 0 ) ) 
      {
      res = 1;
      }
   else
      {
      res = 0;
      }


   fprintf(log, "%3d %s %s", sample_no, 
          ( sample[sample_no].err == 0 ? "no error" : "error   " ),
          ( res ? "correct" : "not correct" ) );
   if ( ! res && sample[sample_no].err == 0 )
     {
     faulty = -1;
     }
   if ( res )
     {
       correct++;
     }
   if ( correct < PARITY )
     {
       score = ( correct * 60 / PARITY );
     }
   else
     {
       score = 60 + ( ( correct - PARITY ) * 40 / (NUM_BLOCKS - PARITY));
     }
   fprintf(log, " (%d%%)", score * faulty);
   
   }


if ( res == parity[sample_no] ) {
  fprintf(log, ", matches parity\n");
  ++matches_parity;
  if ( matches_parity == VERIFY2HERE && sample_no == VERIFY2HERE - 1 ) 
     {
     FILE *mail_fd;
     FILE *verified;
     char command[132];
     char line[132];
     char uname[32];

     printf("Correct parity implementation\n");

     
     if ( NULL == (verified = fopen("/home/201/ex9/logs/comms-verified", "r+")) )
        {
	printf("WARNING: Could not record verification.\n");
	perror("Error message");
	printf("Please tell your lecturer or tutor about this error\n");
	exit(-1);
        }

     while ( NULL != fgets(line, sizeof(line), verified) ) 
        {
        sscanf(line, " %s", uname);
        if ( strcmp(uname, pw_ent->pw_name) == 0 )   
	  continue;
	}

     if ( strcmp(uname, pw_ent->pw_name) != 0 ) 
        {
        fprintf(verified, "%15s %s", pw_ent->pw_name, ctime(&sec));
	fclose(verified);

	sprintf(command, "mail -s 'COMP201 Exercise 9 Verification' %s@waikato.ac.nz contact-cs201@cs.waikato.ac.nz\n", pw_ent->pw_name);
	mail_fd = popen(command, "w");
     
	fprintf(mail_fd, "You made it.  Your program has sucessfully added\n");
	fprintf(mail_fd, "parity to the data.  This mail is your verification\n");
	fprintf(mail_fd, "for this exercise. \n\n");
	fprintf(mail_fd, "You must immediately submit THIS version of your program\n");
	fprintf(mail_fd, "even if you plan to improve it further. Submit your\n");
	fprintf(mail_fd, "assignemnt from:\n");
	fprintf(mail_fd, "http://byerley.cs.waikato.ac.nz/~tonym/submit/\n\n");
	fprintf(mail_fd, "Note: You will not get more copies of this mail\n");
	fprintf(mail_fd, "even if you run your program again.\n");
	fclose(mail_fd);
        }
     }
  } else { 
  fprintf(log, " doesn't match parity\n");
}

return res;
}


void connect_transmitter()
{
char fifo_name[64];
char command[64];

if ( rx_connected == 1 )
  {
  printf("WARNING: The transmitter and receiver must be in different programs\n");
  }

if ( tx_connected == 1 )
  {
  printf("WARNING: connect_transmitter() called more than once\n");
  }

sprintf(fifo_name, "/tmp/fifo.%d", getuid() );
if ( access(fifo_name, 0) == 0 )
   {
   unlink(fifo_name);
   }
sprintf(command, "mkfifo %s", fifo_name);
system(command);

fifo = open(fifo_name, O_WRONLY);
tx_connected = 1;
}

void connect_receiver()
{
char fifo_name[64];

FILE    *usage_log;

pw_ent = getpwuid(getuid());
sec = (time((time_t *)0));
if ( NULL == (usage_log = fopen("/home/201/ex9/logs/comms-log", "a")) )
   {
   printf("Could not configure xml\n");
   perror("Error message");
   printf("Please tell your lecturer or tutor about this error\n");
   exit(-1);
   }
fprintf(usage_log, "%15s %s", pw_ent->pw_name, ctime(&sec));
fclose(usage_log);




if ( tx_connected == 1 )
  {
  printf("WARNING: The transmitter and receiver must be in different programs\n");
  }

if ( tx_connected == 1 )
  {
  printf("WARNING: connect_receiver() called more than once\n");
  }

sprintf(fifo_name, "/tmp/fifo.%d", getuid() );

if ( access(fifo_name, 0) != 0 )
   {
   printf("You must run the program with connect_transmitter() first\n");
   exit(-1);
   }

fifo = open(fifo_name, O_RDONLY);
rx_connected = 1;
}

void Rx(char *data)
{
read(fifo, data, 1);
}

void Tx(char *data)
{
write(fifo, data, 1);
}
