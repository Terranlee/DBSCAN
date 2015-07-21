#ifndef WATCH_H_
#define WATCH_H_

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <limits>
#include <stdint.h>
#include <stdio.h>

#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>


#include "Data.h"
#include "ManagerInfrastructure.h"

//#define WATCH_WRITEOUT_INTERNAL_DATA_STREAMS


namespace maxcompilersim {

// TODO: handle format string
class DataWriter {
public:
	virtual ~DataWriter() {}

	virtual void write(const Data &data, std::ostream &os) const = 0;
	virtual void writeHeader(const std::string &name, std::ostream &os) const = 0;
};


class WatchSync :
	public PushSinkSync
{
private:
	typedef std::map<t_port_number, boost::shared_ptr<DataWriter> > WriterMap; // needs to be ordered!
	typedef std::list<std::pair<t_port_number, Data> > ValueList;
	typedef std::map<t_port_number, Data> ValueMap;
	typedef std::map<t_port_number, boost::shared_ptr<std::ofstream> > StreamMap;
	typedef std::map<t_port_number, std::string> NameMap;

	const std::string m_maxfile_stem;
	const std::string m_kernel_name;

	bool m_enabled;

	std::ofstream m_csv_stream;

	WriterMap m_writer_map;
	ValueList m_value_list;
#ifdef WATCH_WRITEOUT_INTERNAL_DATA_STREAMS
	StreamMap m_stream_map;
#endif
	NameMap   m_name_map;

	WriterMap m_const_writer_map;
	ValueMap  m_const_value_map;

	bool m_restriction_set;
	int m_current_cycle;
	int m_cycle_first;
	int m_cycle_last;

	boost::recursive_mutex m_mutex;

public:
	WatchSync(const std::string &block_name, const std::string &maxfile_stem, const std::string &kernel_name);
	virtual ~WatchSync() {};

	virtual void pushInput(const t_port_number input_port, const Data &data);
	virtual bool isPushInputStalled(const t_port_number input_port) const;

	virtual void reset();
	virtual bool runCycle();

	t_port_number addInput(const std::string &name, const std::string &unmangled_name, boost::shared_ptr<DataWriter> writer);
	t_port_number addConstant(const std::string &name, const std::string &unmangled_name, boost::shared_ptr<DataWriter> writer, const Data &data);

	void enable(const std::string &dest_path);
	void disable();
	void flush();
	void restrict_(const std::string &kernel_name, int begin, int end); // restrict is a keyword in C99 and soon will be in C++

private:
	void writeCSVHeader();
	void writeCSVDataLine();
};



template <typename T, bool has_doubt>
class BinaryWriter : public DataWriter {
public:
	virtual ~BinaryWriter() {}

	virtual void write(const Data &data, std::ostream &os) const {
		if (has_doubt) {
			os << "0b";
			for(int i = T::totalBitSize - 1; i >= 0; i--)
				os << (data.get_bit(i) ? "1" : "0");

			if (data.get_bit(T::totalBitSize))
				os << "?";
		}
		else {
			os << "0b";
			for(int i = T::totalBitSize - 1; i >= 0; i--)
				os << (data.get_bit(i) ? "1" : "0");
		}
	}

	virtual void writeHeader(const std::string &name, std::ostream &os) const {
		os << name;
	}
};


template <typename T, bool has_doubt>
class HexadecimalWriter : public DataWriter {
public:
	virtual ~HexadecimalWriter() {}

	virtual void write(const Data &data, std::ostream &os) const {
		const std::ios_base::fmtflags orig = os.flags();

		if (has_doubt) {
			const int data_size =  T::totalBitSize;
			os << "0x" << std::hex << T(data.slice(0, data_size).toVarIntU<T::totalBitSize>());
			os.flags(orig);
			if (data.get_bit(T::totalBitSize))
				os << "?";
		}
		else {
			os << "0x" << std::hex << T(data.toVarIntU<T::totalBitSize>());
			os.flags(orig);
		}

	}

