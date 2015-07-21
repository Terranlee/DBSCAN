/*
 * This is a copy of maxcompilerrt/internal/toe_internal_regs.h adapted to work with MappedMemories.
 */

#include <unistd.h>

// Mask out bits from the value
#define MaskOut(X,B,W) (X & ~(((1L<<W)-1)<<B))
// Mask the value then shift up (left)
#define MaskShiftLeft(X,B,W) ((X & ((1L<<W)-1)) << B)
// Mask the value shifted down (right)
#define MaskShiftRight(X,B,W) ((X >> B) & ((1L<<W)-1))
//#define DPRINTF(A...) printf(A)
#define DPRINTF(A...)
#define SetRegAt(MEM, VALUE, ADDR, BASE, WIDTH) { uint64_t tmp = MEM->getData(ADDR); MEM->setData(ADDR, Data(MEM->getWidth(), MaskOut(tmp, BASE, WIDTH) | MaskShiftLeft(VALUE, BASE, WIDTH))); DPRINTF("Setting register @ 0x%08x : %2d bits at %2d, old value: %08lx, supplied value: 0x%08lx, new value %08lx\n", ADDR, WIDTH, BASE, (uint64_t)tmp, (uint64_t) VALUE,(uint64_t) MaskOut(tmp, BASE, WIDTH) | MaskShiftLeft(VALUE, BASE, WIDTH)); }
#define GetRegAt(MEM, ADDR, BASE, WIDTH) (uint32_t)MaskShiftRight((uint64_t) MEM->getData(ADDR), BASE, WIDTH)
#define SetConRegAt(CON, MEM, VALUE, ADDR, BASE, WIDTH) { uint64_t tmp = MEM->getConnectionData(CON, ADDR); MEM->setConnectionData(CON, ADDR, Data(MEM->getWidth(), MaskOut(tmp, BASE, WIDTH) | MaskShiftLeft(VALUE, BASE, WIDTH))); DPRINTF("Setting register @ 0x%08x : %2d bits at %2d, old value: %08lx, supplied value: 0x%08lx, new value %08lx\n", ADDR, WIDTH, BASE, (uint64_t)tmp, (uint64_t) VALUE,(uint64_t) MaskOut(tmp, BASE, WIDTH) | MaskShiftLeft(VALUE, BASE, WIDTH)); }
#define GetConRegAt(CON, MEM, ADDR, BASE, WIDTH) (uint32_t)MaskShiftRight((uint64_t) MEM->getConnectionData(CON, ADDR), BASE, WIDTH)

// Block TCPE
// ==========
/* TOE core identification */
#define TcpeCoreId                     0x00000400 
/* TcpeCoreType - TOE core type ID */
#define GetTcpeCoreType(MEM) GetRegAt(MEM, TcpeCoreId, 24,8)
/* TcpeCoreMajorVers - TOE core major version */
#define GetTcpeCoreMajorVers(MEM) GetRegAt(MEM, TcpeCoreId, 16,8)
/* TcpeCoreMinorVers - TOE core minor version */
#define GetTcpeCoreMinorVers(MEM) GetRegAt(MEM, TcpeCoreId, 0,16)

/* TCPE control */
#define TcpeCtrl                       0x00000404 
/* SkipTimeWait 0 Skip TIME_WAIT and go directly to CLEAR_TCB. */
#define GetSkipTimeWait(MEM) GetRegAt(MEM, TcpeCtrl, 8,1)
#define SetSkipTimeWait(MEM, VALUE) SetRegAt(MEM, VALUE, TcpeCtrl, 8,1)
/* TcpeKeepTcb 0 Keep the Toe in closed, if entered and do not reset */
#define GetTcpeKeepTcb(MEM) GetRegAt(MEM, TcpeCtrl, 3,1)
#define SetTcpeKeepTcb(MEM, VALUE) SetRegAt(MEM, VALUE, TcpeCtrl, 3,1)
/* TcpePushDis 0 Disable assertion of push flag in last segment of the */
#define GetTcpePushDis(MEM) GetRegAt(MEM, TcpeCtrl, 2,1)
#define SetTcpePushDis(MEM, VALUE) SetRegAt(MEM, VALUE, TcpeCtrl, 2,1)
/* TcpeSillyWindowDis 0 Disable TCP silly window avoidance if set to 1 */
#define GetTcpeSillyWindowDis(MEM) GetRegAt(MEM, TcpeCtrl, 1,1)
#define SetTcpeSillyWindowDis(MEM, VALUE) SetRegAt(MEM, VALUE, TcpeCtrl, 1,1)
/* TcpeSlowStartDis 1 Disable TCP slow start protocol if set to 1 */
#define GetTcpeSlowStartDis(MEM) GetRegAt(MEM, TcpeCtrl, 0,1)
#define SetTcpeSlowStartDis(MEM, VALUE) SetRegAt(MEM, VALUE, TcpeCtrl, 0,1)

// Block TCPI
// ==========
/* TCPI control */
#define TcpiCtrl                       0x00000500 
/* TcpiFastReXmitEna 1 Fast retransmit enable 0: disable 1: enable */
#define GetTcpiFastReXmitEna(MEM) GetRegAt(MEM, TcpiCtrl, 22,1)
#define SetTcpiFastReXmitEna(MEM, VALUE) SetRegAt(MEM, VALUE, TcpiCtrl, 22,1)
/* TcpiTimDelAck 2 Value to be loaded in delayed ACK timer. Time is given in units of SaTimFastFastInterval */
#define GetTcpiTimDelAck(MEM) GetRegAt(MEM, TcpiCtrl, 16,2)
#define SetTcpiTimDelAck(MEM, VALUE) SetRegAt(MEM, VALUE, TcpiCtrl, 16,2)
/* TcpiTimKeepAlive 255 Value to be loaded in keep alive timer. Time is given in units of SaTimFastSlowInterval */
#define GetTcpiTimKeepAlive(MEM) GetRegAt(MEM, TcpiCtrl, 8,8)
#define SetTcpiTimKeepAlive(MEM, VALUE) SetRegAt(MEM, VALUE, TcpiCtrl, 8,8)
/* TcpiDupAckThresh 4 Threshold for Tcb.DupAcks. When this number of duplicate ACKs has been received, a fast re-transmit is initiated. */
#define GetTcpiDupAckThresh(MEM) GetRegAt(MEM, TcpiCtrl, 4,4)
#define SetTcpiDupAckThresh(MEM, VALUE) SetRegAt(MEM, VALUE, TcpiCtrl, 4,4)
/* TcpiFastRecovEna 1 Fast recovery mode enable. 0: disable 1: enable If enabled also TcpiFastReXmitEna must be enabled. */
#define GetTcpiFastRecovEna(MEM) GetRegAt(MEM, TcpiCtrl, 3,1)
#define SetTcpiFastRecovEna(MEM, VALUE) SetRegAt(MEM, VALUE, TcpiCtrl, 3,1)

