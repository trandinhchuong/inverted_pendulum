#include <stdbool.h>
#include <stdlib.h>

#include "fifo.h"
#include "sys_dbg.h"


void fifo_init(fifo_t* fifo, memcpy_f memcpy, void* buffer, uint32_t buffer_size, uint32_t element_size) {
	if (fifo == NULL || buffer == NULL || memcpy == NULL) {
		FATAL("FIFO", 0x01);
	}

	fifo->tail_index = 0;
	fifo->head_index = 0;
	fifo->fill_size = 0;

	fifo->memcpy = memcpy;

	fifo->buffer_size = buffer_size;
	fifo->buffer = buffer;
	fifo->element_size = element_size;
}

uint8_t fifo_availble(fifo_t* fifo) {
	return fifo->fill_size;
}

bool fifo_is_empty(fifo_t* fifo) {
	return (fifo->fill_size == 0) ? true : false;
}

bool fifo_is_full(fifo_t* fifo) {
	return (fifo->fill_size == fifo->buffer_size) ? true : false;
}

uint8_t fifo_put(fifo_t* fifo, void* data) {
	uint8_t next_tail_index;

	SYS_DBG("[fifo_put]\n");

	if (fifo->fill_size == fifo->buffer_size) {
		FATAL("FIFO", 0x02);
		return RET_FIFO_NG;
	}

	if (data != NULL) {
		fifo->fill_size++;

		fifo->memcpy((uint8_t*)(fifo->buffer + fifo->tail_index * fifo->element_size), (uint8_t*)data, fifo->element_size);

		next_tail_index = (++fifo->tail_index) % fifo->buffer_size;
		fifo->tail_index = next_tail_index;
	}
	else {
		return RET_FIFO_NG;
	}

	return RET_FIFO_OK;
}

uint8_t fifo_get(fifo_t* fifo, void* data) {
	uint8_t next_head_index;

	SYS_DBG("[fifo_get]\n");

	if (fifo_is_empty(fifo)) {
		FATAL("FIFO", 0x03);
		return RET_FIFO_NG;
	}

	if (data != NULL) {
		fifo->memcpy((uint8_t*)data, (uint8_t*)(fifo->buffer + fifo->head_index * fifo->element_size),  fifo->element_size);

		next_head_index = (++fifo->head_index) % fifo->buffer_size;
		fifo->head_index = next_head_index;

		fifo->fill_size--;
	}
	else {
		return RET_FIFO_NG;
	}

	return RET_FIFO_OK;
}
