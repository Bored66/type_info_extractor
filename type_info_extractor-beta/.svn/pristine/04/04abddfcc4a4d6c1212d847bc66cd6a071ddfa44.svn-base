#pragma once
#include <string>

typedef struct
{
	//char c;
	float f;
    double i;
}
user_def_struct0;
typedef struct
{
    char const volatile *const volatile *const volatile *i1;
    short f;
    int i;
    char c0;
    user_def_struct0 st;
    char c;
    short sh;
    //std::string str;
} user_def_struct1;
typedef struct
{
	int i;//0
	user_def_struct1 st1;
    int i1;
	float f;
    float f1;
} simple_enclosure;
typedef struct
{
    int i;//0
    user_def_struct1 st1;
    int i1;
    user_def_struct0 st;
    int i2;
} simple_enclosure2;

typedef struct
{
	int i;//0
	float f;
	user_def_struct0 st;
	int i1;
	user_def_struct1 st1;//5
	float f1;
	user_def_struct1 st2;
    char c;
    short sh;
    char ch1;
} user_def_struct;

struct ms
{
	int in, i2, i3;
    char ch;
    short sh;
    char ch1;
    long l;
	double d;
    long double ld;
    float fl;
	user_def_struct st;
    char ch2;
    short sh1;
};
struct almost_empty
{
	int i;
	ms us;
	char c;
	user_def_struct1 st;
};
using uint = unsigned;
struct TYPE_KOEFF_4
{
    float k1;
    float k2;
    float k3;
    float k4;

};

struct TYPE_TEMPERATURE
{
    TYPE_KOEFF_4 line_koef;
    int calib_gain;
    int calib_zero;
    float resistor_v_ref;
    float resistor_rt;
    float low_pass_const;
    int crc8;
};
struct STATUS_BITS
{
    unsigned char err_conn_mdmp : 1;
    unsigned char err_wind : 1;
    unsigned char err_press : 1;
    unsigned char err_humid : 1;
    unsigned char err_temp1 : 1;
    unsigned char err_temp2 : 1;
    unsigned char err_temp3 : 1;
    unsigned char err_temp4 : 1;
    unsigned char err_connect : 1;
    unsigned char err_power : 1;
    unsigned char err_bit10 : 1;
    unsigned char err_bit11 : 1;
    unsigned char err_bit12 : 1;
    unsigned char err_bit13 : 1;
    unsigned char err_bit14 : 1;
    unsigned char err_bit15 : 1;

    bool operator==( const STATUS_BITS & oth ) {
        return err_conn_mdmp==oth.err_conn_mdmp && err_wind==oth.err_wind && err_press==oth.err_press && err_humid==oth.err_humid && err_temp1==oth.err_temp1 && err_temp2==oth.err_temp2 && err_temp3==oth.err_temp3 && err_temp4==oth.err_temp4 && err_connect==oth.err_connect && err_power==oth.err_power && err_bit10==oth.err_bit10 && err_bit11==oth.err_bit11 && err_bit12==oth.err_bit12 && err_bit13==oth.err_bit13 && err_bit14==oth.err_bit14 && err_bit15==oth.err_bit15;
    }
};
struct compose_rppt
{
    TYPE_TEMPERATURE temp_type;
    STATUS_BITS bits;
};