/* TCPI debug control */
#define TcpiDbgCtrl                    0x00000504 
/* TcpiDelAckDis 0 Disable delayed ACK 0: Do delayed ACK (normal operation) 1: Disable delayed ACK, i.e., send ACK for each segment with data. */
#define GetTcpiDelAckDis(MEM) GetRegAt(MEM, TcpiDbgCtrl, 5,1)
#define SetTcpiDelAckDis(MEM, VALUE) SetRegAt(MEM, VALUE, TcpiDbgCtrl, 5,1)

/* Ingress TCP frame processing cause */
#define TcpiProcCause                  0x00000518 
/* TcpiProcDropCause C 0 Reason for a TCP frame is dropped. Each bit position corresponds to the occurrence of a particular reason since the bit was last cleared. Each bit position is cleared upon writing a '1' to to the bit position. 23: Not used 22: Not used 21: Not used 20: Not used 19: Not used 18: PAWS triggered 17: Wrong ACK number 16: Sequence number above RcvAdv */
#define GetTcpiProcDropCause(MEM) GetRegAt(MEM, TcpiProcCause, 16,8)
#define SetTcpiProcDropCause(MEM, VALUE) SetRegAt(MEM, VALUE, TcpiProcCause, 16,8)
/* TcpiProcAckCause C 0 Reason for triggering TCPE to generate an ACK Each bit position corresponds to the occurrence of a particular reason since the bit was last cleared. Each bit position is cleared upon writing a '1' to to the bit position. 15: Not used 14: Not used 13: FIN received 12: End of window reached 11: normal ack of data 10: Window probe received 9: Duplicate payload 8: Packet loss */
#define GetTcpiProcAckCause(MEM) GetRegAt(MEM, TcpiProcCause, 8,8)
#define SetTcpiProcAckCause(MEM, VALUE) SetRegAt(MEM, VALUE, TcpiProcCause, 8,8)
/* TcpiProcSendCause C 0 Reason for triggering TCPE to send a segment Each bit position corresponds to the occurrence of a particular reason since the bit was last cleared. Each bit position is cleared upon writing a '1' to to the bit position. 7: Not used 6: Fast rexmit of first seg in next hole 5: Send FIN 4: Window update 3: All data is ACKed and more data is waiting 2: RTT updated 1: Fast rexmit of next seg in hole 0: Fast rexmit of first seg in first hole */
#define GetTcpiProcSendCause(MEM) GetRegAt(MEM, TcpiProcCause, 0,8)
#define SetTcpiProcSendCause(MEM, VALUE) SetRegAt(MEM, VALUE, TcpiProcCause, 0,8)

/* Ingress TCP timer control */
#define TcpiTimerCtrl                  0x0000051c
/* TcpiTimReXmitMax 128 Maximum value used when loading retransmit timer (tcb.TimReXmit) Time is given in units of SaTimFastUpdInterval and should be identical to SaTimReXmitMax */
#define GetTcpiTimReXmitMax(MEM) GetRegAt(MEM, TcpiTimerCtrl, 24,8)
#define SetTcpiTimReXmitMax(MEM, VALUE) SetRegAt(MEM, VALUE, TcpiTimerCtrl, 24,8)
/* TcpiTimReXmitMin 2 Minimum value used when loading retransmit timer (tcb.TimReXmit) Time is given in units of SaTimFastUpdInterval and should be identical to SaTimReXmitMin */
#define GetTcpiTimReXmitMin(MEM) GetRegAt(MEM, TcpiTimerCtrl, 16,8)
#define SetTcpiTimReXmitMin(MEM, VALUE) SetRegAt(MEM, VALUE, TcpiTimerCtrl, 16,8)
/* TcpiTimPersistMax 120 Maximum value used when loading persistence timer (tcb.TimPersist) Time is given in units of SaTimFastUpdInterval and should be identical to SaTimPersistMax */
#define GetTcpiTimPersistMax(MEM) GetRegAt(MEM, TcpiTimerCtrl, 8,8)
#define SetTcpiTimPersistMax(MEM, VALUE) SetRegAt(MEM, VALUE, TcpiTimerCtrl, 8,8)
/* TcpiTimPersistMin 10 Minimum value used when loading persistence timer (tcb.TimPersist) Time is given in units of SaTimFastUpdInterval and should be identical to SaTimPersistMin */
#define GetTcpiTimPersistMin(MEM) GetRegAt(MEM, TcpiTimerCtrl, 0,8)
#define SetTcpiTimPersistMin(MEM, VALUE) SetRegAt(MEM, VALUE, TcpiTimerCtrl, 0,8)


// Block SA
// ========
/* State Access timer control */
#define SaTimerCtrl0                   0x00000904 
/* SaTimEna 0 Enable TCP timer processing 0: TCP Timers are not processed. No TCB fields are modified for any connection. 1: TCP timers for connections residing on-chip are processed normally. The bits below selects which timers to process */
#define GetSaTimEna(MEM) GetRegAt(MEM, SaTimerCtrl0, 31,1)
#define SetSaTimEna(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl0, 31,1)
/* SaTimIdleEna 1 Enable processing of the idle timer in the TCBs */
#define GetSaTimIdleEna(MEM) GetRegAt(MEM, SaTimerCtrl0, 23,1)
#define SetSaTimIdleEna(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl0, 23,1)
/* SaTimReXmitEna 1 Enable processing of the retransmit timer in the TCBs */
#define GetSaTimReXmitEna(MEM) GetRegAt(MEM, SaTimerCtrl0, 21,1)
#define SetSaTimReXmitEna(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl0, 21,1)
/* SaTimKeepAliveEna 1 Enable processing of the keep alive timer in the TCBs */
#define GetSaTimKeepAliveEna(MEM) GetRegAt(MEM, SaTimerCtrl0, 20,1)
#define SetSaTimKeepAliveEna(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl0, 20,1)
/* SaTimPushEna 1 Enable processing of the push timer in the TCBs */
#define GetSaTimPushEna(MEM) GetRegAt(MEM, SaTimerCtrl0, 19,1)
#define SetSaTimPushEna(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl0, 19,1)
/* SaTimPersistEna 1 Enable processing of the persistence timer in the TCBs */
#define GetSaTimPersistEna(MEM) GetRegAt(MEM, SaTimerCtrl0, 18,1)
#define SetSaTimPersistEna(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl0, 18,1)
/* SaTimDelAckEna 1 Enable processing of delayed acknowledge timer in the TCBs */
#define GetSaTimDelAckEna(MEM) GetRegAt(MEM, SaTimerCtrl0, 17,1)
#define SetSaTimDelAckEna(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl0, 17,1)
/* SaTimReXmitShiftMax 12 Maximum value for Tcb.ReXmitShift. */
#define GetSaTimReXmitShiftMax(MEM) GetRegAt(MEM, SaTimerCtrl0, 12,4)
#define SetSaTimReXmitShiftMax(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl0, 12,4)
/* SaTimSlowUpdInterval 6 Interval between the slow counters are processed Time is given in units of SaTimFastUpdInterval n: 2^n*SaTimFastUpdInterval SaTimFastUpdInterval 5-4 RW 0 Interval between the fast counters are processed, i.e., the resolution of the TCP fast timers. 0: 100 ms 1: 12.5 ms 2: 1.5 ms 3: 100 us (for test only) */
#define GetSaTimSlowUpdInterval(MEM) GetRegAt(MEM, SaTimerCtrl0, 8,4)
#define SetSaTimSlowUpdInterval(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl0, 8,4)
/* SaTimFastUpdInterval 0 Interval between the fast counters are processed, i.e., the resolution of the TCP fast timers: 0: 100 ms 1: 12.5 ms 2: 1.5 ms 3: 100 us (for test only) */
#define GetSaTimFastUpdInterval(MEM) GetRegAt(MEM, SaTimerCtrl0, 4,2)
#define SetSaTimFastUpdInterval(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl0, 4,2)
/* SaTimUpdInterval 2 Interval between a TCB entry is processed by the timer update process. Time is specified in core clock cycles. Value must be smaller than the period specified by SaTimFastUpdInterval divided by the number of connections supported in on-chip TCB memory. 0: 32 core clocks (for test only) 1: 256 core clocks 2: 2048 core clocks 3: 16384 core clocks */
#define GetSaTimUpdInterval(MEM) GetRegAt(MEM, SaTimerCtrl0, 0,2)
#define SetSaTimUpdInterval(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl0, 0,2)

