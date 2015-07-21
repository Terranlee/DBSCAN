#ifndef TCP_H
#define TCP_H

#include <fstream>
#include <string>
#include <queue>
#include <sys/time.h>

#include "ManagerSync.h"
#include "PushSync.h"
#include "Data.h"
#include "Ethernet.h"


// LWIP forward declarations
struct tcp_pcb;
struct netif;


namespace maxcompilersim {

class TCPSync;
class TCPConnection;
class TCPRxOutput;



class TOEMappedMemory : public AbstractMappedMemory {
private:
	TCPSync& m_tcp;

	std::vector<Data> m_data;

	static const int INTERRUPT_STATUS_LOW_ADDR;
	static const int INTERRUPT_STATUS_HIGH_ADDR;
	static const int INTERRUPT_TOGGLE_LOW_ADDR;
	static const int INTERRUPT_TOGGLE_HIGH_ADDR;

	typedef std::map<int, AbstractMappedMemoryPtr> per_connection_data_t;
	per_connection_data_t m_per_connection_data;

	AbstractMappedMemoryPtr getOrAddConnectionData(int connection_id) const;

	int getSelectorForEntry(uint entry) const;

	void setStateChange(int connection_id, bool changed);
	void clearStateChange(const Data &old_value, const Data &new_value, bool is_low);

public:
	TOEMappedMemory(TCPSync& tcp, uint bit_width, uint depth);
	virtual ~TOEMappedMemory();

	virtual const Data &getData(uint entry) const;
	virtual void setData(uint entry, const Data &value);

	virtual uint64_t get(uint32_t address, bool mec) const;
	virtual void set(uint32_t address, uint64_t value, uint mask, bool mec);

	const Data &getConnectionData(int connection_id, uint entry) const;
	void setConnectionData(int connection_id, uint entry, const Data &value);

	void initGlobalData();
	void initConnectionData(int connection_id);

	void checkForEvents(uint entry);
};

typedef boost::shared_ptr<TOEMappedMemory> TOEMappedMemoryPtr;




struct QueuedPacket {
	TCPConnection* m_connection;
	uint8_t* m_data;
	int m_length;

	QueuedPacket(TCPConnection* connection, uint8_t* data, int length) :
		m_connection(connection), m_data(data), m_length(length) { }

	~QueuedPacket()	{ if (m_data) { delete[] m_data; m_data = NULL; } }
};




class TCPRxDecoder {
protected:
	TCPSync& m_tcp;

public:
	TCPRxDecoder(TCPSync& tcp)
		: m_tcp( tcp) { }

	virtual ~TCPRxDecoder() { };

	virtual void decode(TCPConnection& connection, uint8_t* data, int length) = 0;
	virtual void reset() = 0;
};




class TCPRxDecoderFramed : public TCPRxDecoder {
public:
	TCPRxDecoderFramed(TCPSync& tcp);
	virtual ~TCPRxDecoderFramed();

	virtual void decode(TCPConnection& connection, uint8_t* data, int length);
	virtual void reset();
};




class TCPRxDecoderDisabled : public TCPRxDecoder {
public:
	TCPRxDecoderDisabled(TCPSync& tcp);
	virtual ~TCPRxDecoderDisabled();

	virtual void decode(TCPConnection& connection, uint8_t* data, int length);
	virtual void reset();
};




class TCPRxDecoderSizedFIX : public TCPRxDecoder {
private:
	static const int MAX_PACKET_LENGTH;

	bool m_seen_prefix;
	bool m_seen_length;
	int m_packet_length_total;
	int m_packet_length_received;

	uint8_t* m_packet;

	int m_data_stream;

	int decodeInternal(TCPConnection& connection, uint8_t* data, int length);

public:
	TCPRxDecoderSizedFIX(TCPSync& tcp, int data_stream);
	virtual ~TCPRxDecoderSizedFIX();

	virtual void decode(TCPConnection& connection, uint8_t* data, int length);
	virtual void reset();
};




class TCPRxDecoderSizedSoupBinTCP : public TCPRxDecoder {
private:
	static const int MAX_PACKET_LENGTH;

	bool m_seen_length;
	int m_packet_length_total;
	int m_packet_length_received;

	uint8_t* m_packet;

	int m_data_stream;

	int decodeInternal(TCPConnection& connection, uint8_t* data, int length);

public:
	TCPRxDecoderSizedSoupBinTCP(TCPSync& tcp, int data_stream);
	virtual ~TCPRxDecoderSizedSoupBinTCP();

	virtual void decode(TCPConnection& connection, uint8_t* data, int length);
	virtual void reset();
};


class TCPRxDecoderSizedETI : public TCPRxDecoder {
private:
	static const int MAX_PACKET_LENGTH;

	bool m_seen_length;
	int m_packet_length_total;
	int m_packet_length_received;

	uint8_t* m_packet;

	int m_data_stream;

	int decodeInternal(TCPConnection& connection, uint8_t* data, int length);

public:
	TCPRxDecoderSizedETI(TCPSync& tcp, int data_stream);
	virtual ~TCPRxDecoderSizedETI();

	virtual void decode(TCPConnection& connection, uint8_t* data, int length);
	virtual void reset();
};



class TCPRxOutput {
private:
	TCPSync& m_tcp;

	const t_port_number m_port;

	int m_packet_queue_length;
	int m_packet_length_received;

	std::queue<QueuedPacket*> m_packet_queue;

public:
	explicit TCPRxOutput(TCPSync& tcp, const t_port_number port);

