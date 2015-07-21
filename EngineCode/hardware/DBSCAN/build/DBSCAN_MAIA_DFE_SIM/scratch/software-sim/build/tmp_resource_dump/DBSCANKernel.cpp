#include "stdsimheader.h"
#include "DBSCANKernel.h"

namespace maxcompilersim {

DBSCANKernel::DBSCANKernel(const std::string &instance_name) : 
  ManagerBlockSync(instance_name),
  KernelManagerBlockSync(instance_name, 4248, 2, 0, 0, "",1)
, c_hw_fix_1_0_uns_bits((HWOffsetFix<1,0,UNSIGNED>(varint_u<1>(0x0l))))
, c_hw_flt_8_24_undef((HWFloat<8,24>()))
, c_hw_fix_1_0_uns_bits_1((HWOffsetFix<1,0,UNSIGNED>(varint_u<1>(0x1l))))
, c_hw_fix_49_0_uns_bits((HWOffsetFix<49,0,UNSIGNED>(varint_u<49>(0x1000000000000l))))
, c_hw_fix_49_0_uns_bits_1((HWOffsetFix<49,0,UNSIGNED>(varint_u<49>(0x0000000000000l))))
, c_hw_fix_49_0_uns_bits_2((HWOffsetFix<49,0,UNSIGNED>(varint_u<49>(0x0000000000001l))))
{
  { // Node ID: 1006 (NodeInputMappedReg)
    registerMappedRegister("io_output_force_disabled", Data(1));
  }
  { // Node ID: 0 (NodeInputMappedReg)
    registerMappedRegister("io_input_force_disabled", Data(1));
  }
  { // Node ID: 2 (NodeInput)
     m_input =  registerInput("input",0,5);
  }
  { // Node ID: 1012 (NodeOutput)
    m_output = registerOutput("output",0 );
  }
  { // Node ID: 1017 (NodeConstantRawBits)
    id1017out_value = (c_hw_fix_1_0_uns_bits_1);
  }
  { // Node ID: 1761 (NodeConstantRawBits)
    id1761out_value = (c_hw_fix_1_0_uns_bits_1);
  }
  { // Node ID: 1014 (NodeConstantRawBits)
    id1014out_value = (c_hw_fix_49_0_uns_bits);
  }
  { // Node ID: 1018 (NodeOutputMappedReg)
    registerMappedRegister("current_run_cycle_count", Data(48));
  }
  { // Node ID: 1760 (NodeConstantRawBits)
    id1760out_value = (c_hw_fix_1_0_uns_bits_1);
  }
  { // Node ID: 1020 (NodeConstantRawBits)
    id1020out_value = (c_hw_fix_49_0_uns_bits);
  }
  { // Node ID: 1023 (NodeInputMappedReg)
    registerMappedRegister("run_cycle_count", Data(48));
  }
  { // Node ID: 4 (NodeInputMappedReg)
    registerMappedRegister("eps", Data(32));
  }
  { // Node ID: 5 (NodeInputMappedReg)
    registerMappedRegister("minPts", Data(32));
  }
}

void DBSCANKernel::resetComputation() {
  resetComputationAfterFlush();
}

void DBSCANKernel::resetComputationAfterFlush() {
  { // Node ID: 1006 (NodeInputMappedReg)
    id1006out_io_output_force_disabled = getMappedRegValue<HWOffsetFix<1,0,UNSIGNED> >("io_output_force_disabled");
  }
  { // Node ID: 0 (NodeInputMappedReg)
    id0out_io_input_force_disabled = getMappedRegValue<HWOffsetFix<1,0,UNSIGNED> >("io_input_force_disabled");
  }
  { // Node ID: 2 (NodeInput)

    (id2st_read_next_cycle) = (c_hw_fix_1_0_uns_bits);
    (id2st_last_read_value) = (c_hw_flt_8_24_undef);
  }
  { // Node ID: 1346 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1346out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1532 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1532out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1533 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1533out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1534 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1534out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1535 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1535out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1536 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1536out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1537 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1537out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1538 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1538out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1539 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1539out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1540 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1540out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1541 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1541out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1542 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1542out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1543 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1543out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1544 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1544out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1545 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1545out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1546 (NodeFIFO)

    for(int i=0; i<6; i++)
    {
      id1546out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1547 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1547out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1548 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1548out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1549 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1549out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1550 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1550out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1551 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1551out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1552 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1552out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1553 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1553out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1554 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1554out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1555 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1555out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1556 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1556out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1557 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1557out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1558 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1558out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1559 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1559out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1560 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1560out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1561 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1561out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1562 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1562out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1563 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1563out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1564 (NodeFIFO)

    for(int i=0; i<4; i++)
    {
      id1564out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1565 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1565out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1566 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1566out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1567 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1567out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1568 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1568out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1569 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1569out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1570 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1570out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1571 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1571out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1572 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1572out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1573 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1573out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1574 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1574out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1575 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1575out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1576 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1576out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1577 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1577out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1578 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1578out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1579 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1579out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1580 (NodeFIFO)

    for(int i=0; i<6; i++)
    {
      id1580out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1581 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1581out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1582 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1582out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1583 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1583out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1584 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1584out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1585 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1585out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1586 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1586out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1587 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1587out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1588 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1588out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1589 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1589out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1590 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1590out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1591 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1591out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1592 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1592out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1593 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1593out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1594 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1594out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1595 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1595out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1596 (NodeFIFO)

    for(int i=0; i<10; i++)
    {
      id1596out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1597 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1597out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1598 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1598out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1599 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1599out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1600 (NodeFIFO)

    for(int i=0; i<1834; i++)
    {
      id1600out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1601 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1601out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1602 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1602out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1603 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1603out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1604 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1604out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1605 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1605out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1606 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1606out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1607 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1607out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1608 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1608out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1609 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1609out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1610 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1610out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1611 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1611out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1612 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1612out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1613 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1613out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1614 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1614out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1615 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1615out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1616 (NodeFIFO)

    for(int i=0; i<6; i++)
    {
      id1616out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1617 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1617out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1618 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1618out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1619 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1619out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1620 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1620out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1621 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1621out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1622 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1622out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1623 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1623out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1624 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1624out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1625 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1625out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1626 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1626out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1627 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1627out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1628 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1628out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1629 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1629out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1630 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1630out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1631 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1631out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1632 (NodeFIFO)

    for(int i=0; i<6; i++)
    {
      id1632out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1633 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1633out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1634 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1634out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1635 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1635out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1636 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1636out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1637 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1637out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1638 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1638out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1639 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1639out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1640 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1640out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1641 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1641out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1642 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1642out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1643 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1643out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1644 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1644out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1645 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1645out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1646 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1646out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1647 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1647out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1648 (NodeFIFO)

    for(int i=0; i<6; i++)
    {
      id1648out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1649 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1649out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1650 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1650out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1651 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1651out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1652 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1652out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1653 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1653out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1654 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1654out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1655 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1655out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1656 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1656out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1657 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1657out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1658 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1658out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1659 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1659out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1660 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1660out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1661 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1661out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1662 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1662out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1663 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1663out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1664 (NodeFIFO)

    for(int i=0; i<6; i++)
    {
      id1664out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1665 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1665out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1666 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1666out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1667 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1667out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1668 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1668out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1669 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1669out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1670 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1670out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1671 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1671out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1672 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1672out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1673 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1673out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1674 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1674out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1675 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1675out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1676 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1676out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1677 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1677out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1678 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1678out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1679 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1679out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1680 (NodeFIFO)

    for(int i=0; i<6; i++)
    {
      id1680out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1681 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1681out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1682 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1682out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1683 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1683out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1684 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1684out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1685 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1685out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1686 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1686out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1687 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1687out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1688 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1688out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1689 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1689out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1690 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1690out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1691 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1691out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1692 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1692out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1693 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1693out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1694 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1694out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1695 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1695out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1696 (NodeFIFO)

    for(int i=0; i<6; i++)
    {
      id1696out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1697 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1697out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1698 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1698out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1699 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1699out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1700 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1700out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1701 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1701out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1702 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1702out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1703 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1703out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1704 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1704out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1705 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1705out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1706 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1706out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1707 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1707out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1708 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1708out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1709 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1709out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1710 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1710out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1711 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1711out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1712 (NodeFIFO)

    for(int i=0; i<10; i++)
    {
      id1712out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1713 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1713out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1714 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1714out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1715 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1715out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1716 (NodeFIFO)

    for(int i=0; i<1854; i++)
    {
      id1716out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1717 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1717out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1718 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1718out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1719 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1719out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1720 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1720out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1721 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1721out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1722 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1722out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1723 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1723out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1724 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1724out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1725 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1725out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1726 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1726out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1727 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1727out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1728 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1728out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1729 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1729out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1730 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1730out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1731 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1731out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1732 (NodeFIFO)

    for(int i=0; i<6; i++)
    {
      id1732out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1733 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1733out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1734 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1734out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1735 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1735out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1736 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1736out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1737 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1737out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1738 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1738out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1739 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1739out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1740 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1740out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1741 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1741out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1742 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1742out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1743 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1743out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1744 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1744out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1745 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1745out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1746 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1746out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1747 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1747out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1748 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1748out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1749 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1749out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1750 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1750out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1751 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1751out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1752 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1752out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1753 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1753out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1754 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1754out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1755 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1755out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1756 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1756out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1757 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1757out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1758 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1758out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1759 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1759out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1044 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1044out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1047 (NodeFIFO)

    for(int i=0; i<2; i++)
    {
      id1047out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1048 (NodeFIFO)

    for(int i=0; i<11; i++)
    {
      id1048out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1069 (NodeFIFO)

    for(int i=0; i<9; i++)
    {
      id1069out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1110 (NodeFIFO)

    for(int i=0; i<21; i++)
    {
      id1110out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1191 (NodeFIFO)

    for(int i=0; i<41; i++)
    {
      id1191out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1351 (NodeFIFO)

    for(int i=0; i<1981; i++)
    {
      id1351out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1531 (NodeFIFO)

    for(int i=0; i<2061; i++)
    {
      id1531out_output[i] = (c_hw_flt_8_24_undef);
    }
  }
  { // Node ID: 1015 (NodeCounterV1)

    (id1015st_count) = (c_hw_fix_49_0_uns_bits_1);
  }
  { // Node ID: 1021 (NodeCounterV1)

    (id1021st_count) = (c_hw_fix_49_0_uns_bits_1);
  }
  { // Node ID: 1023 (NodeInputMappedReg)
    id1023out_run_cycle_count = getMappedRegValue<HWOffsetFix<48,0,UNSIGNED> >("run_cycle_count");
  }
  { // Node ID: 4 (NodeInputMappedReg)
    id4out_eps = getMappedRegValue<HWFloat<8,24> >("eps");
  }
  { // Node ID: 5 (NodeInputMappedReg)
    id5out_minPts = getMappedRegValue<HWOffsetFix<32,0,TWOSCOMPLEMENT> >("minPts");
  }
}

void DBSCANKernel::updateState() {
  { // Node ID: 1006 (NodeInputMappedReg)
    id1006out_io_output_force_disabled = getMappedRegValue<HWOffsetFix<1,0,UNSIGNED> >("io_output_force_disabled");
  }
  { // Node ID: 0 (NodeInputMappedReg)
    id0out_io_input_force_disabled = getMappedRegValue<HWOffsetFix<1,0,UNSIGNED> >("io_input_force_disabled");
  }
  { // Node ID: 1023 (NodeInputMappedReg)
    id1023out_run_cycle_count = getMappedRegValue<HWOffsetFix<48,0,UNSIGNED> >("run_cycle_count");
  }
  { // Node ID: 4 (NodeInputMappedReg)
    id4out_eps = getMappedRegValue<HWFloat<8,24> >("eps");
  }
  { // Node ID: 5 (NodeInputMappedReg)
    id5out_minPts = getMappedRegValue<HWOffsetFix<32,0,TWOSCOMPLEMENT> >("minPts");
  }
}

void DBSCANKernel::preExecute() {
  { // Node ID: 2 (NodeInput)
    if(((needsToReadInput(m_input))&(((getFlushLevel())<((68l)+(5)))|(!(isFlushingActive()))))) {
      (id2st_last_read_value) = (readInput<HWFloat<8,24> >(m_input));
    }
    id2out_data = (id2st_last_read_value);
  }
}

void DBSCANKernel::runComputationCycle() {
  if (m_mappedElementsChanged) {
    m_mappedElementsChanged = false;
    updateState();
    std::cout << "DBSCANKernel: Mapped Elements Changed: Reloaded" << std::endl;
  }
  preExecute();
  DBSCANKernelBlock1Vars blk1 = execute0();
  DBSCANKernelBlock2Vars blk2 = execute1(blk1);
  execute2(blk2);
}

int DBSCANKernel::getFlushLevelStart() {
  return ((65l)+(3l));
}

}