/* State Access timer control */
#define SaTimerCtrl1                   0x00000908 
/* SaTimRes 2500 Timer resolution. Resolution used for timer operation and time stamps. Must be set to 1us. Resolution is specified in units of CoreFreq 0: reserved n: n = (core clock cycles times 16) per us Example: Core clock is 156.25MHz, n must be set to 156.25 * 16. Or in other words n = 2500. */
#define GetSaTimRes(MEM) GetRegAt(MEM, SaTimerCtrl1, 0,28)
#define SetSaTimRes(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl1, 0,28)

/* State Access timer control */
#define SaTimerCtrl2                   0x0000090c 
/* SaTimReXmitMax 128 Maximum value used when loading retransmit timer (tcb.TimReXmit) Time is given in units of SaTimFastUpdInterval and should be identical to TcpiTimReXmitMax */
#define GetSaTimReXmitMax(MEM) GetRegAt(MEM, SaTimerCtrl2, 24,8)
#define SetSaTimReXmitMax(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl2, 24,8)
/* SaTimReXmitMin 2 Minimum value used when loading retransmit timer (tcb.TimReXmit) Time is given in units of SaTimFastUpdInterval and should be identical to TcpiTimReXmitMin */
#define GetSaTimReXmitMin(MEM) GetRegAt(MEM, SaTimerCtrl2, 16,8)
#define SetSaTimReXmitMin(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl2, 16,8)
/* SaTimPersistMax 120 Maximum value used when loading persistence */
#define GetSaTimPersistMax(MEM) GetRegAt(MEM, SaTimerCtrl2, 8,8)
#define SetSaTimPersistMax(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl2, 8,8)
/* SaTimPersistMin 10 Minimum value used when loading persistence timer (tcb.TimPersist) Time is given in units of SaTimFastUpdInterval and should be identical to TcpiTimPersistMin */
#define GetSaTimPersistMin(MEM) GetRegAt(MEM, SaTimerCtrl2, 0,8)
#define SetSaTimPersistMin(MEM, VALUE) SetRegAt(MEM, VALUE, SaTimerCtrl2, 0,8)

/* State Access status */
#define SaStatus                       0x00000910 
/* SaProcConnFillLvl 0 Current fill level of Process Connection FIFO. Number of process connection indications currently waiting to be applied to TCPE. Write '1' to clear bit. */
#define GetSaProcConnFillLvl(MEM) GetRegAt(MEM, SaStatus, 16,10)
/* SaProcConnOvflwDet 0 Overflow is detected in process connection fifo Fatal error. Should never occur. Write '1' to clear bit. */
#define GetSaProcConnOvflwDet(MEM) GetRegAt(MEM, SaStatus, 6,1)
#define SetSaProcConnOvflwDet(MEM, VALUE) SetRegAt(MEM, VALUE, SaStatus, 6,1)

/* SA TCB memory access command */
#define SaTcbAccCmd                    0x00000944 
/* SaTcbSelConnId 0 Select connection ID to access in the TCB memory. */
#define GetSaTcbSelConnId(MEM) GetRegAt(MEM, SaTcbAccCmd, 0,16)
#define SetSaTcbSelConnId(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccCmd, 0,16)

/* State Access debug TCB Access */
#define SaDbgTcbCtrl                   0x00000954 
/* SaTcbAccSglStepEna 0 Enable single step operation 0: Normal operation 1: Service of TCB requests are stopped when the client(s) indicated in SaTcbAccSglStepStop* has requested data SaTcbAccSglCnt times. */
#define GetSaTcbAccSglStepEna(MEM) GetRegAt(MEM, SaDbgTcbCtrl, 30,1)
#define SetSaTcbAccSglStepEna(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgTcbCtrl, 30,1)
/* SaTcbAccSglStepMode 0 Mode of single step operation 0: The request triggering the stop is serviced before stopping 1: The request triggering the stop is not serviced before stopping */
#define GetSaTcbAccSglStepMode(MEM) GetRegAt(MEM, SaDbgTcbCtrl, 29,1)
#define SetSaTcbAccSglStepMode(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgTcbCtrl, 29,1)
/* SaTcbAccSglStepOneShot 0 One shot restart of single step operation: 0: No operation 1: Resume servicing of TCB requests until stopped by trigger again. This bit is always 0 on read. */
#define SetSaTcbAccSglStepOneShot(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgTcbCtrl, 28,1)
/* SaTcbAccSglStepCnt 1 Single step trigger count 0: not used 1-4095: number of times trigger condition must occur before stopping */
#define GetSaTcbAccSglStepCnt(MEM) GetRegAt(MEM, SaDbgTcbCtrl, 16,12)
#define SetSaTcbAccSglStepCnt(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgTcbCtrl, 16,12)
/* SaLastClient 3 Last client TCB request serviced 0: TCPI 1: TCPE 2: SA Client 3: No requests serviced since reset */
#define GetSaLastClient(MEM) GetRegAt(MEM, SaDbgTcbCtrl, 12,2)
/* SaReqStateSa 0 Current state of SA TCB request 0: No request is present 1: TCB read request is present */
#define GetSaReqStateSa(MEM) GetRegAt(MEM, SaDbgTcbCtrl, 10,1)
/* SaReqStateTcpe 0 Current state of TCPE TCB request: 0: No request is present 1: TCB read request is present */
#define GetSaReqStateTcpe(MEM) GetRegAt(MEM, SaDbgTcbCtrl, 9,1)
/* SaReqStateTcpi 0 Current state of TCPI TCB request: 0: No request is present 1: TCB read request is present */
#define GetSaReqStateTcpi(MEM) GetRegAt(MEM, SaDbgTcbCtrl, 8,1)
/* SaTcbAccSaDis 0 Disable service of TCB requests from SA Client: 0: Normal operation 1: Requests are not serviced When disabled only requests from host (TCB reads and writes) are serviced. */
#define GetSaTcbAccSaDis(MEM) GetRegAt(MEM, SaDbgTcbCtrl, 6,1)
#define SetSaTcbAccSaDis(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgTcbCtrl, 6,1)
/* SaTcbAccTcpeDis 0 Disable service of TCB requests from TCPE. 0: Normal operation 1: Requests are not serviced */
#define GetSaTcbAccTcpeDis(MEM) GetRegAt(MEM, SaDbgTcbCtrl, 5,1)
#define SetSaTcbAccTcpeDis(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgTcbCtrl, 5,1)
/* SaTcbAccTcpiDis 0 Disable service of TCB requests from TCPI. 0: Normal operation 1: Requests are not serviced */
#define GetSaTcbAccTcpiDis(MEM) GetRegAt(MEM, SaDbgTcbCtrl, 4,1)
#define SetSaTcbAccTcpiDis(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgTcbCtrl, 4,1)
/* SaTcbAccSglStepStopSa 0 Enable SA Client being part of trigger 0: disable 1: enable */
#define GetSaTcbAccSglStepStopSa(MEM) GetRegAt(MEM, SaDbgTcbCtrl, 2,1)
#define SetSaTcbAccSglStepStopSa(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgTcbCtrl, 2,1)
/* SaTcbAccSglStepStopTcpe 0 Enable TCPE being part of trigger 0: disable 1: enable */
#define GetSaTcbAccSglStepStopTcpe(MEM) GetRegAt(MEM, SaDbgTcbCtrl, 1,1)
#define SetSaTcbAccSglStepStopTcpe(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgTcbCtrl, 1,1)
/* SaTcbAccSglStepStopTcpi 0 Enable TCPI being part of trigger 0: disable 1: enable */
#define GetSaTcbAccSglStepStopTcpi(MEM) GetRegAt(MEM, SaDbgTcbCtrl, 0,1)
#define SetSaTcbAccSglStepStopTcpi(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgTcbCtrl, 0,1)

