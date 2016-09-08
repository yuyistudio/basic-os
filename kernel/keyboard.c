#include "type.h"
#include "const.h"
#include "proto.h"
#include "global.h"

#define KEY_BUFFER_SIZE	1024

// key queue: save input keys
u8  key_queue[ KEY_BUFFER_SIZE];	
int head 		= 0;
int tail 		= 0;		
u32 key_counter		= 0;

PRIVATE BOOL ReadKeyboard();
PRIVATE BOOL PushKey( u8 _key);
// public---------------------------------------------------------------------

// keyboard status varible
BOOL lshift = FALSE;
BOOL rshift = FALSE;

// handler keyboard interrupt
void KeyboardHandler()
{
	ReadKeyboard();
}

// init keyboard module
PUBLIC void InitKeyboardHandler()
{
	// init, do nothng now
}

// key queue: get one input key
PUBLIC u8 GetKey()
{
	u8 key;
	key = ViewKey( 0);
	AbandonKey( 1);
	return key;
}

// key queue: preview one key
PUBLIC u8 ViewKey( int _offset)
{
	if( key_counter == 0 || _offset > key_counter - 1)
	{
		return NULL;
	}
	return key_queue[ head + _offset];
}

// key queue: abandon one key
PUBLIC void AbandonKey( int _num)
{
	int i;
	for( i = 0; i < _num; ++ i)
	{
		if( key_counter != 0)
		{
			// conter
			key_counter --;
			// move pointer head to right position
			head ++;
			head %= KEY_BUFFER_SIZE;
		}
		else{
			return;
		}
	}
}

// private---------------------------------------------------------------------


// key euque: save one key
PRIVATE BOOL PushKey( u8 _key)
{
	if( key_counter != KEY_BUFFER_SIZE)
	{
		key_counter ++;
		// save the origin scan code
		key_queue[ tail] = _key;
		tail ++;
		tail %= KEY_BUFFER_SIZE;
		return TRUE;
	}
	else{
		return FALSE;
	}
}



// basic---------------------------------------------------------------------

// read one byte ( scan code) from register
PUBLIC BOOL ReadKeyboard()
{
	u8 key;
	key = in_byte( 0x60);
	if( PushKey( key) == FALSE)
	{
		// I should handle "buffer full" exception here
		return FALSE;
	}
	return TRUE;
}


