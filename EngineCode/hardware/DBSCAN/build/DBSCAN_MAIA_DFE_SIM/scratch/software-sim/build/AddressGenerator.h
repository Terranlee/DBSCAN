
#ifndef THREEDADDRESSGENERATOR_H_
#define THREEDADDRESSGENERATOR_H_

#include "ManagerSync.h"
#include "PushSync.h"
#include "Data.h"					// defines Data
#include "MappedElements.h"			// defines MappedRegisterPtr
#include "MemoryControllerConfig.h"	// defines MemoryControllerConfig struct

#include <boost/cstdint.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <vector>


namespace maxcompilersim {



enum OperationMode {LINEAR1D, STRIDED2D, BLOCK3D};

/*
 * AddressgeneratorSync Block.
 *
 * Notice: LINEAR1D and BLOCK3D modes are NOT bit-precise. (STRIDED2D
 *         only to a certain extend.)
 *
 * IOW: LINEAR1D and BLOCK3D do the arithmetic on 64 bit numbers. All
 * contents of the mapped registers are interpreted as unsigned when
 * read. Potential problem is the different warp around in software and
 * hardware. (STRIDED2D does not suffer from this issue.)
 * Technical reason for not using Data types: Lack of division, and
 * modulo operators.
 *
 */


// should be refactored in three/four different classes..
class AddressGeneratorSync:
	public PushSourceSync
{
private:
	unsigned int m_number_streams;
	MemoryControllerConfigPtr m_config;
	OperationMode m_mode;
	uint64_t m_enabled;
	uint64_t m_c_size;
	uint64_t m_bx, m_by, m_bz;
	uint64_t m_ystride, m_zstride;
	uint64_t m_sx, m_sy, m_sz;
	uint64_t m_xsize, m_ysize, m_zsize;

	Data m_2d_buffer_start_addr;
	Data m_2d_buffer_slow_top;
	Data m_2d_buffer_slow_stride_modified;
	Data m_2d_buffer_slow_top_wrap;
	Data m_2d_buffer_slow_offset_plus_start_addr;
	Data m_2d_buffer_fast_stride;
	Data m_2d_buffer_fast_stride_mul;
	Data m_2d_buffer_cmd_mul_faststride;
	Data m_2d_buffer_fast_top_cmds;
	Data m_2d_buffer_fast_lastcmd_size;

    std::vector<uint64_t> m_offsets;
    uint64_t m_current_stream;
    uint64_t m_pos_x, m_pos_y, m_pos_z;
    uint64_t m_cntr_x, m_cntr_y, m_cntr_z;
    Data m_2d_addr, m_2d_slow, m_2d_fast;
protected:
    // regs for 1D,2D,3D
	MappedRegisterPtr m_addr_en;
	MappedRegisterPtr m_cmd_size;
	// regs for 1D, 3D
	MappedRegisterPtr m_block_size_x;
	MappedRegisterPtr m_block_size_y;
	MappedRegisterPtr m_block_size_z;
	MappedRegisterPtr m_wrap_x;
	MappedRegisterPtr m_wrap_y;
	MappedRegisterPtr m_wrap_z;
	MappedRegisterPtr m_start_x_addr;
	MappedRegisterPtr m_start_y_addr;
	MappedRegisterPtr m_start_z_addr;
	MappedRegisterPtr m_stride_y;
	MappedRegisterPtr m_stride_z;
	// regs for 2D
	MappedRegisterPtr m_2d_start_addr;
	MappedRegisterPtr m_2d_slow_top;
	MappedRegisterPtr m_2d_slow_stride_modified;
	MappedRegisterPtr m_2d_slow_top_wrap;
	MappedRegisterPtr m_2d_slow_offset_plus_start_addr;
	MappedRegisterPtr m_2d_fast_stride;
	MappedRegisterPtr m_2d_fast_stride_mul;
	MappedRegisterPtr m_2d_cmd_mul_faststride;
	MappedRegisterPtr m_2d_fast_top_cmds;
	MappedRegisterPtr m_2d_fast_lastcmd_size;
	// regs for pentagraph mode
	std::vector<MappedRegisterPtr> m_offset_registers;
	std::vector<t_port_number>     m_output_ports;
	bool m_finished;

	bool isStalled();
	void check1D();
	void check2D();
	void check3D();
	void run1DCycle();
	void run2DCycle();
	void run3DCycle();
	void writeOutputCommand(uint64_t Cmd, int Tag, uint64_t addr, int inc);
public:

	const unsigned int getRegisterSize(const std::string &register_name);

	// MODIFIERS
	explicit AddressGeneratorSync(
			const std::string &name,
			int num_of_streams,
			MemoryControllerConfigPtr config,
			OperationMode mode);

	virtual ~AddressGeneratorSync() {}
	virtual void reset();
	virtual bool runCycle();

	// DEBUG
#ifdef MEMCONTROLLER_DEBUG
	MappedRegisterPtr getMappedReg(const std::string &name) {return ManagerBlock::getMappedReg(name);}
	const bool isDone() const { return m_finished;};
#endif
};

typedef boost::shared_ptr<AddressGeneratorSync> AddressGeneratorSyncPtr;

}  // namespace maxcompilersim


#endif /* THREEDADDRESSGENERATOR_H_ */
