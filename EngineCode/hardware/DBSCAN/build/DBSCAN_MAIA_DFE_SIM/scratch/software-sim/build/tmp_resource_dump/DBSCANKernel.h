#ifndef DBSCANKERNEL_H_
#define DBSCANKERNEL_H_

// #include "KernelManagerBlockSync.h"
#include "DBSCANKernel_exec1.h"
#include "DBSCANKernel_exec2.h"


namespace maxcompilersim {

class DBSCANKernel : public KernelManagerBlockSync {
public:
  DBSCANKernel(const std::string &instance_name);

protected:
  virtual void runComputationCycle();
  virtual void resetComputation();
  virtual void resetComputationAfterFlush();
          void updateState();
          void preExecute();
  virtual int  getFlushLevelStart();

private:
  t_port_number m_input;
  t_port_number m_output;
  HWOffsetFix<1,0,UNSIGNED> id1006out_io_output_force_disabled;

  HWOffsetFix<1,0,UNSIGNED> id0out_io_input_force_disabled;

  HWFloat<8,24> id2out_data;

  HWOffsetFix<1,0,UNSIGNED> id2st_read_next_cycle;
  HWFloat<8,24> id2st_last_read_value;

  HWFloat<8,24> id1346out_output[2];

  HWFloat<8,24> id1532out_output[2];

  HWFloat<8,24> id1533out_output[2];

  HWFloat<8,24> id1534out_output[2];

  HWFloat<8,24> id1535out_output[2];

  HWFloat<8,24> id1536out_output[2];

  HWFloat<8,24> id1537out_output[2];

  HWFloat<8,24> id1538out_output[2];

  HWFloat<8,24> id1539out_output[2];

  HWFloat<8,24> id1540out_output[2];

  HWFloat<8,24> id1541out_output[2];

  HWFloat<8,24> id1542out_output[2];

  HWFloat<8,24> id1543out_output[2];

  HWFloat<8,24> id1544out_output[2];

  HWFloat<8,24> id1545out_output[2];

  HWFloat<8,24> id1546out_output[6];

  HWFloat<8,24> id1547out_output[2];

  HWFloat<8,24> id1548out_output[2];

  HWFloat<8,24> id1549out_output[2];

  HWFloat<8,24> id1550out_output[2];

  HWFloat<8,24> id1551out_output[2];

  HWFloat<8,24> id1552out_output[2];

  HWFloat<8,24> id1553out_output[2];

  HWFloat<8,24> id1554out_output[2];

  HWFloat<8,24> id1555out_output[2];

  HWFloat<8,24> id1556out_output[2];

  HWFloat<8,24> id1557out_output[2];

  HWFloat<8,24> id1558out_output[2];

  HWFloat<8,24> id1559out_output[2];

  HWFloat<8,24> id1560out_output[2];

  HWFloat<8,24> id1561out_output[2];

  HWFloat<8,24> id1562out_output[2];

  HWFloat<8,24> id1563out_output[2];

  HWFloat<8,24> id1564out_output[4];

  HWFloat<8,24> id1565out_output[2];

  HWFloat<8,24> id1566out_output[2];

  HWFloat<8,24> id1567out_output[2];

  HWFloat<8,24> id1568out_output[2];

  HWFloat<8,24> id1569out_output[2];

  HWFloat<8,24> id1570out_output[2];

  HWFloat<8,24> id1571out_output[2];

  HWFloat<8,24> id1572out_output[2];

  HWFloat<8,24> id1573out_output[2];

  HWFloat<8,24> id1574out_output[2];

  HWFloat<8,24> id1575out_output[2];

  HWFloat<8,24> id1576out_output[2];

  HWFloat<8,24> id1577out_output[2];

  HWFloat<8,24> id1578out_output[2];

  HWFloat<8,24> id1579out_output[2];

  HWFloat<8,24> id1580out_output[6];

  HWFloat<8,24> id1581out_output[2];

  HWFloat<8,24> id1582out_output[2];

  HWFloat<8,24> id1583out_output[2];

  HWFloat<8,24> id1584out_output[2];

  HWFloat<8,24> id1585out_output[2];

  HWFloat<8,24> id1586out_output[2];

  HWFloat<8,24> id1587out_output[2];

  HWFloat<8,24> id1588out_output[2];

  HWFloat<8,24> id1589out_output[2];

  HWFloat<8,24> id1590out_output[2];

  HWFloat<8,24> id1591out_output[2];

  HWFloat<8,24> id1592out_output[2];

  HWFloat<8,24> id1593out_output[2];

  HWFloat<8,24> id1594out_output[2];

  HWFloat<8,24> id1595out_output[2];

  HWFloat<8,24> id1596out_output[10];

  HWFloat<8,24> id1597out_output[2];

  HWFloat<8,24> id1598out_output[2];

  HWFloat<8,24> id1599out_output[2];

  HWFloat<8,24> id1600out_output[1834];

  HWFloat<8,24> id1601out_output[2];

  HWFloat<8,24> id1602out_output[2];

  HWFloat<8,24> id1603out_output[2];

  HWFloat<8,24> id1604out_output[2];

