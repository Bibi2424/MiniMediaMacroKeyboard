#include <Arduino.h>
#include <EEPROM.h>
#include "nvm.h"
#include "shortcuts.h"


#define MAGIC_WORD	0xAA55


nvm_t nvm_config = {
	.init = MAGIC_WORD,
    .os = WINDOWS
};


extern void nvm_read(nvm_t &nvm) {
	nvm_t read;
	EEPROM.get(0, read);
	if(read.init != MAGIC_WORD) {
		nvm.init = MAGIC_WORD;
		EEPROM.put(0, nvm);
	}
	else {
		nvm = read;
	}
}


extern void nvm_write(nvm_t &nvm) {
	nvm.init = MAGIC_WORD;
	EEPROM.put(0, nvm);
}
