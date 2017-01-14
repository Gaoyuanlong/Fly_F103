#ifndef __COMMON_H__
#define __COMMON_H__

typedef enum
{
	False,
	True

}BOOL;

#define BOOL_REVERSE(TMP) { \
if(TMP == True) TMP = False;\
else TMP = True;\
};

#define Enable True
#define Disable False




#endif