  HWFloat<8,24> id1605out_output[2];

  HWFloat<8,24> id1606out_output[2];

  HWFloat<8,24> id1607out_output[2];

  HWFloat<8,24> id1608out_output[2];

  HWFloat<8,24> id1609out_output[2];

  HWFloat<8,24> id1610out_output[2];

  HWFloat<8,24> id1611out_output[2];

  HWFloat<8,24> id1612out_output[2];

  HWFloat<8,24> id1613out_output[2];

  HWFloat<8,24> id1614out_output[2];

  HWFloat<8,24> id1615out_output[2];

  HWFloat<8,24> id1616out_output[6];

  HWFloat<8,24> id1617out_output[2];

  HWFloat<8,24> id1618out_output[2];

  HWFloat<8,24> id1619out_output[2];

  HWFloat<8,24> id1620out_output[2];

  HWFloat<8,24> id1621out_output[2];

  HWFloat<8,24> id1622out_output[2];

  HWFloat<8,24> id1623out_output[2];

  HWFloat<8,24> id1624out_output[2];

  HWFloat<8,24> id1625out_output[2];

  HWFloat<8,24> id1626out_output[2];

  HWFloat<8,24> id1627out_output[2];

  HWFloat<8,24> id1628out_output[2];

  HWFloat<8,24> id1629out_output[2];

  HWFloat<8,24> id1630out_output[2];

  HWFloat<8,24> id1631out_output[2];

  HWFloat<8,24> id1632out_output[6];

  HWFloat<8,24> id1633out_output[2];

  HWFloat<8,24> id1634out_output[2];

  HWFloat<8,24> id1635out_output[2];

  HWFloat<8,24> id1636out_output[2];

  HWFloat<8,24> id1637out_output[2];

  HWFloat<8,24> id1638out_output[2];

  HWFloat<8,24> id1639out_output[2];

  HWFloat<8,24> id1640out_output[2];

  HWFloat<8,24> id1641out_output[2];

  HWFloat<8,24> id1642out_output[2];

  HWFloat<8,24> id1643out_output[2];

  HWFloat<8,24> id1644out_output[2];

  HWFloat<8,24> id1645out_output[2];

  HWFloat<8,24> id1646out_output[2];

  HWFloat<8,24> id1647out_output[2];

  HWFloat<8,24> id1648out_output[6];

  HWFloat<8,24> id1649out_output[2];

  HWFloat<8,24> id1650out_output[2];

  HWFloat<8,24> id1651out_output[2];

  HWFloat<8,24> id1652out_output[2];

  HWFloat<8,24> id1653out_output[2];

  HWFloat<8,24> id1654out_output[2];

  HWFloat<8,24> id1655out_output[2];

  HWFloat<8,24> id1656out_output[2];

  HWFloat<8,24> id1657out_output[2];

  HWFloat<8,24> id1658out_output[2];

  HWFloat<8,24> id1659out_output[2];

  HWFloat<8,24> id1660out_output[2];

  HWFloat<8,24> id1661out_output[2];

  HWFloat<8,24> id1662out_output[2];

  HWFloat<8,24> id1663out_output[2];

  HWFloat<8,24> id1664out_output[6];

  HWFloat<8,24> id1665out_output[2];

  HWFloat<8,24> id1666out_output[2];

  HWFloat<8,24> id1667out_output[2];

  HWFloat<8,24> id1668out_output[2];

  HWFloat<8,24> id1669out_output[2];

  HWFloat<8,24> id1670out_output[2];

  HWFloat<8,24> id1671out_output[2];

  HWFloat<8,24> id1672out_output[2];

  HWFloat<8,24> id1673out_output[2];

  HWFloat<8,24> id1674out_output[2];

  HWFloat<8,24> id1675out_output[2];

  HWFloat<8,24> id1676out_output[2];

  HWFloat<8,24> id1677out_output[2];

  HWFloat<8,24> id1678out_output[2];

  HWFloat<8,24> id1679out_output[2];

  HWFloat<8,24> id1680out_output[6];

  HWFloat<8,24> id1681out_output[2];

  HWFloat<8,24> id1682out_output[2];

  HWFloat<8,24> id1683out_output[2];

  HWFloat<8,24> id1684out_output[2];

  HWFloat<8,24> id1685out_output[2];

  HWFloat<8,24> id1686out_output[2];

  HWFloat<8,24> id1687out_output[2];

  HWFloat<8,24> id1688out_output[2];

  HWFloat<8,24> id1689out_output[2];

  HWFloat<8,24> id1690out_output[2];

  HWFloat<8,24> id1691out_output[2];

  HWFloat<8,24> id1692out_output[2];

  HWFloat<8,24> id1693out_output[2];

  HWFloat<8,24> id1694out_output[2];

  HWFloat<8,24> id1695out_output[2];

  HWFloat<8,24> id1696out_output[6];

  HWFloat<8,24> id1697out_output[2];

  HWFloat<8,24> id1698out_output[2];

  HWFloat<8,24> id1699out_output[2];

  HWFloat<8,24> id1700out_output[2];

  HWFloat<8,24> id1701out_output[2];