	virtual void writeHeader(const std::string &name, std::ostream &os) const {
		os << name;
	}
};


template<typename T, bool has_doubt>
class DecimalWriter : public DataWriter {
public:
	virtual ~DecimalWriter() {}

	virtual void write(const Data &data, std::ostream &os) const {
		const std::ios_base::fmtflags orig = os.flags();

		if (has_doubt) {
			const int data_size =  T::totalBitSize;
			os << std::setprecision (std::numeric_limits<double>::digits10 + 1)
			   << T(data.slice(0, data_size).toVarIntU<T::totalBitSize>()).getValueAsDouble();
			os.flags(orig);
			
			if (data.get_bit(T::totalBitSize))
				os << "?";
		}
		else {
			os << std::setprecision (std::numeric_limits<double>::digits10 + 1)
			   << T(data.toVarIntU<T::totalBitSize>()).getValueAsDouble();
			os.flags(orig);
		}
	}

	virtual void writeHeader(const std::string &name, std::ostream &os) const {
		os << name;
	}
};

// Specialised to clean up formatting of HWOffsetFix numbers with no fractional part
template<uint size, int offset, SignMode sign_mode, bool has_doubt>
class DecimalWriter<HWOffsetFix<size,offset,sign_mode>, has_doubt>  : public DataWriter {
public:
	virtual ~DecimalWriter() {}

	virtual void write(const Data &data, std::ostream &os) const {
		if(offset >= 0 && (offset + size <= sizeof(long) * 8)) {
			if (has_doubt) {
				os << HWOffsetFix<size,offset,sign_mode>(data.slice(0, size).toVarIntU<HWOffsetFix<size,offset,sign_mode>::totalBitSize>()).getValueAsLong();

				if (data.get_bit(size))
					os << "?";
			}
			else {
				os << HWOffsetFix<size,offset,sign_mode>(data.toVarIntU<HWOffsetFix<size,offset,sign_mode>::totalBitSize>()).getValueAsLong();
			}
		} else {
			const std::ios_base::fmtflags orig = os.flags();
		
			if (has_doubt) {
				os << std::setprecision (std::numeric_limits<double>::digits10 + 1)
				   << HWOffsetFix<size,offset,sign_mode>(data.slice(0, size).toVarIntU<HWOffsetFix<size,offset,sign_mode>::totalBitSize>()).getValueAsDouble();
				os.flags(orig);

				if (data.get_bit(size))
					os << "?";
			}
			else {
				os << std::setprecision (std::numeric_limits<double>::digits10 + 1)
				   << HWOffsetFix<size,offset,sign_mode>(data.toVarIntU<size>()).getValueAsDouble();
				os.flags(orig);
			}
		}
	}

	virtual void writeHeader(const std::string &name, std::ostream &os) const {
		os << name;
	}
};


template <uint size, bool has_doubt>
class DecimalWriter<HWRawBits<size>, has_doubt> : public DataWriter {
public:
	virtual ~DecimalWriter() {}

	virtual void write(const Data &data, std::ostream &os) const {

		if (has_doubt) {
			os << HWRawBits<size>(data.slice(0, size).toVarIntU<size>()).getValueAsLong();

			if (data.get_bit(size))
				os << "?";
		}
		else {
			os << HWRawBits<size>(data.toVarIntU<size>()).getValueAsLong();
		}
	}

	virtual void writeHeader(const std::string &name, std::ostream &os) const {
		os << name;
	}
};



template <typename T>
Data bits_to_data(const char *bits) {
	int bit_width = T::totalBitSize;

	if(strlen(bits) != T::totalBitSize)
		throw new SimException("strlen(bits) != T::totalBitSize"); // XXX: error message

	Data data(T::totalBitSize);
	for(int i = 0; i < bit_width; i++)
		data.set_bit(i, bits[bit_width - 1 - i] != '0');

	return data;
}

} // namespace maxcompilersim

#endif /* WATCH_H_ */
