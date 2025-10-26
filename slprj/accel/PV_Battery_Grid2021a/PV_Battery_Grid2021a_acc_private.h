#ifndef RTW_HEADER_PV_Battery_Grid2021a_acc_private_h_
#define RTW_HEADER_PV_Battery_Grid2021a_acc_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include "zero_crossing_types.h"
#include "PV_Battery_Grid2021a_acc.h"
#include "PV_Battery_Grid2021a_acc_types.h"
#if !defined(ss_VALIDATE_MEMORY)
#define ss_VALIDATE_MEMORY(S, ptr)     if(!(ptr)) {\
    ssSetErrorStatus(S, RT_MEMORY_ALLOCATION_ERROR);\
    }
#endif
#if !defined(rt_FREE)
#if !defined(_WIN32)
#define rt_FREE(ptr)     if((ptr) != (NULL)) {\
    free((ptr));\
    (ptr) = (NULL);\
    }
#else
#define rt_FREE(ptr)     if((ptr) != (NULL)) {\
    free((void *)(ptr));\
    (ptr) = (NULL);\
    }
#endif
#endif
#ifndef __RTW_UTFREE__
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T *
bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T
tMinusDelay , real_T * * uBufPtr , boolean_T isfixedbuf , boolean_T
istransportdelay , int_T * maxNewBufSzPtr ) ; real_T
PV_Battery_Grid2021a_acc_rt_VTDelayfindtDInterpolate ( real_T x , real_T *
uBuf , int_T bufSz , int_T head , int_T tail , int_T * pLast , real_T t ,
real_T tStart , boolean_T discrete , boolean_T minorStepAndTAtLastMajorOutput
, real_T initOutput , real_T * appliedDelay ) ; real_T
PV_Battery_Grid2021a_acc_rt_TDelayInterpolate ( real_T tMinusDelay , real_T
tStart , real_T * uBuf , int_T bufSz , int_T * lastIdx , int_T oldestIdx ,
int_T newIdx , real_T initOutput , boolean_T discrete , boolean_T
minorStepAndTAtLastMajorOutput ) ; extern real_T look1_binlxpw ( real_T u0 ,
const real_T bp0 [ ] , const real_T table [ ] , uint32_T maxIndex ) ; extern
real_T look1_pbinlxpw ( real_T u0 , const real_T bp0 [ ] , const real_T table
[ ] , uint32_T prevIndex [ ] , uint32_T maxIndex ) ; void
PV_Battery_Grid2021a_Subsystempi2delay_Init ( SimStruct * S ,
B_Subsystempi2delay_PV_Battery_Grid2021a_T * localB ,
P_Subsystempi2delay_PV_Battery_Grid2021a_T * localP ) ; void
PV_Battery_Grid2021a_Subsystempi2delay_Disable ( SimStruct * S ,
DW_Subsystempi2delay_PV_Battery_Grid2021a_T * localDW ) ; void
PV_Battery_Grid2021a_Subsystempi2delay ( SimStruct * S , uint8_T rtu_Enable ,
const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystempi2delay_PV_Battery_Grid2021a_T * localB ,
DW_Subsystempi2delay_PV_Battery_Grid2021a_T * localDW ) ; void
PV_Battery_Grid2021a_Subsystem1_Init ( SimStruct * S ,
B_Subsystem1_PV_Battery_Grid2021a_T * localB ,
P_Subsystem1_PV_Battery_Grid2021a_T * localP ) ; void
PV_Battery_Grid2021a_Subsystem1_Disable ( SimStruct * S ,
DW_Subsystem1_PV_Battery_Grid2021a_T * localDW ) ; void
PV_Battery_Grid2021a_Subsystem1 ( SimStruct * S , uint8_T rtu_Enable , const
real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystem1_PV_Battery_Grid2021a_T * localB ,
DW_Subsystem1_PV_Battery_Grid2021a_T * localDW ) ; void
PV_Battery_Grid2021a_AutomaticGainControl_Init ( SimStruct * S ,
B_AutomaticGainControl_PV_Battery_Grid2021a_T * localB ,
P_AutomaticGainControl_PV_Battery_Grid2021a_T * localP ,
X_AutomaticGainControl_PV_Battery_Grid2021a_T * localX ) ; void
PV_Battery_Grid2021a_AutomaticGainControl_Deriv ( SimStruct * S ,
B_AutomaticGainControl_PV_Battery_Grid2021a_T * localB ,
DW_AutomaticGainControl_PV_Battery_Grid2021a_T * localDW ,
P_AutomaticGainControl_PV_Battery_Grid2021a_T * localP ,
XDot_AutomaticGainControl_PV_Battery_Grid2021a_T * localXdot ) ; void
PV_Battery_Grid2021a_AutomaticGainControl_ZC ( SimStruct * S ,
B_AutomaticGainControl_PV_Battery_Grid2021a_T * localB ,
DW_AutomaticGainControl_PV_Battery_Grid2021a_T * localDW ,
P_AutomaticGainControl_PV_Battery_Grid2021a_T * localP ,
ZCV_AutomaticGainControl_PV_Battery_Grid2021a_T * localZCSV ) ; void
PV_Battery_Grid2021a_AutomaticGainControl_Disable ( SimStruct * S ,
DW_AutomaticGainControl_PV_Battery_Grid2021a_T * localDW ) ; void
PV_Battery_Grid2021a_AutomaticGainControl_Update ( SimStruct * S ,
B_AutomaticGainControl_PV_Battery_Grid2021a_T * localB ,
DW_AutomaticGainControl_PV_Battery_Grid2021a_T * localDW ,
P_AutomaticGainControl_PV_Battery_Grid2021a_T * localP ,
X_AutomaticGainControl_PV_Battery_Grid2021a_T * localX ) ; void
PV_Battery_Grid2021a_AutomaticGainControl ( SimStruct * S , real_T rtu_Enable
, real_T rtu_Freq , real_T rtu_wt , const real_T rtu_Vabc [ 3 ] ,
B_AutomaticGainControl_PV_Battery_Grid2021a_T * localB ,
DW_AutomaticGainControl_PV_Battery_Grid2021a_T * localDW ,
P_AutomaticGainControl_PV_Battery_Grid2021a_T * localP ,
X_AutomaticGainControl_PV_Battery_Grid2021a_T * localX ,
XDis_AutomaticGainControl_PV_Battery_Grid2021a_T * localXdis ) ; void
PV_Battery_Grid2021a_AutomaticGainControlTID9 ( SimStruct * S ,
B_AutomaticGainControl_PV_Battery_Grid2021a_T * localB ,
DW_AutomaticGainControl_PV_Battery_Grid2021a_T * localDW ,
P_AutomaticGainControl_PV_Battery_Grid2021a_T * localP ) ; void
PV_Battery_Grid2021a_NegSeqComputation_Init ( SimStruct * S ,
B_NegSeqComputation_PV_Battery_Grid2021a_T * localB ,
P_NegSeqComputation_PV_Battery_Grid2021a_T * localP ) ; void
PV_Battery_Grid2021a_NegSeqComputation_Disable ( SimStruct * S ,
DW_NegSeqComputation_PV_Battery_Grid2021a_T * localDW ) ; void
PV_Battery_Grid2021a_NegSeqComputation ( SimStruct * S , real_T rtu_Enable ,
creal_T rtu_In , creal_T rtu_In_p , creal_T rtu_In_c ,
B_NegSeqComputation_PV_Battery_Grid2021a_T * localB ,
DW_NegSeqComputation_PV_Battery_Grid2021a_T * localDW ,
P_NegSeqComputation_PV_Battery_Grid2021a_T * localP ) ; void
PV_Battery_Grid2021a_ZeroSeqComputation_Init ( SimStruct * S ,
B_ZeroSeqComputation_PV_Battery_Grid2021a_T * localB ,
P_ZeroSeqComputation_PV_Battery_Grid2021a_T * localP ) ; void
PV_Battery_Grid2021a_ZeroSeqComputation_Disable ( SimStruct * S ,
DW_ZeroSeqComputation_PV_Battery_Grid2021a_T * localDW ) ; void
PV_Battery_Grid2021a_ZeroSeqComputation ( SimStruct * S , real_T rtu_Enable ,
creal_T rtu_In , creal_T rtu_In_a , creal_T rtu_In_m ,
B_ZeroSeqComputation_PV_Battery_Grid2021a_T * localB ,
DW_ZeroSeqComputation_PV_Battery_Grid2021a_T * localDW ,
P_ZeroSeqComputation_PV_Battery_Grid2021a_T * localP ) ; void
PV_Battery_Grid2021a_Subsystempi2delay_a_Init ( SimStruct * S ,
B_Subsystempi2delay_PV_Battery_Grid2021a_j_T * localB ,
P_Subsystempi2delay_PV_Battery_Grid2021a_k_T * localP ) ; void
PV_Battery_Grid2021a_Subsystempi2delay_d ( SimStruct * S , uint8_T rtu_Enable
, const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystempi2delay_PV_Battery_Grid2021a_j_T * localB ,
DW_Subsystempi2delay_PV_Battery_Grid2021a_k_T * localDW ) ; void
PV_Battery_Grid2021a_Subsystem1_c_Init ( SimStruct * S ,
B_Subsystem1_PV_Battery_Grid2021a_g_T * localB ,
P_Subsystem1_PV_Battery_Grid2021a_j_T * localP ) ; void
PV_Battery_Grid2021a_Subsystem1_h ( SimStruct * S , uint8_T rtu_Enable ,
const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystem1_PV_Battery_Grid2021a_g_T * localB ,
DW_Subsystem1_PV_Battery_Grid2021a_f_T * localDW ) ; void
PV_Battery_Grid2021a_Subsystempi2delay_Term ( SimStruct * const S ) ; void
PV_Battery_Grid2021a_Subsystem1_Term ( SimStruct * const S ) ; void
PV_Battery_Grid2021a_AutomaticGainControl_Term ( SimStruct * const S ) ; void
PV_Battery_Grid2021a_NegSeqComputation_Term ( SimStruct * const S ) ; void
PV_Battery_Grid2021a_ZeroSeqComputation_Term ( SimStruct * const S ) ; void
PV_Battery_Grid2021a_Subsystempi2delay_e_Term ( SimStruct * const S ) ; void
PV_Battery_Grid2021a_Subsystem1_o_Term ( SimStruct * const S ) ;
#endif
