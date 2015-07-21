#include "stdsimheader.h"
//#define BOOST_NO_STD_LOCALE
//#include <boost/format.hpp>

//#include "DBSCANKernel_exec2.h"
//#include "DBSCANKernel.h"

namespace maxcompilersim {

void DBSCANKernel::execute2(const DBSCANKernelBlock2Vars &in_vars) {
  HWFloat<8,24> id386out_output;

  { // Node ID: 386 (NodeStreamOffset)
    const HWFloat<8,24> &id386in_input = id1663out_output[getCycle()%2];

    id386out_output = id386in_input;
  }
  HWFloat<8,24> id387out_output;

  { // Node ID: 387 (NodeStreamOffset)
    const HWFloat<8,24> &id387in_input = id1662out_output[getCycle()%2];

    id387out_output = id387in_input;
  }
  { // Node ID: 867 (NodeAdd)
    const HWFloat<8,24> &id867in_a = id386out_output;
    const HWFloat<8,24> &id867in_b = id387out_output;

    id867out_result[(getCycle()+14)%15] = (add_float(id867in_a,id867in_b));
  }
  HWFloat<8,24> id388out_output;

  { // Node ID: 388 (NodeStreamOffset)
    const HWFloat<8,24> &id388in_input = id1661out_output[getCycle()%2];

    id388out_output = id388in_input;
  }
  HWFloat<8,24> id389out_output;

  { // Node ID: 389 (NodeStreamOffset)
    const HWFloat<8,24> &id389in_input = id1660out_output[getCycle()%2];

    id389out_output = id389in_input;
  }
  { // Node ID: 868 (NodeAdd)
    const HWFloat<8,24> &id868in_a = id388out_output;
    const HWFloat<8,24> &id868in_b = id389out_output;

    id868out_result[(getCycle()+14)%15] = (add_float(id868in_a,id868in_b));
  }
  { // Node ID: 877 (NodeAdd)
    const HWFloat<8,24> &id877in_a = id867out_result[getCycle()%15];
    const HWFloat<8,24> &id877in_b = id868out_result[getCycle()%15];

    id877out_result[(getCycle()+14)%15] = (add_float(id877in_a,id877in_b));
  }
  HWFloat<8,24> id390out_output;

  { // Node ID: 390 (NodeStreamOffset)
    const HWFloat<8,24> &id390in_input = id1659out_output[getCycle()%2];

    id390out_output = id390in_input;
  }
  HWFloat<8,24> id391out_output;

  { // Node ID: 391 (NodeStreamOffset)
    const HWFloat<8,24> &id391in_input = id1658out_output[getCycle()%2];

    id391out_output = id391in_input;
  }
  { // Node ID: 869 (NodeAdd)
    const HWFloat<8,24> &id869in_a = id390out_output;
    const HWFloat<8,24> &id869in_b = id391out_output;

    id869out_result[(getCycle()+14)%15] = (add_float(id869in_a,id869in_b));
  }
  HWFloat<8,24> id392out_output;

  { // Node ID: 392 (NodeStreamOffset)
    const HWFloat<8,24> &id392in_input = id1657out_output[getCycle()%2];

    id392out_output = id392in_input;
  }
  HWFloat<8,24> id393out_output;

  { // Node ID: 393 (NodeStreamOffset)
    const HWFloat<8,24> &id393in_input = id1656out_output[getCycle()%2];

    id393out_output = id393in_input;
  }
  { // Node ID: 870 (NodeAdd)
    const HWFloat<8,24> &id870in_a = id392out_output;
    const HWFloat<8,24> &id870in_b = id393out_output;

    id870out_result[(getCycle()+14)%15] = (add_float(id870in_a,id870in_b));
  }
  { // Node ID: 878 (NodeAdd)
    const HWFloat<8,24> &id878in_a = id869out_result[getCycle()%15];
    const HWFloat<8,24> &id878in_b = id870out_result[getCycle()%15];

    id878out_result[(getCycle()+14)%15] = (add_float(id878in_a,id878in_b));
  }
  { // Node ID: 882 (NodeAdd)
    const HWFloat<8,24> &id882in_a = id877out_result[getCycle()%15];
    const HWFloat<8,24> &id882in_b = id878out_result[getCycle()%15];

    id882out_result[(getCycle()+14)%15] = (add_float(id882in_a,id882in_b));
  }
  HWFloat<8,24> id394out_output;

  { // Node ID: 394 (NodeStreamOffset)
    const HWFloat<8,24> &id394in_input = id1655out_output[getCycle()%2];

    id394out_output = id394in_input;
  }
  HWFloat<8,24> id395out_output;

  { // Node ID: 395 (NodeStreamOffset)
    const HWFloat<8,24> &id395in_input = id1654out_output[getCycle()%2];

    id395out_output = id395in_input;
  }
  { // Node ID: 871 (NodeAdd)
    const HWFloat<8,24> &id871in_a = id394out_output;
    const HWFloat<8,24> &id871in_b = id395out_output;

    id871out_result[(getCycle()+14)%15] = (add_float(id871in_a,id871in_b));
  }
  HWFloat<8,24> id396out_output;

  { // Node ID: 396 (NodeStreamOffset)
    const HWFloat<8,24> &id396in_input = id1653out_output[getCycle()%2];

    id396out_output = id396in_input;
  }
  HWFloat<8,24> id397out_output;

  { // Node ID: 397 (NodeStreamOffset)
    const HWFloat<8,24> &id397in_input = id1652out_output[getCycle()%2];

    id397out_output = id397in_input;
  }
  { // Node ID: 872 (NodeAdd)
    const HWFloat<8,24> &id872in_a = id396out_output;
    const HWFloat<8,24> &id872in_b = id397out_output;

    id872out_result[(getCycle()+14)%15] = (add_float(id872in_a,id872in_b));
  }
  { // Node ID: 879 (NodeAdd)
    const HWFloat<8,24> &id879in_a = id871out_result[getCycle()%15];
    const HWFloat<8,24> &id879in_b = id872out_result[getCycle()%15];

    id879out_result[(getCycle()+14)%15] = (add_float(id879in_a,id879in_b));
  }
  HWFloat<8,24> id398out_output;

  { // Node ID: 398 (NodeStreamOffset)
    const HWFloat<8,24> &id398in_input = id1651out_output[getCycle()%2];

    id398out_output = id398in_input;
  }
  HWFloat<8,24> id399out_output;

  { // Node ID: 399 (NodeStreamOffset)
    const HWFloat<8,24> &id399in_input = id1650out_output[getCycle()%2];

    id399out_output = id399in_input;
  }
  { // Node ID: 873 (NodeAdd)
    const HWFloat<8,24> &id873in_a = id398out_output;
    const HWFloat<8,24> &id873in_b = id399out_output;

    id873out_result[(getCycle()+14)%15] = (add_float(id873in_a,id873in_b));
  }
  HWFloat<8,24> id400out_output;

  { // Node ID: 400 (NodeStreamOffset)
    const HWFloat<8,24> &id400in_input = id1649out_output[getCycle()%2];

    id400out_output = id400in_input;
  }
  HWFloat<8,24> id401out_output;

  { // Node ID: 401 (NodeStreamOffset)
    const HWFloat<8,24> &id401in_input = id1648out_output[getCycle()%6];

    id401out_output = id401in_input;
  }
  { // Node ID: 874 (NodeAdd)
    const HWFloat<8,24> &id874in_a = id400out_output;
    const HWFloat<8,24> &id874in_b = id401out_output;

    id874out_result[(getCycle()+14)%15] = (add_float(id874in_a,id874in_b));
  }
  { // Node ID: 880 (NodeAdd)
    const HWFloat<8,24> &id880in_a = id873out_result[getCycle()%15];
    const HWFloat<8,24> &id880in_b = id874out_result[getCycle()%15];

    id880out_result[(getCycle()+14)%15] = (add_float(id880in_a,id880in_b));
  }
  { // Node ID: 883 (NodeAdd)
    const HWFloat<8,24> &id883in_a = id879out_result[getCycle()%15];
    const HWFloat<8,24> &id883in_b = id880out_result[getCycle()%15];

    id883out_result[(getCycle()+14)%15] = (add_float(id883in_a,id883in_b));
  }
  { // Node ID: 884 (NodeAdd)
    const HWFloat<8,24> &id884in_a = id882out_result[getCycle()%15];
    const HWFloat<8,24> &id884in_b = id883out_result[getCycle()%15];

    id884out_result[(getCycle()+14)%15] = (add_float(id884in_a,id884in_b));
  }
  HWFloat<8,24> id402out_output;

  { // Node ID: 402 (NodeStreamOffset)
    const HWFloat<8,24> &id402in_input = id1671out_output[getCycle()%2];

    id402out_output = id402in_input;
  }
  HWFloat<8,24> id403out_output;

  { // Node ID: 403 (NodeStreamOffset)
    const HWFloat<8,24> &id403in_input = id1670out_output[getCycle()%2];

    id403out_output = id403in_input;
  }
  { // Node ID: 875 (NodeAdd)
    const HWFloat<8,24> &id875in_a = id402out_output;
    const HWFloat<8,24> &id875in_b = id403out_output;

    id875out_result[(getCycle()+14)%15] = (add_float(id875in_a,id875in_b));
  }
  HWFloat<8,24> id404out_output;

  { // Node ID: 404 (NodeStreamOffset)
    const HWFloat<8,24> &id404in_input = id1669out_output[getCycle()%2];

    id404out_output = id404in_input;
  }
  HWFloat<8,24> id405out_output;

  { // Node ID: 405 (NodeStreamOffset)
    const HWFloat<8,24> &id405in_input = id1668out_output[getCycle()%2];

    id405out_output = id405in_input;
  }
  { // Node ID: 876 (NodeAdd)
    const HWFloat<8,24> &id876in_a = id404out_output;
    const HWFloat<8,24> &id876in_b = id405out_output;

    id876out_result[(getCycle()+14)%15] = (add_float(id876in_a,id876in_b));
  }
  { // Node ID: 881 (NodeAdd)
    const HWFloat<8,24> &id881in_a = id875out_result[getCycle()%15];
    const HWFloat<8,24> &id881in_b = id876out_result[getCycle()%15];

    id881out_result[(getCycle()+14)%15] = (add_float(id881in_a,id881in_b));
  }
  { // Node ID: 885 (NodeAdd)
    const HWFloat<8,24> &id885in_a = id884out_result[getCycle()%15];
    const HWFloat<8,24> &id885in_b = id881out_result[getCycle()%15];

    id885out_result[(getCycle()+14)%15] = (add_float(id885in_a,id885in_b));
  }
  { // Node ID: 990 (NodeAdd)
    const HWFloat<8,24> &id990in_a = id866out_result[getCycle()%15];
    const HWFloat<8,24> &id990in_b = id885out_result[getCycle()%15];

    id990out_result[(getCycle()+14)%15] = (add_float(id990in_a,id990in_b));
  }
  { // Node ID: 997 (NodeAdd)
    const HWFloat<8,24> &id997in_a = id989out_result[getCycle()%15];
    const HWFloat<8,24> &id997in_b = id990out_result[getCycle()%15];

    id997out_result[(getCycle()+14)%15] = (add_float(id997in_a,id997in_b));
  }
  HWFloat<8,24> id406out_output;

  { // Node ID: 406 (NodeStreamOffset)
    const HWFloat<8,24> &id406in_input = id1595out_output[getCycle()%2];

    id406out_output = id406in_input;
  }
  HWFloat<8,24> id407out_output;

  { // Node ID: 407 (NodeStreamOffset)
    const HWFloat<8,24> &id407in_input = id1594out_output[getCycle()%2];

    id407out_output = id407in_input;
  }
  { // Node ID: 886 (NodeAdd)
    const HWFloat<8,24> &id886in_a = id406out_output;
    const HWFloat<8,24> &id886in_b = id407out_output;

    id886out_result[(getCycle()+14)%15] = (add_float(id886in_a,id886in_b));
  }
  HWFloat<8,24> id408out_output;

  { // Node ID: 408 (NodeStreamOffset)
    const HWFloat<8,24> &id408in_input = id1593out_output[getCycle()%2];

    id408out_output = id408in_input;
  }
  HWFloat<8,24> id409out_output;

  { // Node ID: 409 (NodeStreamOffset)
    const HWFloat<8,24> &id409in_input = id1592out_output[getCycle()%2];

    id409out_output = id409in_input;
  }
  { // Node ID: 887 (NodeAdd)
    const HWFloat<8,24> &id887in_a = id408out_output;
    const HWFloat<8,24> &id887in_b = id409out_output;

    id887out_result[(getCycle()+14)%15] = (add_float(id887in_a,id887in_b));
  }
  { // Node ID: 896 (NodeAdd)
    const HWFloat<8,24> &id896in_a = id886out_result[getCycle()%15];
    const HWFloat<8,24> &id896in_b = id887out_result[getCycle()%15];

    id896out_result[(getCycle()+14)%15] = (add_float(id896in_a,id896in_b));
  }
  HWFloat<8,24> id410out_output;

  { // Node ID: 410 (NodeStreamOffset)
    const HWFloat<8,24> &id410in_input = id1591out_output[getCycle()%2];

    id410out_output = id410in_input;
  }
  HWFloat<8,24> id411out_output;

  { // Node ID: 411 (NodeStreamOffset)
    const HWFloat<8,24> &id411in_input = id1590out_output[getCycle()%2];

    id411out_output = id411in_input;
  }
  { // Node ID: 888 (NodeAdd)
    const HWFloat<8,24> &id888in_a = id410out_output;
    const HWFloat<8,24> &id888in_b = id411out_output;

    id888out_result[(getCycle()+14)%15] = (add_float(id888in_a,id888in_b));
  }
  HWFloat<8,24> id412out_output;

  { // Node ID: 412 (NodeStreamOffset)
    const HWFloat<8,24> &id412in_input = id1589out_output[getCycle()%2];

    id412out_output = id412in_input;
  }
  HWFloat<8,24> id413out_output;

  { // Node ID: 413 (NodeStreamOffset)
    const HWFloat<8,24> &id413in_input = id1588out_output[getCycle()%2];

    id413out_output = id413in_input;
  }
  { // Node ID: 889 (NodeAdd)
    const HWFloat<8,24> &id889in_a = id412out_output;
    const HWFloat<8,24> &id889in_b = id413out_output;

    id889out_result[(getCycle()+14)%15] = (add_float(id889in_a,id889in_b));
  }
  { // Node ID: 897 (NodeAdd)
    const HWFloat<8,24> &id897in_a = id888out_result[getCycle()%15];
    const HWFloat<8,24> &id897in_b = id889out_result[getCycle()%15];

    id897out_result[(getCycle()+14)%15] = (add_float(id897in_a,id897in_b));
  }
  { // Node ID: 901 (NodeAdd)
    const HWFloat<8,24> &id901in_a = id896out_result[getCycle()%15];
    const HWFloat<8,24> &id901in_b = id897out_result[getCycle()%15];

    id901out_result[(getCycle()+14)%15] = (add_float(id901in_a,id901in_b));
  }
  HWFloat<8,24> id414out_output;

  { // Node ID: 414 (NodeStreamOffset)
    const HWFloat<8,24> &id414in_input = id1587out_output[getCycle()%2];

    id414out_output = id414in_input;
  }
  HWFloat<8,24> id415out_output;

  { // Node ID: 415 (NodeStreamOffset)
    const HWFloat<8,24> &id415in_input = id1586out_output[getCycle()%2];

    id415out_output = id415in_input;
  }
  { // Node ID: 890 (NodeAdd)
    const HWFloat<8,24> &id890in_a = id414out_output;
    const HWFloat<8,24> &id890in_b = id415out_output;

    id890out_result[(getCycle()+14)%15] = (add_float(id890in_a,id890in_b));
  }
  HWFloat<8,24> id416out_output;

  { // Node ID: 416 (NodeStreamOffset)
    const HWFloat<8,24> &id416in_input = id1585out_output[getCycle()%2];

    id416out_output = id416in_input;
  }
  HWFloat<8,24> id417out_output;

  { // Node ID: 417 (NodeStreamOffset)
    const HWFloat<8,24> &id417in_input = id1584out_output[getCycle()%2];

    id417out_output = id417in_input;
  }
  { // Node ID: 891 (NodeAdd)
    const HWFloat<8,24> &id891in_a = id416out_output;
    const HWFloat<8,24> &id891in_b = id417out_output;

    id891out_result[(getCycle()+14)%15] = (add_float(id891in_a,id891in_b));
  }
  { // Node ID: 898 (NodeAdd)
    const HWFloat<8,24> &id898in_a = id890out_result[getCycle()%15];
    const HWFloat<8,24> &id898in_b = id891out_result[getCycle()%15];

    id898out_result[(getCycle()+14)%15] = (add_float(id898in_a,id898in_b));
  }
  HWFloat<8,24> id418out_output;

  { // Node ID: 418 (NodeStreamOffset)
    const HWFloat<8,24> &id418in_input = id1583out_output[getCycle()%2];

    id418out_output = id418in_input;
  }
  HWFloat<8,24> id419out_output;

  { // Node ID: 419 (NodeStreamOffset)
    const HWFloat<8,24> &id419in_input = id1582out_output[getCycle()%2];

    id419out_output = id419in_input;
  }
  { // Node ID: 892 (NodeAdd)
    const HWFloat<8,24> &id892in_a = id418out_output;
    const HWFloat<8,24> &id892in_b = id419out_output;

    id892out_result[(getCycle()+14)%15] = (add_float(id892in_a,id892in_b));
  }
  HWFloat<8,24> id420out_output;

  { // Node ID: 420 (NodeStreamOffset)
    const HWFloat<8,24> &id420in_input = id1581out_output[getCycle()%2];

    id420out_output = id420in_input;
  }
  HWFloat<8,24> id421out_output;

  { // Node ID: 421 (NodeStreamOffset)
    const HWFloat<8,24> &id421in_input = id1580out_output[getCycle()%6];

    id421out_output = id421in_input;
  }
  { // Node ID: 893 (NodeAdd)
    const HWFloat<8,24> &id893in_a = id420out_output;
    const HWFloat<8,24> &id893in_b = id421out_output;

    id893out_result[(getCycle()+14)%15] = (add_float(id893in_a,id893in_b));
  }
  { // Node ID: 899 (NodeAdd)
    const HWFloat<8,24> &id899in_a = id892out_result[getCycle()%15];
    const HWFloat<8,24> &id899in_b = id893out_result[getCycle()%15];

    id899out_result[(getCycle()+14)%15] = (add_float(id899in_a,id899in_b));
  }
  { // Node ID: 902 (NodeAdd)
    const HWFloat<8,24> &id902in_a = id898out_result[getCycle()%15];
    const HWFloat<8,24> &id902in_b = id899out_result[getCycle()%15];

    id902out_result[(getCycle()+14)%15] = (add_float(id902in_a,id902in_b));
  }
  { // Node ID: 903 (NodeAdd)
    const HWFloat<8,24> &id903in_a = id901out_result[getCycle()%15];
    const HWFloat<8,24> &id903in_b = id902out_result[getCycle()%15];

    id903out_result[(getCycle()+14)%15] = (add_float(id903in_a,id903in_b));
  }
  HWFloat<8,24> id422out_output;

  { // Node ID: 422 (NodeStreamOffset)
    const HWFloat<8,24> &id422in_input = id1599out_output[getCycle()%2];

    id422out_output = id422in_input;
  }
  HWFloat<8,24> id423out_output;

  { // Node ID: 423 (NodeStreamOffset)
    const HWFloat<8,24> &id423in_input = id1598out_output[getCycle()%2];

    id423out_output = id423in_input;
  }
  { // Node ID: 894 (NodeAdd)
    const HWFloat<8,24> &id894in_a = id422out_output;
    const HWFloat<8,24> &id894in_b = id423out_output;

    id894out_result[(getCycle()+14)%15] = (add_float(id894in_a,id894in_b));
  }
  HWFloat<8,24> id424out_output;

  { // Node ID: 424 (NodeStreamOffset)
    const HWFloat<8,24> &id424in_input = id1597out_output[getCycle()%2];

    id424out_output = id424in_input;
  }
  HWFloat<8,24> id425out_output;

  { // Node ID: 425 (NodeStreamOffset)
    const HWFloat<8,24> &id425in_input = id1596out_output[getCycle()%10];

    id425out_output = id425in_input;
  }
  { // Node ID: 895 (NodeAdd)
    const HWFloat<8,24> &id895in_a = id424out_output;
    const HWFloat<8,24> &id895in_b = id425out_output;

    id895out_result[(getCycle()+14)%15] = (add_float(id895in_a,id895in_b));
  }
  { // Node ID: 900 (NodeAdd)
    const HWFloat<8,24> &id900in_a = id894out_result[getCycle()%15];
    const HWFloat<8,24> &id900in_b = id895out_result[getCycle()%15];

    id900out_result[(getCycle()+14)%15] = (add_float(id900in_a,id900in_b));
  }
  { // Node ID: 904 (NodeAdd)
    const HWFloat<8,24> &id904in_a = id903out_result[getCycle()%15];
    const HWFloat<8,24> &id904in_b = id900out_result[getCycle()%15];

    id904out_result[(getCycle()+14)%15] = (add_float(id904in_a,id904in_b));
  }
  HWFloat<8,24> id426out_output;

  { // Node ID: 426 (NodeStreamOffset)
    const HWFloat<8,24> &id426in_input = id1579out_output[getCycle()%2];

    id426out_output = id426in_input;
  }
  HWFloat<8,24> id427out_output;

  { // Node ID: 427 (NodeStreamOffset)
    const HWFloat<8,24> &id427in_input = id1578out_output[getCycle()%2];

    id427out_output = id427in_input;
  }
  { // Node ID: 905 (NodeAdd)
    const HWFloat<8,24> &id905in_a = id426out_output;
    const HWFloat<8,24> &id905in_b = id427out_output;

    id905out_result[(getCycle()+14)%15] = (add_float(id905in_a,id905in_b));
  }
  HWFloat<8,24> id428out_output;

  { // Node ID: 428 (NodeStreamOffset)
    const HWFloat<8,24> &id428in_input = id1577out_output[getCycle()%2];

    id428out_output = id428in_input;
  }
  HWFloat<8,24> id429out_output;

  { // Node ID: 429 (NodeStreamOffset)
    const HWFloat<8,24> &id429in_input = id1576out_output[getCycle()%2];

    id429out_output = id429in_input;
  }
  { // Node ID: 906 (NodeAdd)
    const HWFloat<8,24> &id906in_a = id428out_output;
    const HWFloat<8,24> &id906in_b = id429out_output;

    id906out_result[(getCycle()+14)%15] = (add_float(id906in_a,id906in_b));
  }
  { // Node ID: 915 (NodeAdd)
    const HWFloat<8,24> &id915in_a = id905out_result[getCycle()%15];
    const HWFloat<8,24> &id915in_b = id906out_result[getCycle()%15];

    id915out_result[(getCycle()+14)%15] = (add_float(id915in_a,id915in_b));
  }
  HWFloat<8,24> id430out_output;

  { // Node ID: 430 (NodeStreamOffset)
    const HWFloat<8,24> &id430in_input = id1575out_output[getCycle()%2];

    id430out_output = id430in_input;
  }
  HWFloat<8,24> id431out_output;

  { // Node ID: 431 (NodeStreamOffset)
    const HWFloat<8,24> &id431in_input = id1574out_output[getCycle()%2];

    id431out_output = id431in_input;
  }
  { // Node ID: 907 (NodeAdd)
    const HWFloat<8,24> &id907in_a = id430out_output;
    const HWFloat<8,24> &id907in_b = id431out_output;

    id907out_result[(getCycle()+14)%15] = (add_float(id907in_a,id907in_b));
  }
  HWFloat<8,24> id432out_output;

  { // Node ID: 432 (NodeStreamOffset)
    const HWFloat<8,24> &id432in_input = id1573out_output[getCycle()%2];

    id432out_output = id432in_input;
  }
  HWFloat<8,24> id433out_output;

  { // Node ID: 433 (NodeStreamOffset)
    const HWFloat<8,24> &id433in_input = id1572out_output[getCycle()%2];

    id433out_output = id433in_input;
  }
  { // Node ID: 908 (NodeAdd)
    const HWFloat<8,24> &id908in_a = id432out_output;
    const HWFloat<8,24> &id908in_b = id433out_output;

    id908out_result[(getCycle()+14)%15] = (add_float(id908in_a,id908in_b));
  }
  { // Node ID: 916 (NodeAdd)
    const HWFloat<8,24> &id916in_a = id907out_result[getCycle()%15];
    const HWFloat<8,24> &id916in_b = id908out_result[getCycle()%15];

    id916out_result[(getCycle()+14)%15] = (add_float(id916in_a,id916in_b));
  }
  { // Node ID: 920 (NodeAdd)
    const HWFloat<8,24> &id920in_a = id915out_result[getCycle()%15];
    const HWFloat<8,24> &id920in_b = id916out_result[getCycle()%15];

    id920out_result[(getCycle()+14)%15] = (add_float(id920in_a,id920in_b));
  }
  HWFloat<8,24> id434out_output;

  { // Node ID: 434 (NodeStreamOffset)
    const HWFloat<8,24> &id434in_input = id1571out_output[getCycle()%2];

    id434out_output = id434in_input;
  }
  HWFloat<8,24> id435out_output;

  { // Node ID: 435 (NodeStreamOffset)
    const HWFloat<8,24> &id435in_input = id1570out_output[getCycle()%2];

    id435out_output = id435in_input;
  }
  { // Node ID: 909 (NodeAdd)
    const HWFloat<8,24> &id909in_a = id434out_output;
    const HWFloat<8,24> &id909in_b = id435out_output;

    id909out_result[(getCycle()+14)%15] = (add_float(id909in_a,id909in_b));
  }
  HWFloat<8,24> id436out_output;

  { // Node ID: 436 (NodeStreamOffset)
    const HWFloat<8,24> &id436in_input = id1569out_output[getCycle()%2];

    id436out_output = id436in_input;
  }
  HWFloat<8,24> id437out_output;

  { // Node ID: 437 (NodeStreamOffset)
    const HWFloat<8,24> &id437in_input = id1568out_output[getCycle()%2];

    id437out_output = id437in_input;
  }
  { // Node ID: 910 (NodeAdd)
    const HWFloat<8,24> &id910in_a = id436out_output;
    const HWFloat<8,24> &id910in_b = id437out_output;

    id910out_result[(getCycle()+14)%15] = (add_float(id910in_a,id910in_b));
  }
  { // Node ID: 917 (NodeAdd)
    const HWFloat<8,24> &id917in_a = id909out_result[getCycle()%15];
    const HWFloat<8,24> &id917in_b = id910out_result[getCycle()%15];

    id917out_result[(getCycle()+14)%15] = (add_float(id917in_a,id917in_b));
  }
  HWFloat<8,24> id438out_output;

  { // Node ID: 438 (NodeStreamOffset)
    const HWFloat<8,24> &id438in_input = id1567out_output[getCycle()%2];

    id438out_output = id438in_input;
  }
  HWFloat<8,24> id439out_output;

  { // Node ID: 439 (NodeStreamOffset)
    const HWFloat<8,24> &id439in_input = id1566out_output[getCycle()%2];

    id439out_output = id439in_input;
  }
  { // Node ID: 911 (NodeAdd)
    const HWFloat<8,24> &id911in_a = id438out_output;
    const HWFloat<8,24> &id911in_b = id439out_output;

    id911out_result[(getCycle()+14)%15] = (add_float(id911in_a,id911in_b));
  }
  HWFloat<8,24> id440out_output;

  { // Node ID: 440 (NodeStreamOffset)
    const HWFloat<8,24> &id440in_input = id1565out_output[getCycle()%2];

    id440out_output = id440in_input;
  }
  HWFloat<8,24> id441out_output;

  { // Node ID: 441 (NodeStreamOffset)
    const HWFloat<8,24> &id441in_input = id1564out_output[getCycle()%4];

    id441out_output = id441in_input;
  }
  { // Node ID: 912 (NodeAdd)
    const HWFloat<8,24> &id912in_a = id440out_output;
    const HWFloat<8,24> &id912in_b = id441out_output;

    id912out_result[(getCycle()+14)%15] = (add_float(id912in_a,id912in_b));
  }
  { // Node ID: 918 (NodeAdd)
    const HWFloat<8,24> &id918in_a = id911out_result[getCycle()%15];
    const HWFloat<8,24> &id918in_b = id912out_result[getCycle()%15];

    id918out_result[(getCycle()+14)%15] = (add_float(id918in_a,id918in_b));
  }
  { // Node ID: 921 (NodeAdd)
    const HWFloat<8,24> &id921in_a = id917out_result[getCycle()%15];
    const HWFloat<8,24> &id921in_b = id918out_result[getCycle()%15];

    id921out_result[(getCycle()+14)%15] = (add_float(id921in_a,id921in_b));
  }
  { // Node ID: 922 (NodeAdd)
    const HWFloat<8,24> &id922in_a = id920out_result[getCycle()%15];
    const HWFloat<8,24> &id922in_b = id921out_result[getCycle()%15];

    id922out_result[(getCycle()+14)%15] = (add_float(id922in_a,id922in_b));
  }
  HWFloat<8,24> id442out_output;

  { // Node ID: 442 (NodeStreamOffset)
    const HWFloat<8,24> &id442in_input = id1587out_output[getCycle()%2];

    id442out_output = id442in_input;
  }
  HWFloat<8,24> id443out_output;

  { // Node ID: 443 (NodeStreamOffset)
    const HWFloat<8,24> &id443in_input = id1586out_output[getCycle()%2];

    id443out_output = id443in_input;
  }
  { // Node ID: 913 (NodeAdd)
    const HWFloat<8,24> &id913in_a = id442out_output;
    const HWFloat<8,24> &id913in_b = id443out_output;

    id913out_result[(getCycle()+14)%15] = (add_float(id913in_a,id913in_b));
  }
  HWFloat<8,24> id444out_output;

  { // Node ID: 444 (NodeStreamOffset)
    const HWFloat<8,24> &id444in_input = id1585out_output[getCycle()%2];

    id444out_output = id444in_input;
  }
  HWFloat<8,24> id445out_output;

  { // Node ID: 445 (NodeStreamOffset)
    const HWFloat<8,24> &id445in_input = id1584out_output[getCycle()%2];

    id445out_output = id445in_input;
  }
  { // Node ID: 914 (NodeAdd)
    const HWFloat<8,24> &id914in_a = id444out_output;
    const HWFloat<8,24> &id914in_b = id445out_output;

    id914out_result[(getCycle()+14)%15] = (add_float(id914in_a,id914in_b));
  }
  { // Node ID: 919 (NodeAdd)
    const HWFloat<8,24> &id919in_a = id913out_result[getCycle()%15];
    const HWFloat<8,24> &id919in_b = id914out_result[getCycle()%15];

    id919out_result[(getCycle()+14)%15] = (add_float(id919in_a,id919in_b));
  }
  { // Node ID: 923 (NodeAdd)
    const HWFloat<8,24> &id923in_a = id922out_result[getCycle()%15];
    const HWFloat<8,24> &id923in_b = id919out_result[getCycle()%15];

    id923out_result[(getCycle()+14)%15] = (add_float(id923in_a,id923in_b));
  }
  { // Node ID: 991 (NodeAdd)
    const HWFloat<8,24> &id991in_a = id904out_result[getCycle()%15];
    const HWFloat<8,24> &id991in_b = id923out_result[getCycle()%15];

    id991out_result[(getCycle()+14)%15] = (add_float(id991in_a,id991in_b));
  }
  HWFloat<8,24> id446out_output;

  { // Node ID: 446 (NodeStreamOffset)
    const HWFloat<8,24> &id446in_input = id1561out_output[getCycle()%2];

    id446out_output = id446in_input;
  }
  HWFloat<8,24> id447out_output;

  { // Node ID: 447 (NodeStreamOffset)
    const HWFloat<8,24> &id447in_input = id1560out_output[getCycle()%2];

    id447out_output = id447in_input;
  }
  { // Node ID: 924 (NodeAdd)
    const HWFloat<8,24> &id924in_a = id446out_output;
    const HWFloat<8,24> &id924in_b = id447out_output;

    id924out_result[(getCycle()+14)%15] = (add_float(id924in_a,id924in_b));
  }
  HWFloat<8,24> id448out_output;

  { // Node ID: 448 (NodeStreamOffset)
    const HWFloat<8,24> &id448in_input = id1559out_output[getCycle()%2];

    id448out_output = id448in_input;
  }
  HWFloat<8,24> id449out_output;

  { // Node ID: 449 (NodeStreamOffset)
    const HWFloat<8,24> &id449in_input = id1558out_output[getCycle()%2];

    id449out_output = id449in_input;
  }
  { // Node ID: 925 (NodeAdd)
    const HWFloat<8,24> &id925in_a = id448out_output;
    const HWFloat<8,24> &id925in_b = id449out_output;

    id925out_result[(getCycle()+14)%15] = (add_float(id925in_a,id925in_b));
  }
  { // Node ID: 934 (NodeAdd)
    const HWFloat<8,24> &id934in_a = id924out_result[getCycle()%15];
    const HWFloat<8,24> &id934in_b = id925out_result[getCycle()%15];

    id934out_result[(getCycle()+14)%15] = (add_float(id934in_a,id934in_b));
  }
  HWFloat<8,24> id450out_output;

  { // Node ID: 450 (NodeStreamOffset)
    const HWFloat<8,24> &id450in_input = id1557out_output[getCycle()%2];

    id450out_output = id450in_input;
  }
  HWFloat<8,24> id451out_output;

  { // Node ID: 451 (NodeStreamOffset)
    const HWFloat<8,24> &id451in_input = id1556out_output[getCycle()%2];

    id451out_output = id451in_input;
  }
  { // Node ID: 926 (NodeAdd)
    const HWFloat<8,24> &id926in_a = id450out_output;
    const HWFloat<8,24> &id926in_b = id451out_output;

    id926out_result[(getCycle()+14)%15] = (add_float(id926in_a,id926in_b));
  }
  HWFloat<8,24> id452out_output;

  { // Node ID: 452 (NodeStreamOffset)
    const HWFloat<8,24> &id452in_input = id1555out_output[getCycle()%2];

    id452out_output = id452in_input;
  }
  HWFloat<8,24> id453out_output;

  { // Node ID: 453 (NodeStreamOffset)
    const HWFloat<8,24> &id453in_input = id1554out_output[getCycle()%2];

    id453out_output = id453in_input;
  }
  { // Node ID: 927 (NodeAdd)
    const HWFloat<8,24> &id927in_a = id452out_output;
    const HWFloat<8,24> &id927in_b = id453out_output;

    id927out_result[(getCycle()+14)%15] = (add_float(id927in_a,id927in_b));
  }
  { // Node ID: 935 (NodeAdd)
    const HWFloat<8,24> &id935in_a = id926out_result[getCycle()%15];
    const HWFloat<8,24> &id935in_b = id927out_result[getCycle()%15];

    id935out_result[(getCycle()+14)%15] = (add_float(id935in_a,id935in_b));
  }
  { // Node ID: 939 (NodeAdd)
    const HWFloat<8,24> &id939in_a = id934out_result[getCycle()%15];
    const HWFloat<8,24> &id939in_b = id935out_result[getCycle()%15];

    id939out_result[(getCycle()+14)%15] = (add_float(id939in_a,id939in_b));
  }
  HWFloat<8,24> id454out_output;

  { // Node ID: 454 (NodeStreamOffset)
    const HWFloat<8,24> &id454in_input = id1553out_output[getCycle()%2];

    id454out_output = id454in_input;
  }
  HWFloat<8,24> id455out_output;

  { // Node ID: 455 (NodeStreamOffset)
    const HWFloat<8,24> &id455in_input = id1552out_output[getCycle()%2];

    id455out_output = id455in_input;
  }
  { // Node ID: 928 (NodeAdd)
    const HWFloat<8,24> &id928in_a = id454out_output;
    const HWFloat<8,24> &id928in_b = id455out_output;

    id928out_result[(getCycle()+14)%15] = (add_float(id928in_a,id928in_b));
  }
  HWFloat<8,24> id456out_output;

  { // Node ID: 456 (NodeStreamOffset)
    const HWFloat<8,24> &id456in_input = id1551out_output[getCycle()%2];

    id456out_output = id456in_input;
  }
  HWFloat<8,24> id457out_output;

  { // Node ID: 457 (NodeStreamOffset)
    const HWFloat<8,24> &id457in_input = id1550out_output[getCycle()%2];

    id457out_output = id457in_input;
  }
  { // Node ID: 929 (NodeAdd)
    const HWFloat<8,24> &id929in_a = id456out_output;
    const HWFloat<8,24> &id929in_b = id457out_output;

    id929out_result[(getCycle()+14)%15] = (add_float(id929in_a,id929in_b));
  }
  { // Node ID: 936 (NodeAdd)
    const HWFloat<8,24> &id936in_a = id928out_result[getCycle()%15];
    const HWFloat<8,24> &id936in_b = id929out_result[getCycle()%15];

    id936out_result[(getCycle()+14)%15] = (add_float(id936in_a,id936in_b));
  }
  HWFloat<8,24> id458out_output;

  { // Node ID: 458 (NodeStreamOffset)
    const HWFloat<8,24> &id458in_input = id1549out_output[getCycle()%2];

    id458out_output = id458in_input;
  }
  HWFloat<8,24> id459out_output;

  { // Node ID: 459 (NodeStreamOffset)
    const HWFloat<8,24> &id459in_input = id1548out_output[getCycle()%2];

    id459out_output = id459in_input;
  }
  { // Node ID: 930 (NodeAdd)
    const HWFloat<8,24> &id930in_a = id458out_output;
    const HWFloat<8,24> &id930in_b = id459out_output;

    id930out_result[(getCycle()+14)%15] = (add_float(id930in_a,id930in_b));
  }
  HWFloat<8,24> id460out_output;

  { // Node ID: 460 (NodeStreamOffset)
    const HWFloat<8,24> &id460in_input = id1547out_output[getCycle()%2];

    id460out_output = id460in_input;
  }
  HWFloat<8,24> id461out_output;

  { // Node ID: 461 (NodeStreamOffset)
    const HWFloat<8,24> &id461in_input = id1546out_output[getCycle()%6];

    id461out_output = id461in_input;
  }
  { // Node ID: 931 (NodeAdd)
    const HWFloat<8,24> &id931in_a = id460out_output;
    const HWFloat<8,24> &id931in_b = id461out_output;

    id931out_result[(getCycle()+14)%15] = (add_float(id931in_a,id931in_b));
  }
  { // Node ID: 937 (NodeAdd)
    const HWFloat<8,24> &id937in_a = id930out_result[getCycle()%15];
    const HWFloat<8,24> &id937in_b = id931out_result[getCycle()%15];

    id937out_result[(getCycle()+14)%15] = (add_float(id937in_a,id937in_b));
  }
  { // Node ID: 940 (NodeAdd)
    const HWFloat<8,24> &id940in_a = id936out_result[getCycle()%15];
    const HWFloat<8,24> &id940in_b = id937out_result[getCycle()%15];

    id940out_result[(getCycle()+14)%15] = (add_float(id940in_a,id940in_b));
  }
  { // Node ID: 941 (NodeAdd)
    const HWFloat<8,24> &id941in_a = id939out_result[getCycle()%15];
    const HWFloat<8,24> &id941in_b = id940out_result[getCycle()%15];

    id941out_result[(getCycle()+14)%15] = (add_float(id941in_a,id941in_b));
  }
  HWFloat<8,24> id462out_output;

  { // Node ID: 462 (NodeStreamOffset)
    const HWFloat<8,24> &id462in_input = id1571out_output[getCycle()%2];

    id462out_output = id462in_input;
  }
  HWFloat<8,24> id463out_output;

  { // Node ID: 463 (NodeStreamOffset)
    const HWFloat<8,24> &id463in_input = id1570out_output[getCycle()%2];

    id463out_output = id463in_input;
  }
  { // Node ID: 932 (NodeAdd)
    const HWFloat<8,24> &id932in_a = id462out_output;
    const HWFloat<8,24> &id932in_b = id463out_output;

    id932out_result[(getCycle()+14)%15] = (add_float(id932in_a,id932in_b));
  }
  HWFloat<8,24> id464out_output;

  { // Node ID: 464 (NodeStreamOffset)
    const HWFloat<8,24> &id464in_input = id1569out_output[getCycle()%2];

    id464out_output = id464in_input;
  }
  HWFloat<8,24> id465out_output;

  { // Node ID: 465 (NodeStreamOffset)
    const HWFloat<8,24> &id465in_input = id1568out_output[getCycle()%2];

    id465out_output = id465in_input;
  }
  { // Node ID: 933 (NodeAdd)
    const HWFloat<8,24> &id933in_a = id464out_output;
    const HWFloat<8,24> &id933in_b = id465out_output;

    id933out_result[(getCycle()+14)%15] = (add_float(id933in_a,id933in_b));
  }
  { // Node ID: 938 (NodeAdd)
    const HWFloat<8,24> &id938in_a = id932out_result[getCycle()%15];
    const HWFloat<8,24> &id938in_b = id933out_result[getCycle()%15];

    id938out_result[(getCycle()+14)%15] = (add_float(id938in_a,id938in_b));
  }
  { // Node ID: 942 (NodeAdd)
    const HWFloat<8,24> &id942in_a = id941out_result[getCycle()%15];
    const HWFloat<8,24> &id942in_b = id938out_result[getCycle()%15];

    id942out_result[(getCycle()+14)%15] = (add_float(id942in_a,id942in_b));
  }
  HWFloat<8,24> id466out_output;

  { // Node ID: 466 (NodeStreamOffset)
    const HWFloat<8,24> &id466in_input = id1545out_output[getCycle()%2];

    id466out_output = id466in_input;
  }
  HWFloat<8,24> id467out_output;

  { // Node ID: 467 (NodeStreamOffset)
    const HWFloat<8,24> &id467in_input = id1544out_output[getCycle()%2];

    id467out_output = id467in_input;
  }
  { // Node ID: 943 (NodeAdd)
    const HWFloat<8,24> &id943in_a = id466out_output;
    const HWFloat<8,24> &id943in_b = id467out_output;

    id943out_result[(getCycle()+14)%15] = (add_float(id943in_a,id943in_b));
  }
  HWFloat<8,24> id468out_output;

  { // Node ID: 468 (NodeStreamOffset)
    const HWFloat<8,24> &id468in_input = id1543out_output[getCycle()%2];

    id468out_output = id468in_input;
  }
  HWFloat<8,24> id469out_output;

  { // Node ID: 469 (NodeStreamOffset)
    const HWFloat<8,24> &id469in_input = id1542out_output[getCycle()%2];

    id469out_output = id469in_input;
  }
  { // Node ID: 944 (NodeAdd)
    const HWFloat<8,24> &id944in_a = id468out_output;
    const HWFloat<8,24> &id944in_b = id469out_output;

    id944out_result[(getCycle()+14)%15] = (add_float(id944in_a,id944in_b));
  }
  { // Node ID: 953 (NodeAdd)
    const HWFloat<8,24> &id953in_a = id943out_result[getCycle()%15];
    const HWFloat<8,24> &id953in_b = id944out_result[getCycle()%15];

    id953out_result[(getCycle()+14)%15] = (add_float(id953in_a,id953in_b));
  }
  HWFloat<8,24> id470out_output;

  { // Node ID: 470 (NodeStreamOffset)
    const HWFloat<8,24> &id470in_input = id1541out_output[getCycle()%2];

    id470out_output = id470in_input;
  }
  HWFloat<8,24> id471out_output;

  { // Node ID: 471 (NodeStreamOffset)
    const HWFloat<8,24> &id471in_input = id1540out_output[getCycle()%2];

    id471out_output = id471in_input;
  }
  { // Node ID: 945 (NodeAdd)
    const HWFloat<8,24> &id945in_a = id470out_output;
    const HWFloat<8,24> &id945in_b = id471out_output;

    id945out_result[(getCycle()+14)%15] = (add_float(id945in_a,id945in_b));
  }
  HWFloat<8,24> id472out_output;

  { // Node ID: 472 (NodeStreamOffset)
    const HWFloat<8,24> &id472in_input = id1539out_output[getCycle()%2];

    id472out_output = id472in_input;
  }
  HWFloat<8,24> id473out_output;

  { // Node ID: 473 (NodeStreamOffset)
    const HWFloat<8,24> &id473in_input = id1538out_output[getCycle()%2];

    id473out_output = id473in_input;
  }
  { // Node ID: 946 (NodeAdd)
    const HWFloat<8,24> &id946in_a = id472out_output;
    const HWFloat<8,24> &id946in_b = id473out_output;

    id946out_result[(getCycle()+14)%15] = (add_float(id946in_a,id946in_b));
  }
  { // Node ID: 954 (NodeAdd)
    const HWFloat<8,24> &id954in_a = id945out_result[getCycle()%15];
    const HWFloat<8,24> &id954in_b = id946out_result[getCycle()%15];

    id954out_result[(getCycle()+14)%15] = (add_float(id954in_a,id954in_b));
  }
  { // Node ID: 958 (NodeAdd)
    const HWFloat<8,24> &id958in_a = id953out_result[getCycle()%15];
    const HWFloat<8,24> &id958in_b = id954out_result[getCycle()%15];

    id958out_result[(getCycle()+14)%15] = (add_float(id958in_a,id958in_b));
  }
  HWFloat<8,24> id474out_output;

  { // Node ID: 474 (NodeStreamOffset)
    const HWFloat<8,24> &id474in_input = id1537out_output[getCycle()%2];

    id474out_output = id474in_input;
  }
  HWFloat<8,24> id475out_output;

  { // Node ID: 475 (NodeStreamOffset)
    const HWFloat<8,24> &id475in_input = id1536out_output[getCycle()%2];

    id475out_output = id475in_input;
  }
  { // Node ID: 947 (NodeAdd)
    const HWFloat<8,24> &id947in_a = id474out_output;
    const HWFloat<8,24> &id947in_b = id475out_output;

    id947out_result[(getCycle()+14)%15] = (add_float(id947in_a,id947in_b));
  }
  HWFloat<8,24> id476out_output;

  { // Node ID: 476 (NodeStreamOffset)
    const HWFloat<8,24> &id476in_input = id1535out_output[getCycle()%2];

    id476out_output = id476in_input;
  }
  HWFloat<8,24> id477out_output;

  { // Node ID: 477 (NodeStreamOffset)
    const HWFloat<8,24> &id477in_input = id1534out_output[getCycle()%2];

    id477out_output = id477in_input;
  }
  { // Node ID: 948 (NodeAdd)
    const HWFloat<8,24> &id948in_a = id476out_output;
    const HWFloat<8,24> &id948in_b = id477out_output;

    id948out_result[(getCycle()+14)%15] = (add_float(id948in_a,id948in_b));
  }
  { // Node ID: 955 (NodeAdd)
    const HWFloat<8,24> &id955in_a = id947out_result[getCycle()%15];
    const HWFloat<8,24> &id955in_b = id948out_result[getCycle()%15];

    id955out_result[(getCycle()+14)%15] = (add_float(id955in_a,id955in_b));
  }
  HWFloat<8,24> id478out_output;

  { // Node ID: 478 (NodeStreamOffset)
    const HWFloat<8,24> &id478in_input = id1533out_output[getCycle()%2];

    id478out_output = id478in_input;
  }
  HWFloat<8,24> id479out_output;

  { // Node ID: 479 (NodeStreamOffset)
    const HWFloat<8,24> &id479in_input = id1532out_output[getCycle()%2];

    id479out_output = id479in_input;
  }
  { // Node ID: 949 (NodeAdd)
    const HWFloat<8,24> &id949in_a = id478out_output;
    const HWFloat<8,24> &id949in_b = id479out_output;

    id949out_result[(getCycle()+14)%15] = (add_float(id949in_a,id949in_b));
  }
  HWFloat<8,24> id480out_output;

  { // Node ID: 480 (NodeStreamOffset)
    const HWFloat<8,24> &id480in_input = id1346out_output[getCycle()%2];

    id480out_output = id480in_input;
  }
  HWFloat<8,24> id481out_output;

  { // Node ID: 481 (NodeStreamOffset)
    const HWFloat<8,24> &id481in_input = id2out_data;

    id481out_output = id481in_input;
  }
  { // Node ID: 950 (NodeAdd)
    const HWFloat<8,24> &id950in_a = id480out_output;
    const HWFloat<8,24> &id950in_b = id481out_output;

    id950out_result[(getCycle()+14)%15] = (add_float(id950in_a,id950in_b));
  }
  { // Node ID: 956 (NodeAdd)
    const HWFloat<8,24> &id956in_a = id949out_result[getCycle()%15];
    const HWFloat<8,24> &id956in_b = id950out_result[getCycle()%15];

    id956out_result[(getCycle()+14)%15] = (add_float(id956in_a,id956in_b));
  }
  { // Node ID: 959 (NodeAdd)
    const HWFloat<8,24> &id959in_a = id955out_result[getCycle()%15];
    const HWFloat<8,24> &id959in_b = id956out_result[getCycle()%15];

    id959out_result[(getCycle()+14)%15] = (add_float(id959in_a,id959in_b));
  }
  { // Node ID: 960 (NodeAdd)
    const HWFloat<8,24> &id960in_a = id958out_result[getCycle()%15];
    const HWFloat<8,24> &id960in_b = id959out_result[getCycle()%15];

    id960out_result[(getCycle()+14)%15] = (add_float(id960in_a,id960in_b));
  }
  HWFloat<8,24> id482out_output;

  { // Node ID: 482 (NodeStreamOffset)
    const HWFloat<8,24> &id482in_input = id1553out_output[getCycle()%2];

    id482out_output = id482in_input;
  }
  HWFloat<8,24> id483out_output;

  { // Node ID: 483 (NodeStreamOffset)
    const HWFloat<8,24> &id483in_input = id1552out_output[getCycle()%2];

    id483out_output = id483in_input;
  }
  { // Node ID: 951 (NodeAdd)
    const HWFloat<8,24> &id951in_a = id482out_output;
    const HWFloat<8,24> &id951in_b = id483out_output;

    id951out_result[(getCycle()+14)%15] = (add_float(id951in_a,id951in_b));
  }
  HWFloat<8,24> id484out_output;

  { // Node ID: 484 (NodeStreamOffset)
    const HWFloat<8,24> &id484in_input = id1551out_output[getCycle()%2];

    id484out_output = id484in_input;
  }
  HWFloat<8,24> id485out_output;

  { // Node ID: 485 (NodeStreamOffset)
    const HWFloat<8,24> &id485in_input = id1550out_output[getCycle()%2];

    id485out_output = id485in_input;
  }
  { // Node ID: 952 (NodeAdd)
    const HWFloat<8,24> &id952in_a = id484out_output;
    const HWFloat<8,24> &id952in_b = id485out_output;

    id952out_result[(getCycle()+14)%15] = (add_float(id952in_a,id952in_b));
  }
  { // Node ID: 957 (NodeAdd)
    const HWFloat<8,24> &id957in_a = id951out_result[getCycle()%15];
    const HWFloat<8,24> &id957in_b = id952out_result[getCycle()%15];

    id957out_result[(getCycle()+14)%15] = (add_float(id957in_a,id957in_b));
  }
  { // Node ID: 961 (NodeAdd)
    const HWFloat<8,24> &id961in_a = id960out_result[getCycle()%15];
    const HWFloat<8,24> &id961in_b = id957out_result[getCycle()%15];

    id961out_result[(getCycle()+14)%15] = (add_float(id961in_a,id961in_b));
  }
  { // Node ID: 992 (NodeAdd)
    const HWFloat<8,24> &id992in_a = id942out_result[getCycle()%15];
    const HWFloat<8,24> &id992in_b = id961out_result[getCycle()%15];

    id992out_result[(getCycle()+14)%15] = (add_float(id992in_a,id992in_b));
  }
  { // Node ID: 998 (NodeAdd)
    const HWFloat<8,24> &id998in_a = id991out_result[getCycle()%15];
    const HWFloat<8,24> &id998in_b = id992out_result[getCycle()%15];

    id998out_result[(getCycle()+14)%15] = (add_float(id998in_a,id998in_b));
  }
  { // Node ID: 1001 (NodeAdd)
    const HWFloat<8,24> &id1001in_a = id997out_result[getCycle()%15];
    const HWFloat<8,24> &id1001in_b = id998out_result[getCycle()%15];

    id1001out_result[(getCycle()+14)%15] = (add_float(id1001in_a,id1001in_b));
  }
  HWFloat<8,24> id486out_output;

  { // Node ID: 486 (NodeStreamOffset)
    const HWFloat<8,24> &id486in_input = id1563out_output[getCycle()%2];

    id486out_output = id486in_input;
  }
  HWFloat<8,24> id487out_output;

  { // Node ID: 487 (NodeStreamOffset)
    const HWFloat<8,24> &id487in_input = id1562out_output[getCycle()%2];

    id487out_output = id487in_input;
  }
  { // Node ID: 962 (NodeAdd)
    const HWFloat<8,24> &id962in_a = id486out_output;
    const HWFloat<8,24> &id962in_b = id487out_output;

    id962out_result[(getCycle()+14)%15] = (add_float(id962in_a,id962in_b));
  }
  HWFloat<8,24> id488out_output;

  { // Node ID: 488 (NodeStreamOffset)
    const HWFloat<8,24> &id488in_input = id1561out_output[getCycle()%2];

    id488out_output = id488in_input;
  }
  HWFloat<8,24> id489out_output;

  { // Node ID: 489 (NodeStreamOffset)
    const HWFloat<8,24> &id489in_input = id1560out_output[getCycle()%2];

    id489out_output = id489in_input;
  }
  { // Node ID: 963 (NodeAdd)
    const HWFloat<8,24> &id963in_a = id488out_output;
    const HWFloat<8,24> &id963in_b = id489out_output;

    id963out_result[(getCycle()+14)%15] = (add_float(id963in_a,id963in_b));
  }
  { // Node ID: 972 (NodeAdd)
    const HWFloat<8,24> &id972in_a = id962out_result[getCycle()%15];
    const HWFloat<8,24> &id972in_b = id963out_result[getCycle()%15];

    id972out_result[(getCycle()+14)%15] = (add_float(id972in_a,id972in_b));
  }
  HWFloat<8,24> id490out_output;

  { // Node ID: 490 (NodeStreamOffset)
    const HWFloat<8,24> &id490in_input = id1559out_output[getCycle()%2];

    id490out_output = id490in_input;
  }
  HWFloat<8,24> id491out_output;

  { // Node ID: 491 (NodeStreamOffset)
    const HWFloat<8,24> &id491in_input = id1558out_output[getCycle()%2];

    id491out_output = id491in_input;
  }
  { // Node ID: 964 (NodeAdd)
    const HWFloat<8,24> &id964in_a = id490out_output;
    const HWFloat<8,24> &id964in_b = id491out_output;

    id964out_result[(getCycle()+14)%15] = (add_float(id964in_a,id964in_b));
  }
  HWFloat<8,24> id492out_output;

  { // Node ID: 492 (NodeStreamOffset)
    const HWFloat<8,24> &id492in_input = id1557out_output[getCycle()%2];

    id492out_output = id492in_input;
  }
  HWFloat<8,24> id493out_output;

  { // Node ID: 493 (NodeStreamOffset)
    const HWFloat<8,24> &id493in_input = id1556out_output[getCycle()%2];

    id493out_output = id493in_input;
  }
  { // Node ID: 965 (NodeAdd)
    const HWFloat<8,24> &id965in_a = id492out_output;
    const HWFloat<8,24> &id965in_b = id493out_output;

    id965out_result[(getCycle()+14)%15] = (add_float(id965in_a,id965in_b));
  }
  { // Node ID: 973 (NodeAdd)
    const HWFloat<8,24> &id973in_a = id964out_result[getCycle()%15];
    const HWFloat<8,24> &id973in_b = id965out_result[getCycle()%15];

    id973out_result[(getCycle()+14)%15] = (add_float(id973in_a,id973in_b));
  }
  { // Node ID: 977 (NodeAdd)
    const HWFloat<8,24> &id977in_a = id972out_result[getCycle()%15];
    const HWFloat<8,24> &id977in_b = id973out_result[getCycle()%15];

    id977out_result[(getCycle()+14)%15] = (add_float(id977in_a,id977in_b));
  }
  HWFloat<8,24> id494out_output;

  { // Node ID: 494 (NodeStreamOffset)
    const HWFloat<8,24> &id494in_input = id1555out_output[getCycle()%2];

    id494out_output = id494in_input;
  }
  HWFloat<8,24> id495out_output;

  { // Node ID: 495 (NodeStreamOffset)
    const HWFloat<8,24> &id495in_input = id1554out_output[getCycle()%2];

    id495out_output = id495in_input;
  }
  { // Node ID: 966 (NodeAdd)
    const HWFloat<8,24> &id966in_a = id494out_output;
    const HWFloat<8,24> &id966in_b = id495out_output;

    id966out_result[(getCycle()+14)%15] = (add_float(id966in_a,id966in_b));
  }
  HWFloat<8,24> id496out_output;

  { // Node ID: 496 (NodeStreamOffset)
    const HWFloat<8,24> &id496in_input = id1553out_output[getCycle()%2];

    id496out_output = id496in_input;
  }
  HWFloat<8,24> id497out_output;

  { // Node ID: 497 (NodeStreamOffset)
    const HWFloat<8,24> &id497in_input = id1552out_output[getCycle()%2];

    id497out_output = id497in_input;
  }
  { // Node ID: 967 (NodeAdd)
    const HWFloat<8,24> &id967in_a = id496out_output;
    const HWFloat<8,24> &id967in_b = id497out_output;

    id967out_result[(getCycle()+14)%15] = (add_float(id967in_a,id967in_b));
  }
  { // Node ID: 974 (NodeAdd)
    const HWFloat<8,24> &id974in_a = id966out_result[getCycle()%15];
    const HWFloat<8,24> &id974in_b = id967out_result[getCycle()%15];

    id974out_result[(getCycle()+14)%15] = (add_float(id974in_a,id974in_b));
  }
  HWFloat<8,24> id498out_output;

  { // Node ID: 498 (NodeStreamOffset)
    const HWFloat<8,24> &id498in_input = id1551out_output[getCycle()%2];

    id498out_output = id498in_input;
  }
  HWFloat<8,24> id499out_output;

  { // Node ID: 499 (NodeStreamOffset)
    const HWFloat<8,24> &id499in_input = id1550out_output[getCycle()%2];

    id499out_output = id499in_input;
  }
  { // Node ID: 968 (NodeAdd)
    const HWFloat<8,24> &id968in_a = id498out_output;
    const HWFloat<8,24> &id968in_b = id499out_output;

    id968out_result[(getCycle()+14)%15] = (add_float(id968in_a,id968in_b));
  }
  HWFloat<8,24> id500out_output;

  { // Node ID: 500 (NodeStreamOffset)
    const HWFloat<8,24> &id500in_input = id1549out_output[getCycle()%2];

    id500out_output = id500in_input;
  }
  HWFloat<8,24> id501out_output;

  { // Node ID: 501 (NodeStreamOffset)
    const HWFloat<8,24> &id501in_input = id1548out_output[getCycle()%2];

    id501out_output = id501in_input;
  }
  { // Node ID: 969 (NodeAdd)
    const HWFloat<8,24> &id969in_a = id500out_output;
    const HWFloat<8,24> &id969in_b = id501out_output;

    id969out_result[(getCycle()+14)%15] = (add_float(id969in_a,id969in_b));
  }
  { // Node ID: 975 (NodeAdd)
    const HWFloat<8,24> &id975in_a = id968out_result[getCycle()%15];
    const HWFloat<8,24> &id975in_b = id969out_result[getCycle()%15];

    id975out_result[(getCycle()+14)%15] = (add_float(id975in_a,id975in_b));
  }
  { // Node ID: 978 (NodeAdd)
    const HWFloat<8,24> &id978in_a = id974out_result[getCycle()%15];
    const HWFloat<8,24> &id978in_b = id975out_result[getCycle()%15];

    id978out_result[(getCycle()+14)%15] = (add_float(id978in_a,id978in_b));
  }
  { // Node ID: 979 (NodeAdd)
    const HWFloat<8,24> &id979in_a = id977out_result[getCycle()%15];
    const HWFloat<8,24> &id979in_b = id978out_result[getCycle()%15];

    id979out_result[(getCycle()+14)%15] = (add_float(id979in_a,id979in_b));
  }
  HWFloat<8,24> id502out_output;

  { // Node ID: 502 (NodeStreamOffset)
    const HWFloat<8,24> &id502in_input = id1573out_output[getCycle()%2];

    id502out_output = id502in_input;
  }
  HWFloat<8,24> id503out_output;

  { // Node ID: 503 (NodeStreamOffset)
    const HWFloat<8,24> &id503in_input = id1572out_output[getCycle()%2];

    id503out_output = id503in_input;
  }
  { // Node ID: 970 (NodeAdd)
    const HWFloat<8,24> &id970in_a = id502out_output;
    const HWFloat<8,24> &id970in_b = id503out_output;

    id970out_result[(getCycle()+14)%15] = (add_float(id970in_a,id970in_b));
  }
  HWFloat<8,24> id504out_output;

  { // Node ID: 504 (NodeStreamOffset)
    const HWFloat<8,24> &id504in_input = id1571out_output[getCycle()%2];

    id504out_output = id504in_input;
  }
  HWFloat<8,24> id505out_output;

  { // Node ID: 505 (NodeStreamOffset)
    const HWFloat<8,24> &id505in_input = id1570out_output[getCycle()%2];

    id505out_output = id505in_input;
  }
  { // Node ID: 971 (NodeAdd)
    const HWFloat<8,24> &id971in_a = id504out_output;
    const HWFloat<8,24> &id971in_b = id505out_output;

    id971out_result[(getCycle()+14)%15] = (add_float(id971in_a,id971in_b));
  }
  { // Node ID: 976 (NodeAdd)
    const HWFloat<8,24> &id976in_a = id970out_result[getCycle()%15];
    const HWFloat<8,24> &id976in_b = id971out_result[getCycle()%15];

    id976out_result[(getCycle()+14)%15] = (add_float(id976in_a,id976in_b));
  }
  { // Node ID: 980 (NodeAdd)
    const HWFloat<8,24> &id980in_a = id979out_result[getCycle()%15];
    const HWFloat<8,24> &id980in_b = id976out_result[getCycle()%15];

    id980out_result[(getCycle()+14)%15] = (add_float(id980in_a,id980in_b));
  }
  { // Node ID: 1003 (NodeAdd)
    const HWFloat<8,24> &id1003in_a = id1001out_result[getCycle()%15];
    const HWFloat<8,24> &id1003in_b = id980out_result[getCycle()%15];

    id1003out_result[(getCycle()+14)%15] = (add_float(id1003in_a,id1003in_b));
  }
  { // Node ID: 1004 (NodeAdd)
    const HWFloat<8,24> &id1004in_a = id1531out_output[getCycle()%2061];
    const HWFloat<8,24> &id1004in_b = id1003out_result[getCycle()%15];

    id1004out_result[(getCycle()+14)%15] = (add_float(id1004in_a,id1004in_b));
  }
  if ( (getFillLevel() >= (4248l)) && (getFlushLevel() < (4248l)|| !isFlushingActive() ))
  { // Node ID: 1012 (NodeOutput)
    const HWOffsetFix<1,0,UNSIGNED> &id1012in_output_control = in_vars.id1011out_output;
    const HWFloat<8,24> &id1012in_data = id1004out_result[getCycle()%15];

    bool id1012x_1;

    (id1012x_1) = ((id1012in_output_control.getValueAsBool())&(!(((getFlushLevel())>=(4248l))&(isFlushingActive()))));
    if((id1012x_1)) {
      writeOutput(m_output, id1012in_data);
    }
  }
  { // Node ID: 1017 (NodeConstantRawBits)
  }
  { // Node ID: 1761 (NodeConstantRawBits)
  }
  { // Node ID: 1014 (NodeConstantRawBits)
  }
  if ( (getFillLevel() >= (4247l)))
  { // Node ID: 1015 (NodeCounterV1)
    const HWOffsetFix<1,0,UNSIGNED> &id1015in_enable = id1761out_value;
    const HWOffsetFix<49,0,UNSIGNED> &id1015in_max = id1014out_value;

    HWOffsetFix<49,0,UNSIGNED> id1015x_1;
    HWOffsetFix<1,0,UNSIGNED> id1015x_2;
    HWOffsetFix<1,0,UNSIGNED> id1015x_3;
    HWOffsetFix<49,0,UNSIGNED> id1015x_4t_1e_1;

    id1015out_count = (cast_fixed2fixed<48,0,UNSIGNED,TRUNCATE>((id1015st_count)));
    (id1015x_1) = (add_fixed<49,0,UNSIGNED,TRUNCATE>((id1015st_count),(c_hw_fix_49_0_uns_bits_2)));
    (id1015x_2) = (gte_fixed((id1015x_1),id1015in_max));
    (id1015x_3) = (and_fixed((id1015x_2),id1015in_enable));
    id1015out_wrap = (id1015x_3);
    if((id1015in_enable.getValueAsBool())) {
      if(((id1015x_3).getValueAsBool())) {
        (id1015st_count) = (c_hw_fix_49_0_uns_bits_1);
      }
      else {
        (id1015x_4t_1e_1) = (id1015x_1);
        (id1015st_count) = (id1015x_4t_1e_1);
      }
    }
    else {
    }
  }
  HWOffsetFix<48,0,UNSIGNED> id1016out_output;

  { // Node ID: 1016 (NodeStreamOffset)
    const HWOffsetFix<48,0,UNSIGNED> &id1016in_input = id1015out_count;

    id1016out_output = id1016in_input;
  }
  if ( (getFillLevel() >= (4248l)) && (getFlushLevel() < (4248l)|| !isFlushingActive() ))
  { // Node ID: 1018 (NodeOutputMappedReg)
    const HWOffsetFix<1,0,UNSIGNED> &id1018in_load = id1017out_value;
    const HWOffsetFix<48,0,UNSIGNED> &id1018in_data = id1016out_output;

    bool id1018x_1;

    (id1018x_1) = ((id1018in_load.getValueAsBool())&(!(((getFlushLevel())>=(4248l))&(isFlushingActive()))));
    if((id1018x_1)) {
      setMappedRegValue("current_run_cycle_count", id1018in_data);
    }
  }
  { // Node ID: 1760 (NodeConstantRawBits)
  }
  { // Node ID: 1020 (NodeConstantRawBits)
  }
  if ( (getFillLevel() >= (64l)))
  { // Node ID: 1021 (NodeCounterV1)
    const HWOffsetFix<1,0,UNSIGNED> &id1021in_enable = id1760out_value;
    const HWOffsetFix<49,0,UNSIGNED> &id1021in_max = id1020out_value;

    HWOffsetFix<49,0,UNSIGNED> id1021x_1;
    HWOffsetFix<1,0,UNSIGNED> id1021x_2;
    HWOffsetFix<1,0,UNSIGNED> id1021x_3;
    HWOffsetFix<49,0,UNSIGNED> id1021x_4t_1e_1;

    id1021out_count = (cast_fixed2fixed<48,0,UNSIGNED,TRUNCATE>((id1021st_count)));
    (id1021x_1) = (add_fixed<49,0,UNSIGNED,TRUNCATE>((id1021st_count),(c_hw_fix_49_0_uns_bits_2)));
    (id1021x_2) = (gte_fixed((id1021x_1),id1021in_max));
    (id1021x_3) = (and_fixed((id1021x_2),id1021in_enable));
    id1021out_wrap = (id1021x_3);
    if((id1021in_enable.getValueAsBool())) {
      if(((id1021x_3).getValueAsBool())) {
        (id1021st_count) = (c_hw_fix_49_0_uns_bits_1);
      }
      else {
        (id1021x_4t_1e_1) = (id1021x_1);
        (id1021st_count) = (id1021x_4t_1e_1);
      }
    }
    else {
    }
  }
  { // Node ID: 1023 (NodeInputMappedReg)
  }
  { // Node ID: 1024 (NodeEq)
    const HWOffsetFix<48,0,UNSIGNED> &id1024in_a = id1021out_count;
    const HWOffsetFix<48,0,UNSIGNED> &id1024in_b = id1023out_run_cycle_count;

    id1024out_result[(getCycle()+1)%2] = (eq_fixed(id1024in_a,id1024in_b));
  }
  if ( (getFillLevel() >= (65l)))
  { // Node ID: 1022 (NodeFlush)
    const HWOffsetFix<1,0,UNSIGNED> &id1022in_start = id1024out_result[getCycle()%2];

    if((id1022in_start.getValueAsBool())) {
      startFlushing();
    }
  }
  { // Node ID: 4 (NodeInputMappedReg)
  }
  { // Node ID: 5 (NodeInputMappedReg)
  }
};

};
