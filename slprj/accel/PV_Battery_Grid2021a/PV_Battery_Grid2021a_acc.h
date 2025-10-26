#ifndef RTW_HEADER_PV_Battery_Grid2021a_acc_h_
#define RTW_HEADER_PV_Battery_Grid2021a_acc_h_
#ifndef PV_Battery_Grid2021a_acc_COMMON_INCLUDES_
#define PV_Battery_Grid2021a_acc_COMMON_INCLUDES_
#include <stdlib.h>
#define S_FUNCTION_NAME simulink_only_sfcn
#define S_FUNCTION_LEVEL 2
#ifndef RTW_GENERATED_S_FUNCTION
#define RTW_GENERATED_S_FUNCTION
#endif
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#endif
#include "PV_Battery_Grid2021a_acc_types.h"
#include <stddef.h>
#include <string.h>
#include "rtGetInf.h"
#include "rt_nonfinite.h"
#include <float.h>
#include "mwmathutil.h"
#include "rt_defines.h"
#include "simstruc_types.h"
typedef struct { real_T B_13_0_0 ; real_T B_13_1_0 ; }
B_Subsystempi2delay_PV_Battery_Grid2021a_T ; typedef struct { int32_T
Subsystempi2delay_sysIdxToRun ; int8_T Subsystempi2delay_SubsysRanBC ;
boolean_T Subsystempi2delay_MODE ; char_T pad_Subsystempi2delay_MODE [ 2 ] ;
} DW_Subsystempi2delay_PV_Battery_Grid2021a_T ; typedef struct { real_T
B_14_0_0 ; real_T B_14_1_0 ; } B_Subsystem1_PV_Battery_Grid2021a_T ; typedef
struct { int32_T Subsystem1_sysIdxToRun ; int8_T Subsystem1_SubsysRanBC ;
boolean_T Subsystem1_MODE ; char_T pad_Subsystem1_MODE [ 2 ] ; }
DW_Subsystem1_PV_Battery_Grid2021a_T ; typedef struct { creal_T B_15_12_0 ;
real_T B_15_0_0 ; real_T B_15_1_0 ; real_T B_15_2_0 ; real_T B_15_5_0 ;
real_T B_15_6_0 ; real_T B_15_7_0 ; real_T B_15_8_0 ; real_T B_15_11_0 ;
real_T B_15_13_0 ; real_T B_15_14_0 ; real_T B_15_15_0 ; real_T B_15_16_0 ;
real_T B_15_17_0 ; real_T B_15_18_0 ; real_T B_15_19_0 ; real_T B_15_20_0 [ 3
] ; real_T B_15_21_0 [ 3 ] ; real_T B_15_24_0 [ 2 ] ; real_T B_15_0_0_m ;
real_T B_15_1_0_c ; real_T B_15_2_0_k ; real_T B_15_3_0 ; real_T B_12_0_0 ;
real_T B_12_1_0 ; real_T B_11_0_0 ; real_T B_11_1_0 ; uint8_T B_15_6_0_c ;
uint8_T B_15_8_0_b ; boolean_T B_15_3_0_p ; boolean_T B_15_9_0 ; char_T
pad_B_15_9_0 [ 4 ] ; B_Subsystem1_PV_Battery_Grid2021a_T Subsystem1 ;
B_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay ; }
B_AutomaticGainControl_PV_Battery_Grid2021a_T ; typedef struct { struct {
real_T modelTStart ; } VariableTransportDelay_RWORK ; struct { real_T
modelTStart ; } VariableTransportDelay_RWORK_m ; struct { void * TUbufferPtrs
[ 3 ] ; } VariableTransportDelay_PWORK ; struct { void * TUbufferPtrs [ 3 ] ;
} VariableTransportDelay_PWORK_o ; int32_T AutomaticGainControl_sysIdxToRun ;
int32_T TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_i ; struct { int_T Tail ; int_T
Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
VariableTransportDelay_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last
; int_T CircularBufSize ; int_T MaxNewBufSize ; }
VariableTransportDelay_IWORK_i ; int_T Saturation_MODE ; int8_T
AutomaticGainControl_SubsysRanBC ; boolean_T RelationalOperator_Mode ;
boolean_T RelationalOperator_Mode_b ; boolean_T AutomaticGainControl_MODE ;
char_T pad_AutomaticGainControl_MODE [ 4 ] ;
DW_Subsystem1_PV_Battery_Grid2021a_T Subsystem1 ;
DW_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay ; }
DW_AutomaticGainControl_PV_Battery_Grid2021a_T ; typedef struct { real_T
VariableTransportDelay_CSTATE_d ; real_T integrator_CSTATE_k ; real_T
VariableTransportDelay_CSTATE_c ; real_T integrator_CSTATE_nk ; }
X_AutomaticGainControl_PV_Battery_Grid2021a_T ; typedef struct { real_T
VariableTransportDelay_CSTATE_d ; real_T integrator_CSTATE_k ; real_T
VariableTransportDelay_CSTATE_c ; real_T integrator_CSTATE_nk ; }
XDot_AutomaticGainControl_PV_Battery_Grid2021a_T ; typedef struct { boolean_T
VariableTransportDelay_CSTATE_d ; boolean_T integrator_CSTATE_k ; boolean_T
VariableTransportDelay_CSTATE_c ; boolean_T integrator_CSTATE_nk ; }
XDis_AutomaticGainControl_PV_Battery_Grid2021a_T ; typedef struct { real_T
VariableTransportDelay_CSTATE_d ; real_T integrator_CSTATE_k ; real_T
VariableTransportDelay_CSTATE_c ; real_T integrator_CSTATE_nk ; }
XAbsTol_AutomaticGainControl_PV_Battery_Grid2021a_T ; typedef struct { real_T
VariableTransportDelay_CSTATE_d ; real_T integrator_CSTATE_k ; real_T
VariableTransportDelay_CSTATE_c ; real_T integrator_CSTATE_nk ; }
XPtMin_AutomaticGainControl_PV_Battery_Grid2021a_T ; typedef struct { real_T
VariableTransportDelay_CSTATE_d ; real_T integrator_CSTATE_k ; real_T
VariableTransportDelay_CSTATE_c ; real_T integrator_CSTATE_nk ; }
XPtMax_AutomaticGainControl_PV_Battery_Grid2021a_T ; typedef struct { real_T
RelationalOperator_RelopInput_ZC_n ; real_T
RelationalOperator_RelopInput_ZC_c ; real_T Saturation_UprLim_ZC ; real_T
Saturation_LwrLim_ZC ; } ZCV_AutomaticGainControl_PV_Battery_Grid2021a_T ;
typedef struct { ZCSigState RelationalOperator_RelopInput_ZCE_e ; ZCSigState
RelationalOperator_RelopInput_ZCE_ai ; ZCSigState Saturation_UprLim_ZCE ;
ZCSigState Saturation_LwrLim_ZCE ; }
ZCE_AutomaticGainControl_PV_Battery_Grid2021a_T ; typedef struct { creal_T
B_25_0_0 [ 3 ] ; creal_T B_25_1_0 ; creal_T B_25_2_0 ; }
B_NegSeqComputation_PV_Battery_Grid2021a_T ; typedef struct { int32_T
NegSeqComputation_sysIdxToRun ; int8_T NegSeqComputation_SubsysRanBC ;
boolean_T NegSeqComputation_MODE ; char_T pad_NegSeqComputation_MODE [ 2 ] ;
} DW_NegSeqComputation_PV_Battery_Grid2021a_T ; typedef struct { creal_T
B_27_0_0 ; creal_T B_27_1_0 ; } B_ZeroSeqComputation_PV_Battery_Grid2021a_T ;
typedef struct { int32_T ZeroSeqComputation_sysIdxToRun ; int8_T
ZeroSeqComputation_SubsysRanBC ; boolean_T ZeroSeqComputation_MODE ; char_T
pad_ZeroSeqComputation_MODE [ 2 ] ; }
DW_ZeroSeqComputation_PV_Battery_Grid2021a_T ; typedef struct { real_T
B_53_0_0 ; real_T B_53_1_0 ; } B_Subsystempi2delay_PV_Battery_Grid2021a_j_T ;
typedef struct { int32_T Subsystempi2delay_sysIdxToRun ; int8_T
Subsystempi2delay_SubsysRanBC ; char_T pad_Subsystempi2delay_SubsysRanBC [ 3
] ; } DW_Subsystempi2delay_PV_Battery_Grid2021a_k_T ; typedef struct { real_T
B_54_0_0 ; real_T B_54_1_0 ; } B_Subsystem1_PV_Battery_Grid2021a_g_T ;
typedef struct { int32_T Subsystem1_sysIdxToRun ; int8_T
Subsystem1_SubsysRanBC ; char_T pad_Subsystem1_SubsysRanBC [ 3 ] ; }
DW_Subsystem1_PV_Battery_Grid2021a_f_T ; typedef struct { real_T B_63_3_0 ;
real_T B_63_5_0 ; real_T B_63_6_0 ; real_T B_63_8_0 ; real_T B_63_11_0 ;
real_T B_63_22_0 ; real_T B_63_28_0 ; real_T B_63_35_0 ; real_T B_63_37_0 ;
real_T B_63_38_0 ; real_T B_63_42_0 ; real_T B_63_43_0 ; real_T B_63_44_0 ;
real_T B_63_45_0 ; real_T B_63_46_0 [ 38 ] ; real_T B_63_46_1 [ 13 ] ; real_T
B_63_59_0 ; real_T B_63_66_0 ; real_T B_63_68_0 ; real_T B_63_69_0 ; real_T
B_63_78_0 ; real_T B_63_79_0 ; real_T B_63_80_0 ; real_T B_63_81_0 ; real_T
B_63_98_0 ; real_T B_63_99_0 ; real_T B_63_100_0 ; real_T B_63_104_0 ; real_T
B_63_106_0 ; real_T B_63_107_0 ; real_T B_63_108_0 ; real_T B_63_110_0 ;
real_T B_63_113_0 ; real_T B_63_114_0 ; real_T B_63_117_0 ; real_T B_63_119_0
; real_T B_63_121_0 ; real_T B_63_122_0 ; real_T B_63_125_0 ; real_T
B_63_127_0 ; real_T B_63_129_0 ; real_T B_63_135_0 ; real_T B_63_138_0 ;
real_T B_63_140_0 ; real_T B_63_142_0 ; real_T B_63_143_0 ; real_T B_63_146_0
; real_T B_63_148_0 ; real_T B_63_150_0 ; real_T B_63_156_0 ; real_T
B_63_159_0 ; real_T B_63_161_0 ; real_T B_63_163_0 ; real_T B_63_164_0 ;
real_T B_63_167_0 ; real_T B_63_169_0 ; real_T B_63_171_0 ; real_T B_63_181_0
; real_T B_63_184_0 ; real_T B_63_186_0 ; real_T B_63_188_0 ; real_T
B_63_189_0 ; real_T B_63_192_0 ; real_T B_63_194_0 ; real_T B_63_196_0 ;
real_T B_63_202_0 ; real_T B_63_205_0 ; real_T B_63_207_0 ; real_T B_63_209_0
; real_T B_63_210_0 ; real_T B_63_213_0 ; real_T B_63_215_0 ; real_T
B_63_217_0 ; real_T B_63_223_0 ; real_T B_63_226_0 ; real_T B_63_228_0 ;
real_T B_63_230_0 ; real_T B_63_231_0 ; real_T B_63_234_0 ; real_T B_63_236_0
; real_T B_63_238_0 ; real_T B_63_255_0 ; real_T B_63_257_0 ; real_T
B_63_259_0 ; real_T B_63_260_0 ; real_T B_63_261_0 ; real_T B_63_266_0 [ 3 ]
; real_T B_63_279_0 [ 6 ] ; real_T B_63_315_0 ; real_T B_63_319_0 ; real_T
B_63_327_0 ; real_T B_63_328_0 ; real_T B_63_330_0 ; real_T B_63_331_0 ;
real_T B_63_333_0 ; real_T B_63_337_0 ; real_T B_63_338_0 ; real_T B_63_340_0
; real_T B_63_342_0 ; real_T B_63_344_0 ; real_T B_63_346_0 ; real_T
B_63_348_0 ; real_T B_63_350_0 ; real_T B_63_352_0 ; real_T B_63_354_0 ;
real_T B_63_356_0 ; real_T B_63_358_0 ; real_T B_63_360_0 ; real_T B_63_362_0
; real_T B_63_366_0 ; real_T B_63_369_0 ; real_T B_63_372_0 ; real_T
B_63_374_0 ; real_T B_63_376_0 ; real_T B_63_378_0 ; real_T B_63_381_0 ;
real_T B_63_385_0 ; real_T B_63_386_0 ; real_T B_63_387_0 ; real_T B_63_391_0
; real_T B_63_393_0 ; real_T B_63_394_0 ; real_T B_63_397_0 [ 3 ] ; real_T
B_63_398_0 ; real_T B_63_399_0 ; real_T B_63_400_0 ; real_T B_63_403_0 [ 2 ]
; real_T B_63_406_0 [ 3 ] ; real_T B_63_409_0 [ 2 ] ; real_T B_63_412_0 [ 3 ]
; real_T B_63_414_0 ; real_T B_63_416_0 ; real_T B_63_420_0 ; real_T
B_63_423_0 ; real_T B_63_427_0 ; real_T B_63_429_0 ; real_T B_63_438_0 [ 2 ]
; real_T B_63_476_0 [ 3 ] ; real_T B_63_478_0 ; real_T B_63_480_0 ; real_T
B_63_484_0 ; real_T B_63_487_0 ; real_T B_63_492_0 [ 6 ] ; real_T B_63_494_0
; real_T B_63_495_0 ; real_T B_63_496_0 ; real_T B_63_497_0 ; real_T
B_63_498_0 ; real_T B_63_499_0 ; real_T B_63_500_0 ; real_T B_63_502_0 ;
real_T B_63_503_0 ; real_T B_63_504_0 ; real_T B_63_507_0 ; real_T B_63_508_0
; real_T B_63_509_0 ; real_T B_63_510_0 ; real_T B_63_513_0 ; real_T
B_63_514_0 ; real_T B_63_515_0 ; real_T B_63_516_0 ; real_T B_63_517_0 ;
real_T B_63_518_0 ; real_T B_63_519_0 ; real_T B_63_520_0 ; real_T B_63_521_0
; real_T B_63_522_0 ; real_T B_63_523_0 ; real_T B_63_524_0 ; real_T
B_63_525_0 ; real_T B_63_526_0 ; real_T B_63_527_0 ; real_T B_63_528_0 ;
real_T B_63_529_0 ; real_T B_63_530_0 ; real_T B_63_531_0 ; real_T B_63_532_0
; real_T B_63_533_0 ; real_T B_63_534_0 ; real_T B_63_539_0 ; real_T
B_63_540_0 ; real_T B_63_541_0 ; real_T B_63_542_0 [ 3 ] ; real_T B_63_543_0
[ 3 ] ; real_T B_63_546_0 [ 2 ] ; real_T B_63_548_0 ; real_T B_63_549_0 ;
real_T B_63_550_0 ; real_T B_63_552_0 ; real_T B_63_553_0 ; real_T B_63_556_0
; real_T B_63_557_0 ; real_T B_63_558_0 ; real_T B_63_560_0 ; real_T
B_63_561_0 ; real_T B_63_566_0 ; real_T B_63_570_0 ; real_T B_63_573_0 ;
real_T B_63_575_0 ; real_T B_63_576_0 ; real_T B_63_581_0 [ 3 ] ; real_T
B_63_582_0 ; real_T B_63_585_0 ; real_T B_63_586_0 ; real_T B_63_587_0 ;
real_T B_63_588_0 ; real_T B_63_589_0 ; real_T B_63_603_0 ; real_T B_63_607_0
; real_T B_63_608_0 ; real_T B_63_609_0 ; real_T B_63_612_0 ; real_T
B_63_613_0 ; real_T B_63_614_0 ; real_T B_63_615_0 ; real_T B_63_618_0 ;
real_T B_63_619_0 ; real_T B_63_620_0 ; real_T B_63_621_0 ; real_T B_63_622_0
; real_T B_63_623_0 ; real_T B_63_624_0 ; real_T B_63_625_0 ; real_T
B_63_626_0 ; real_T B_63_627_0 ; real_T B_63_628_0 ; real_T B_63_629_0 ;
real_T B_63_630_0 ; real_T B_63_631_0 ; real_T B_63_632_0 ; real_T B_63_633_0
; real_T B_63_634_0 ; real_T B_63_635_0 ; real_T B_63_636_0 ; real_T
B_63_637_0 ; real_T B_63_638_0 ; real_T B_63_639_0 ; real_T B_63_644_0 ;
real_T B_63_645_0 ; real_T B_63_646_0 ; real_T B_63_647_0 [ 3 ] ; real_T
B_63_648_0 [ 3 ] ; real_T B_63_651_0 [ 2 ] ; real_T B_63_652_0 ; real_T
B_63_731_0 ; real_T B_63_739_0 ; real_T B_63_747_0 ; real_T B_63_0_0 [ 7 ] ;
real_T B_63_2_0 ; real_T B_63_10_0 ; real_T B_63_11_0_m ; real_T B_63_27_0 ;
real_T B_63_28_0_c ; real_T B_63_29_0 ; real_T B_63_30_0 ; real_T B_63_31_0 ;
real_T B_63_32_0 ; real_T B_63_33_0 ; real_T B_63_34_0 ; real_T B_63_35_0_k ;
real_T B_63_36_0 ; real_T B_63_37_0_c ; real_T B_63_38_0_b ; real_T B_63_39_0
; real_T B_63_40_0 ; real_T B_63_41_0 ; real_T B_63_42_0_p ; real_T
B_63_43_0_c ; real_T B_63_44_0_f ; real_T B_63_45_0_g ; real_T B_63_46_0_g ;
real_T B_63_47_0 ; real_T B_63_48_0 ; real_T B_63_49_0 ; real_T B_63_50_0 ;
real_T B_63_51_0 ; real_T B_63_52_0 ; real_T B_63_53_0 ; real_T B_63_54_0 ;
real_T B_63_55_0 ; real_T B_63_56_0 ; real_T B_63_57_0 ; real_T B_63_63_0 ;
real_T B_63_64_0 ; real_T B_63_65_0 ; real_T B_63_66_0_m ; real_T B_63_68_0_n
; real_T B_63_69_0_p ; real_T B_63_70_0 ; real_T B_63_72_0 ; real_T B_63_73_0
; real_T B_63_74_0 ; real_T B_63_75_0 ; real_T B_63_83_0 ; real_T B_63_84_0 ;
real_T B_63_85_0 ; real_T B_63_91_0 [ 2 ] ; real_T B_63_93_0 ; real_T
B_63_94_0 ; real_T B_63_95_0 [ 2 ] ; real_T B_63_97_0 ; real_T B_63_100_0_l ;
real_T B_63_108_0_j ; real_T B_63_124_0 ; real_T B_63_126_0 ; real_T
B_63_127_0_d ; real_T B_63_128_0 ; real_T B_63_129_0_g ; real_T B_63_135_0_l
; real_T B_63_136_0 ; real_T B_63_137_0 ; real_T B_63_138_0_d ; real_T
B_63_139_0 ; real_T B_63_140_0_d ; real_T B_50_0_0 ; real_T B_50_1_0 ; real_T
B_49_0_0 ; real_T B_49_1_0 ; real_T B_48_0_0 ; real_T B_48_1_0 ; real_T
B_47_0_0 ; real_T B_47_1_0 ; real_T B_44_0_0 ; real_T B_44_1_0 ; real_T
B_16_0_0 ; real_T B_16_1_0 ; real_T B_63_262_0 [ 3 ] ; uint8_T B_63_60_0 ;
uint8_T B_63_62_0 ; uint8_T B_63_78_0_l ; uint8_T B_63_80_0_o ; uint8_T
B_63_88_0 ; uint8_T B_63_90_0 ; uint8_T B_63_103_0 ; uint8_T B_63_105_0 ;
uint8_T B_63_111_0 ; uint8_T B_63_113_0_b ; uint8_T B_63_116_0 ; uint8_T
B_63_118_0 ; uint8_T B_63_121_0_n ; uint8_T B_63_123_0 ; uint8_T B_63_132_0 ;
uint8_T B_63_134_0 ; boolean_T B_63_392_0 ; boolean_T B_63_424_0 [ 3 ] ;
boolean_T B_63_488_0 [ 3 ] ; boolean_T B_63_501_0 ; boolean_T B_63_511_0 ;
boolean_T B_63_606_0 ; boolean_T B_63_616_0 ; char_T pad_B_63_616_0 [ 5 ] ;
B_Subsystem1_PV_Battery_Grid2021a_g_T Subsystem1_m ;
B_Subsystempi2delay_PV_Battery_Grid2021a_j_T Subsystempi2delay_f ;
B_Subsystem1_PV_Battery_Grid2021a_g_T Subsystem1_h ;
B_Subsystempi2delay_PV_Battery_Grid2021a_j_T Subsystempi2delay_d ;
B_Subsystem1_PV_Battery_Grid2021a_T Subsystem1_j ;
B_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay_a ;
B_Subsystem1_PV_Battery_Grid2021a_T Subsystem1_b ;
B_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay_e ;
B_AutomaticGainControl_PV_Battery_Grid2021a_T AutomaticGainControl_p ;
B_Subsystem1_PV_Battery_Grid2021a_T Subsystem1_i ;
B_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay_n ;
B_ZeroSeqComputation_PV_Battery_Grid2021a_T ZeroSeqComputation_f ;
B_NegSeqComputation_PV_Battery_Grid2021a_T PosSeqComputation_o ;
B_NegSeqComputation_PV_Battery_Grid2021a_T NegSeqComputation_h ;
B_ZeroSeqComputation_PV_Battery_Grid2021a_T ZeroSeqComputation ;
B_NegSeqComputation_PV_Battery_Grid2021a_T PosSeqComputation ;
B_NegSeqComputation_PV_Battery_Grid2021a_T NegSeqComputation ;
B_Subsystem1_PV_Battery_Grid2021a_T Subsystem1 ;
B_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay ;
B_AutomaticGainControl_PV_Battery_Grid2021a_T AutomaticGainControl ; }
B_PV_Battery_Grid2021a_T ; typedef struct { real_T Currentfilter_states ;
real_T inti_DSTATE ; real_T DiscreteTimeIntegrator_DSTATE ; real_T
UnitDelay_DSTATE ; real_T StateSpace_DSTATE [ 13 ] ; real_T
UnitDelay_DSTATE_e [ 6 ] ; real_T Integrator_DSTATE ; real_T
UnitDelay_DSTATE_o ; real_T Integrator_DSTATE_p ; real_T UnitDelay_DSTATE_ev
; real_T UnitDelay_DSTATE_b [ 3 ] ; real_T Integrator_DSTATE_l ; real_T
Integrator_DSTATE_d ; real_T Integrator_DSTATE_pi ; real_T
Integrator_DSTATE_pe ; real_T UnitDelay_DSTATE_p ; real_T UnitDelay_DSTATE_j
; real_T itinit1_PreviousInput ; real_T itinit_PreviousInput ; real_T
Memory2_PreviousInput ; real_T lastSin ; real_T lastCos ; real_T lastSin_d ;
real_T lastCos_b ; real_T lastSin_b ; real_T lastCos_j ; real_T
Memory_PreviousInput ; real_T Memory_PreviousInput_a ; real_T
Memory_PreviousInput_p ; real_T Memory_PreviousInput_d ; real_T
Memory_PreviousInput_d0 ; real_T Memory_PreviousInput_m ; real_T
Memory_PreviousInput_mf ; real_T Memory_PreviousInput_c ; real_T
Memory_PreviousInput_b ; real_T Memory_PreviousInput_i ; real_T
Memory_PreviousInput_j ; real_T Memory_PreviousInput_i3 ; real_T
Memory_PreviousInput_dr ; real_T Memory1_PreviousInput ; real_T
Memory2_PreviousInput_m ; real_T Memory_PreviousInput_e ; real_T
Initial_FirstOutputTime ; real_T PrevYA ; real_T PrevYB ; real_T
LastMajorTimeA ; real_T LastMajorTimeB ; real_T Memory_PreviousInput_at ;
real_T Initial_FirstOutputTime_a ; real_T PrevYA_l ; real_T PrevYB_l ; real_T
LastMajorTimeA_l ; real_T LastMajorTimeB_c ; struct { real_T modelTStart ; }
TransportDelay_RWORK ; struct { real_T modelTStart ; } TransportDelay_RWORK_p
; struct { real_T modelTStart ; } TransportDelay_RWORK_o ; struct { real_T
modelTStart ; } TransportDelay_RWORK_m ; struct { real_T modelTStart ; }
TransportDelay_RWORK_l ; struct { real_T modelTStart ; }
TransportDelay_RWORK_n ; struct { real_T modelTStart ; }
TransportDelay_RWORK_h ; struct { real_T modelTStart ; }
TransportDelay_RWORK_e ; struct { real_T modelTStart ; }
TransportDelay_RWORK_a ; struct { real_T modelTStart ; }
TransportDelay_RWORK_c ; struct { real_T modelTStart ; }
TransportDelay_RWORK_h1 ; struct { real_T modelTStart ; }
TransportDelay_RWORK_k ; struct { real_T modelTStart ; }
VariableTransportDelay_RWORK ; struct { real_T modelTStart ; }
VariableTransportDelay_RWORK_a ; struct { void * AS ; void * BS ; void * CS ;
void * DS ; void * DX_COL ; void * BD_COL ; void * TMP1 ; void * TMP2 ; void
* XTMP ; void * SWITCH_STATUS ; void * SWITCH_STATUS_INIT ; void * SW_CHG ;
void * G_STATE ; void * USWLAST ; void * XKM12 ; void * XKP12 ; void * XLAST
; void * ULAST ; void * IDX_SW_CHG ; void * Y_SWITCH ; void * SWITCH_TYPES ;
void * IDX_OUT_SW ; void * SWITCH_TOPO_SAVED_IDX ; void * SWITCH_MAP ; }
StateSpace_PWORK ; void * Scope3_PWORK [ 3 ] ; struct { void * AQHandles ; }
TAQSigLogging_InsertedFor_BusSelector_at_outport_0_PWORK ; struct { void *
AQHandles ; } TAQSigLogging_InsertedFor_BusSelector_at_outport_1_PWORK ;
struct { void * AQHandles ; }
TAQSigLogging_InsertedFor_BusSelector_at_outport_2_PWORK ; void *
Scope1_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK ;
struct { void * TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK_p ; struct { void
* TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK_l ; struct { void *
TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK_o ; struct { void * TUbufferPtrs
[ 2 ] ; } TransportDelay_PWORK_n ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_m ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_b ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_nb ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_f ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_c ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_h ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_d ; void * Scope13_PWORK [ 4 ] ; void * Scope2_PWORK [ 2
] ; void * Scope8_PWORK [ 2 ] ; struct { void * AQHandles ; }
TAQSigLogging_InsertedFor_From1_at_outport_0_PWORK ; struct { void *
AQHandles ; } TAQSigLogging_InsertedFor_From26_at_outport_0_PWORK ; struct {
void * AQHandles ; } TAQSigLogging_InsertedFor_From7_at_outport_0_PWORK ;
struct { void * AQHandles ; }
TAQSigLogging_InsertedFor_Iin_at_outport_0_PWORK ; struct { void * AQHandles
; } TAQSigLogging_InsertedFor_Product1_at_outport_0_PWORK ; struct { void *
AQHandles ; } TAQSigLogging_InsertedFor_Product2_at_outport_0_PWORK ; struct
{ void * AQHandles ; } TAQSigLogging_InsertedFor_Product3_at_outport_0_PWORK
; struct { void * AQHandles ; }
TAQSigLogging_InsertedFor_Vin_at_outport_0_PWORK ; void * _PWORK [ 3 ] ; void
* Scope_PWORK ; void * Scope10_PWORK ; void * Scope11_PWORK ; void *
Scope9_PWORK ; void * Scope_PWORK_p ; void * Scope2_PWORK_e [ 2 ] ; void *
Scope6_PWORK [ 2 ] ; void * Scope7_PWORK ; struct { void * TUbufferPtrs [ 3 ]
; } VariableTransportDelay_PWORK ; void * Scope_PWORK_j ; void *
Scope1_PWORK_k ; void * Scope2_PWORK_h ; void * Scope3_PWORK_m ; void *
Scope4_PWORK ; void * Scope5_PWORK ; void * Scope5_PWORK_c [ 2 ] ; void *
Scope6_PWORK_d [ 2 ] ; void * Scope1_PWORK_c ; void * Scope_PWORK_b ; struct
{ void * AQHandles ; } TAQSigLogging_InsertedFor_PLL3ph_at_outport_0_PWORK ;
struct { void * TUbufferPtrs [ 3 ] ; } VariableTransportDelay_PWORK_m ; void
* Scope6_PWORK_l ; int32_T systemEnable ; int32_T systemEnable_i ; int32_T
systemEnable_p ; int32_T TmpAtomicSubsysAtSwitch3Inport3_sysIdxToRun ;
int32_T TmpAtomicSubsysAtSwitch3Inport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitch3Inport3_sysIdxToRun_j ; int32_T
TmpAtomicSubsysAtSwitch3Inport1_sysIdxToRun_c ; int32_T
TmpAtomicSubsysAtSwitch3Inport3_sysIdxToRun_l ; int32_T
TmpAtomicSubsysAtSwitch3Inport1_sysIdxToRun_m ; int32_T
Subsystem1_sysIdxToRun ; int32_T Subsystempi2delay_sysIdxToRun ; int32_T
Subsystem1_sysIdxToRun_j ; int32_T Subsystempi2delay_sysIdxToRun_i ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_i ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_ir ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_c ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_l ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_g ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_gp ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_h ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_b ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_n ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_o ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_hq ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_b5 ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_hy ; int32_T
TmpAtomicSubsysAtSwitch2Inport3_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitch1Inport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitch2Inport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_p ; int32_T
TmpAtomicSubsysAtSwitch2Inport3_sysIdxToRun_p ; int32_T
TmpAtomicSubsysAtSwitch2Inport3_sysIdxToRun_o ; int32_T
TmpAtomicSubsysAtMultiportSwitch1Inport3_sysIdxToRun ; int32_T
TmpAtomicSubsysAtMultiportSwitch1Inport5_sysIdxToRun ; int32_T
TmpAtomicSubsysAtMultiportSwitch1Inport4_sysIdxToRun ; int32_T
TmpAtomicSubsysAtMultiportSwitch1Inport3_sysIdxToRun_m ; int32_T
TmpAtomicSubsysAtMultiportSwitch1Inport2_sysIdxToRun ; uint32_T m_bpIndex ;
uint32_T m_bpIndex_i ; int_T StateSpace_IWORK [ 11 ] ; struct { int_T Tail ;
int_T Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last ; int_T
CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK_c ; struct {
int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay_IWORK_h ; struct { int_T Tail ; int_T Head ;
int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_f ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK_ff ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay_IWORK_c2 ; struct { int_T Tail ; int_T Head
; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_g ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK_ck ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay_IWORK_l ; struct { int_T Tail ; int_T Head ;
int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_p ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK_o ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay_IWORK_b ; int_T Integrator_IWORK ; struct {
int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } VariableTransportDelay_IWORK ; int_T Integrator_IWORK_b ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } VariableTransportDelay_IWORK_c ; int_T Step_MODE ; int_T
HitCrossing_MODE ; int_T Step3_MODE ; int_T Step1_MODE ; int_T Step2_MODE ;
int_T Step4_MODE ; int_T Integrator_MODE ; int_T Integrator_MODE_a ; int_T
Saturation2_MODE ; int_T Qref_MODE ; int_T Pref_MODE ; int_T
Integrator_MODE_at ; int_T Integrator_MODE_az ; int_T Saturation2_MODE_e ;
int8_T inti_PrevResetState ; int8_T Subsystem1_SubsysRanBC ; int8_T
Subsystempi2delay_SubsysRanBC ; int8_T Subsystem1_SubsysRanBC_g ; int8_T
Subsystempi2delay_SubsysRanBC_a ; uint8_T inti_IC_LOADING ; uint8_T
inti_NumInitCond ; uint8_T DiscreteTimeIntegrator_NumInitCond ; boolean_T
Switch_Mode ; boolean_T RelationalOperator_Mode ; boolean_T
RelationalOperator2_Mode [ 3 ] ; boolean_T RelationalOperator2_Mode_k [ 3 ] ;
boolean_T RelationalOperator_Mode_c ; boolean_T RelationalOperator_Mode_n ;
boolean_T RelationalOperator_Mode_nz ; boolean_T RelationalOperator_Mode_i ;
boolean_T Subsystem1_MODE ; boolean_T Subsystempi2delay_MODE ; char_T
pad_Subsystempi2delay_MODE [ 6 ] ; DW_Subsystem1_PV_Battery_Grid2021a_f_T
Subsystem1_m ; DW_Subsystempi2delay_PV_Battery_Grid2021a_k_T
Subsystempi2delay_f ; DW_Subsystem1_PV_Battery_Grid2021a_f_T Subsystem1_h ;
DW_Subsystempi2delay_PV_Battery_Grid2021a_k_T Subsystempi2delay_d ;
DW_Subsystem1_PV_Battery_Grid2021a_T Subsystem1_j ;
DW_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay_a ;
DW_Subsystem1_PV_Battery_Grid2021a_T Subsystem1_b ;
DW_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay_e ;
DW_AutomaticGainControl_PV_Battery_Grid2021a_T AutomaticGainControl_p ;
DW_Subsystem1_PV_Battery_Grid2021a_T Subsystem1_i ;
DW_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay_n ;
DW_ZeroSeqComputation_PV_Battery_Grid2021a_T ZeroSeqComputation_f ;
DW_NegSeqComputation_PV_Battery_Grid2021a_T PosSeqComputation_o ;
DW_NegSeqComputation_PV_Battery_Grid2021a_T NegSeqComputation_h ;
DW_ZeroSeqComputation_PV_Battery_Grid2021a_T ZeroSeqComputation ;
DW_NegSeqComputation_PV_Battery_Grid2021a_T PosSeqComputation ;
DW_NegSeqComputation_PV_Battery_Grid2021a_T NegSeqComputation ;
DW_Subsystem1_PV_Battery_Grid2021a_T Subsystem1 ;
DW_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay ;
DW_AutomaticGainControl_PV_Battery_Grid2021a_T AutomaticGainControl ; }
DW_PV_Battery_Grid2021a_T ; typedef struct { real_T integrator_CSTATE ;
real_T integrator_CSTATE_a ; real_T integrator_CSTATE_af ; real_T
integrator_CSTATE_f ; real_T integrator_CSTATE_e ; real_T integrator_CSTATE_p
; real_T integrator_CSTATE_c ; real_T integrator_CSTATE_fr ; real_T
integrator_CSTATE_pb ; real_T integrator_CSTATE_o ; real_T
integrator_CSTATE_oo ; real_T integrator_CSTATE_ec ; real_T Integrator_CSTATE
; real_T Integrator_CSTATE_b ; real_T Integrator_CSTATE_o ; real_T
Integrator_CSTATE_c ; real_T VariableTransportDelay_CSTATE ; real_T
integrator_CSTATE_i ; real_T TransferFcn_CSTATE ; real_T Integrator_x1_CSTATE
; real_T Integrator_x2_CSTATE ; real_T Integrator_CSTATE_a ; real_T
Integrator_CSTATE_p ; real_T Integrator_CSTATE_n ; real_T Integrator_CSTATE_f
; real_T VariableTransportDelay_CSTATE_h ; real_T integrator_CSTATE_n ;
real_T TransferFcn_CSTATE_k ; real_T Integrator_x1_CSTATE_p ; real_T
Integrator_x2_CSTATE_k ; X_AutomaticGainControl_PV_Battery_Grid2021a_T
AutomaticGainControl_p ; X_AutomaticGainControl_PV_Battery_Grid2021a_T
AutomaticGainControl ; } X_PV_Battery_Grid2021a_T ; typedef struct { real_T
integrator_CSTATE ; real_T integrator_CSTATE_a ; real_T integrator_CSTATE_af
; real_T integrator_CSTATE_f ; real_T integrator_CSTATE_e ; real_T
integrator_CSTATE_p ; real_T integrator_CSTATE_c ; real_T
integrator_CSTATE_fr ; real_T integrator_CSTATE_pb ; real_T
integrator_CSTATE_o ; real_T integrator_CSTATE_oo ; real_T
integrator_CSTATE_ec ; real_T Integrator_CSTATE ; real_T Integrator_CSTATE_b
; real_T Integrator_CSTATE_o ; real_T Integrator_CSTATE_c ; real_T
VariableTransportDelay_CSTATE ; real_T integrator_CSTATE_i ; real_T
TransferFcn_CSTATE ; real_T Integrator_x1_CSTATE ; real_T
Integrator_x2_CSTATE ; real_T Integrator_CSTATE_a ; real_T
Integrator_CSTATE_p ; real_T Integrator_CSTATE_n ; real_T Integrator_CSTATE_f
; real_T VariableTransportDelay_CSTATE_h ; real_T integrator_CSTATE_n ;
real_T TransferFcn_CSTATE_k ; real_T Integrator_x1_CSTATE_p ; real_T
Integrator_x2_CSTATE_k ; XDot_AutomaticGainControl_PV_Battery_Grid2021a_T
AutomaticGainControl_p ; XDot_AutomaticGainControl_PV_Battery_Grid2021a_T
AutomaticGainControl ; } XDot_PV_Battery_Grid2021a_T ; typedef struct {
boolean_T integrator_CSTATE ; boolean_T integrator_CSTATE_a ; boolean_T
integrator_CSTATE_af ; boolean_T integrator_CSTATE_f ; boolean_T
integrator_CSTATE_e ; boolean_T integrator_CSTATE_p ; boolean_T
integrator_CSTATE_c ; boolean_T integrator_CSTATE_fr ; boolean_T
integrator_CSTATE_pb ; boolean_T integrator_CSTATE_o ; boolean_T
integrator_CSTATE_oo ; boolean_T integrator_CSTATE_ec ; boolean_T
Integrator_CSTATE ; boolean_T Integrator_CSTATE_b ; boolean_T
Integrator_CSTATE_o ; boolean_T Integrator_CSTATE_c ; boolean_T
VariableTransportDelay_CSTATE ; boolean_T integrator_CSTATE_i ; boolean_T
TransferFcn_CSTATE ; boolean_T Integrator_x1_CSTATE ; boolean_T
Integrator_x2_CSTATE ; boolean_T Integrator_CSTATE_a ; boolean_T
Integrator_CSTATE_p ; boolean_T Integrator_CSTATE_n ; boolean_T
Integrator_CSTATE_f ; boolean_T VariableTransportDelay_CSTATE_h ; boolean_T
integrator_CSTATE_n ; boolean_T TransferFcn_CSTATE_k ; boolean_T
Integrator_x1_CSTATE_p ; boolean_T Integrator_x2_CSTATE_k ;
XDis_AutomaticGainControl_PV_Battery_Grid2021a_T AutomaticGainControl_p ;
XDis_AutomaticGainControl_PV_Battery_Grid2021a_T AutomaticGainControl ; }
XDis_PV_Battery_Grid2021a_T ; typedef struct { real_T integrator_CSTATE ;
real_T integrator_CSTATE_a ; real_T integrator_CSTATE_af ; real_T
integrator_CSTATE_f ; real_T integrator_CSTATE_e ; real_T integrator_CSTATE_p
; real_T integrator_CSTATE_c ; real_T integrator_CSTATE_fr ; real_T
integrator_CSTATE_pb ; real_T integrator_CSTATE_o ; real_T
integrator_CSTATE_oo ; real_T integrator_CSTATE_ec ; real_T Integrator_CSTATE
; real_T Integrator_CSTATE_b ; real_T Integrator_CSTATE_o ; real_T
Integrator_CSTATE_c ; real_T VariableTransportDelay_CSTATE ; real_T
integrator_CSTATE_i ; real_T TransferFcn_CSTATE ; real_T Integrator_x1_CSTATE
; real_T Integrator_x2_CSTATE ; real_T Integrator_CSTATE_a ; real_T
Integrator_CSTATE_p ; real_T Integrator_CSTATE_n ; real_T Integrator_CSTATE_f
; real_T VariableTransportDelay_CSTATE_h ; real_T integrator_CSTATE_n ;
real_T TransferFcn_CSTATE_k ; real_T Integrator_x1_CSTATE_p ; real_T
Integrator_x2_CSTATE_k ; XAbsTol_AutomaticGainControl_PV_Battery_Grid2021a_T
AutomaticGainControl_p ; XAbsTol_AutomaticGainControl_PV_Battery_Grid2021a_T
AutomaticGainControl ; } CStateAbsTol_PV_Battery_Grid2021a_T ; typedef struct
{ real_T integrator_CSTATE ; real_T integrator_CSTATE_a ; real_T
integrator_CSTATE_af ; real_T integrator_CSTATE_f ; real_T
integrator_CSTATE_e ; real_T integrator_CSTATE_p ; real_T integrator_CSTATE_c
; real_T integrator_CSTATE_fr ; real_T integrator_CSTATE_pb ; real_T
integrator_CSTATE_o ; real_T integrator_CSTATE_oo ; real_T
integrator_CSTATE_ec ; real_T Integrator_CSTATE ; real_T Integrator_CSTATE_b
; real_T Integrator_CSTATE_o ; real_T Integrator_CSTATE_c ; real_T
VariableTransportDelay_CSTATE ; real_T integrator_CSTATE_i ; real_T
TransferFcn_CSTATE ; real_T Integrator_x1_CSTATE ; real_T
Integrator_x2_CSTATE ; real_T Integrator_CSTATE_a ; real_T
Integrator_CSTATE_p ; real_T Integrator_CSTATE_n ; real_T Integrator_CSTATE_f
; real_T VariableTransportDelay_CSTATE_h ; real_T integrator_CSTATE_n ;
real_T TransferFcn_CSTATE_k ; real_T Integrator_x1_CSTATE_p ; real_T
Integrator_x2_CSTATE_k ; XPtMin_AutomaticGainControl_PV_Battery_Grid2021a_T
AutomaticGainControl_p ; XPtMin_AutomaticGainControl_PV_Battery_Grid2021a_T
AutomaticGainControl ; } CXPtMin_PV_Battery_Grid2021a_T ; typedef struct {
real_T integrator_CSTATE ; real_T integrator_CSTATE_a ; real_T
integrator_CSTATE_af ; real_T integrator_CSTATE_f ; real_T
integrator_CSTATE_e ; real_T integrator_CSTATE_p ; real_T integrator_CSTATE_c
; real_T integrator_CSTATE_fr ; real_T integrator_CSTATE_pb ; real_T
integrator_CSTATE_o ; real_T integrator_CSTATE_oo ; real_T
integrator_CSTATE_ec ; real_T Integrator_CSTATE ; real_T Integrator_CSTATE_b
; real_T Integrator_CSTATE_o ; real_T Integrator_CSTATE_c ; real_T
VariableTransportDelay_CSTATE ; real_T integrator_CSTATE_i ; real_T
TransferFcn_CSTATE ; real_T Integrator_x1_CSTATE ; real_T
Integrator_x2_CSTATE ; real_T Integrator_CSTATE_a ; real_T
Integrator_CSTATE_p ; real_T Integrator_CSTATE_n ; real_T Integrator_CSTATE_f
; real_T VariableTransportDelay_CSTATE_h ; real_T integrator_CSTATE_n ;
real_T TransferFcn_CSTATE_k ; real_T Integrator_x1_CSTATE_p ; real_T
Integrator_x2_CSTATE_k ; XPtMax_AutomaticGainControl_PV_Battery_Grid2021a_T
AutomaticGainControl_p ; XPtMax_AutomaticGainControl_PV_Battery_Grid2021a_T
AutomaticGainControl ; } CXPtMax_PV_Battery_Grid2021a_T ; typedef struct {
real_T Step_StepTime_ZC ; real_T Switch_SwitchCond_ZC ; real_T
RelationalOperator_RelopInput_ZC ; real_T HitCrossing_Input_ZC ; real_T
HitCrossing_Output_ZC ; real_T Step3_StepTime_ZC ; real_T
RelationalOperator2_RelopInput_ZC [ 3 ] ; real_T Step1_StepTime_ZC ; real_T
Step2_StepTime_ZC ; real_T RelationalOperator2_RelopInput_ZC_j [ 3 ] ; real_T
Step4_StepTime_ZC ; real_T RelationalOperator_RelopInput_ZC_e ; real_T
Integrator_Reset_ZC ; real_T Integrator_IntgUpLimit_ZC ; real_T
Integrator_IntgLoLimit_ZC ; real_T Integrator_LeaveSaturate_ZC ; real_T
HitCrossing_HitNoOutput_ZC ; real_T Integrator_IntgUpLimit_ZC_n ; real_T
Integrator_IntgLoLimit_ZC_n ; real_T Integrator_LeaveSaturate_ZC_n ; real_T
RelationalOperator_RelopInput_ZC_m ; real_T Saturation2_UprLim_ZC ; real_T
Saturation2_LwrLim_ZC ; real_T Qref_StepTime_ZC ; real_T Pref_StepTime_ZC ;
real_T RelationalOperator_RelopInput_ZC_l ; real_T Integrator_Reset_ZC_h ;
real_T Integrator_IntgUpLimit_ZC_e ; real_T Integrator_IntgLoLimit_ZC_m ;
real_T Integrator_LeaveSaturate_ZC_nd ; real_T HitCrossing_HitNoOutput_ZC_p ;
real_T Integrator_IntgUpLimit_ZC_f ; real_T Integrator_IntgLoLimit_ZC_o ;
real_T Integrator_LeaveSaturate_ZC_e ; real_T
RelationalOperator_RelopInput_ZC_lw ; real_T Saturation2_UprLim_ZC_o ; real_T
Saturation2_LwrLim_ZC_h ; ZCV_AutomaticGainControl_PV_Battery_Grid2021a_T
AutomaticGainControl_p ; ZCV_AutomaticGainControl_PV_Battery_Grid2021a_T
AutomaticGainControl ; } ZCV_PV_Battery_Grid2021a_T ; typedef struct {
ZCSigState Step_StepTime_ZCE ; ZCSigState Switch_SwitchCond_ZCE ; ZCSigState
RelationalOperator_RelopInput_ZCE ; ZCSigState HitCrossing_Input_ZCE ;
ZCSigState HitCrossing_Output_ZCE ; ZCSigState Step3_StepTime_ZCE ;
ZCSigState RelationalOperator2_RelopInput_ZCE [ 3 ] ; ZCSigState
Step1_StepTime_ZCE ; ZCSigState Step2_StepTime_ZCE ; ZCSigState
RelationalOperator2_RelopInput_ZCE_e [ 3 ] ; ZCSigState Step4_StepTime_ZCE ;
ZCSigState RelationalOperator_RelopInput_ZCE_h ; ZCSigState
Integrator_Reset_ZCE ; ZCSigState Integrator_IntgUpLimit_ZCE ; ZCSigState
Integrator_IntgLoLimit_ZCE ; ZCSigState Integrator_LeaveSaturate_ZCE ;
ZCSigState HitCrossing_HitNoOutput_ZCE ; ZCSigState
Integrator_IntgUpLimit_ZCE_i ; ZCSigState Integrator_IntgLoLimit_ZCE_j ;
ZCSigState Integrator_LeaveSaturate_ZCE_b ; ZCSigState
RelationalOperator_RelopInput_ZCE_n ; ZCSigState Saturation2_UprLim_ZCE ;
ZCSigState Saturation2_LwrLim_ZCE ; ZCSigState Qref_StepTime_ZCE ; ZCSigState
Pref_StepTime_ZCE ; ZCSigState RelationalOperator_RelopInput_ZCE_p ;
ZCSigState Integrator_Reset_ZCE_p ; ZCSigState Integrator_IntgUpLimit_ZCE_g ;
ZCSigState Integrator_IntgLoLimit_ZCE_f ; ZCSigState
Integrator_LeaveSaturate_ZCE_c ; ZCSigState HitCrossing_HitNoOutput_ZCE_c ;
ZCSigState Integrator_IntgUpLimit_ZCE_m ; ZCSigState
Integrator_IntgLoLimit_ZCE_i ; ZCSigState Integrator_LeaveSaturate_ZCE_p ;
ZCSigState RelationalOperator_RelopInput_ZCE_a ; ZCSigState
Saturation2_UprLim_ZCE_i ; ZCSigState Saturation2_LwrLim_ZCE_g ;
ZCE_AutomaticGainControl_PV_Battery_Grid2021a_T AutomaticGainControl_p ;
ZCE_AutomaticGainControl_PV_Battery_Grid2021a_T AutomaticGainControl ; }
PrevZCX_PV_Battery_Grid2021a_T ; typedef struct { const real_T B_63_0_0 ;
const real_T B_63_1_0 ; const real_T B_63_2_0 ; const real_T B_63_3_0 ; }
ConstB_PV_Battery_Grid2021a_T ;
#define PV_Battery_Grid2021a_rtC(S) ((ConstB_PV_Battery_Grid2021a_T *) _ssGetConstBlockIO(S))
struct P_Subsystempi2delay_PV_Battery_Grid2021a_T_ { real_T P_0 [ 2 ] ; } ;
struct P_Subsystem1_PV_Battery_Grid2021a_T_ { real_T P_0 [ 2 ] ; } ; struct
P_AutomaticGainControl_PV_Battery_Grid2021a_T_ { real_T P_0 ; real_T P_1 ;
real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ;
real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T P_12 ; real_T
P_13 [ 9 ] ; real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ; real_T
P_18 ; real_T P_19 ; real_T P_20 ; real_T P_21 ;
P_Subsystem1_PV_Battery_Grid2021a_T Subsystem1 ;
P_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay ; } ; struct
P_NegSeqComputation_PV_Battery_Grid2021a_T_ { real_T P_0 ; real_T P_1 ;
creal_T P_2 [ 3 ] ; } ; struct P_ZeroSeqComputation_PV_Battery_Grid2021a_T_ {
real_T P_0 ; real_T P_1 ; } ; struct
P_Subsystempi2delay_PV_Battery_Grid2021a_k_T_ { real_T P_0 [ 2 ] ; } ; struct
P_Subsystem1_PV_Battery_Grid2021a_j_T_ { real_T P_0 [ 2 ] ; } ; struct
P_PV_Battery_Grid2021a_T_ { real_T P_0 ; real_T P_1 ; real_T P_2 ; real_T P_3
; real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ; real_T P_8 ; real_T P_9
; real_T P_10 ; real_T P_11 ; real_T P_12 ; real_T P_13 ; real_T P_14 ;
real_T P_15 ; real_T P_16 [ 2 ] ; real_T P_17 [ 2 ] ; real_T P_18 [ 2 ] ;
real_T P_19 [ 2 ] ; real_T P_20 ; real_T P_21 [ 5 ] ; real_T P_22 [ 5 ] ;
real_T P_23 ; real_T P_24 [ 5 ] ; real_T P_25 [ 5 ] ; real_T P_26 ; real_T
P_27 [ 5 ] ; real_T P_28 [ 5 ] ; real_T P_29 ; real_T P_30 ; real_T P_31 ;
real_T P_32 ; real_T P_33 ; real_T P_34 ; real_T P_35 ; real_T P_36 [ 2 ] ;
real_T P_37 ; real_T P_38 ; real_T P_39 ; real_T P_40 ; real_T P_41 ; real_T
P_42 ; real_T P_43 ; real_T P_44 ; real_T P_45 ; real_T P_46 ; real_T P_47 ;
real_T P_48 ; real_T P_49 ; real_T P_50 ; real_T P_51 ; real_T P_52 ; real_T
P_53 ; real_T P_54 ; real_T P_55 ; real_T P_56 ; real_T P_57 ; real_T P_58 ;
real_T P_59 ; real_T P_60 ; real_T P_61 ; real_T P_62 ; real_T P_63 ; real_T
P_64 ; real_T P_65 ; real_T P_66 ; real_T P_67 ; real_T P_68 ; real_T P_69 ;
real_T P_70 ; real_T P_71 ; real_T P_72 ; real_T P_73 [ 2 ] ; real_T P_74 [
169 ] ; real_T P_75 [ 2 ] ; real_T P_76 [ 260 ] ; real_T P_77 [ 2 ] ; real_T
P_78 [ 494 ] ; real_T P_79 [ 2 ] ; real_T P_80 [ 760 ] ; real_T P_81 [ 2 ] ;
real_T P_82 [ 13 ] ; real_T P_83 ; real_T P_84 ; real_T P_85 ; real_T P_86 ;
real_T P_87 ; real_T P_88 ; real_T P_89 ; real_T P_90 ; real_T P_91 ; real_T
P_92 ; real_T P_93 ; real_T P_94 ; real_T P_95 ; real_T P_96 ; real_T P_97 ;
real_T P_98 ; real_T P_99 ; real_T P_100 ; real_T P_101 ; real_T P_102 ;
real_T P_103 ; real_T P_104 ; real_T P_105 ; real_T P_106 ; real_T P_107 ;
real_T P_108 ; real_T P_109 ; real_T P_110 ; real_T P_111 ; real_T P_112 ;
real_T P_113 ; real_T P_114 ; real_T P_115 ; real_T P_116 ; real_T P_117 ;
real_T P_118 ; real_T P_119 ; real_T P_120 ; real_T P_121 ; real_T P_122 ;
real_T P_123 ; real_T P_124 ; real_T P_125 ; real_T P_126 ; real_T P_127 ;
real_T P_128 ; real_T P_129 ; real_T P_130 ; real_T P_131 ; real_T P_132 ;
real_T P_133 ; real_T P_134 ; real_T P_135 ; real_T P_136 ; real_T P_137 ;
real_T P_138 ; real_T P_139 ; real_T P_140 ; real_T P_141 ; real_T P_142 ;
real_T P_143 ; real_T P_144 ; real_T P_145 ; real_T P_146 ; real_T P_147 ;
real_T P_148 ; real_T P_149 ; real_T P_150 ; real_T P_151 ; real_T P_152 ;
real_T P_153 ; real_T P_154 ; real_T P_155 ; real_T P_156 ; real_T P_157 ;
real_T P_158 ; real_T P_159 ; real_T P_160 ; real_T P_161 ; real_T P_162 ;
real_T P_163 ; real_T P_164 ; real_T P_165 ; real_T P_166 ; real_T P_167 ;
real_T P_168 ; real_T P_169 ; real_T P_170 ; real_T P_171 ; real_T P_172 ;
real_T P_173 ; real_T P_174 ; real_T P_175 ; real_T P_176 ; real_T P_177 ;
real_T P_178 ; real_T P_179 ; real_T P_180 ; real_T P_181 ; real_T P_182 ;
real_T P_183 ; real_T P_184 ; real_T P_185 ; real_T P_186 ; real_T P_187 ;
real_T P_188 ; real_T P_189 ; real_T P_190 ; real_T P_191 ; real_T P_192 ;
real_T P_193 ; real_T P_194 ; real_T P_195 ; real_T P_196 ; real_T P_197 [ 2
] ; real_T P_198 [ 2 ] ; real_T P_199 ; real_T P_200 ; real_T P_201 ; real_T
P_202 ; real_T P_203 ; real_T P_204 ; real_T P_205 ; real_T P_206 ; real_T
P_207 ; real_T P_208 ; real_T P_209 ; real_T P_210 ; real_T P_211 ; real_T
P_212 ; real_T P_213 ; real_T P_214 ; real_T P_215 ; real_T P_216 ; real_T
P_217 ; real_T P_218 ; real_T P_219 ; real_T P_220 ; real_T P_221 ; real_T
P_222 ; real_T P_223 ; real_T P_224 ; real_T P_225 ; real_T P_226 ; real_T
P_227 ; real_T P_228 ; real_T P_229 ; real_T P_230 ; real_T P_231 ; real_T
P_232 ; real_T P_233 ; real_T P_234 ; real_T P_235 ; real_T P_236 ; real_T
P_237 ; real_T P_238 ; real_T P_239 ; real_T P_240 ; real_T P_241 ; real_T
P_242 ; real_T P_243 ; real_T P_244 ; real_T P_245 ; real_T P_246 ; real_T
P_247 ; real_T P_248 ; real_T P_249 ; real_T P_250 ; real_T P_251 ; real_T
P_252 ; real_T P_253 ; real_T P_254 ; real_T P_255 ; real_T P_256 ; real_T
P_257 ; real_T P_258 ; real_T P_259 ; real_T P_260 ; real_T P_261 ; real_T
P_262 ; real_T P_263 ; real_T P_264 ; real_T P_265 [ 9 ] ; real_T P_266 ;
real_T P_267 ; real_T P_268 ; real_T P_269 ; real_T P_270 ; real_T P_271 [ 9
] ; real_T P_272 ; real_T P_273 ; real_T P_274 ; real_T P_275 ; real_T P_276
; real_T P_277 [ 3 ] ; real_T P_278 [ 3 ] ; real_T P_279 ; real_T P_280 ;
real_T P_281 ; real_T P_282 ; real_T P_283 ; real_T P_284 ; real_T P_285 ;
real_T P_286 [ 9 ] ; real_T P_287 ; real_T P_288 ; real_T P_289 ; real_T
P_290 ; real_T P_291 ; real_T P_292 ; real_T P_293 ; real_T P_294 ; real_T
P_295 [ 9 ] ; real_T P_296 ; real_T P_297 ; real_T P_298 ; real_T P_299 ;
real_T P_300 ; real_T P_301 ; real_T P_302 ; real_T P_303 ; real_T P_304 ;
real_T P_305 ; real_T P_306 [ 9 ] ; real_T P_307 ; real_T P_308 ; real_T
P_309 ; real_T P_310 [ 3 ] ; real_T P_311 [ 3 ] ; real_T P_312 ; real_T P_313
; real_T P_314 ; real_T P_315 ; real_T P_316 ; real_T P_317 ; real_T P_318 ;
real_T P_319 ; real_T P_320 ; real_T P_321 ; real_T P_322 ; real_T P_323 ;
real_T P_324 ; real_T P_325 ; real_T P_326 ; real_T P_327 ; real_T P_328 ;
real_T P_329 ; real_T P_330 ; real_T P_331 ; real_T P_332 ; real_T P_333 ;
real_T P_334 ; real_T P_335 ; real_T P_336 ; real_T P_337 ; real_T P_338 ;
real_T P_339 ; real_T P_340 ; real_T P_341 ; real_T P_342 ; real_T P_343 ;
real_T P_344 ; real_T P_345 ; real_T P_346 ; real_T P_347 ; real_T P_348 ;
real_T P_349 ; real_T P_350 ; real_T P_351 ; real_T P_352 ; real_T P_353 ;
real_T P_354 [ 9 ] ; real_T P_355 ; real_T P_356 ; real_T P_357 ; real_T
P_358 ; real_T P_359 ; real_T P_360 ; real_T P_361 ; real_T P_362 ; real_T
P_363 ; real_T P_364 ; real_T P_365 ; real_T P_366 ; real_T P_367 ; real_T
P_368 ; real_T P_369 ; real_T P_370 [ 9 ] ; real_T P_371 ; real_T P_372 ;
real_T P_373 ; real_T P_374 ; real_T P_375 ; real_T P_376 ; real_T P_377 ;
real_T P_378 ; real_T P_379 ; real_T P_380 ; real_T P_381 ; real_T P_382 ;
real_T P_383 ; real_T P_384 ; real_T P_385 ; real_T P_386 ; real_T P_387 ;
real_T P_388 ; real_T P_389 ; real_T P_390 ; real_T P_391 ; real_T P_392 ;
real_T P_393 ; real_T P_394 ; real_T P_395 ; real_T P_396 ; real_T P_397 ;
real_T P_398 ; real_T P_399 ; real_T P_400 ; real_T P_401 ; real_T P_402 ;
real_T P_403 ; real_T P_404 ; real_T P_405 ; real_T P_406 ; real_T P_407 ;
real_T P_408 ; real_T P_409 ; real_T P_410 ; real_T P_411 ; real_T P_412 [ 9
] ; real_T P_413 ; real_T P_414 ; real_T P_415 ; real_T P_416 [ 6 ] ; real_T
P_417 [ 6 ] ; real_T P_418 ; real_T P_419 ; real_T P_420 ; real_T P_421 ;
real_T P_422 ; real_T P_423 ; real_T P_424 [ 7 ] ; real_T P_425 ; real_T
P_426 ; real_T P_427 ; real_T P_428 ; real_T P_429 ; real_T P_430 ; real_T
P_431 ; real_T P_432 ; real_T P_433 ; real_T P_434 ; real_T P_435 ; real_T
P_436 ; real_T P_437 ; real_T P_438 ; real_T P_439 ; real_T P_440 ; real_T
P_441 ; real_T P_442 ; real_T P_443 ; real_T P_444 ; real_T P_445 ; real_T
P_446 ; real_T P_447 ; real_T P_448 ; real_T P_449 ; real_T P_450 ; real_T
P_451 ; real_T P_452 ; real_T P_453 ; real_T P_454 ; real_T P_455 ; real_T
P_456 ; real_T P_457 ; real_T P_458 ; real_T P_459 ; real_T P_460 ; real_T
P_461 ; real_T P_462 ; real_T P_463 ; real_T P_464 ; real_T P_465 ; real_T
P_466 ; real_T P_467 ; real_T P_468 ; real_T P_469 ; real_T P_470 ; real_T
P_471 ; real_T P_472 ; real_T P_473 ; real_T P_474 ; real_T P_475 ; real_T
P_476 ; real_T P_477 ; real_T P_478 ; real_T P_479 ; real_T P_480 ; real_T
P_481 ; real_T P_482 ; real_T P_483 ; real_T P_484 ; real_T P_485 ; real_T
P_486 ; real_T P_487 ; real_T P_488 ; real_T P_489 ; real_T P_490 ; real_T
P_491 ; real_T P_492 ; real_T P_493 ; real_T P_494 ; real_T P_495 ; real_T
P_496 [ 2 ] ; real_T P_497 ; real_T P_498 ; real_T P_499 [ 2 ] ; real_T P_500
; real_T P_501 ; real_T P_502 ; real_T P_503 ; real_T P_504 ; real_T P_505 ;
real_T P_506 ; real_T P_507 ; real_T P_508 ; real_T P_509 ; real_T P_510 ;
real_T P_511 ; real_T P_512 ; real_T P_513 ; real_T P_514 ; real_T P_515 ;
real_T P_516 ; real_T P_517 ; real_T P_518 ; real_T P_519 ; real_T P_520 ;
real_T P_521 ; real_T P_522 ; real_T P_523 ; real_T P_524 ; real_T P_525 ;
real_T P_526 ; real_T P_527 ; real_T P_528 ; real_T P_529 ; real_T P_530 ;
P_Subsystem1_PV_Battery_Grid2021a_j_T Subsystem1_m ;
P_Subsystempi2delay_PV_Battery_Grid2021a_k_T Subsystempi2delay_f ;
P_Subsystem1_PV_Battery_Grid2021a_j_T Subsystem1_h ;
P_Subsystempi2delay_PV_Battery_Grid2021a_k_T Subsystempi2delay_d ;
P_Subsystem1_PV_Battery_Grid2021a_T Subsystem1_j ;
P_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay_a ;
P_Subsystem1_PV_Battery_Grid2021a_T Subsystem1_b ;
P_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay_e ;
P_AutomaticGainControl_PV_Battery_Grid2021a_T AutomaticGainControl_p ;
P_Subsystem1_PV_Battery_Grid2021a_T Subsystem1_i ;
P_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay_n ;
P_ZeroSeqComputation_PV_Battery_Grid2021a_T ZeroSeqComputation_f ;
P_NegSeqComputation_PV_Battery_Grid2021a_T PosSeqComputation_o ;
P_NegSeqComputation_PV_Battery_Grid2021a_T NegSeqComputation_h ;
P_ZeroSeqComputation_PV_Battery_Grid2021a_T ZeroSeqComputation ;
P_NegSeqComputation_PV_Battery_Grid2021a_T PosSeqComputation ;
P_NegSeqComputation_PV_Battery_Grid2021a_T NegSeqComputation ;
P_Subsystem1_PV_Battery_Grid2021a_T Subsystem1 ;
P_Subsystempi2delay_PV_Battery_Grid2021a_T Subsystempi2delay ;
P_AutomaticGainControl_PV_Battery_Grid2021a_T AutomaticGainControl ; } ;
extern P_PV_Battery_Grid2021a_T PV_Battery_Grid2021a_rtDefaultP ; extern
const ConstB_PV_Battery_Grid2021a_T PV_Battery_Grid2021a_rtInvariant ;
#endif
