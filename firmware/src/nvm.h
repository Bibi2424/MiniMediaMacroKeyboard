#ifndef NVM_H
#define NVM_H

#include <stdint.h>

struct nvm_t {
	uint16_t init;
	bool os;
};


extern nvm_t nvm_config;


extern void nvm_read(nvm_t &nvm);
extern void nvm_write(nvm_t &nvm);


#endif