  HWFloat<8,24> id1702out_output[2];

  HWFloat<8,24> id1703out_output[2];

  HWFloat<8,24> id1704out_output[2];

  HWFloat<8,24> id1705out_output[2];

  HWFloat<8,24> id1706out_output[2];

  HWFloat<8,24> id1707out_output[2];

  HWFloat<8,24> id1708out_output[2];

  HWFloat<8,24> id1709out_output[2];

  HWFloat<8,24> id1710out_output[2];

  HWFloat<8,24> id1711out_output[2];

  HWFloat<8,24> id1712out_output[10];

  HWFloat<8,24> id1713out_output[2];

  HWFloat<8,24> id1714out_output[2];

  HWFloat<8,24> id1715out_output[2];

  HWFloat<8,24> id1716out_output[1854];

  HWFloat<8,24> id1717out_output[2];

  HWFloat<8,24> id1718out_output[2];

  HWFloat<8,24> id1719out_output[2];

  HWFloat<8,24> id1720out_output[2];

  HWFloat<8,24> id1721out_output[2];

  HWFloat<8,24> id1722out_output[2];

  HWFloat<8,24> id1723out_output[2];

  HWFloat<8,24> id1724out_output[2];

  HWFloat<8,24> id1725out_output[2];

  HWFloat<8,24> id1726out_output[2];

  HWFloat<8,24> id1727out_output[2];

  HWFloat<8,24> id1728out_output[2];

  HWFloat<8,24> id1729out_output[2];

  HWFloat<8,24> id1730out_output[2];

  HWFloat<8,24> id1731out_output[2];

  HWFloat<8,24> id1732out_output[6];

  HWFloat<8,24> id1733out_output[2];

  HWFloat<8,24> id1734out_output[2];

  HWFloat<8,24> id1735out_output[2];

  HWFloat<8,24> id1736out_output[2];

  HWFloat<8,24> id1737out_output[2];

  HWFloat<8,24> id1738out_output[2];

  HWFloat<8,24> id1739out_output[2];

  HWFloat<8,24> id1740out_output[2];

  HWFloat<8,24> id1741out_output[2];

  HWFloat<8,24> id1742out_output[2];

  HWFloat<8,24> id1743out_output[2];

  HWFloat<8,24> id1744out_output[2];

  HWFloat<8,24> id1745out_output[2];

  HWFloat<8,24> id1746out_output[2];

  HWFloat<8,24> id1747out_output[2];

  HWFloat<8,24> id1748out_output[2];

  HWFloat<8,24> id1749out_output[2];

  HWFloat<8,24> id1750out_output[2];

  HWFloat<8,24> id1751out_output[2];

  HWFloat<8,24> id1752out_output[2];

  HWFloat<8,24> id1753out_output[2];

  HWFloat<8,24> id1754out_output[2];

  HWFloat<8,24> id1755out_output[2];

  HWFloat<8,24> id1756out_output[2];

  HWFloat<8,24> id1757out_output[2];

  HWFloat<8,24> id1758out_output[2];

  HWFloat<8,24> id1759out_output[2];

  HWFloat<8,24> id506out_result[15];

  HWFloat<8,24> id507out_result[15];

  HWFloat<8,24> id516out_result[15];

  HWFloat<8,24> id508out_result[15];

  HWFloat<8,24> id509out_result[15];

  HWFloat<8,24> id517out_result[15];

  HWFloat<8,24> id521out_result[15];

  HWFloat<8,24> id510out_result[15];

  HWFloat<8,24> id511out_result[15];

  HWFloat<8,24> id518out_result[15];

  HWFloat<8,24> id512out_result[15];

  HWFloat<8,24> id513out_result[15];

  HWFloat<8,24> id519out_result[15];

  HWFloat<8,24> id522out_result[15];

  HWFloat<8,24> id523out_result[15];

  HWFloat<8,24> id1044out_output[2];

  HWFloat<8,24> id514out_result[15];

  HWFloat<8,24> id1047out_output[2];

  HWFloat<8,24> id515out_result[15];

  HWFloat<8,24> id520out_result[15];

  HWFloat<8,24> id1048out_output[11];

  HWFloat<8,24> id524out_result[15];

  HWFloat<8,24> id1069out_output[9];

  HWFloat<8,24> id525out_result[15];

  HWFloat<8,24> id526out_result[15];

  HWFloat<8,24> id535out_result[15];

  HWFloat<8,24> id527out_result[15];

  HWFloat<8,24> id528out_result[15];

  HWFloat<8,24> id536out_result[15];

  HWFloat<8,24> id540out_result[15];

  HWFloat<8,24> id529out_result[15];

  HWFloat<8,24> id530out_result[15];

  HWFloat<8,24> id537out_result[15];

  HWFloat<8,24> id531out_result[15];

  HWFloat<8,24> id532out_result[15];

  HWFloat<8,24> id538out_result[15];

  HWFloat<8,24> id541out_result[15];

  HWFloat<8,24> id542out_result[15];

  HWFloat<8,24> id533out_result[15];

  HWFloat<8,24> id534out_result[15];