	void enquePacket(TCPConnection& connection, uint8_t* data, int length);
	bool hasInFlightPacket(TCPConnection& connection);
	void clearPackets(TCPConnection& connection, bool keep_in_flight_packets);

	bool runCycle();

	void reset();
};




class TCPConnection {
private:
	TCPSync& m_tcp;

	tcp_pcb* m_pcb;

	int m_id;

	TCPRxDecoder* m_rx_decoder;

	void clearPackets(bool keep_in_flight_packets);

public:

	enum SimSendResult {
		SIM_SEND_OK,
		SIM_SEND_CLOSED,
		SIM_SEND_RETRY
	};

	TCPConnection(TCPSync& tcp, int id);
	~TCPConnection();

	int getID() { return m_id; }

	void reset();

	void simListen();
	bool simIsListening();
	void simConnect();
	SimSendResult simSend(uint8_t* data, int length);
	void simClose();
	void simClosed();
	void simAbort(bool send_reset);
	void simConfirmReceived(int length, bool eof);

	// Methods to be called from LWIP callbacks
	void simAccepted(tcp_pcb *new_pcb);
	void simConnected(tcp_pcb *new_pcb);
	void simReceived(uint8_t* data, int length);

	bool operator==(const TCPConnection &rhs) const;
	bool operator!=(const TCPConnection &rhs) const;
};




class TCPSync :
	public PushSinkSync,
	public PushSourceSync
{
public:
	enum RxMode {
		RX_MODE_FRAMED,
		RX_MODE_SIZED_MESSAGE,
		RX_MODE_SIZED_STATIC_FIX_4_2,
		RX_MODE_SIZED_STATIC_SOUPBINTCP,
		RX_MODE_SIZED_STATIC_ETI
	};

private:
	const t_port_number m_tx_port;

	typedef std::tr1::unordered_map<std::string, TCPRxOutput> rx_outputs_t;
	rx_outputs_t m_rx_outputs;

	typedef std::tr1::unordered_map<int, std::string> rx_output_id_to_name_map_t;
	rx_output_id_to_name_map_t m_rx_output_id_to_name_map;

	const t_port_number m_net_mac_rx_port;
	const t_port_number m_net_mac_tx_port;

	const t_port_number m_up_mac_tx_port;
	const t_port_number m_up_mac_rx_port;

	static const int TOE_MEM_WIDTH;
	static const int TOE_MEM_DEPTH;

	static const int RX_DECODE_MEM_WIDTH;
	static const int RX_DECODE_MEM_DEPTH;

	TOEMappedMemoryPtr m_toe_mem;
	AbstractMappedMemoryPtr m_rx_decoder_mem;

	RxMode m_rx_mode;
	int m_num_connections;

	typedef std::map<int, TCPConnection*> tcp_connections_t;
	tcp_connections_t m_connections;


	// MAC
	bool m_got_net_mac_rx_raw_data;
	Data m_net_mac_rx_raw_data;

	bool m_got_up_mac_tx_raw_data;
	Data m_up_mac_tx_raw_data;

	bool m_net_mac_rx_sof_seen;
	int m_net_mac_rx_length_received;
	uint8_t m_net_mac_rx_frame_data[ETHERNET_MTU];


	static const int NET_MAC_TX_MAX_QUEUE_LENGTH;
	int m_net_mac_tx_queue_length;
	std::queue<QueuedPacket*> m_net_mac_tx_queue;
	int m_net_mac_tx_length_sent;

	// TCP
	bool m_tx_sof_seen;
	bool m_got_tx_raw_data;
	Data m_tx_raw_data;
	int m_tx_packet_lenght;
	uint8_t m_tx_packet_data[ETHERNET_MTU];


	// LWIP
	static bool m_lwip_initialised;

	bool m_network_if_initialised;

	netif* m_network_if;

	timeval m_last_tcp_tick_time;
	timeval m_last_arp_tick_time;
	timeval m_last_ip_reass_tick_time;


	void initMappedMemory();

	TCPConnection* getOrAddConnection(int tx_con_id);
	TCPConnection* getConnection(int con_id);

	bool handleNetMacTx();
	bool handleNetAndUpMacRx();

	bool handleUpMacTx();

	bool handleTx();

	void initNetworkIf();
	void tickTimers();
	void passNetMacRxToLWIP();

public:
	explicit TCPSync(const std::string &name, RxMode rx_mode);
	virtual ~TCPSync();

	// Allow access to protected pushOutput
	void pushOutput(t_port_number port_number, const Data &data);

	virtual void pushInput(const t_port_number port_number, const Data &data);
	virtual bool isPushInputStalled(const t_port_number port_number) const;
	virtual bool runCycle();
	virtual void reset();

	virtual void setSimParameter(const std::string &name, const std::string &value);

	void addRxOutput(const std::string &name, int stream_id);

	TCPRxDecoder* makeRxDecoder(TCPConnection& connection);

	RxMode getRxMode();

	void enqueRxPacket(const std::string& output, TCPConnection& connection, uint8_t* data, int length);
	void enqueRxPacket(int output_stream_id, TCPConnection& connection, uint8_t* data, int length);

	bool hasInFlightRxPackets(TCPConnection& connection);
	void clearRxPackets(TCPConnection& connection, bool keep_in_flight_packets);

	void enqueNetMacTxPacket(uint8_t* data, int length);

	TOEMappedMemoryPtr getTOEMemory();
	void dispatchMappedMemoryEvents();

	netif* getNetworkIf() { return m_network_if; }

};

} // namespace maxcompilersim

#endif // TCP_H