/* Process Connection debug */
#define SaDbgProcConn                  0x00000958 
/* SaProcConnClr 0 One shot clear of waiting Tx Proc Conn indication: 0: normal operation 1: clear waiting Tx Proc Conn indication Bit is always 0 on read. Should only be used when SaTcbProcConnDis is asserted. */
#define SetSaProcConnClr(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgProcConn, 31,1)
/* SaProcConnStatRdy 0 Tx Proc Conn indication ready for TCPE. 0: no indication is ready 1: indication is ready (TCPE can only see it if SaTcbProcConnDis is de-asserted) */
#define GetSaProcConnStatRdy(MEM) GetRegAt(MEM, SaDbgProcConn, 30,1)
/* SaProcConnStatId 0 Connection ID of next Tx Proc Conn indication to be generated to TCPE. */
#define GetSaProcConnStatId(MEM) GetRegAt(MEM, SaDbgProcConn, 16,12)
/* SaProcConnDis 0 Disable normal Tx Proc Conn generation. 0: normal operation 1: Only one-shot process connection indications are generated. High priority requests are just acknowledged (i.e., the timer- and event processing blocks are not stalled) */
#define GetSaProcConnDis(MEM) GetRegAt(MEM, SaDbgProcConn, 15,1)
#define SetSaProcConnDis(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgProcConn, 15,1)
/* SaProcConnOneShot 0 One shot generation of Tx Process Connection: 0: No operation 1: Generate a Tx Proc Conn for the connection identified by SaTcbProcConnId Can be used regardless of SaTcbProcConnDis. Bit is always 0 on read. */
#define SetSaProcConnOneShot(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgProcConn, 14,1)
#define GetSaProcConnOneShot(MEM) GetRegAt(MEM, SaDbgProcConn, 14,1)
/* SaProcConnId 0 Connection ID to be used in single shot of Tx Process Connection. */
#define GetSaProcConnId(MEM) GetRegAt(MEM, SaDbgProcConn, 0,12)
#define SetSaProcConnId(MEM, VALUE) SetRegAt(MEM, VALUE, SaDbgProcConn, 0,12)

/* Timer tick counter used for timestamping */
#define SaTimerTickCnt                 0x0000096c 
/* SaTimerTickCnt 0 Timer Tick counter used for time stamp options */
#define GetSaTimerTickCnt(MEM) GetRegAt(MEM, SaTimerTickCnt, 0,32)

