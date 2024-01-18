


#include "Sig.h"


Sig::Sig(uint16_t *data): data(data)
{
	uint64_t ltmp;
	data_length = (data[0] & 0x00FF) >> 0;
	trigger_type = (data[0] & 0xFF00) >> 2;

	tcb_trigger_number = 0;
	ltmp = data[1] & 0xFFFF;
	tcb_trigger_number += ltmp << 0;
	ltmp = data[2] & 0xFFFF;
	tcb_trigger_number += ltmp << 16;

	tcb_trigger_fine_time = data[3] & 0x00FF;

	tcb_trigger_coarse_time = 0;
	ltmp = data[3] & 0xFF00;
	tcb_trigger_coarse_time += ltmp << 0;
	ltmp = data[4] & 0xFFFF;
	tcb_trigger_coarse_time += ltmp << 16;
	tcb_trigger_coarse_time = tcb_trigger_coarse_time >> 8;


	mid = data[5] & 0x000F; mid = mid >> 0;
	sid = data[5] & 0x0070; sid = sid >> 4;
	ch = data[5] & 0xFF00; ch = ch >> 8;

	local_trigger_number = 0; 
	ltmp = data[6] & 0xFFFF;
	local_trigger_number += ltmp << 0;
	ltmp = data[7] & 0xFFFF;
	local_trigger_number += ltmp << 16;

	local_trigger_pattern = 0; 
	ltmp = data[8] & 0xFFFF;
	local_trigger_pattern += ltmp << 0;
	ltmp = data[9] & 0xFFFF;
	local_trigger_pattern += ltmp << 16;

	local_gate_fine_time = data[10] & 0x00FF;

	local_gate_coarse_time=0;
	ltmp = data[10] & 0xFF00;
	local_gate_coarse_time += ltmp << 0;
	ltmp = data[11] & 0xFFFF;
	local_gate_coarse_time += ltmp << 16;
	ltmp = data[12] & 0xFFFF;
	local_gate_coarse_time += ltmp << 32;
	ltmp = data[13] & 0x00FF;
	local_gate_coarse_time += ltmp << 48;
	local_gate_coarse_time = local_gate_coarse_time >> 8;


	peak = 0;
	ltmp = data[13] & 0xFF00;
	peak += ltmp; 
	ltmp = data[14] & 0x00FF;
	peak += ltmp << 16; 
	peak = peak >> 8;

	timing = 0;
	ltmp = data[14] & 0xFF00;
	timing += ltmp; 
	ltmp = data[15] & 0x00FF;
	timing += ltmp << 16; 
	timing = peak >> 8;

	flag = (data[15] & 0xFF00) >> 8;


	tcb_trigger_time = (uint64_t)tcb_trigger_coarse_time * 1000 + tcb_trigger_fine_time * 8;
	local_gate_time = (uint64_t)local_gate_coarse_time * 1000 + local_gate_fine_time * 8;




}