  HWFloat<8,24> id539out_result[15];

  HWFloat<8,24> id543out_result[15];

  HWFloat<8,24> id981out_result[15];

  HWFloat<8,24> id1110out_output[21];

  HWFloat<8,24> id544out_result[15];

  HWFloat<8,24> id545out_result[15];

  HWFloat<8,24> id554out_result[15];

  HWFloat<8,24> id546out_result[15];

  HWFloat<8,24> id547out_result[15];

  HWFloat<8,24> id555out_result[15];

  HWFloat<8,24> id559out_result[15];

  HWFloat<8,24> id548out_result[15];

  HWFloat<8,24> id549out_result[15];

  HWFloat<8,24> id556out_result[15];

  HWFloat<8,24> id550out_result[15];

  HWFloat<8,24> id551out_result[15];

  HWFloat<8,24> id557out_result[15];

  HWFloat<8,24> id560out_result[15];

  HWFloat<8,24> id561out_result[15];

  HWFloat<8,24> id552out_result[15];

  HWFloat<8,24> id553out_result[15];

  HWFloat<8,24> id558out_result[15];

  HWFloat<8,24> id562out_result[15];

  HWFloat<8,24> id563out_result[15];

  HWFloat<8,24> id564out_result[15];

  HWFloat<8,24> id573out_result[15];

  HWFloat<8,24> id565out_result[15];

  HWFloat<8,24> id566out_result[15];

  HWFloat<8,24> id574out_result[15];

  HWFloat<8,24> id578out_result[15];

  HWFloat<8,24> id567out_result[15];

  HWFloat<8,24> id568out_result[15];

  HWFloat<8,24> id575out_result[15];

  HWFloat<8,24> id569out_result[15];

  HWFloat<8,24> id570out_result[15];

  HWFloat<8,24> id576out_result[15];

  HWFloat<8,24> id579out_result[15];

  HWFloat<8,24> id580out_result[15];

  HWFloat<8,24> id571out_result[15];

  HWFloat<8,24> id572out_result[15];

  HWFloat<8,24> id577out_result[15];

  HWFloat<8,24> id581out_result[15];

  HWFloat<8,24> id982out_result[15];

  HWFloat<8,24> id993out_result[15];

  HWFloat<8,24> id1191out_output[41];

  HWFloat<8,24> id582out_result[15];

  HWFloat<8,24> id583out_result[15];

  HWFloat<8,24> id592out_result[15];

  HWFloat<8,24> id584out_result[15];

  HWFloat<8,24> id585out_result[15];

  HWFloat<8,24> id593out_result[15];

  HWFloat<8,24> id597out_result[15];

  HWFloat<8,24> id586out_result[15];

  HWFloat<8,24> id587out_result[15];

  HWFloat<8,24> id594out_result[15];

  HWFloat<8,24> id588out_result[15];

  HWFloat<8,24> id589out_result[15];

  HWFloat<8,24> id595out_result[15];

  HWFloat<8,24> id598out_result[15];

  HWFloat<8,24> id599out_result[15];

  HWFloat<8,24> id590out_result[15];

  HWFloat<8,24> id591out_result[15];

  HWFloat<8,24> id596out_result[15];

  HWFloat<8,24> id600out_result[15];

  HWFloat<8,24> id601out_result[15];

  HWFloat<8,24> id602out_result[15];

  HWFloat<8,24> id611out_result[15];

  HWFloat<8,24> id603out_result[15];

  HWFloat<8,24> id604out_result[15];

  HWFloat<8,24> id612out_result[15];

  HWFloat<8,24> id616out_result[15];

  HWFloat<8,24> id605out_result[15];

  HWFloat<8,24> id606out_result[15];

  HWFloat<8,24> id613out_result[15];

  HWFloat<8,24> id607out_result[15];

  HWFloat<8,24> id608out_result[15];

  HWFloat<8,24> id614out_result[15];

  HWFloat<8,24> id617out_result[15];

  HWFloat<8,24> id618out_result[15];

  HWFloat<8,24> id609out_result[15];

  HWFloat<8,24> id610out_result[15];

  HWFloat<8,24> id615out_result[15];

  HWFloat<8,24> id619out_result[15];

  HWFloat<8,24> id983out_result[15];

  HWFloat<8,24> id620out_result[15];

  HWFloat<8,24> id621out_result[15];

  HWFloat<8,24> id630out_result[15];

  HWFloat<8,24> id622out_result[15];

  HWFloat<8,24> id623out_result[15];

  HWFloat<8,24> id631out_result[15];

  HWFloat<8,24> id635out_result[15];

  HWFloat<8,24> id624out_result[15];

  HWFloat<8,24> id625out_result[15];

  HWFloat<8,24> id632out_result[15];

  HWFloat<8,24> id626out_result[15];

  HWFloat<8,24> id627out_result[15];

  HWFloat<8,24> id633out_result[15];

  HWFloat<8,24> id636out_result[15];

  HWFloat<8,24> id637out_result[15];

  HWFloat<8,24> id628out_result[15];

  HWFloat<8,24> id629out_result[15];

  HWFloat<8,24> id634out_result[15];