/* SA TCB memory indirect access data */
/* SaTcbAccData 0 TCB Data word read or written from TCB memory. The lowest address corresponds to LS 32-bit word of TCB record and the highest address corresponds to MS 32-bit word of TCB. The connection to access is selected by SaTcbSelConnId. Note, the TCB is read and written on the fly, i.e., the TCB may be modified by the core in between reads and writes of individual words. */
#define SaTcbAccData0                  0x00000980 /* SA TCB memory indirect access data 0 */
#define GetSaTcbAccData0(MEM) GetRegAt(MEM, SaTcbAccData0, 0, 32)
#define SetSaTcbAccData0(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData0, 0,32)
#define SaTcbAccData1                  0x00000984 /* SA TCB memory indirect access data 1 */
#define GetSaTcbAccData1(MEM) GetRegAt(MEM, SaTcbAccData1, 0, 32)
#define SetSaTcbAccData1(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData1, 0,32)
#define SaTcbAccData2                  0x00000988 /* SA TCB memory indirect access data 2 */
#define GetSaTcbAccData2(MEM) GetRegAt(MEM, SaTcbAccData2, 0, 32)
#define SetSaTcbAccData2(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData2, 0,32)
#define SaTcbAccData3                  0x0000098C /* SA TCB memory indirect access data 3 */
#define GetSaTcbAccData3(MEM) GetRegAt(MEM, SaTcbAccData3, 0, 32)
#define SetSaTcbAccData3(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData3, 0,32)
#define SaTcbAccData4                  0x00000990 /* SA TCB memory indirect access data 4 */
#define GetSaTcbAccData4(MEM) GetRegAt(MEM, SaTcbAccData4, 0, 32)
#define SetSaTcbAccData4(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData4, 0,32)
#define SaTcbAccData5                  0x00000994 /* SA TCB memory indirect access data 5 */
#define GetSaTcbAccData5(MEM) GetRegAt(MEM, SaTcbAccData5, 0, 32)
#define SetSaTcbAccData5(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData5, 0,32)
#define SaTcbAccData6                  0x00000998 /* SA TCB memory indirect access data 6 */
#define GetSaTcbAccData6(MEM) GetRegAt(MEM, SaTcbAccData6, 0, 32)
#define SetSaTcbAccData6(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData6, 0,32)
#define SaTcbAccData7                  0x0000099C /* SA TCB memory indirect access data 7 */
#define GetSaTcbAccData7(MEM) GetRegAt(MEM, SaTcbAccData7, 0, 32)
#define SetSaTcbAccData7(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData7, 0,32)
#define SaTcbAccData8                  0x000009A0 /* SA TCB memory indirect access data 8 */
#define GetSaTcbAccData8(MEM) GetRegAt(MEM, SaTcbAccData8, 0, 32)
#define SetSaTcbAccData8(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData8, 0,32)
#define SaTcbAccData9                  0x000009A4 /* SA TCB memory indirect access data 9 */
#define GetSaTcbAccData9(MEM) GetRegAt(MEM, SaTcbAccData9, 0, 32)
#define SetSaTcbAccData9(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData9, 0,32)
#define SaTcbAccData10                  0x000009A8 /* SA TCB memory indirect access data 10 */
#define GetSaTcbAccData10(MEM) GetRegAt(MEM, SaTcbAccData10, 0, 32)
#define SetSaTcbAccData10(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData10, 0,32)
#define SaTcbAccData11                  0x000009AC /* SA TCB memory indirect access data 11 */
#define GetSaTcbAccData11(MEM) GetRegAt(MEM, SaTcbAccData11, 0, 32)
#define SetSaTcbAccData11(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData11, 0,32)
#define SaTcbAccData12                  0x000009B0 /* SA TCB memory indirect access data 12 */
#define GetSaTcbAccData12(MEM) GetRegAt(MEM, SaTcbAccData12, 0, 32)
#define SetSaTcbAccData12(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData12, 0,32)
#define SaTcbAccData13                  0x000009B4 /* SA TCB memory indirect access data 13 */
#define GetSaTcbAccData13(MEM) GetRegAt(MEM, SaTcbAccData13, 0, 32)
#define SetSaTcbAccData13(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData13, 0,32)
#define SaTcbAccData14                  0x000009B8 /* SA TCB memory indirect access data 14 */
#define GetSaTcbAccData14(MEM) GetRegAt(MEM, SaTcbAccData14, 0, 32)
#define SetSaTcbAccData14(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData14, 0,32)
#define SaTcbAccData15                  0x000009BC /* SA TCB memory indirect access data 15 */
#define GetSaTcbAccData15(MEM) GetRegAt(MEM, SaTcbAccData15, 0, 32)
#define SetSaTcbAccData15(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData15, 0,32)
#define SaTcbAccData16                  0x000009C0 /* SA TCB memory indirect access data 16 */
#define GetSaTcbAccData16(MEM) GetRegAt(MEM, SaTcbAccData16, 0, 32)
#define SetSaTcbAccData16(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData16, 0,32)
#define SaTcbAccData17                  0x000009C4 /* SA TCB memory indirect access data 17 */
#define GetSaTcbAccData17(MEM) GetRegAt(MEM, SaTcbAccData17, 0, 32)
#define SetSaTcbAccData17(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData17, 0,32)
#define SaTcbAccData18                  0x000009C8 /* SA TCB memory indirect access data 18 */
#define GetSaTcbAccData18(MEM) GetRegAt(MEM, SaTcbAccData18, 0, 32)
#define SetSaTcbAccData18(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData18, 0,32)
#define SaTcbAccData19                  0x000009CC /* SA TCB memory indirect access data 19 */
#define GetSaTcbAccData19(MEM) GetRegAt(MEM, SaTcbAccData19, 0, 32)
#define SetSaTcbAccData19(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData19, 0,32)
#define SaTcbAccData20                  0x000009D0 /* SA TCB memory indirect access data 20 */
#define GetSaTcbAccData20(MEM) GetRegAt(MEM, SaTcbAccData20, 0, 32)
#define SetSaTcbAccData20(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData20, 0,32)
#define SaTcbAccData21                  0x000009D4 /* SA TCB memory indirect access data 21 */
#define GetSaTcbAccData21(MEM) GetRegAt(MEM, SaTcbAccData21, 0, 32)
#define SetSaTcbAccData21(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData21, 0,32)
#define SaTcbAccData22                  0x000009D8 /* SA TCB memory indirect access data 22 */
#define GetSaTcbAccData22(MEM) GetRegAt(MEM, SaTcbAccData22, 0, 32)
#define SetSaTcbAccData22(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData22, 0,32)
#define SaTcbAccData23                  0x000009DC /* SA TCB memory indirect access data 23 */
#define GetSaTcbAccData23(MEM) GetRegAt(MEM, SaTcbAccData23, 0, 32)
#define SetSaTcbAccData23(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData23, 0,32)
#define SaTcbAccData24                  0x000009E0 /* SA TCB memory indirect access data 24 */
#define GetSaTcbAccData24(MEM) GetRegAt(MEM, SaTcbAccData24, 0, 32)
#define SetSaTcbAccData24(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData24, 0,32)
#define SaTcbAccData25                  0x000009E4 /* SA TCB memory indirect access data 25 */
#define GetSaTcbAccData25(MEM) GetRegAt(MEM, SaTcbAccData25, 0, 32)
#define SetSaTcbAccData25(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData25, 0,32)
#define SaTcbAccData26                  0x000009E8 /* SA TCB memory indirect access data 26 */
#define GetSaTcbAccData26(MEM) GetRegAt(MEM, SaTcbAccData26, 0, 32)
#define SetSaTcbAccData26(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData26, 0,32)
#define SaTcbAccData27                  0x000009EC /* SA TCB memory indirect access data 27 */
#define GetSaTcbAccData27(MEM) GetRegAt(MEM, SaTcbAccData27, 0, 32)
#define SetSaTcbAccData27(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData27, 0,32)
#define SaTcbAccData28                  0x000009F0 /* SA TCB memory indirect access data 28 */
#define GetSaTcbAccData28(MEM) GetRegAt(MEM, SaTcbAccData28, 0, 32)
#define SetSaTcbAccData28(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData28, 0,32)
#define SaTcbAccData29                  0x000009F4 /* SA TCB memory indirect access data 29 */
#define GetSaTcbAccData29(MEM) GetRegAt(MEM, SaTcbAccData29, 0, 32)
#define SetSaTcbAccData29(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData29, 0,32)
#define SaTcbAccData30                  0x000009F8 /* SA TCB memory indirect access data 30 */
#define GetSaTcbAccData30(MEM) GetRegAt(MEM, SaTcbAccData30, 0, 32)
#define SetSaTcbAccData30(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData30, 0,32)
#define SaTcbAccData31                  0x000009FC /* SA TCB memory indirect access data 31 */
#define GetSaTcbAccData31(MEM) GetRegAt(MEM, SaTcbAccData31, 0, 32)
#define SetSaTcbAccData31(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData31, 0,32)
#define SaTcbAccData32                  0x00000A00 /* SA TCB memory indirect access data 32 */
#define GetSaTcbAccData32(MEM) GetRegAt(MEM, SaTcbAccData32, 0, 32)
#define SetSaTcbAccData32(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData32, 0,32)
#define SaTcbAccData33                  0x00000A04 /* SA TCB memory indirect access data 33 */
#define GetSaTcbAccData33(MEM) GetRegAt(MEM, SaTcbAccData33, 0, 32)
#define SetSaTcbAccData33(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData33, 0,32)

