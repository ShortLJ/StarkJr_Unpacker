

#include <stdint.h>



#ifndef __SIG__
#define __SIG__

class Sig
{
	public: 
		uint8_t data_length;
		uint8_t trigger_type;

		uint32_t tcb_trigger_number;
		uint16_t tcb_trigger_fine_time;
		uint32_t tcb_trigger_coarse_time;

		uint8_t sid;
		uint8_t mid;
		uint8_t ch;

		uint32_t local_trigger_number;
		uint32_t local_trigger_pattern;
		uint16_t local_gate_fine_time;
		uint64_t local_gate_coarse_time;

		uint16_t peak;
		uint16_t timing;

		uint8_t flag;

		uint64_t tcb_trigger_time;
		uint64_t local_gate_time;

		Sig() {	}
		Sig(uint8_t *data);
		~Sig() {	}
		bool operator<(const Sig *sig) const
		{
			return this->local_gate_time > sig->local_gate_time;
		}
	private:
		uint8_t *data;
};


#endif // __SIG__
