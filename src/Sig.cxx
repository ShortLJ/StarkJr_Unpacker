


#include "Sig.h"


Sig::Sig(uint8_t *data): data(data)
{
	uint64_t ltmp;
	uint32_t itmp;
	uint16_t stmp;

    data_length = data[0] & 0xFF;
	trigger_type = data[1] & 0xFF;

	tcb_trigger_number = 0;
	itmp = data[2] & 0xFF;
	itmp = itmp << 0;
	tcb_trigger_number = tcb_trigger_number + itmp;
	itmp = data[3] & 0xFF;
	itmp = itmp << 8;
	tcb_trigger_number = tcb_trigger_number + itmp;
	itmp = data[4] & 0xFF;
	itmp = itmp << 16;
	tcb_trigger_number = tcb_trigger_number + itmp;
	itmp = data[5] & 0xFF;
	itmp = itmp << 24;
	tcb_trigger_number = tcb_trigger_number + itmp;

	tcb_trigger_fine_time = data[6] & 0xFF;

	tcb_trigger_coarse_time = 0;
	itmp = data[7] & 0xFF;
	itmp = itmp << 0;
	tcb_trigger_coarse_time = tcb_trigger_coarse_time + itmp;
	itmp = data[8] & 0xFF;
	itmp = itmp << 8;
	tcb_trigger_coarse_time = tcb_trigger_coarse_time + itmp;
	itmp = data[9] & 0xFF;
	itmp = itmp << 16;
	tcb_trigger_coarse_time = tcb_trigger_coarse_time + itmp;


	mid = data[10] & 0x0F;
	sid = (data[10] & 0x70) >> 4;

	ch = data[11] & 0xFF;
	ch = ch-1;

	local_trigger_number = 0;
	itmp = data[12] & 0xFF;
	itmp = itmp << 0;
	local_trigger_number = local_trigger_number + itmp;
	itmp = data[13] & 0xFF;
	itmp = itmp << 8;
	local_trigger_number = local_trigger_number + itmp;
	itmp = data[14] & 0xFF;
	itmp = itmp << 16;
	local_trigger_number = local_trigger_number + itmp;
	itmp = data[15] & 0xFF;
	itmp = itmp << 24;
	local_trigger_number = local_trigger_number + itmp;

	local_trigger_pattern = 0;
	itmp = data[16] & 0xFF;
	itmp = itmp << 0;
	local_trigger_pattern = local_trigger_pattern + itmp;
	itmp = data[17] & 0xFF;
	itmp = itmp << 8;
	local_trigger_pattern = local_trigger_pattern + itmp;
	itmp = data[18] & 0xFF;
	itmp = itmp << 16;
	local_trigger_pattern = local_trigger_pattern + itmp;
	itmp = data[19] & 0xFF;
	itmp = itmp << 24;
	local_trigger_pattern = local_trigger_pattern + itmp;

	local_gate_fine_time = data[20] & 0xFF;

	local_gate_coarse_time = 0;
	ltmp = data[21] & 0xFF;
	ltmp = ltmp << 0;
	local_gate_coarse_time = local_gate_coarse_time + ltmp;
	ltmp = data[22] & 0xFF;
	ltmp = ltmp << 8;
	local_gate_coarse_time = local_gate_coarse_time + ltmp;
	ltmp = data[23] & 0xFF;
	ltmp = ltmp << 16;
	local_gate_coarse_time = local_gate_coarse_time + ltmp;
	ltmp = data[24] & 0xFF;
	ltmp = ltmp << 24;
	local_gate_coarse_time = local_gate_coarse_time + ltmp;
	ltmp = data[25] & 0xFF;
	ltmp = ltmp << 32;
	local_gate_coarse_time = local_gate_coarse_time + ltmp;
	ltmp = data[26] & 0xFF;
	ltmp = ltmp << 40;
	local_gate_coarse_time = local_gate_coarse_time + ltmp;


	peak = 0;
	stmp = data[27] & 0xFF;
	stmp = stmp << 0;
	peak = peak + stmp;
	stmp = data[28] & 0xFF;
	stmp = stmp << 8;
	peak = peak + stmp;

	timing = 0;
	stmp = data[29] & 0xFF;
	stmp = stmp << 8;
	timing = timing + stmp;
	stmp = data[30] & 0xFF;
	stmp = stmp << 8;
	timing = timing + stmp;

	flag = data[31] & 0xFF;    // pile_up flag @ bit1, hit flag @ bit0


	tcb_trigger_time = (uint64_t)tcb_trigger_coarse_time * 1000 + (tcb_trigger_fine_time << 3);
	local_gate_time = (uint64_t)local_gate_coarse_time * 1000 + (local_gate_fine_time << 3);




}

void Sig::Print()
{
	fprintf(stdout,"tcb_trigger_number %u\t",tcb_trigger_number);
	fprintf(stdout,"local_trigger_number %u\t",local_trigger_number);
	fprintf(stdout,"sid %u mid %u ch %u\t",sid,mid,ch);
	fprintf(stdout,"tcb_trigger_time %lu\t",tcb_trigger_time);
	fprintf(stdout,"local_gate_time %lu\n",local_gate_time);
}


/*void Sig::Print()
{
	fprintf(stdout,"tcb_trigger_number %u\n",tcb_trigger_number);
	fprintf(stdout,"local_trigger_number %u\n",local_trigger_number);
	fprintf(stdout,"sid %u mid %u ch %u\n",sid,mid,ch);
	fprintf(stdout,"tcb_trigger_time %lu\n",tcb_trigger_time);
	fprintf(stdout,"local_gate_time %lu\n",local_gate_time);
}*/