  HWFloat<8,24> id638out_result[15];

  HWFloat<8,24> id639out_result[15];

  HWFloat<8,24> id640out_result[15];

  HWFloat<8,24> id649out_result[15];

  HWFloat<8,24> id641out_result[15];

  HWFloat<8,24> id642out_result[15];

  HWFloat<8,24> id650out_result[15];

  HWFloat<8,24> id654out_result[15];

  HWFloat<8,24> id643out_result[15];

  HWFloat<8,24> id644out_result[15];

  HWFloat<8,24> id651out_result[15];

  HWFloat<8,24> id645out_result[15];

  HWFloat<8,24> id646out_result[15];

  HWFloat<8,24> id652out_result[15];

  HWFloat<8,24> id655out_result[15];

  HWFloat<8,24> id656out_result[15];

  HWFloat<8,24> id647out_result[15];

  HWFloat<8,24> id648out_result[15];

  HWFloat<8,24> id653out_result[15];

  HWFloat<8,24> id657out_result[15];

  HWFloat<8,24> id984out_result[15];

  HWFloat<8,24> id994out_result[15];

  HWFloat<8,24> id999out_result[15];

  HWFloat<8,24> id1351out_output[1981];

  HWFloat<8,24> id658out_result[15];

  HWFloat<8,24> id659out_result[15];

  HWFloat<8,24> id668out_result[15];

  HWFloat<8,24> id660out_result[15];

  HWFloat<8,24> id661out_result[15];

  HWFloat<8,24> id669out_result[15];

  HWFloat<8,24> id673out_result[15];

  HWFloat<8,24> id662out_result[15];

  HWFloat<8,24> id663out_result[15];

  HWFloat<8,24> id670out_result[15];

  HWFloat<8,24> id664out_result[15];

  HWFloat<8,24> id665out_result[15];

  HWFloat<8,24> id671out_result[15];

  HWFloat<8,24> id674out_result[15];

  HWFloat<8,24> id675out_result[15];

  HWFloat<8,24> id666out_result[15];

  HWFloat<8,24> id667out_result[15];

  HWFloat<8,24> id672out_result[15];

  HWFloat<8,24> id676out_result[15];

  HWFloat<8,24> id677out_result[15];

  HWFloat<8,24> id678out_result[15];

  HWFloat<8,24> id687out_result[15];

  HWFloat<8,24> id679out_result[15];

  HWFloat<8,24> id680out_result[15];

  HWFloat<8,24> id688out_result[15];

  HWFloat<8,24> id692out_result[15];

  HWFloat<8,24> id681out_result[15];

  HWFloat<8,24> id682out_result[15];

  HWFloat<8,24> id689out_result[15];

  HWFloat<8,24> id683out_result[15];

  HWFloat<8,24> id684out_result[15];

  HWFloat<8,24> id690out_result[15];

  HWFloat<8,24> id693out_result[15];

  HWFloat<8,24> id694out_result[15];

  HWFloat<8,24> id685out_result[15];

  HWFloat<8,24> id686out_result[15];

  HWFloat<8,24> id691out_result[15];

  HWFloat<8,24> id695out_result[15];

  HWFloat<8,24> id985out_result[15];

  HWFloat<8,24> id696out_result[15];

  HWFloat<8,24> id697out_result[15];

  HWFloat<8,24> id706out_result[15];

  HWFloat<8,24> id698out_result[15];

  HWFloat<8,24> id699out_result[15];

  HWFloat<8,24> id707out_result[15];

  HWFloat<8,24> id711out_result[15];

  HWFloat<8,24> id700out_result[15];

  HWFloat<8,24> id701out_result[15];

  HWFloat<8,24> id708out_result[15];

  HWFloat<8,24> id702out_result[15];

  HWFloat<8,24> id703out_result[15];

  HWFloat<8,24> id709out_result[15];

  HWFloat<8,24> id712out_result[15];

  HWFloat<8,24> id713out_result[15];

  HWFloat<8,24> id704out_result[15];

  HWFloat<8,24> id705out_result[15];

  HWFloat<8,24> id710out_result[15];

  HWFloat<8,24> id714out_result[15];

  HWFloat<8,24> id715out_result[15];

  HWFloat<8,24> id716out_result[15];

  HWFloat<8,24> id725out_result[15];

  HWFloat<8,24> id717out_result[15];

  HWFloat<8,24> id718out_result[15];

  HWFloat<8,24> id726out_result[15];

  HWFloat<8,24> id730out_result[15];

  HWFloat<8,24> id719out_result[15];

  HWFloat<8,24> id720out_result[15];

  HWFloat<8,24> id727out_result[15];

  HWFloat<8,24> id721out_result[15];

  HWFloat<8,24> id722out_result[15];

  HWFloat<8,24> id728out_result[15];

  HWFloat<8,24> id731out_result[15];

  HWFloat<8,24> id732out_result[15];

  HWFloat<8,24> id723out_result[15];

  HWFloat<8,24> id724out_result[15];

  HWFloat<8,24> id729out_result[15];

  HWFloat<8,24> id733out_result[15];