#if 0
#define SaTcbReg7                      0x0000099c
/* Cmd bit from timer to TCPE: Enter slow start */
#define GetTcbEnterSlowStart(MEM) GetRegAt(MEM, SaTcbReg7,31,1)
#define SetTcbEnterSlowStart(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg7,31,1)
/* Send ACK immediately */
#define GetTcbFlAckNow(MEM) GetRegAt(MEM, SaTcbReg7,30,1)
#define SetTcbFlAckNow(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg7,30,1)
/* Send 1 byte window probe */
#define GetTcbFlSndForce(MEM) GetRegAt(MEM, SaTcbReg7,29,1)
#define SetTcbFlSndForce(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg7,29,1)
/* Host is owner of connection */
#define GetTcbOwnHost(MEM) GetRegAt(MEM, SaTcbReg7,28,1)
#define SetTcbOwnHost(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg7,28,1)
/* Current number of outstanding process connection requests */
#define GetTcbProcConnCnt(MEM) GetRegAt(MEM, SaTcbReg7,26,2)
#define SetTcbProcConnCnt(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg7,26,2)
/* Max number of outstanding process conn requests allowed (0: disable)*/
#define GetTcbProcConnCntMax(MEM) GetRegAt(MEM, SaTcbReg7,24,2)
#define SetTcbProcConnCntMax(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg7,24,2)
/* log(2) of retransmit exponential backoff with upper limit */
#define GetTcbReXmitShift(MEM) GetRegAt(MEM, SaTcbReg7,20,4)
#define SetTcbReXmitShift(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg7,20,4)
/* Delayed ACK timer */
#define GetTcbTimDelAck(MEM) GetRegAt(MEM, SaTcbReg7,18,2)
#define SetTcbTimDelAck(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg7,18,2)
/* Push timer (to push data to the application) */
#define GetTcbTimPush(MEM) GetRegAt(MEM, SaTcbReg7,16,2)
#define SetTcbTimPush(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg7,16,2)
/* Persist timer */
#define GetTcbTimPersist(MEM) GetRegAt(MEM, SaTcbReg7,8,8)
#define SetTcbTimPersist(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg7,8,8)
/* Idle time - This counter is increasing up to max */
#define GetTcbTimIdle(MEM) GetRegAt(MEM, SaTcbReg7,0,8)
#define SetTcbTimIdle(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg7,0,8)
#endif

#define GetTcbTimReXmit(MEM) GetRegAt(MEM, SaTcbAccData5, 24, 8)
#define SetTcbTimReXmit(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData5, 24, 8)
#define GetTcbFlSndRst(CON, MEM) GetConRegAt(CON, MEM, SaTcbAccData6, 23, 1)
#define SetTcbFlSndRst(CON, MEM, VALUE) SetConRegAt(CON, MEM, VALUE, SaTcbAccData6, 23, 1)
#define GetTcbFlAckNow(MEM) GetRegAt(MEM, SaTcbAccData7, 30, 1)
#define SetTcbFlAckNow(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData7, 30, 1)
#define GetTcbState(CON, MEM) GetConRegAt(CON, MEM, SaTcbAccData9, 24, 4)
#define SetTcbState(CON, MEM, VALUE) SetConRegAt(CON, MEM, VALUE, SaTcbAccData9, 24, 4)
#define GetTcbL23RemoteMacAddrLow(CON, MEM) GetRegAt(MEM, SaTcbAccData26, 0, 32)
#define SetTcbL23RemoteMacAddrLow(CON, MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData26, 0, 32)
#define GetTcbL23RemoteMacAddrHigh(CON, MEM) GetConRegAt(CON, MEM, SaTcbAccData27, 0, 16)
#define SetTcbL23RemoteMacAddrHigh(CON, MEM, VALUE) SetConRegAt(CON, MEM, VALUE, SaTcbAccData27, 0, 16)
#define GetTcbL23RemoteIp(CON, MEM) GetConRegAt(CON, MEM, SaTcbAccData28, 0, 32)
#define SetTcbL23RemoteIp(CON, MEM, VALUE) SetConRegAt(CON, MEM, VALUE, SaTcbAccData28, 0, 32)
#define GetTcbL23LocalPort(CON, MEM) GetConRegAt(CON, MEM, SaTcbAccData30, 0, 16)
#define SetTcbL23LocalPort(CON, MEM, VALUE) SetConRegAt(CON, MEM, VALUE, SaTcbAccData30, 0, 16)
#define GetTcbL23RemotePort(CON, MEM) GetConRegAt(CON, MEM, SaTcbAccData30, 16, 16)
#define SetTcbL23RemotePort(CON, MEM, VALUE) SetConRegAt(CON, MEM, VALUE, SaTcbAccData30, 16, 16)
#define GetTcbFlSndFinReqSw(CON, MEM) GetConRegAt(CON, MEM, SaTcbAccData31, 19, 1)
#define SetTcbFlSndFinReqSw(CON, MEM, VALUE) SetConRegAt(CON, MEM, VALUE, SaTcbAccData31, 19, 1)
#define GetTcbMaxSegSize(MEM) GetRegAt(MEM, SaTcbAccData31, 0, 16)
#define SetTcbMaxSegSize(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbAccData31, 0, 16)

#define TCB_STATE_CLOSED 0
#define TCB_STATE_LISTEN 1
#define TCB_STATE_SYN_SENT 2
#define TCB_STATE_SYN_RECEIVED 3
#define TCB_STATE_ESTABLISHED 4
#define TCB_STATE_CLOSE_WAIT 5
#define TCB_STATE_FIN_WAIT 6
#define TCB_STATE_CLOSING 7
#define TCB_STATE_LAST_ACK 8
#define TCB_STATE_FIN_WAIT_2 9
#define TCB_STATE_TIME_WAIT 10
#define TCB_STATE_CLEAR_TCB 11
#define TCB_STATE_CLOSED_DATA_PENDING 13

#if 0
#define SaTcbReg9                      0x000009a4
/* FIN is sent by /TCPE */
#define GetTcbFlSndFinDone(MEM) GetRegAt(MEM, SaTcbReg9,28,1)
#define SetTcbFlSndFinDone(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg9,28,1)

/* The TCP protocol state values */
#define GetTcbState(MEM) GetRegAt(MEM, SaTcbReg9,24,4)
#define SetTcbState(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg9,24,4)

/* Congestion control window */
#define GetTcbSndCongWin(MEM) GetRegAt(MEM, SaTcbReg9,0,24)
#define SetTcbSndCongWin(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg9,0,24)


#define SaTcbReg23                     0x000009dc
/* Request the TOE to send a FIN (close the transmit direction) after all data is transmitted. */
#define GetTcbFlSndFinReqSw(MEM) GetRegAt(MEM, SaTcbReg23,31,1)
#define SetTcbFlSndFinReqSw(MEM, VALUE) SetRegAt(MEM, VALUE, SaTcbReg23,31,1)
#endif

// Block ETLM
// ==========
/* The LSBs of the source MAC address */
#define EtlmLocMacAddrLsb              0x00001000
/* EtlmLocMacAddrLsb 0x010020 The 24 LSBs of source MAC address for connections. */
#define GetEtlmLocMacAddrLsb(MEM) GetRegAt(MEM, EtlmLocMacAddrLsb, 0,24)
#define SetEtlmLocMacAddrLsb(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmLocMacAddrLsb, 0,24)

