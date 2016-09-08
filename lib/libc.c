// this file contains all C lib functions

#include "type.h"
#include "const.h"
#include "proto.h"
#include "global.h"

// get string length
int strlen( char * _str){
	int i;
	for( i = 0;; ++ i){
		if( _str[i] == '\0'){
			return i;
		}
	}
}

// string compare
BOOL strequ( char * _str1, char * _str2){
	int i;
	int len = strlen( _str1);
	if( len != strlen( _str2)){
		return FALSE;
	}
	for( i = 0; i < len; ++ i){
		if( _str1[i] != _str2[i]){
			return FALSE;
		}
	}
	return TRUE;
}

// itoa
PUBLIC char * itoa(char * str, int num)
{
	char *	p = str;
	char	ch;
	int	i;
	int	flag = FALSE;

	*p++ = '0';
	*p++ = 'x';

	if(num == 0){
		*p++ = '0';
	}
	else{	
		for(i=28;i>=0;i-=4){
			ch = (num >> i) & 0xF;
			if(flag || (ch > 0)){
				flag = TRUE;
				ch += '0';
				if(ch > '9'){
					ch += 7;
				}
				*p++ = ch;
			}
		}
	}

	*p = 0;

	return str;
}