  HWFloat<8,24> id986out_result[15];

  HWFloat<8,24> id995out_result[15];

  HWFloat<8,24> id734out_result[15];

  HWFloat<8,24> id735out_result[15];

  HWFloat<8,24> id744out_result[15];

  HWFloat<8,24> id736out_result[15];

  HWFloat<8,24> id737out_result[15];

  HWFloat<8,24> id745out_result[15];

  HWFloat<8,24> id749out_result[15];

  HWFloat<8,24> id738out_result[15];

  HWFloat<8,24> id739out_result[15];

  HWFloat<8,24> id746out_result[15];

  HWFloat<8,24> id740out_result[15];

  HWFloat<8,24> id741out_result[15];

  HWFloat<8,24> id747out_result[15];

  HWFloat<8,24> id750out_result[15];

  HWFloat<8,24> id751out_result[15];

  HWFloat<8,24> id742out_result[15];

  HWFloat<8,24> id743out_result[15];

  HWFloat<8,24> id748out_result[15];

  HWFloat<8,24> id752out_result[15];

  HWFloat<8,24> id753out_result[15];

  HWFloat<8,24> id754out_result[15];

  HWFloat<8,24> id763out_result[15];

  HWFloat<8,24> id755out_result[15];

  HWFloat<8,24> id756out_result[15];

  HWFloat<8,24> id764out_result[15];

  HWFloat<8,24> id768out_result[15];

  HWFloat<8,24> id757out_result[15];

  HWFloat<8,24> id758out_result[15];

  HWFloat<8,24> id765out_result[15];

  HWFloat<8,24> id759out_result[15];

  HWFloat<8,24> id760out_result[15];

  HWFloat<8,24> id766out_result[15];

  HWFloat<8,24> id769out_result[15];

  HWFloat<8,24> id770out_result[15];

  HWFloat<8,24> id761out_result[15];

  HWFloat<8,24> id762out_result[15];

  HWFloat<8,24> id767out_result[15];

  HWFloat<8,24> id771out_result[15];

  HWFloat<8,24> id987out_result[15];

  HWFloat<8,24> id772out_result[15];

  HWFloat<8,24> id773out_result[15];

  HWFloat<8,24> id782out_result[15];

  HWFloat<8,24> id774out_result[15];

  HWFloat<8,24> id775out_result[15];

  HWFloat<8,24> id783out_result[15];

  HWFloat<8,24> id787out_result[15];

  HWFloat<8,24> id776out_result[15];

  HWFloat<8,24> id777out_result[15];

  HWFloat<8,24> id784out_result[15];

  HWFloat<8,24> id778out_result[15];

  HWFloat<8,24> id779out_result[15];

  HWFloat<8,24> id785out_result[15];

  HWFloat<8,24> id788out_result[15];

  HWFloat<8,24> id789out_result[15];

  HWFloat<8,24> id780out_result[15];

  HWFloat<8,24> id781out_result[15];

  HWFloat<8,24> id786out_result[15];

  HWFloat<8,24> id790out_result[15];

  HWFloat<8,24> id791out_result[15];

  HWFloat<8,24> id792out_result[15];

  HWFloat<8,24> id801out_result[15];

  HWFloat<8,24> id793out_result[15];

  HWFloat<8,24> id794out_result[15];

  HWFloat<8,24> id802out_result[15];

  HWFloat<8,24> id806out_result[15];

  HWFloat<8,24> id795out_result[15];

  HWFloat<8,24> id796out_result[15];

  HWFloat<8,24> id803out_result[15];

  HWFloat<8,24> id797out_result[15];

  HWFloat<8,24> id798out_result[15];

  HWFloat<8,24> id804out_result[15];

  HWFloat<8,24> id807out_result[15];

  HWFloat<8,24> id808out_result[15];

  HWFloat<8,24> id799out_result[15];

  HWFloat<8,24> id800out_result[15];

  HWFloat<8,24> id805out_result[15];

  HWFloat<8,24> id809out_result[15];

  HWFloat<8,24> id988out_result[15];

  HWFloat<8,24> id996out_result[15];

  HWFloat<8,24> id1000out_result[15];

  HWFloat<8,24> id1002out_result[15];

  HWFloat<8,24> id1531out_output[2061];

  HWFloat<8,24> id810out_result[15];

  HWFloat<8,24> id811out_result[15];

  HWFloat<8,24> id820out_result[15];

  HWFloat<8,24> id812out_result[15];

  HWFloat<8,24> id813out_result[15];

  HWFloat<8,24> id821out_result[15];

  HWFloat<8,24> id825out_result[15];

  HWFloat<8,24> id814out_result[15];

  HWFloat<8,24> id815out_result[15];

  HWFloat<8,24> id822out_result[15];

  HWFloat<8,24> id816out_result[15];

  HWFloat<8,24> id817out_result[15];

  HWFloat<8,24> id823out_result[15];

  HWFloat<8,24> id826out_result[15];

  HWFloat<8,24> id827out_result[15];

  HWFloat<8,24> id818out_result[15];

  HWFloat<8,24> id819out_result[15];

  HWFloat<8,24> id824out_result[15];