/* The MSBs of the source MAC address */
#define EtlmLocMacAddrMsb              0x00001004
/* EtlmLocMacAddrMsb 0x04dd4c The 24 MSBs of source MAC address for connections. */
#define GetEtlmLocMacAddrMsb(MEM) GetRegAt(MEM, EtlmLocMacAddrMsb, 0,24)
#define SetEtlmLocMacAddrMsb(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmLocMacAddrMsb, 0,24)

/* The source IP address */
#define EtlmLocIpAddr                  0x00001008
/* EtlmLocIpAddr 0x0a0a0a0a Source IP address to be used for the connections. */
#define GetEtlmLocIpAddr(MEM) GetRegAt(MEM, EtlmLocIpAddr, 0,32)
#define SetEtlmLocIpAddr(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmLocIpAddr, 0,32)

/* Core identification for the ETLM */
#define EtlmCoreId                     0x00001010
/* EtlmCoreType - Core type ID */
#define GetEtlmCoreType(MEM) GetRegAt(MEM, EtlmCoreId, 24,8)
#define SetEtlmCoreType(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmCoreId, 24,8)
/* EtlmCoreMajorVers - Core major version */
#define GetEtlmCoreMajorVers(MEM) GetRegAt(MEM, EtlmCoreId, 16,8)
#define SetEtlmCoreMajorVers(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmCoreId, 16,8)
/* Etlm CoreMinorVers - Core minor version */
#define GetEtlmCoreMinorVers(MEM) GetRegAt(MEM, EtlmCoreId, 0,16)

// Block ETLM_RX
// =============
/* Cfg of ETLM Rx Side */
#define EtlmRxCtrl                     0x00001100
/* EtlmEvNewBytes 0 Select the what sould be presented on fifo_rx_ev_fill_o */
#define GetEtlmEvNewBytes(MEM) GetRegAt(MEM, EtlmRxCtrl, 3,1)
#define SetEtlmEvNewBytes(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmRxCtrl, 3,1)
/* EtlmInbandFcsEn 1 Set to 1 if MAC delivers inband FCS */
#define GetEtlmInbandFcsEn(MEM) GetRegAt(MEM, EtlmRxCtrl, 2,1)
#define SetEtlmInbandFcsEn(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmRxCtrl, 2,1)
/* EtlmDisRxCsum 1 Disable Rx checksum checks */
#define GetEtlmDisRxCsum(MEM) GetRegAt(MEM, EtlmRxCtrl, 1,1)
#define SetEtlmDisRxCsum(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmRxCtrl, 1,1)
/* EtlmDisArpReply 0 Do not reply to ARP request, rather let them pass to the upper MAC interface. */
#define GetEtlmDisArpReply(MEM) GetRegAt(MEM, EtlmRxCtrl, 0,1)
#define SetEtlmDisArpReply(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmRxCtrl, 0,1)

/* Cfg of ETLM Rx Window */
#define EtlmRxWinCtrlConnId            0x00001104
/* EtlmRxWinCtrlConnId 0 Connection ID to update window settings for. */
#define SetEtlmRxWinCtrlConnId(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmRxWinCtrlConnId, 0,32)
#define GetEtlmRxWinCtrlConnId(MEM) GetRegAt(MEM, EtlmRxWinCtrlConnId, 0,32)

/* Cfg of ETLM Rx Window */
#define EtlmRxWinStartAddr             0x00001108
/* EtlmRxWinStartAddr 0 The start address (byte address) for the Rx window for the connection specified in EtlmRxWinCtrlConnId. */
#define SetEtlmRxWinStartAddr(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmRxWinStartAddr, 0,32)
#define GetEtlmRxWinStartAddr(MEM) GetRegAt(MEM, EtlmRxWinStartAddr, 0,32)

/* Cfg of ETLM Rx Window */
#define EtlmRxWinSize                  0x0000110c
/* EtlmRxWinSize 0 The size, in bytes, of the Rx window for the connection specified in EtlmRxWinCtrlConnId. - Size must be a power of 2 - The minimum size supported is 512 bytes. - The maximum size currently supported is 0xffff bytes! */
#define SetEtlmRxWinSize(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmRxWinSize, 0,32)
#define GetEtlmRxWinSize(MEM) GetRegAt(MEM, EtlmRxWinSize, 0,32)

/* Status from the Rx Input Block */
#define EtlmRxInputStats               0x000011a0
/* EtlmRxInputFillLvl 0 Rx input FIFO fill level. */
#define GetEtlmRxInputFillLvl(MEM) GetRegAt(MEM, EtlmRxInputStats, 1,8)
/* EtlmRxInputEmpty 0 Empty flag from the Rx input FIFO. */
#define GetEtlmRxInputEmpty(MEM) GetRegAt(MEM, EtlmRxInputStats, 0,1)

/* Status from the Rx Input Block */
#define EtlmRxInputCntFull             0x000011a4
/* EtlmRxInputCntFull 0 Rx input FIFO has been full this many times. */
#define GetEtlmRxInputCntFull(MEM) GetRegAt(MEM, EtlmRxInputCntFull, 0,32)

/* Status from the Rx Input Block */
#define EtlmRxInputCntOvflw            0x000011a8
/* EtlmRxInputCntOvflw 0 Rx input FIFO has overflowed this many times. */
#define GetEtlmRxInputCntOvflw(MEM) GetRegAt(MEM, EtlmRxInputCntOvflw, 0,32)

/* Status from the Rx Input Block */
#define EtlmRxInputCntTrunc            0x000011ac
/* EtlmRxInputCntTrunc 0 This many packages was truncated in the Rx input FIFO due to the FIFO being almost full. */
#define GetEtlmRxInputCntTrunc(MEM) GetRegAt(MEM, EtlmRxInputCntTrunc, 0,32)

/* Status from the Rx Input Block */
#define EtlmRxInputCntDiscd            0x000011b0
/* EtlmRxInputCntDiscd 0 This many packages was discarded in the Rx input FIFO due to the FIFO being almost full. */
#define GetEtlmRxInputCntDiscd(MEM) GetRegAt(MEM, EtlmRxInputCntDiscd, 0,32)

/* Number of incoming packets matched by the match unit */
#define EtlmRxMatchUnitPktCount        0x000011b4
/* EtlmRxMatchUnitPktCount 0 Counter for matching packets. */
#define GetEtlmRxMatchUnitPktCount(MEM) GetRegAt(MEM, EtlmRxMatchUnitPktCount, 0,32)

/* Status Register for the FIFO controller in ETLM Rx - Read Sequence Nr. */
#define EtlmRxFifoCtrlReadSeqNr        0x000011b8
/* EtlmRxFifoCtrlReadSeqNr 0 Read the Read Sequence nr. */
#define GetEtlmRxFifoCtrlReadSeqNr(MEM) GetRegAt(MEM, EtlmRxFifoCtrlReadSeqNr, 0,32)

/* Status Register for the FIFO controller in ETLM Rx - Receive Sequence Off. */
#define EtlmRxFifoCtrlRcvSeqOff        0x000011bc
/* EtlmRxFifoCtrlRcvSeqOff 0 Read the Receive Sequence Offset. */
#define GetEtlmRxFifoCtrlRcvSeqOff(MEM) GetRegAt(MEM, EtlmRxFifoCtrlRcvSeqOff, 0,32)