  HWFloat<8,24> id828out_result[15];

  HWFloat<8,24> id829out_result[15];

  HWFloat<8,24> id830out_result[15];

  HWFloat<8,24> id839out_result[15];

  HWFloat<8,24> id831out_result[15];

  HWFloat<8,24> id832out_result[15];

  HWFloat<8,24> id840out_result[15];

  HWFloat<8,24> id844out_result[15];

  HWFloat<8,24> id833out_result[15];

  HWFloat<8,24> id834out_result[15];

  HWFloat<8,24> id841out_result[15];

  HWFloat<8,24> id835out_result[15];

  HWFloat<8,24> id836out_result[15];

  HWFloat<8,24> id842out_result[15];

  HWFloat<8,24> id845out_result[15];

  HWFloat<8,24> id846out_result[15];

  HWFloat<8,24> id837out_result[15];

  HWFloat<8,24> id838out_result[15];

  HWFloat<8,24> id843out_result[15];

  HWFloat<8,24> id847out_result[15];

  HWFloat<8,24> id989out_result[15];

  HWFloat<8,24> id848out_result[15];

  HWFloat<8,24> id849out_result[15];

  HWFloat<8,24> id858out_result[15];

  HWFloat<8,24> id850out_result[15];

  HWFloat<8,24> id851out_result[15];

  HWFloat<8,24> id859out_result[15];

  HWFloat<8,24> id863out_result[15];

  HWFloat<8,24> id852out_result[15];

  HWFloat<8,24> id853out_result[15];

  HWFloat<8,24> id860out_result[15];

  HWFloat<8,24> id854out_result[15];

  HWFloat<8,24> id855out_result[15];

  HWFloat<8,24> id861out_result[15];

  HWFloat<8,24> id864out_result[15];

  HWFloat<8,24> id865out_result[15];

  HWFloat<8,24> id856out_result[15];

  HWFloat<8,24> id857out_result[15];

  HWFloat<8,24> id862out_result[15];

  HWFloat<8,24> id866out_result[15];

  HWFloat<8,24> id867out_result[15];

  HWFloat<8,24> id868out_result[15];

  HWFloat<8,24> id877out_result[15];

  HWFloat<8,24> id869out_result[15];

  HWFloat<8,24> id870out_result[15];

  HWFloat<8,24> id878out_result[15];

  HWFloat<8,24> id882out_result[15];

  HWFloat<8,24> id871out_result[15];

  HWFloat<8,24> id872out_result[15];

  HWFloat<8,24> id879out_result[15];

  HWFloat<8,24> id873out_result[15];

  HWFloat<8,24> id874out_result[15];

  HWFloat<8,24> id880out_result[15];

  HWFloat<8,24> id883out_result[15];

  HWFloat<8,24> id884out_result[15];

  HWFloat<8,24> id875out_result[15];

  HWFloat<8,24> id876out_result[15];

  HWFloat<8,24> id881out_result[15];

  HWFloat<8,24> id885out_result[15];

  HWFloat<8,24> id990out_result[15];

  HWFloat<8,24> id997out_result[15];

  HWFloat<8,24> id886out_result[15];

  HWFloat<8,24> id887out_result[15];

  HWFloat<8,24> id896out_result[15];

  HWFloat<8,24> id888out_result[15];

  HWFloat<8,24> id889out_result[15];

  HWFloat<8,24> id897out_result[15];

  HWFloat<8,24> id901out_result[15];

  HWFloat<8,24> id890out_result[15];

  HWFloat<8,24> id891out_result[15];

  HWFloat<8,24> id898out_result[15];

  HWFloat<8,24> id892out_result[15];

  HWFloat<8,24> id893out_result[15];

  HWFloat<8,24> id899out_result[15];

  HWFloat<8,24> id902out_result[15];

  HWFloat<8,24> id903out_result[15];

  HWFloat<8,24> id894out_result[15];

  HWFloat<8,24> id895out_result[15];

  HWFloat<8,24> id900out_result[15];

  HWFloat<8,24> id904out_result[15];

  HWFloat<8,24> id905out_result[15];

  HWFloat<8,24> id906out_result[15];

  HWFloat<8,24> id915out_result[15];

  HWFloat<8,24> id907out_result[15];

  HWFloat<8,24> id908out_result[15];

  HWFloat<8,24> id916out_result[15];

  HWFloat<8,24> id920out_result[15];

  HWFloat<8,24> id909out_result[15];

  HWFloat<8,24> id910out_result[15];

  HWFloat<8,24> id917out_result[15];

  HWFloat<8,24> id911out_result[15];

  HWFloat<8,24> id912out_result[15];

  HWFloat<8,24> id918out_result[15];

  HWFloat<8,24> id921out_result[15];

  HWFloat<8,24> id922out_result[15];

  HWFloat<8,24> id913out_result[15];

  HWFloat<8,24> id914out_result[15];

  HWFloat<8,24> id919out_result[15];

  HWFloat<8,24> id923out_result[15];

  HWFloat<8,24> id991out_result[15];

  HWFloat<8,24> id924out_result[15];