// Block ETLM_TX
// =============
/* Cfg of ETLM Tx Side */
#define EtlmTxCtrl                     0x00001200
/* EtlmTxCfgBit 0 Dummy - Not used. */
#define GetEtlmTxCfgBit(MEM) GetRegAt(MEM, EtlmTxCtrl, 0,1)
#define SetEtlmTxCfgBit(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmTxCtrl, 0,1)

/* Cfg of ETLM Tx Window */
#define EtlmTxWinCtrlConnId            0x00001204
/* EtlmTxWinCtrlConnId 0 Connection ID to update window settings for. */
#define SetEtlmTxWinCtrlConnId(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmTxWinCtrlConnId, 0,32)
#define GetEtlmTxWinCtrlConnId(MEM) GetRegAt(MEM, EtlmTxWinCtrlConnId, 0,32)

/* Cfg of ETLM Tx Window */
#define EtlmTxWinStartAddr             0x00001208
/* EtlmTxWinStartAddr 0 The start address (byte address) for the Tx window for the connection specified in EtlmTxWinCtrlConnId. */
#define SetEtlmTxWinStartAddr(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmTxWinStartAddr, 0,32)
#define GetEtlmTxWinStartAddr(MEM) GetRegAt(MEM, EtlmTxWinStartAddr, 0,32)

/* Cfg of ETLM Tx Window */
#define EtlmTxWinSize                  0x0000120c
/* EtlmTxWinSize 0 The size, in bytes, of the Tx window for the connection specified in EtlmTxWinCtrlConnId. - Size must be a power of 2 - The minimum size supported is 512 bytes. - The maximum size currently supported is 0xffff bytes! */
#define GetEtlmTxWinSize(MEM) GetRegAt(MEM, EtlmTxWinSize, 0, 32)
#define SetEtlmTxWinSize(MEM, VALUE) GetRegAt(MEM, VALUE, EtlmTxWinSize, 0,32)

/* Configuration bits for the FIFO controller in ETLM Tx. */
#define EtlmTxFifoCtrl                 0x00001210
/* EtlmTxFifoCtrlDisTimeoutFlush 0 Disable timeout flush of the Tx Windows. */
#define GetEtlmTxFifoCtrlDisTimeoutFlush(MEM) GetRegAt(MEM, EtlmTxFifoCtrl, 1,1)
#define SetEtlmTxFifoCtrlDisTimeoutFlush(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmTxFifoCtrl, 1,1)
/* EtlmTxFifoCtrlDisFifo2kLvlFlush 0 Disable 2k level flush of the Tx Windows. */
#define GetEtlmTxFifoCtrlDisFifo2kLvlFlush(MEM) GetRegAt(MEM, EtlmTxFifoCtrl, 0,1)
#define SetEtlmTxFifoCtrlDisFifo2kLvlFlush(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmTxFifoCtrl, 0,1)

/* Configuration bits for the FIFO controller in ETLM Tx - Timeout period. */
#define EtlmTxFifoCtrlTimeoutPeriod    0x00001214
/* EtlmTxFifoCtrlTimeoutPeriod 0 Setup timeout period for flushing of the Tx windows. Unit is clock cycles of the mac_clk_i. */
#define GetEtlmTxFifoCtrlTimeoutPeriod(MEM) GetRegAt(MEM, EtlmTxFifoCtrlTimeoutPeriod, 0,32)
#define SetEtlmTxFifoCtrlTimeoutPeriod(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmTxFifoCtrlTimeoutPeriod, 0,32)

/* Configuration bits for the FIFO controller in ETLM Tx - Almost Full Treshold. */
#define EtlmTxFifoCtrlAlmostFullThresh 0x00001218
/* EtlmTxFifoCtrlAlmostFullThresh 0 Setup almost full threshold for the Tx FIFOs. Unit is bytes and value represents bytes free when almost full is asserted. This is a common value for all Tx FIFOs */
#define GetEtlmTxFifoCtrlAlmostFullThresh(MEM) GetRegAt(MEM, EtlmTxFifoCtrlAlmostFullThresh, 0,32)
#define SetEtlmTxFifoCtrlAlmostFullThresh(MEM, VALUE) SetRegAt(MEM, VALUE, EtlmTxFifoCtrlAlmostFullThresh, 0,32)

/* Status register for Tx packet count from the MAC arbiter. */
#define EtlmTxStatTxPck                0x000012a0
/* EtlmTxStatTxPck - Number of transmitted packets from the MAC arbiter */
#define GetEtlmTxStatTxPck(MEM) GetRegAt(MEM, EtlmTxStatTxPck, 0,32)

/* Status register for Tx Build Packet. */
#define EtlmTxStatBuildPkt             0x000012a4
/* EtlmTxStatBuildPktIpId - Id field of the last IP hdr. */
#define GetEtlmTxStatBuildPktIpId(MEM) GetRegAt(MEM, EtlmTxStatBuildPkt, 16,16)
/* EtlmTxStatBuildPktBytC - Current position in frame being build. */
#define GetEtlmTxStatBuildPktBytC(MEM) GetRegAt(MEM, EtlmTxStatBuildPkt, 4,12)
/* EtlmTxStatBuildPktPckTx - Not used - Always 0 for ETLM. */
#define GetEtlmTxStatBuildPktPckTx(MEM) GetRegAt(MEM, EtlmTxStatBuildPkt, 0,4)

/* Status register for Tx Buffer Read. */
#define EtlmTxStatBufRd                0x000012a8
/* EtlmTxStatBufRdAssertSizeFifo - Size FIFO has overflowed. */
#define GetEtlmTxStatBufRdAssertSizeFifo(MEM) GetRegAt(MEM, EtlmTxStatBufRd, 2,1)
/* EtlmTxStatBufRdAssertDataFifo - Data FIFO has overflowed. */
#define GetEtlmTxStatBufRdAssertDataFifo(MEM) GetRegAt(MEM, EtlmTxStatBufRd, 1,1)
/* EtlmTxStatBufRdAssertDataRdFifo - Read FIFO was empty when a read attempt was done. */
#define GetEtlmTxStatBufRdAssertDataRdFifo(MEM) GetRegAt(MEM, EtlmTxStatBufRd, 0,1)

/* Status Register for the FIFO controller in ETLM Tx - Write Sequence Nr. */
#define EtlmTxFifoCtrlWriteSeqNr       0x000012ac
/* EtlmTxFifoCtrlWriteSeqNr - Read the write sequence nr. */
#define GetEtlmTxFifoCtrlWriteSeqNr(MEM) GetRegAt(MEM, EtlmTxFifoCtrlWriteSeqNr, 0,32)

/* Status Register for the FIFO controller in ETLM Tx - Send Sequence Offset. */
#define EtlmTxFifoCtrlSndSeqOff        0x000012b0
/* EtlmTxFifoCtrlSndSeqOff - Read the Send Sequence Offset. */
#define GetEtlmTxFifoCtrlSndSeqOff(MEM) GetRegAt(MEM, EtlmTxFifoCtrlSndSeqOff, 0,32)