  HWFloat<8,24> id925out_result[15];

  HWFloat<8,24> id934out_result[15];

  HWFloat<8,24> id926out_result[15];

  HWFloat<8,24> id927out_result[15];

  HWFloat<8,24> id935out_result[15];

  HWFloat<8,24> id939out_result[15];

  HWFloat<8,24> id928out_result[15];

  HWFloat<8,24> id929out_result[15];

  HWFloat<8,24> id936out_result[15];

  HWFloat<8,24> id930out_result[15];

  HWFloat<8,24> id931out_result[15];

  HWFloat<8,24> id937out_result[15];

  HWFloat<8,24> id940out_result[15];

  HWFloat<8,24> id941out_result[15];

  HWFloat<8,24> id932out_result[15];

  HWFloat<8,24> id933out_result[15];

  HWFloat<8,24> id938out_result[15];

  HWFloat<8,24> id942out_result[15];

  HWFloat<8,24> id943out_result[15];

  HWFloat<8,24> id944out_result[15];

  HWFloat<8,24> id953out_result[15];

  HWFloat<8,24> id945out_result[15];

  HWFloat<8,24> id946out_result[15];

  HWFloat<8,24> id954out_result[15];

  HWFloat<8,24> id958out_result[15];

  HWFloat<8,24> id947out_result[15];

  HWFloat<8,24> id948out_result[15];

  HWFloat<8,24> id955out_result[15];

  HWFloat<8,24> id949out_result[15];

  HWFloat<8,24> id950out_result[15];

  HWFloat<8,24> id956out_result[15];

  HWFloat<8,24> id959out_result[15];

  HWFloat<8,24> id960out_result[15];

  HWFloat<8,24> id951out_result[15];

  HWFloat<8,24> id952out_result[15];

  HWFloat<8,24> id957out_result[15];

  HWFloat<8,24> id961out_result[15];

  HWFloat<8,24> id992out_result[15];

  HWFloat<8,24> id998out_result[15];

  HWFloat<8,24> id1001out_result[15];

  HWFloat<8,24> id962out_result[15];

  HWFloat<8,24> id963out_result[15];

  HWFloat<8,24> id972out_result[15];

  HWFloat<8,24> id964out_result[15];

  HWFloat<8,24> id965out_result[15];

  HWFloat<8,24> id973out_result[15];

  HWFloat<8,24> id977out_result[15];

  HWFloat<8,24> id966out_result[15];

  HWFloat<8,24> id967out_result[15];

  HWFloat<8,24> id974out_result[15];

  HWFloat<8,24> id968out_result[15];

  HWFloat<8,24> id969out_result[15];

  HWFloat<8,24> id975out_result[15];

  HWFloat<8,24> id978out_result[15];

  HWFloat<8,24> id979out_result[15];

  HWFloat<8,24> id970out_result[15];

  HWFloat<8,24> id971out_result[15];

  HWFloat<8,24> id976out_result[15];

  HWFloat<8,24> id980out_result[15];

  HWFloat<8,24> id1003out_result[15];

  HWFloat<8,24> id1004out_result[15];

  HWOffsetFix<1,0,UNSIGNED> id1017out_value;

  HWOffsetFix<1,0,UNSIGNED> id1761out_value;

  HWOffsetFix<49,0,UNSIGNED> id1014out_value;

  HWOffsetFix<48,0,UNSIGNED> id1015out_count;
  HWOffsetFix<1,0,UNSIGNED> id1015out_wrap;

  HWOffsetFix<49,0,UNSIGNED> id1015st_count;

  HWOffsetFix<1,0,UNSIGNED> id1760out_value;

  HWOffsetFix<49,0,UNSIGNED> id1020out_value;

  HWOffsetFix<48,0,UNSIGNED> id1021out_count;
  HWOffsetFix<1,0,UNSIGNED> id1021out_wrap;

  HWOffsetFix<49,0,UNSIGNED> id1021st_count;

  HWOffsetFix<48,0,UNSIGNED> id1023out_run_cycle_count;

  HWOffsetFix<1,0,UNSIGNED> id1024out_result[2];

  HWFloat<8,24> id4out_eps;

  HWOffsetFix<32,0,TWOSCOMPLEMENT> id5out_minPts;

  const HWOffsetFix<1,0,UNSIGNED> c_hw_fix_1_0_uns_bits;
  const HWFloat<8,24> c_hw_flt_8_24_undef;
  const HWOffsetFix<1,0,UNSIGNED> c_hw_fix_1_0_uns_bits_1;
  const HWOffsetFix<49,0,UNSIGNED> c_hw_fix_49_0_uns_bits;
  const HWOffsetFix<49,0,UNSIGNED> c_hw_fix_49_0_uns_bits_1;
  const HWOffsetFix<49,0,UNSIGNED> c_hw_fix_49_0_uns_bits_2;

  DBSCANKernelBlock1Vars execute0();
  DBSCANKernelBlock2Vars execute1(const DBSCANKernelBlock1Vars &in_vars);
  void execute2(const DBSCANKernelBlock2Vars &in_vars);
};

}

#endif /* DBSCANKERNEL_H_ */
