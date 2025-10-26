#include "PV_Battery_Grid2021a_acc.h"
#include "rtwtypes.h"
#include "PV_Battery_Grid2021a_acc_private.h"
#include "mwmathutil.h"
#include "rt_look.h"
#include "rt_look1d.h"
#include "multiword_types.h"
#include "simstruc_types.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "slexec_vm_lookup_functions.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simtarget/slSimTgtMdlrefSfcnBridge.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T *
bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T
tMinusDelay , real_T * * uBufPtr , boolean_T isfixedbuf , boolean_T
istransportdelay , int_T * maxNewBufSzPtr ) { int_T testIdx ; int_T tail = *
tailPtr ; int_T bufSz = * bufSzPtr ; real_T * tBuf = * uBufPtr + bufSz ;
real_T * xBuf = ( NULL ) ; int_T numBuffer = 2 ; if ( istransportdelay ) {
numBuffer = 3 ; xBuf = * uBufPtr + 2 * bufSz ; } testIdx = ( tail < ( bufSz -
1 ) ) ? ( tail + 1 ) : 0 ; if ( ( tMinusDelay <= tBuf [ testIdx ] ) && !
isfixedbuf ) { int_T j ; real_T * tempT ; real_T * tempU ; real_T * tempX = (
NULL ) ; real_T * uBuf = * uBufPtr ; int_T newBufSz = bufSz + 1024 ; if (
newBufSz > * maxNewBufSzPtr ) { * maxNewBufSzPtr = newBufSz ; } tempU = (
real_T * ) utMalloc ( numBuffer * newBufSz * sizeof ( real_T ) ) ; if ( tempU
== ( NULL ) ) { return ( false ) ; } tempT = tempU + newBufSz ; if (
istransportdelay ) tempX = tempT + newBufSz ; for ( j = tail ; j < bufSz ; j
++ ) { tempT [ j - tail ] = tBuf [ j ] ; tempU [ j - tail ] = uBuf [ j ] ; if
( istransportdelay ) tempX [ j - tail ] = xBuf [ j ] ; } for ( j = 0 ; j <
tail ; j ++ ) { tempT [ j + bufSz - tail ] = tBuf [ j ] ; tempU [ j + bufSz -
tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j + bufSz - tail ] =
xBuf [ j ] ; } if ( * lastPtr > tail ) { * lastPtr -= tail ; } else { *
lastPtr += ( bufSz - tail ) ; } * tailPtr = 0 ; * headPtr = bufSz ; utFree (
uBuf ) ; * bufSzPtr = newBufSz ; * uBufPtr = tempU ; } else { * tailPtr =
testIdx ; } return ( true ) ; } real_T
PV_Battery_Grid2021a_acc_rt_VTDelayfindtDInterpolate ( real_T x , real_T *
uBuf , int_T bufSz , int_T head , int_T tail , int_T * pLast , real_T t ,
real_T tStart , boolean_T discrete , boolean_T minorStepAndTAtLastMajorOutput
, real_T initOutput , real_T * appliedDelay ) { int_T n , k ; real_T f ;
int_T kp1 ; real_T tminustD , tL , tR , uD , uL , uR , fU ; real_T * tBuf =
uBuf + bufSz ; real_T * xBuf = uBuf + 2 * bufSz ; if (
minorStepAndTAtLastMajorOutput ) { if ( * pLast == head ) { * pLast = ( *
pLast == 0 ) ? bufSz - 1 : * pLast - 1 ; } head = ( head == 0 ) ? bufSz - 1 :
head - 1 ; } if ( x <= 1 ) { return initOutput ; } k = * pLast ; n = 0 ; for
( ; ; ) { n ++ ; if ( n > bufSz ) break ; if ( x - xBuf [ k ] > 1.0 ) { if (
k == head ) { int_T km1 ; f = ( x - 1.0 - xBuf [ k ] ) / ( x - xBuf [ k ] ) ;
tminustD = ( 1.0 - f ) * tBuf [ k ] + f * t ; km1 = k - 1 ; if ( km1 < 0 )
km1 = bufSz - 1 ; tL = tBuf [ km1 ] ; tR = tBuf [ k ] ; uL = uBuf [ km1 ] ;
uR = uBuf [ k ] ; break ; } kp1 = k + 1 ; if ( kp1 == bufSz ) kp1 = 0 ; if (
x - xBuf [ kp1 ] <= 1.0 ) { f = ( x - 1.0 - xBuf [ k ] ) / ( xBuf [ kp1 ] -
xBuf [ k ] ) ; tL = tBuf [ k ] ; tR = tBuf [ kp1 ] ; uL = uBuf [ k ] ; uR =
uBuf [ kp1 ] ; tminustD = ( 1.0 - f ) * tL + f * tR ; break ; } k = kp1 ; }
else { if ( k == tail ) { f = ( x - 1.0 ) / xBuf [ k ] ; if ( discrete ) {
return ( uBuf [ tail ] ) ; } kp1 = k + 1 ; if ( kp1 == bufSz ) kp1 = 0 ;
tminustD = ( 1 - f ) * tStart + f * tBuf [ k ] ; tL = tBuf [ k ] ; tR = tBuf
[ kp1 ] ; uL = uBuf [ k ] ; uR = uBuf [ kp1 ] ; break ; } k = k - 1 ; if ( k
< 0 ) k = bufSz - 1 ; } } * pLast = k ; if ( tR == tL ) { fU = 1.0 ; } else {
fU = ( tminustD - tL ) / ( tR - tL ) ; } if ( discrete ) { uD = ( fU > ( 1.0
- fU ) ) ? uR : uL ; } else { uD = ( 1.0 - fU ) * uL + fU * uR ; } *
appliedDelay = t - tminustD ; return uD ; } real_T
PV_Battery_Grid2021a_acc_rt_TDelayInterpolate ( real_T tMinusDelay , real_T
tStart , real_T * uBuf , int_T bufSz , int_T * lastIdx , int_T oldestIdx ,
int_T newIdx , real_T initOutput , boolean_T discrete , boolean_T
minorStepAndTAtLastMajorOutput ) { int_T i ; real_T yout , t1 , t2 , u1 , u2
; real_T * tBuf = uBuf + bufSz ; if ( ( newIdx == 0 ) && ( oldestIdx == 0 )
&& ( tMinusDelay > tStart ) ) return initOutput ; if ( tMinusDelay <= tStart
) return initOutput ; if ( ( tMinusDelay <= tBuf [ oldestIdx ] ) ) { if (
discrete ) { return ( uBuf [ oldestIdx ] ) ; } else { int_T tempIdx =
oldestIdx + 1 ; if ( oldestIdx == bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [
oldestIdx ] ; t2 = tBuf [ tempIdx ] ; u1 = uBuf [ oldestIdx ] ; u2 = uBuf [
tempIdx ] ; if ( t2 == t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else {
yout = u1 ; } } else { real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ;
real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2 ; } return yout ; } } if (
minorStepAndTAtLastMajorOutput ) { if ( newIdx != 0 ) { if ( * lastIdx ==
newIdx ) { ( * lastIdx ) -- ; } newIdx -- ; } else { if ( * lastIdx == newIdx
) { * lastIdx = bufSz - 1 ; } newIdx = bufSz - 1 ; } } i = * lastIdx ; if (
tBuf [ i ] < tMinusDelay ) { while ( tBuf [ i ] < tMinusDelay ) { if ( i ==
newIdx ) break ; i = ( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } } else { while
( tBuf [ i ] >= tMinusDelay ) { i = ( i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i =
( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } * lastIdx = i ; if ( discrete ) {
double tempEps = ( DBL_EPSILON ) * 128.0 ; double localEps = tempEps *
muDoubleScalarAbs ( tBuf [ i ] ) ; if ( tempEps > localEps ) { localEps =
tempEps ; } localEps = localEps / 2.0 ; if ( tMinusDelay >= ( tBuf [ i ] -
localEps ) ) { yout = uBuf [ i ] ; } else { if ( i == 0 ) { yout = uBuf [
bufSz - 1 ] ; } else { yout = uBuf [ i - 1 ] ; } } } else { if ( i == 0 ) {
t1 = tBuf [ bufSz - 1 ] ; u1 = uBuf [ bufSz - 1 ] ; } else { t1 = tBuf [ i -
1 ] ; u1 = uBuf [ i - 1 ] ; } t2 = tBuf [ i ] ; u2 = uBuf [ i ] ; if ( t2 ==
t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else {
real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout
= f1 * u1 + f2 * u2 ; } } return ( yout ) ; } real_T look1_binlxpw ( real_T
u0 , const real_T bp0 [ ] , const real_T table [ ] , uint32_T maxIndex ) {
real_T frac ; real_T yL_0d0 ; uint32_T bpIdx ; uint32_T iLeft ; uint32_T
iRght ; if ( u0 <= bp0 [ 0U ] ) { iLeft = 0U ; frac = ( u0 - bp0 [ 0U ] ) / (
bp0 [ 1U ] - bp0 [ 0U ] ) ; } else if ( u0 < bp0 [ maxIndex ] ) { bpIdx =
maxIndex >> 1U ; iLeft = 0U ; iRght = maxIndex ; while ( iRght - iLeft > 1U )
{ if ( u0 < bp0 [ bpIdx ] ) { iRght = bpIdx ; } else { iLeft = bpIdx ; }
bpIdx = ( iRght + iLeft ) >> 1U ; } frac = ( u0 - bp0 [ iLeft ] ) / ( bp0 [
iLeft + 1U ] - bp0 [ iLeft ] ) ; } else { iLeft = maxIndex - 1U ; frac = ( u0
- bp0 [ maxIndex - 1U ] ) / ( bp0 [ maxIndex ] - bp0 [ maxIndex - 1U ] ) ; }
yL_0d0 = table [ iLeft ] ; return ( table [ iLeft + 1U ] - yL_0d0 ) * frac +
yL_0d0 ; } real_T look1_pbinlxpw ( real_T u0 , const real_T bp0 [ ] , const
real_T table [ ] , uint32_T prevIndex [ ] , uint32_T maxIndex ) { real_T frac
; real_T yL_0d0 ; uint32_T bpIdx ; uint32_T found ; uint32_T iLeft ; uint32_T
iRght ; if ( u0 <= bp0 [ 0U ] ) { bpIdx = 0U ; frac = ( u0 - bp0 [ 0U ] ) / (
bp0 [ 1U ] - bp0 [ 0U ] ) ; } else if ( u0 < bp0 [ maxIndex ] ) { bpIdx =
prevIndex [ 0U ] ; iLeft = 0U ; iRght = maxIndex ; found = 0U ; while ( found
== 0U ) { if ( u0 < bp0 [ bpIdx ] ) { iRght = bpIdx - 1U ; bpIdx = ( ( bpIdx
+ iLeft ) - 1U ) >> 1U ; } else if ( u0 < bp0 [ bpIdx + 1U ] ) { found = 1U ;
} else { iLeft = bpIdx + 1U ; bpIdx = ( ( bpIdx + iRght ) + 1U ) >> 1U ; } }
frac = ( u0 - bp0 [ bpIdx ] ) / ( bp0 [ bpIdx + 1U ] - bp0 [ bpIdx ] ) ; }
else { bpIdx = maxIndex - 1U ; frac = ( u0 - bp0 [ maxIndex - 1U ] ) / ( bp0
[ maxIndex ] - bp0 [ maxIndex - 1U ] ) ; } prevIndex [ 0U ] = bpIdx ; yL_0d0
= table [ bpIdx ] ; return ( table [ bpIdx + 1U ] - yL_0d0 ) * frac + yL_0d0
; } void rt_ssGetBlockPath ( SimStruct * S , int_T sysIdx , int_T blkIdx ,
char_T * * path ) { _ssGetBlockPath ( S , sysIdx , blkIdx , path ) ; } void
rt_ssSet_slErrMsg ( void * S , void * diag ) { SimStruct * castedS = (
SimStruct * ) S ; if ( ! _ssIsErrorStatusAslErrMsg ( castedS ) ) {
_ssSet_slErrMsg ( castedS , diag ) ; } else { _ssDiscardDiagnostic ( castedS
, diag ) ; } } void rt_ssReportDiagnosticAsWarning ( void * S , void * diag )
{ _ssReportDiagnosticAsWarning ( ( SimStruct * ) S , diag ) ; } void
rt_ssReportDiagnosticAsInfo ( void * S , void * diag ) {
_ssReportDiagnosticAsInfo ( ( SimStruct * ) S , diag ) ; } void
PV_Battery_Grid2021a_Subsystempi2delay_Init ( SimStruct * S ,
B_Subsystempi2delay_PV_Battery_Grid2021a_T * localB ,
P_Subsystempi2delay_PV_Battery_Grid2021a_T * localP ) { localB -> B_13_0_0 =
localP -> P_0 [ 0 ] ; localB -> B_13_1_0 = localP -> P_0 [ 1 ] ; } void
PV_Battery_Grid2021a_Subsystempi2delay_Disable ( SimStruct * S ,
DW_Subsystempi2delay_PV_Battery_Grid2021a_T * localDW ) { localDW ->
Subsystempi2delay_MODE = false ; } void
PV_Battery_Grid2021a_Subsystempi2delay ( SimStruct * S , uint8_T rtu_Enable ,
const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystempi2delay_PV_Battery_Grid2021a_T * localB ,
DW_Subsystempi2delay_PV_Battery_Grid2021a_T * localDW ) { int32_T isHit ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) &&
ssIsModeUpdateTimeStep ( S ) ) { if ( rtu_Enable > 0 ) { if ( ! localDW ->
Subsystempi2delay_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S )
) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
Subsystempi2delay_MODE = true ; } } else if ( localDW ->
Subsystempi2delay_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
PV_Battery_Grid2021a_Subsystempi2delay_Disable ( S , localDW ) ; } } if (
localDW -> Subsystempi2delay_MODE ) { localB -> B_13_0_0 = rtu_alpha_beta [ 0
] * muDoubleScalarSin ( rtu_wt ) - rtu_alpha_beta [ 1 ] * muDoubleScalarCos (
rtu_wt ) ; localB -> B_13_1_0 = rtu_alpha_beta [ 0 ] * muDoubleScalarCos (
rtu_wt ) + rtu_alpha_beta [ 1 ] * muDoubleScalarSin ( rtu_wt ) ; if (
ssIsModeUpdateTimeStep ( S ) ) { srUpdateBC ( localDW ->
Subsystempi2delay_SubsysRanBC ) ; } } } void
PV_Battery_Grid2021a_Subsystempi2delay_Term ( SimStruct * const S ) { } void
PV_Battery_Grid2021a_Subsystem1_Init ( SimStruct * S ,
B_Subsystem1_PV_Battery_Grid2021a_T * localB ,
P_Subsystem1_PV_Battery_Grid2021a_T * localP ) { localB -> B_14_0_0 = localP
-> P_0 [ 0 ] ; localB -> B_14_1_0 = localP -> P_0 [ 1 ] ; } void
PV_Battery_Grid2021a_Subsystem1_Disable ( SimStruct * S ,
DW_Subsystem1_PV_Battery_Grid2021a_T * localDW ) { localDW -> Subsystem1_MODE
= false ; } void PV_Battery_Grid2021a_Subsystem1 ( SimStruct * S , uint8_T
rtu_Enable , const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystem1_PV_Battery_Grid2021a_T * localB ,
DW_Subsystem1_PV_Battery_Grid2021a_T * localDW ) { int32_T isHit ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) && ssIsModeUpdateTimeStep (
S ) ) { if ( rtu_Enable > 0 ) { if ( ! localDW -> Subsystem1_MODE ) { if (
ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
Subsystem1_MODE = true ; } } else if ( localDW -> Subsystem1_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
PV_Battery_Grid2021a_Subsystem1_Disable ( S , localDW ) ; } } if ( localDW ->
Subsystem1_MODE ) { localB -> B_14_0_0 = rtu_alpha_beta [ 0 ] *
muDoubleScalarCos ( rtu_wt ) + rtu_alpha_beta [ 1 ] * muDoubleScalarSin (
rtu_wt ) ; localB -> B_14_1_0 = - rtu_alpha_beta [ 0 ] * muDoubleScalarSin (
rtu_wt ) + rtu_alpha_beta [ 1 ] * muDoubleScalarCos ( rtu_wt ) ; if (
ssIsModeUpdateTimeStep ( S ) ) { srUpdateBC ( localDW ->
Subsystem1_SubsysRanBC ) ; } } } void PV_Battery_Grid2021a_Subsystem1_Term (
SimStruct * const S ) { } void PV_Battery_Grid2021a_AutomaticGainControl_Init
( SimStruct * S , B_AutomaticGainControl_PV_Battery_Grid2021a_T * localB ,
P_AutomaticGainControl_PV_Battery_Grid2021a_T * localP ,
X_AutomaticGainControl_PV_Battery_Grid2021a_T * localX ) { localX ->
VariableTransportDelay_CSTATE_d = 0.0 ; localX -> integrator_CSTATE_k =
localP -> P_3 ; localX -> VariableTransportDelay_CSTATE_c = 0.0 ; localX ->
integrator_CSTATE_nk = localP -> P_6 ;
PV_Battery_Grid2021a_Subsystempi2delay_Init ( S , & localB ->
Subsystempi2delay , & localP -> Subsystempi2delay ) ;
PV_Battery_Grid2021a_Subsystem1_Init ( S , & localB -> Subsystem1 , & localP
-> Subsystem1 ) ; localB -> B_15_15_0 = localP -> P_0 ; } void
PV_Battery_Grid2021a_AutomaticGainControl_Disable ( SimStruct * S ,
DW_AutomaticGainControl_PV_Battery_Grid2021a_T * localDW ) { if ( localDW ->
Subsystempi2delay . Subsystempi2delay_MODE ) {
PV_Battery_Grid2021a_Subsystempi2delay_Disable ( S , & localDW ->
Subsystempi2delay ) ; } if ( localDW -> Subsystem1 . Subsystem1_MODE ) {
PV_Battery_Grid2021a_Subsystem1_Disable ( S , & localDW -> Subsystem1 ) ; }
localDW -> AutomaticGainControl_MODE = false ; } void
PV_Battery_Grid2021a_AutomaticGainControl ( SimStruct * S , real_T rtu_Enable
, real_T rtu_Freq , real_T rtu_wt , const real_T rtu_Vabc [ 3 ] ,
B_AutomaticGainControl_PV_Battery_Grid2021a_T * localB ,
DW_AutomaticGainControl_PV_Battery_Grid2021a_T * localDW ,
P_AutomaticGainControl_PV_Battery_Grid2021a_T * localP ,
X_AutomaticGainControl_PV_Battery_Grid2021a_T * localX ,
XDis_AutomaticGainControl_PV_Battery_Grid2021a_T * localXdis ) { int32_T
isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) &&
ssIsModeUpdateTimeStep ( S ) ) { if ( rtu_Enable > 0.0 ) { if ( ! localDW ->
AutomaticGainControl_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S
) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } ( void ) memset (
& ( localXdis -> VariableTransportDelay_CSTATE_d ) , 0 , 4 * sizeof (
boolean_T ) ) ; localDW -> AutomaticGainControl_MODE = true ; } } else { if (
ssGetTaskTime ( S , 1 ) == ssGetTStart ( S ) ) { ( void ) memset ( & (
localXdis -> VariableTransportDelay_CSTATE_d ) , 1 , 4 * sizeof ( boolean_T )
) ; } if ( localDW -> AutomaticGainControl_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & (
localXdis -> VariableTransportDelay_CSTATE_d ) , 1 , 4 * sizeof ( boolean_T )
) ; PV_Battery_Grid2021a_AutomaticGainControl_Disable ( S , localDW ) ; } } }
if ( localDW -> AutomaticGainControl_MODE ) { { real_T * * uBuffer = ( real_T
* * ) & localDW -> VariableTransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T
simTime = ssGetT ( S ) ; real_T appliedDelay ; localB -> B_15_0_0 =
PV_Battery_Grid2021a_acc_rt_VTDelayfindtDInterpolate ( localX ->
VariableTransportDelay_CSTATE_d , * uBuffer , localDW ->
VariableTransportDelay_IWORK . CircularBufSize , localDW ->
VariableTransportDelay_IWORK . Head , localDW -> VariableTransportDelay_IWORK
. Tail , & localDW -> VariableTransportDelay_IWORK . Last , simTime , 0.0 , 0
, ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + localDW ->
VariableTransportDelay_IWORK . CircularBufSize ) [ localDW ->
VariableTransportDelay_IWORK . Head ] == ssGetT ( S ) ) ) , localP -> P_2 , &
appliedDelay ) ; } localB -> B_15_1_0 = localX -> integrator_CSTATE_k ;
localB -> B_15_2_0 = ssGetT ( S ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { if ( ssIsModeUpdateTimeStep ( S ) ) { localDW ->
RelationalOperator_Mode = ( localB -> B_15_2_0 >= localB -> B_15_0_0_m ) ; }
localB -> B_15_3_0_p = localDW -> RelationalOperator_Mode ; } if ( localB ->
B_15_3_0_p ) { localB -> B_11_0_0 = localB -> B_15_1_0 - localB -> B_15_0_0 ;
localB -> B_11_1_0 = localB -> B_11_0_0 * rtu_Freq ; localB -> B_15_5_0 =
localB -> B_11_1_0 ; } else { localB -> B_15_5_0 = localB -> B_15_1_0_c ; } {
real_T * * uBuffer = ( real_T * * ) & localDW ->
VariableTransportDelay_PWORK_o . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT
( S ) ; real_T appliedDelay ; localB -> B_15_6_0 =
PV_Battery_Grid2021a_acc_rt_VTDelayfindtDInterpolate ( localX ->
VariableTransportDelay_CSTATE_c , * uBuffer , localDW ->
VariableTransportDelay_IWORK_i . CircularBufSize , localDW ->
VariableTransportDelay_IWORK_i . Head , localDW ->
VariableTransportDelay_IWORK_i . Tail , & localDW ->
VariableTransportDelay_IWORK_i . Last , simTime , 0.0 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + localDW ->
VariableTransportDelay_IWORK_i . CircularBufSize ) [ localDW ->
VariableTransportDelay_IWORK_i . Head ] == ssGetT ( S ) ) ) , localP -> P_5 ,
& appliedDelay ) ; } localB -> B_15_7_0 = localX -> integrator_CSTATE_nk ;
localB -> B_15_8_0 = ssGetT ( S ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { if ( ssIsModeUpdateTimeStep ( S ) ) { localDW ->
RelationalOperator_Mode_b = ( localB -> B_15_8_0 >= localB -> B_15_2_0_k ) ;
} localB -> B_15_9_0 = localDW -> RelationalOperator_Mode_b ; } if ( localB
-> B_15_9_0 ) { localB -> B_12_0_0 = localB -> B_15_7_0 - localB -> B_15_6_0
; localB -> B_12_1_0 = localB -> B_12_0_0 * rtu_Freq ; localB -> B_15_11_0 =
localB -> B_12_1_0 ; } else { localB -> B_15_11_0 = localB -> B_15_3_0 ; }
localB -> B_15_12_0 . re = localB -> B_15_5_0 ; localB -> B_15_12_0 . im =
localB -> B_15_11_0 ; localB -> B_15_13_0 = muDoubleScalarHypot ( localB ->
B_15_12_0 . re , localB -> B_15_12_0 . im ) ; if ( ssIsModeUpdateTimeStep ( S
) ) { localDW -> Saturation_MODE = localB -> B_15_13_0 >= localP -> P_7 ? 1 :
localB -> B_15_13_0 > localP -> P_8 ? 0 : - 1 ; } localB -> B_15_14_0 =
localDW -> Saturation_MODE == 1 ? localP -> P_7 : localDW -> Saturation_MODE
== - 1 ? localP -> P_8 : localB -> B_15_13_0 ; localB -> B_15_15_0 = 1.0 /
localB -> B_15_14_0 ; if ( rtu_Freq > localP -> P_9 ) { localB -> B_15_16_0 =
localP -> P_9 ; } else if ( rtu_Freq < localP -> P_10 ) { localB -> B_15_16_0
= localP -> P_10 ; } else { localB -> B_15_16_0 = rtu_Freq ; } localB ->
B_15_17_0 = 1.0 / localB -> B_15_16_0 ; if ( rtu_Freq > localP -> P_11 ) {
localB -> B_15_18_0 = localP -> P_11 ; } else if ( rtu_Freq < localP -> P_12
) { localB -> B_15_18_0 = localP -> P_12 ; } else { localB -> B_15_18_0 =
rtu_Freq ; } localB -> B_15_19_0 = 1.0 / localB -> B_15_18_0 ; for ( isHit =
0 ; isHit < 3 ; isHit ++ ) { localB -> B_15_20_0 [ isHit ] = 0.0 ; localB ->
B_15_20_0 [ isHit ] += localP -> P_13 [ isHit ] * rtu_Vabc [ 0 ] ; localB ->
B_15_20_0 [ isHit ] += localP -> P_13 [ isHit + 3 ] * rtu_Vabc [ 1 ] ; localB
-> B_15_20_0 [ isHit ] += localP -> P_13 [ isHit + 6 ] * rtu_Vabc [ 2 ] ; }
localB -> B_15_21_0 [ 0 ] = localP -> P_14 * localB -> B_15_20_0 [ 0 ] ;
localB -> B_15_21_0 [ 1 ] = localP -> P_14 * localB -> B_15_20_0 [ 1 ] ;
localB -> B_15_21_0 [ 2 ] = localP -> P_14 * localB -> B_15_20_0 [ 2 ] ;
PV_Battery_Grid2021a_Subsystempi2delay ( S , localB -> B_15_8_0_b , & localB
-> B_15_21_0 [ 0 ] , rtu_wt , & localB -> Subsystempi2delay , & localDW ->
Subsystempi2delay ) ; PV_Battery_Grid2021a_Subsystem1 ( S , localB ->
B_15_6_0_c , & localB -> B_15_21_0 [ 0 ] , rtu_wt , & localB -> Subsystem1 ,
& localDW -> Subsystem1 ) ; if ( localB -> B_15_6_0_c != 0 ) { localB ->
B_15_24_0 [ 0 ] = localB -> Subsystem1 . B_14_0_0 ; localB -> B_15_24_0 [ 1 ]
= localB -> Subsystem1 . B_14_1_0 ; } else { localB -> B_15_24_0 [ 0 ] =
localB -> Subsystempi2delay . B_13_0_0 ; localB -> B_15_24_0 [ 1 ] = localB
-> Subsystempi2delay . B_13_1_0 ; } if ( ssIsModeUpdateTimeStep ( S ) ) {
srUpdateBC ( localDW -> AutomaticGainControl_SubsysRanBC ) ; } } } void
PV_Battery_Grid2021a_AutomaticGainControlTID9 ( SimStruct * S ,
B_AutomaticGainControl_PV_Battery_Grid2021a_T * localB ,
DW_AutomaticGainControl_PV_Battery_Grid2021a_T * localDW ,
P_AutomaticGainControl_PV_Battery_Grid2021a_T * localP ) { localB ->
B_15_0_0_m = localP -> P_15 ; localB -> B_15_1_0_c = localP -> P_16 ; localB
-> B_15_2_0_k = localP -> P_17 ; localB -> B_15_3_0 = localP -> P_18 ; localB
-> B_15_6_0_c = ( uint8_T ) ( localP -> P_19 == localP -> P_20 ) ; localB ->
B_15_8_0_b = ( uint8_T ) ( localP -> P_19 == localP -> P_21 ) ; if (
ssIsModeUpdateTimeStep ( S ) ) { srUpdateBC ( localDW ->
AutomaticGainControl_SubsysRanBC ) ; } } void
PV_Battery_Grid2021a_AutomaticGainControl_Update ( SimStruct * S ,
B_AutomaticGainControl_PV_Battery_Grid2021a_T * localB ,
DW_AutomaticGainControl_PV_Battery_Grid2021a_T * localDW ,
P_AutomaticGainControl_PV_Battery_Grid2021a_T * localP ,
X_AutomaticGainControl_PV_Battery_Grid2021a_T * localX ) { if ( localDW ->
AutomaticGainControl_MODE ) { { real_T * * uBuffer = ( real_T * * ) & localDW
-> VariableTransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime =
ssGetT ( S ) ; localDW -> VariableTransportDelay_IWORK . Head = ( ( localDW
-> VariableTransportDelay_IWORK . Head < ( localDW ->
VariableTransportDelay_IWORK . CircularBufSize - 1 ) ) ? ( localDW ->
VariableTransportDelay_IWORK . Head + 1 ) : 0 ) ; if ( localDW ->
VariableTransportDelay_IWORK . Head == localDW ->
VariableTransportDelay_IWORK . Tail ) { if ( !
PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
VariableTransportDelay_IWORK . CircularBufSize , & localDW ->
VariableTransportDelay_IWORK . Tail , & localDW ->
VariableTransportDelay_IWORK . Head , & localDW ->
VariableTransportDelay_IWORK . Last , simTime - localP -> P_1 , uBuffer , (
boolean_T ) 0 , ( boolean_T ) 1 , & localDW -> VariableTransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "vtdelay memory allocation error"
) ; return ; } } ( * uBuffer + localDW -> VariableTransportDelay_IWORK .
CircularBufSize ) [ localDW -> VariableTransportDelay_IWORK . Head ] =
simTime ; ( * uBuffer + 2 * localDW -> VariableTransportDelay_IWORK .
CircularBufSize ) [ localDW -> VariableTransportDelay_IWORK . Head ] = localX
-> VariableTransportDelay_CSTATE_d ; ( * uBuffer ) [ localDW ->
VariableTransportDelay_IWORK . Head ] = localB -> B_15_1_0 ; } { real_T * *
uBuffer = ( real_T * * ) & localDW -> VariableTransportDelay_PWORK_o .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; localDW ->
VariableTransportDelay_IWORK_i . Head = ( ( localDW ->
VariableTransportDelay_IWORK_i . Head < ( localDW ->
VariableTransportDelay_IWORK_i . CircularBufSize - 1 ) ) ? ( localDW ->
VariableTransportDelay_IWORK_i . Head + 1 ) : 0 ) ; if ( localDW ->
VariableTransportDelay_IWORK_i . Head == localDW ->
VariableTransportDelay_IWORK_i . Tail ) { if ( !
PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
VariableTransportDelay_IWORK_i . CircularBufSize , & localDW ->
VariableTransportDelay_IWORK_i . Tail , & localDW ->
VariableTransportDelay_IWORK_i . Head , & localDW ->
VariableTransportDelay_IWORK_i . Last , simTime - localP -> P_4 , uBuffer , (
boolean_T ) 0 , ( boolean_T ) 1 , & localDW -> VariableTransportDelay_IWORK_i
. MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * uBuffer + localDW ->
VariableTransportDelay_IWORK_i . CircularBufSize ) [ localDW ->
VariableTransportDelay_IWORK_i . Head ] = simTime ; ( * uBuffer + 2 * localDW
-> VariableTransportDelay_IWORK_i . CircularBufSize ) [ localDW ->
VariableTransportDelay_IWORK_i . Head ] = localX ->
VariableTransportDelay_CSTATE_c ; ( * uBuffer ) [ localDW ->
VariableTransportDelay_IWORK_i . Head ] = localB -> B_15_7_0 ; } } } void
PV_Battery_Grid2021a_AutomaticGainControl_Deriv ( SimStruct * S ,
B_AutomaticGainControl_PV_Battery_Grid2021a_T * localB ,
DW_AutomaticGainControl_PV_Battery_Grid2021a_T * localDW ,
P_AutomaticGainControl_PV_Battery_Grid2021a_T * localP ,
XDot_AutomaticGainControl_PV_Battery_Grid2021a_T * localXdot ) { if ( localDW
-> AutomaticGainControl_MODE ) { { real_T instantDelay ; instantDelay =
localB -> B_15_17_0 ; if ( instantDelay > localP -> P_1 ) { instantDelay =
localP -> P_1 ; } if ( instantDelay < 0.0 ) { localXdot ->
VariableTransportDelay_CSTATE_d = 0 ; } else { localXdot ->
VariableTransportDelay_CSTATE_d = 1.0 / instantDelay ; } } localXdot ->
integrator_CSTATE_k = localB -> B_15_24_0 [ 0 ] ; { real_T instantDelay ;
instantDelay = localB -> B_15_19_0 ; if ( instantDelay > localP -> P_4 ) {
instantDelay = localP -> P_4 ; } if ( instantDelay < 0.0 ) { localXdot ->
VariableTransportDelay_CSTATE_c = 0 ; } else { localXdot ->
VariableTransportDelay_CSTATE_c = 1.0 / instantDelay ; } } localXdot ->
integrator_CSTATE_nk = localB -> B_15_24_0 [ 1 ] ; } else { { real_T * dx ;
int_T i ; dx = & ( localXdot -> VariableTransportDelay_CSTATE_d ) ; for ( i =
0 ; i < 4 ; i ++ ) { dx [ i ] = 0.0 ; } } } } void
PV_Battery_Grid2021a_AutomaticGainControl_ZC ( SimStruct * S ,
B_AutomaticGainControl_PV_Battery_Grid2021a_T * localB ,
DW_AutomaticGainControl_PV_Battery_Grid2021a_T * localDW ,
P_AutomaticGainControl_PV_Battery_Grid2021a_T * localP ,
ZCV_AutomaticGainControl_PV_Battery_Grid2021a_T * localZCSV ) { if ( localDW
-> AutomaticGainControl_MODE ) { localZCSV ->
RelationalOperator_RelopInput_ZC_n = localB -> B_15_2_0 - localB ->
B_15_0_0_m ; localZCSV -> RelationalOperator_RelopInput_ZC_c = localB ->
B_15_8_0 - localB -> B_15_2_0_k ; localZCSV -> Saturation_UprLim_ZC = localB
-> B_15_13_0 - localP -> P_7 ; localZCSV -> Saturation_LwrLim_ZC = localB ->
B_15_13_0 - localP -> P_8 ; } else { { real_T * zcsv = & ( localZCSV ->
RelationalOperator_RelopInput_ZC_n ) ; int_T i ; for ( i = 0 ; i < 4 ; i ++ )
{ zcsv [ i ] = 0.0 ; } } } } void
PV_Battery_Grid2021a_AutomaticGainControl_Term ( SimStruct * const S ) { }
void PV_Battery_Grid2021a_NegSeqComputation_Init ( SimStruct * S ,
B_NegSeqComputation_PV_Battery_Grid2021a_T * localB ,
P_NegSeqComputation_PV_Battery_Grid2021a_T * localP ) { localB -> B_25_2_0 .
re = localP -> P_0 ; localB -> B_25_2_0 . im = 0.0 ; } void
PV_Battery_Grid2021a_NegSeqComputation_Disable ( SimStruct * S ,
DW_NegSeqComputation_PV_Battery_Grid2021a_T * localDW ) { localDW ->
NegSeqComputation_MODE = false ; } void
PV_Battery_Grid2021a_NegSeqComputation ( SimStruct * S , real_T rtu_Enable ,
creal_T rtu_In , creal_T rtu_In_p , creal_T rtu_In_c ,
B_NegSeqComputation_PV_Battery_Grid2021a_T * localB ,
DW_NegSeqComputation_PV_Battery_Grid2021a_T * localDW ,
P_NegSeqComputation_PV_Battery_Grid2021a_T * localP ) { int32_T isHit ; isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) && ssIsModeUpdateTimeStep
( S ) ) { if ( rtu_Enable > 0.0 ) { if ( ! localDW -> NegSeqComputation_MODE
) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
NegSeqComputation_MODE = true ; } } else if ( localDW ->
NegSeqComputation_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
PV_Battery_Grid2021a_NegSeqComputation_Disable ( S , localDW ) ; } } if (
localDW -> NegSeqComputation_MODE ) { localB -> B_25_0_0 [ 0 ] . re = localP
-> P_2 [ 0 ] . re * rtu_In . re - localP -> P_2 [ 0 ] . im * rtu_In . im ;
localB -> B_25_0_0 [ 0 ] . im = localP -> P_2 [ 0 ] . re * rtu_In . im +
localP -> P_2 [ 0 ] . im * rtu_In . re ; localB -> B_25_0_0 [ 1 ] . re =
localP -> P_2 [ 1 ] . re * rtu_In_p . re - localP -> P_2 [ 1 ] . im *
rtu_In_p . im ; localB -> B_25_0_0 [ 1 ] . im = localP -> P_2 [ 1 ] . re *
rtu_In_p . im + localP -> P_2 [ 1 ] . im * rtu_In_p . re ; localB -> B_25_0_0
[ 2 ] . re = localP -> P_2 [ 2 ] . re * rtu_In_c . re - localP -> P_2 [ 2 ] .
im * rtu_In_c . im ; localB -> B_25_0_0 [ 2 ] . im = localP -> P_2 [ 2 ] . re
* rtu_In_c . im + localP -> P_2 [ 2 ] . im * rtu_In_c . re ; localB ->
B_25_1_0 . re = ( localB -> B_25_0_0 [ 0 ] . re + localB -> B_25_0_0 [ 1 ] .
re ) + localB -> B_25_0_0 [ 2 ] . re ; localB -> B_25_1_0 . im = ( localB ->
B_25_0_0 [ 0 ] . im + localB -> B_25_0_0 [ 1 ] . im ) + localB -> B_25_0_0 [
2 ] . im ; localB -> B_25_2_0 . re = localP -> P_1 * localB -> B_25_1_0 . re
; localB -> B_25_2_0 . im = localP -> P_1 * localB -> B_25_1_0 . im ; if (
ssIsModeUpdateTimeStep ( S ) ) { srUpdateBC ( localDW ->
NegSeqComputation_SubsysRanBC ) ; } } } void
PV_Battery_Grid2021a_NegSeqComputation_Term ( SimStruct * const S ) { } void
PV_Battery_Grid2021a_ZeroSeqComputation_Init ( SimStruct * S ,
B_ZeroSeqComputation_PV_Battery_Grid2021a_T * localB ,
P_ZeroSeqComputation_PV_Battery_Grid2021a_T * localP ) { localB -> B_27_1_0 .
re = localP -> P_0 ; localB -> B_27_1_0 . im = 0.0 ; } void
PV_Battery_Grid2021a_ZeroSeqComputation_Disable ( SimStruct * S ,
DW_ZeroSeqComputation_PV_Battery_Grid2021a_T * localDW ) { localDW ->
ZeroSeqComputation_MODE = false ; } void
PV_Battery_Grid2021a_ZeroSeqComputation ( SimStruct * S , real_T rtu_Enable ,
creal_T rtu_In , creal_T rtu_In_a , creal_T rtu_In_m ,
B_ZeroSeqComputation_PV_Battery_Grid2021a_T * localB ,
DW_ZeroSeqComputation_PV_Battery_Grid2021a_T * localDW ,
P_ZeroSeqComputation_PV_Battery_Grid2021a_T * localP ) { int32_T isHit ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) &&
ssIsModeUpdateTimeStep ( S ) ) { if ( rtu_Enable > 0.0 ) { if ( ! localDW ->
ZeroSeqComputation_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S )
) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
ZeroSeqComputation_MODE = true ; } } else if ( localDW ->
ZeroSeqComputation_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep ( S )
; PV_Battery_Grid2021a_ZeroSeqComputation_Disable ( S , localDW ) ; } } if (
localDW -> ZeroSeqComputation_MODE ) { localB -> B_27_0_0 . re = ( rtu_In .
re + rtu_In_a . re ) + rtu_In_m . re ; localB -> B_27_0_0 . im = ( rtu_In .
im + rtu_In_a . im ) + rtu_In_m . im ; localB -> B_27_1_0 . re = localP ->
P_1 * localB -> B_27_0_0 . re ; localB -> B_27_1_0 . im = localP -> P_1 *
localB -> B_27_0_0 . im ; if ( ssIsModeUpdateTimeStep ( S ) ) { srUpdateBC (
localDW -> ZeroSeqComputation_SubsysRanBC ) ; } } } void
PV_Battery_Grid2021a_ZeroSeqComputation_Term ( SimStruct * const S ) { } void
PV_Battery_Grid2021a_Subsystempi2delay_a_Init ( SimStruct * S ,
B_Subsystempi2delay_PV_Battery_Grid2021a_j_T * localB ,
P_Subsystempi2delay_PV_Battery_Grid2021a_k_T * localP ) { localB -> B_53_0_0
= localP -> P_0 [ 0 ] ; localB -> B_53_1_0 = localP -> P_0 [ 1 ] ; } void
PV_Battery_Grid2021a_Subsystempi2delay_d ( SimStruct * S , uint8_T rtu_Enable
, const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystempi2delay_PV_Battery_Grid2021a_j_T * localB ,
DW_Subsystempi2delay_PV_Battery_Grid2021a_k_T * localDW ) { if ( rtu_Enable >
0 ) { localB -> B_53_0_0 = rtu_alpha_beta [ 0 ] * muDoubleScalarSin ( rtu_wt
) - rtu_alpha_beta [ 1 ] * muDoubleScalarCos ( rtu_wt ) ; localB -> B_53_1_0
= rtu_alpha_beta [ 0 ] * muDoubleScalarCos ( rtu_wt ) + rtu_alpha_beta [ 1 ]
* muDoubleScalarSin ( rtu_wt ) ; if ( ssIsModeUpdateTimeStep ( S ) ) {
srUpdateBC ( localDW -> Subsystempi2delay_SubsysRanBC ) ; } } } void
PV_Battery_Grid2021a_Subsystempi2delay_e_Term ( SimStruct * const S ) { }
void PV_Battery_Grid2021a_Subsystem1_c_Init ( SimStruct * S ,
B_Subsystem1_PV_Battery_Grid2021a_g_T * localB ,
P_Subsystem1_PV_Battery_Grid2021a_j_T * localP ) { localB -> B_54_0_0 =
localP -> P_0 [ 0 ] ; localB -> B_54_1_0 = localP -> P_0 [ 1 ] ; } void
PV_Battery_Grid2021a_Subsystem1_h ( SimStruct * S , uint8_T rtu_Enable ,
const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystem1_PV_Battery_Grid2021a_g_T * localB ,
DW_Subsystem1_PV_Battery_Grid2021a_f_T * localDW ) { if ( rtu_Enable > 0 ) {
localB -> B_54_0_0 = rtu_alpha_beta [ 0 ] * muDoubleScalarCos ( rtu_wt ) +
rtu_alpha_beta [ 1 ] * muDoubleScalarSin ( rtu_wt ) ; localB -> B_54_1_0 = -
rtu_alpha_beta [ 0 ] * muDoubleScalarSin ( rtu_wt ) + rtu_alpha_beta [ 1 ] *
muDoubleScalarCos ( rtu_wt ) ; if ( ssIsModeUpdateTimeStep ( S ) ) {
srUpdateBC ( localDW -> Subsystem1_SubsysRanBC ) ; } } } void
PV_Battery_Grid2021a_Subsystem1_o_Term ( SimStruct * const S ) { } static
void mdlOutputs ( SimStruct * S , int_T tid ) { creal_T B_63_134_0 ; creal_T
B_63_155_0 ; creal_T B_63_176_0 ; creal_T B_63_201_0 ; creal_T B_63_222_0 ;
creal_T B_63_243_0 ; real_T B_63_115_0 ; real_T B_63_123_0 ; real_T
B_63_136_0 ; real_T B_63_144_0 ; real_T B_63_157_0 ; real_T B_63_165_0 ;
real_T B_63_182_0 ; real_T B_63_190_0 ; real_T B_63_203_0 ; real_T B_63_211_0
; real_T B_63_224_0 ; real_T B_63_232_0 ; real_T B_63_431_0 [ 3 ] ; real_T
B_63_435_0 ; real_T B_63_450_0 [ 3 ] ; B_PV_Battery_Grid2021a_T * _rtB ;
DW_PV_Battery_Grid2021a_T * _rtDW ; P_PV_Battery_Grid2021a_T * _rtP ;
PrevZCX_PV_Battery_Grid2021a_T * _rtZCE ; X_PV_Battery_Grid2021a_T * _rtX ;
real_T rtb_B_63_426_0 [ 6 ] ; real_T B_63_453_0_idx_1 ; real_T rtb_B_63_0_0 ;
real_T rtb_B_63_112_0 ; real_T rtb_B_63_12_0 ; real_T rtb_B_63_15_0 ; real_T
rtb_B_63_16_0 ; real_T rtb_B_63_17_0 ; real_T rtb_B_63_1_0 ; real_T
rtb_B_63_27_0 ; real_T rtb_B_63_2_0 ; real_T rtb_B_63_317_0 ; real_T
rtb_B_63_318_0 ; real_T rtb_B_63_33_0 ; real_T rtb_B_63_363_0 ; real_T
rtb_B_63_384_0 ; real_T rtb_B_63_41_0 ; real_T rtb_B_63_439_0 ; real_T
rtb_B_63_442_0 ; real_T rtb_B_63_443_0 ; real_T rtb_B_63_67_0 ; int32_T i ;
boolean_T rtb_B_63_34_0 ; boolean_T rtb_B_63_370_0 ; boolean_T rtb_B_63_9_0 ;
ZCEventType zcEvent ; _rtDW = ( ( DW_PV_Battery_Grid2021a_T * )
ssGetRootDWork ( S ) ) ; _rtZCE = ( ( PrevZCX_PV_Battery_Grid2021a_T * )
_ssGetPrevZCSigState ( S ) ) ; _rtX = ( ( X_PV_Battery_Grid2021a_T * )
ssGetContStates ( S ) ) ; _rtP = ( ( P_PV_Battery_Grid2021a_T * )
ssGetModelRtp ( S ) ) ; _rtB = ( ( B_PV_Battery_Grid2021a_T * )
_ssGetModelBlockIO ( S ) ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 )
{ rtb_B_63_0_0 = _rtDW -> itinit1_PreviousInput ; rtb_B_63_1_0 = _rtP -> P_34
* _rtDW -> itinit1_PreviousInput ; rtb_B_63_2_0 = 1.000001 * rtb_B_63_1_0 *
0.9009009009009098 / 0.9999 ; } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0
) { _rtB -> B_63_3_0 = _rtP -> P_35 * _rtDW -> Currentfilter_states ; _rtB ->
B_63_5_0 = ( _rtB -> B_63_3_0 > PV_Battery_Grid2021a_rtC ( S ) -> B_63_0_0 )
; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_6_0 =
_rtDW -> itinit_PreviousInput ; } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i !=
0 ) { if ( _rtDW -> inti_IC_LOADING != 0 ) { _rtDW -> inti_DSTATE = _rtB ->
B_63_6_0 ; if ( _rtDW -> inti_DSTATE >= _rtP -> P_40 ) { _rtDW -> inti_DSTATE
= _rtP -> P_40 ; } else if ( _rtDW -> inti_DSTATE <= _rtP -> P_41 ) { _rtDW
-> inti_DSTATE = _rtP -> P_41 ; } } if ( ( _rtB -> B_63_5_0 > 0.0 ) && (
_rtDW -> inti_PrevResetState <= 0 ) ) { _rtDW -> inti_DSTATE = _rtB ->
B_63_6_0 ; if ( _rtDW -> inti_DSTATE >= _rtP -> P_40 ) { _rtDW -> inti_DSTATE
= _rtP -> P_40 ; } else if ( _rtDW -> inti_DSTATE <= _rtP -> P_41 ) { _rtDW
-> inti_DSTATE = _rtP -> P_41 ; } } if ( _rtDW -> inti_DSTATE >= _rtP -> P_40
) { _rtDW -> inti_DSTATE = _rtP -> P_40 ; } else if ( _rtDW -> inti_DSTATE <=
_rtP -> P_41 ) { _rtDW -> inti_DSTATE = _rtP -> P_41 ; } _rtB -> B_63_8_0 =
_rtP -> P_42 * _rtDW -> inti_DSTATE ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if
( i != 0 ) { rtb_B_63_9_0 = ( _rtB -> B_63_8_0 > rtb_B_63_1_0 ) ; } i =
ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { if ( _rtB -> B_63_8_0 < _rtB ->
B_63_35_0_k ) { _rtB -> B_63_11_0 = _rtB -> B_63_35_0_k ; } else { _rtB ->
B_63_11_0 = _rtB -> B_63_8_0 ; } } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i
!= 0 ) { if ( rtb_B_63_9_0 ) { rtb_B_63_12_0 = rtb_B_63_1_0 ; } else {
rtb_B_63_12_0 = _rtB -> B_63_11_0 ; } if ( rtb_B_63_2_0 <= rtb_B_63_12_0 ) {
rtb_B_63_12_0 = rtb_B_63_1_0 ; } rtb_B_63_15_0 = - 1.19128690917505 *
rtb_B_63_0_0 / ( rtb_B_63_0_0 - rtb_B_63_12_0 ) * rtb_B_63_12_0 ;
rtb_B_63_16_0 = - _rtB -> B_63_5_0 * 1.19128690917505 * _rtB -> B_63_3_0 *
rtb_B_63_0_0 / ( rtb_B_63_0_0 - rtb_B_63_12_0 ) ; rtb_B_63_17_0 = _rtP ->
P_43 * rtb_B_63_0_0 ; if ( ! ( _rtB -> B_63_8_0 > rtb_B_63_17_0 ) ) {
rtb_B_63_1_0 = - rtb_B_63_17_0 * 0.999 * 0.1 * 0.9999 ; if ( _rtB -> B_63_8_0
< rtb_B_63_1_0 ) { rtb_B_63_17_0 = rtb_B_63_1_0 ; } else { rtb_B_63_17_0 =
_rtB -> B_63_8_0 ; } } } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) {
_rtB -> B_63_22_0 = ( _rtB -> B_63_3_0 < PV_Battery_Grid2021a_rtC ( S ) ->
B_63_1_0 ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { switch ( (
int32_T ) _rtB -> B_63_36_0 ) { case 1 : rtb_B_63_27_0 = - ( _rtB ->
B_63_40_0 * _rtB -> B_63_22_0 ) * 1.19128690917505 * ( _rtB -> B_63_40_0 *
_rtB -> B_63_3_0 ) * ( 5.7812499999999423 / ( _rtB -> B_63_40_0 *
rtb_B_63_17_0 + 0.57812499999999423 ) ) ; break ; case 2 : rtb_B_63_27_0 =
_rtB -> B_63_37_0_c * rtb_B_63_0_0 ; rtb_B_63_27_0 = - ( _rtB -> B_63_37_0_c
* _rtB -> B_63_22_0 ) * 1.19128690917505 * ( _rtB -> B_63_37_0_c * _rtB ->
B_63_3_0 ) * rtb_B_63_27_0 / ( _rtB -> B_63_37_0_c * rtb_B_63_17_0 +
rtb_B_63_27_0 * 0.1 ) ; break ; case 3 : rtb_B_63_27_0 = - ( _rtB ->
B_63_39_0 * _rtB -> B_63_22_0 ) * 1.19128690917505 * ( _rtB -> B_63_39_0 *
_rtB -> B_63_3_0 ) * ( 5.7812499999999423 / ( muDoubleScalarAbs ( _rtB ->
B_63_39_0 * rtb_B_63_17_0 ) + 0.57812499999999423 ) ) ; break ; default :
rtb_B_63_27_0 = - ( _rtB -> B_63_38_0_b * _rtB -> B_63_22_0 ) *
1.19128690917505 * ( _rtB -> B_63_38_0_b * _rtB -> B_63_3_0 ) * (
5.7812499999999423 / ( muDoubleScalarAbs ( _rtB -> B_63_38_0_b *
rtb_B_63_17_0 ) + 0.57812499999999423 ) ) ; break ; } } i = ssIsSampleHit ( S
, 3 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_28_0 = _rtDW ->
DiscreteTimeIntegrator_DSTATE ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i !=
0 ) { switch ( ( int32_T ) _rtB -> B_63_41_0 ) { case 1 : rtb_B_63_33_0 =
_rtB -> B_63_28_0 ; break ; case 2 : if ( rtb_B_63_12_0 > _rtP -> P_0 ) {
rtb_B_63_17_0 = _rtP -> P_0 ; } else if ( rtb_B_63_12_0 < _rtP -> P_1 ) {
rtb_B_63_17_0 = _rtP -> P_1 ; } else { rtb_B_63_17_0 = rtb_B_63_12_0 ; }
rtb_B_63_33_0 = muDoubleScalarExp ( - 180.0 * rtb_B_63_17_0 ) *
13.607571397381571 ; break ; case 3 : rtb_B_63_33_0 = _rtB -> B_63_28_0 ;
break ; default : rtb_B_63_33_0 = _rtB -> B_63_28_0 ; break ; } rtb_B_63_33_0
= ( ( ( ( rtb_B_63_15_0 + rtb_B_63_16_0 ) + rtb_B_63_27_0 ) + rtb_B_63_33_0 )
+ - 0.0 * rtb_B_63_12_0 ) + _rtB -> B_63_32_0 ; rtb_B_63_34_0 = (
rtb_B_63_33_0 > _rtB -> B_63_33_0 ) ; } i = ssIsSampleHit ( S , 3 , 0 ) ; if
( i != 0 ) { _rtB -> B_63_35_0 = _rtDW -> Memory2_PreviousInput ; } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( rtb_B_63_34_0 ) { _rtB ->
B_63_37_0 = _rtB -> B_63_33_0 ; } else if ( rtb_B_63_33_0 < _rtB -> B_63_35_0
) { _rtB -> B_63_37_0 = _rtB -> B_63_35_0 ; } else { _rtB -> B_63_37_0 =
rtb_B_63_33_0 ; } } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_38_0 = _rtDW -> UnitDelay_DSTATE ; } i = ssIsSampleHit ( S , 1 , 0 ) ;
if ( i != 0 ) { _rtDW -> Step_MODE = ( ssGetTaskTime ( S , 1 ) >= _rtP ->
P_48 ) ; if ( _rtDW -> Step_MODE == 1 ) { rtb_B_63_17_0 = _rtP -> P_50 ; }
else { rtb_B_63_17_0 = _rtP -> P_49 ; } rtb_B_63_41_0 = ( rtb_B_63_17_0 +
_rtB -> B_63_2_0 ) * _rtP -> P_51 ; _rtB -> B_63_42_0 = _rtB -> B_63_10_0 *
rtb_B_63_41_0 ; } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { if (
_rtDW -> systemEnable != 0 ) { _rtDW -> lastSin = muDoubleScalarSin ( _rtP ->
P_54 * ssGetTaskTime ( S , 3 ) ) ; _rtDW -> lastCos = muDoubleScalarCos (
_rtP -> P_54 * ssGetTaskTime ( S , 3 ) ) ; _rtDW -> systemEnable = 0 ; } _rtB
-> B_63_43_0 = ( ( _rtDW -> lastSin * _rtP -> P_58 + _rtDW -> lastCos * _rtP
-> P_57 ) * _rtP -> P_56 + ( _rtDW -> lastCos * _rtP -> P_58 - _rtDW ->
lastSin * _rtP -> P_57 ) * _rtP -> P_55 ) * _rtP -> P_52 + _rtP -> P_53 ; if
( _rtDW -> systemEnable_i != 0 ) { _rtDW -> lastSin_d = muDoubleScalarSin (
_rtP -> P_61 * ssGetTaskTime ( S , 3 ) ) ; _rtDW -> lastCos_b =
muDoubleScalarCos ( _rtP -> P_61 * ssGetTaskTime ( S , 3 ) ) ; _rtDW ->
systemEnable_i = 0 ; } _rtB -> B_63_44_0 = ( ( _rtDW -> lastSin_d * _rtP ->
P_65 + _rtDW -> lastCos_b * _rtP -> P_64 ) * _rtP -> P_63 + ( _rtDW ->
lastCos_b * _rtP -> P_65 - _rtDW -> lastSin_d * _rtP -> P_64 ) * _rtP -> P_62
) * _rtP -> P_59 + _rtP -> P_60 ; if ( _rtDW -> systemEnable_p != 0 ) { _rtDW
-> lastSin_b = muDoubleScalarSin ( _rtP -> P_68 * ssGetTaskTime ( S , 3 ) ) ;
_rtDW -> lastCos_j = muDoubleScalarCos ( _rtP -> P_68 * ssGetTaskTime ( S , 3
) ) ; _rtDW -> systemEnable_p = 0 ; } _rtB -> B_63_45_0 = ( ( _rtDW ->
lastSin_b * _rtP -> P_72 + _rtDW -> lastCos_j * _rtP -> P_71 ) * _rtP -> P_70
+ ( _rtDW -> lastCos_j * _rtP -> P_72 - _rtDW -> lastSin_b * _rtP -> P_71 ) *
_rtP -> P_69 ) * _rtP -> P_66 + _rtP -> P_67 ; ssCallAccelRunBlock ( S , 63 ,
46 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_63_59_0 = _rtP -> P_83 * _rtB ->
B_63_46_0 [ 37 ] * _rtB -> B_63_11_0_m + _rtP -> P_84 * _rtB -> B_63_46_0 [
19 ] ; _rtB -> B_63_66_0 = ( muDoubleScalarExp ( 1.0 / _rtB -> B_63_30_0 *
_rtB -> B_63_59_0 ) - _rtB -> B_63_31_0 ) * _rtB -> B_63_27_0 ; } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { rtb_B_63_67_0 = _rtB ->
B_63_59_0 / _rtB -> B_63_28_0_c * rtb_B_63_41_0 ; } i = ssIsSampleHit ( S , 3
, 0 ) ; if ( i != 0 ) { _rtB -> B_63_68_0 = _rtB -> B_63_59_0 / _rtB ->
B_63_29_0 ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_69_0 = ( _rtB -> B_63_66_0 + rtb_B_63_67_0 ) - _rtB -> B_63_68_0 ;
rtb_B_63_0_0 = ( 1.0 - rtb_B_63_12_0 / ( _rtP -> P_85 * rtb_B_63_0_0 ) ) *
100.0 ; if ( rtb_B_63_0_0 > _rtP -> P_86 ) { _rtB -> B_63_78_0 = _rtP -> P_86
; } else if ( rtb_B_63_0_0 < _rtP -> P_87 ) { _rtB -> B_63_78_0 = _rtP ->
P_87 ; } else { _rtB -> B_63_78_0 = rtb_B_63_0_0 ; } } i = ssIsSampleHit ( S
, 3 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_79_0 = _rtP -> P_88 * _rtB ->
B_63_46_0 [ 24 ] ; _rtB -> B_63_80_0 = _rtP -> P_89 * _rtB -> B_63_79_0 ; } i
= ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_81_0 = _rtB ->
B_63_37_0 - _rtB -> B_63_80_0 ; ssCallAccelRunBlock ( S , 63 , 82 ,
SS_CALL_MDL_OUTPUTS ) ; { if ( _rtDW ->
TAQSigLogging_InsertedFor_BusSelector_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_BusSelector_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_63_78_0 + 0 ) ; } } } i =
ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { { if ( _rtDW ->
TAQSigLogging_InsertedFor_BusSelector_at_outport_1_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_BusSelector_at_outport_1_PWORK . AQHandles ,
ssGetTaskTime ( S , 3 ) , ( char * ) & _rtB -> B_63_79_0 + 0 ) ; } } } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { { if ( _rtDW ->
TAQSigLogging_InsertedFor_BusSelector_at_outport_2_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_BusSelector_at_outport_2_PWORK . AQHandles ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_63_81_0 + 0 ) ; } } } i =
ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_98_0 = ( ( real_T
) ( _rtB -> B_63_79_0 < PV_Battery_Grid2021a_rtC ( S ) -> B_63_2_0 ) * _rtP
-> P_90 - _rtB -> B_63_28_0 ) * muDoubleScalarAbs ( _rtB -> B_63_79_0 ) *
_rtP -> P_91 ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_99_0 = _rtP -> P_92 * rtb_B_63_12_0 ; } i = ssIsSampleHit ( S , 3 , 0 )
; if ( i != 0 ) { _rtB -> B_63_100_0 = _rtP -> P_93 * _rtB -> B_63_79_0 ;
_rtB -> B_63_104_0 = _rtP -> P_94 * _rtB -> B_63_46_0 [ 21 ] ;
ssCallAccelRunBlock ( S , 63 , 105 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_63_106_0 = _rtP -> P_95 * _rtB -> B_63_46_0 [ 30 ] ; _rtB -> B_63_107_0 =
_rtP -> P_96 * _rtB -> B_63_46_0 [ 20 ] ; _rtB -> B_63_108_0 = _rtB ->
B_63_106_0 * _rtB -> B_63_107_0 ; _rtB -> B_63_110_0 = _rtP -> P_97 * _rtB ->
B_63_46_0 [ 29 ] * _rtB -> B_63_104_0 ; rtb_B_63_112_0 = _rtP -> P_99 * _rtB
-> B_63_46_0 [ 28 ] ; _rtB -> B_63_113_0 = _rtP -> P_98 * _rtB -> B_63_46_0 [
22 ] * rtb_B_63_112_0 ; } _rtB -> B_63_114_0 = _rtX -> integrator_CSTATE ; {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - _rtP -> P_101 ; B_63_115_0 =
PV_Battery_Grid2021a_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK . CircularBufSize , & _rtDW ->
TransportDelay_IWORK . Last , _rtDW -> TransportDelay_IWORK . Tail , _rtDW ->
TransportDelay_IWORK . Head , _rtP -> P_102 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK . Head ] == ssGetT ( S ) )
) ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_117_0
= _rtP -> P_103 ; } rtb_B_63_9_0 = ( ssGetT ( S ) >= _rtB -> B_63_117_0 ) ; i
= ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_119_0 = _rtDW ->
Memory_PreviousInput ; } if ( rtb_B_63_9_0 ) { _rtB -> B_63_121_0 = ( _rtB ->
B_63_114_0 - B_63_115_0 ) * _rtP -> P_5 ; } else { _rtB -> B_63_121_0 = _rtB
-> B_63_119_0 ; } _rtB -> B_63_122_0 = _rtX -> integrator_CSTATE_a ; { real_T
* * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_p . TUbufferPtrs
[ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP
-> P_106 ; B_63_123_0 = PV_Battery_Grid2021a_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * uBuffer , _rtDW -> TransportDelay_IWORK_c .
CircularBufSize , & _rtDW -> TransportDelay_IWORK_c . Last , _rtDW ->
TransportDelay_IWORK_c . Tail , _rtDW -> TransportDelay_IWORK_c . Head , _rtP
-> P_107 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer +
_rtDW -> TransportDelay_IWORK_c . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_c . Head ] == ssGetT ( S ) ) ) ) ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_125_0 = _rtP -> P_108 ; }
rtb_B_63_9_0 = ( ssGetT ( S ) >= _rtB -> B_63_125_0 ) ; i = ssIsSampleHit ( S
, 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_127_0 = _rtDW ->
Memory_PreviousInput_a ; } if ( rtb_B_63_9_0 ) { _rtB -> B_63_129_0 = ( _rtB
-> B_63_122_0 - B_63_123_0 ) * _rtP -> P_4 ; } else { _rtB -> B_63_129_0 =
_rtB -> B_63_127_0 ; } rtb_B_63_0_0 = _rtP -> P_110 * muDoubleScalarAtan2 (
_rtB -> B_63_129_0 , _rtB -> B_63_121_0 ) * _rtP -> P_111 ; B_63_134_0 . re =
muDoubleScalarHypot ( _rtB -> B_63_121_0 , _rtB -> B_63_129_0 ) *
muDoubleScalarCos ( rtb_B_63_0_0 ) ; B_63_134_0 . im = muDoubleScalarHypot (
_rtB -> B_63_121_0 , _rtB -> B_63_129_0 ) * muDoubleScalarSin ( rtb_B_63_0_0
) ; _rtB -> B_63_135_0 = _rtX -> integrator_CSTATE_af ; { real_T * * uBuffer
= ( real_T * * ) & _rtDW -> TransportDelay_PWORK_l . TUbufferPtrs [ 0 ] ;
real_T simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_113
; B_63_136_0 = PV_Battery_Grid2021a_acc_rt_TDelayInterpolate ( tMinusDelay ,
0.0 , * uBuffer , _rtDW -> TransportDelay_IWORK_h . CircularBufSize , & _rtDW
-> TransportDelay_IWORK_h . Last , _rtDW -> TransportDelay_IWORK_h . Tail ,
_rtDW -> TransportDelay_IWORK_h . Head , _rtP -> P_114 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_h .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_h . Head ] == ssGetT ( S )
) ) ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_138_0 = _rtP -> P_115 ; } rtb_B_63_9_0 = ( ssGetT ( S ) >= _rtB ->
B_63_138_0 ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_140_0 = _rtDW -> Memory_PreviousInput_p ; } if ( rtb_B_63_9_0 ) { _rtB
-> B_63_142_0 = ( _rtB -> B_63_135_0 - B_63_136_0 ) * _rtP -> P_7 ; } else {
_rtB -> B_63_142_0 = _rtB -> B_63_140_0 ; } _rtB -> B_63_143_0 = _rtX ->
integrator_CSTATE_f ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_o . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_118 ; B_63_144_0 =
PV_Battery_Grid2021a_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK_f . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_f . Last , _rtDW -> TransportDelay_IWORK_f . Tail ,
_rtDW -> TransportDelay_IWORK_f . Head , _rtP -> P_119 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_f .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_f . Head ] == ssGetT ( S )
) ) ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_146_0 = _rtP -> P_120 ; } rtb_B_63_9_0 = ( ssGetT ( S ) >= _rtB ->
B_63_146_0 ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_148_0 = _rtDW -> Memory_PreviousInput_d ; } if ( rtb_B_63_9_0 ) { _rtB
-> B_63_150_0 = ( _rtB -> B_63_143_0 - B_63_144_0 ) * _rtP -> P_6 ; } else {
_rtB -> B_63_150_0 = _rtB -> B_63_148_0 ; } rtb_B_63_0_0 = _rtP -> P_122 *
muDoubleScalarAtan2 ( _rtB -> B_63_150_0 , _rtB -> B_63_142_0 ) * _rtP ->
P_123 ; B_63_155_0 . re = muDoubleScalarHypot ( _rtB -> B_63_142_0 , _rtB ->
B_63_150_0 ) * muDoubleScalarCos ( rtb_B_63_0_0 ) ; B_63_155_0 . im =
muDoubleScalarHypot ( _rtB -> B_63_142_0 , _rtB -> B_63_150_0 ) *
muDoubleScalarSin ( rtb_B_63_0_0 ) ; _rtB -> B_63_156_0 = _rtX ->
integrator_CSTATE_e ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_n . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_125 ; B_63_157_0 =
PV_Battery_Grid2021a_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK_ff . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_ff . Last , _rtDW -> TransportDelay_IWORK_ff . Tail ,
_rtDW -> TransportDelay_IWORK_ff . Head , _rtP -> P_126 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_ff .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_ff . Head ] == ssGetT ( S )
) ) ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_159_0 = _rtP -> P_127 ; } rtb_B_63_9_0 = ( ssGetT ( S ) >= _rtB ->
B_63_159_0 ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_161_0 = _rtDW -> Memory_PreviousInput_d0 ; } if ( rtb_B_63_9_0 ) { _rtB
-> B_63_163_0 = ( _rtB -> B_63_156_0 - B_63_157_0 ) * _rtP -> P_9 ; } else {
_rtB -> B_63_163_0 = _rtB -> B_63_161_0 ; } _rtB -> B_63_164_0 = _rtX ->
integrator_CSTATE_p ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_m . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_130 ; B_63_165_0 =
PV_Battery_Grid2021a_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK_c2 . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_c2 . Last , _rtDW -> TransportDelay_IWORK_c2 . Tail ,
_rtDW -> TransportDelay_IWORK_c2 . Head , _rtP -> P_131 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_c2 .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_c2 . Head ] == ssGetT ( S )
) ) ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_167_0 = _rtP -> P_132 ; } rtb_B_63_9_0 = ( ssGetT ( S ) >= _rtB ->
B_63_167_0 ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_169_0 = _rtDW -> Memory_PreviousInput_m ; } if ( rtb_B_63_9_0 ) { _rtB
-> B_63_171_0 = ( _rtB -> B_63_164_0 - B_63_165_0 ) * _rtP -> P_8 ; } else {
_rtB -> B_63_171_0 = _rtB -> B_63_169_0 ; } rtb_B_63_0_0 = _rtP -> P_134 *
muDoubleScalarAtan2 ( _rtB -> B_63_171_0 , _rtB -> B_63_163_0 ) * _rtP ->
P_135 ; B_63_176_0 . re = muDoubleScalarHypot ( _rtB -> B_63_163_0 , _rtB ->
B_63_171_0 ) * muDoubleScalarCos ( rtb_B_63_0_0 ) ; B_63_176_0 . im =
muDoubleScalarHypot ( _rtB -> B_63_163_0 , _rtB -> B_63_171_0 ) *
muDoubleScalarSin ( rtb_B_63_0_0 ) ; PV_Battery_Grid2021a_NegSeqComputation (
S , _rtB -> B_63_50_0 , B_63_134_0 , B_63_155_0 , B_63_176_0 , & _rtB ->
PosSeqComputation , & _rtDW -> PosSeqComputation , & _rtP ->
PosSeqComputation ) ; PV_Battery_Grid2021a_NegSeqComputation ( S , _rtB ->
B_63_51_0 , B_63_134_0 , B_63_155_0 , B_63_176_0 , & _rtB ->
NegSeqComputation , & _rtDW -> NegSeqComputation , & _rtP ->
NegSeqComputation ) ; PV_Battery_Grid2021a_ZeroSeqComputation ( S , _rtB ->
B_63_52_0 , B_63_134_0 , B_63_155_0 , B_63_176_0 , & _rtB ->
ZeroSeqComputation , & _rtDW -> ZeroSeqComputation , & _rtP ->
ZeroSeqComputation ) ; rtb_B_63_41_0 = muDoubleScalarHypot ( _rtB ->
PosSeqComputation . B_25_2_0 . re , _rtB -> PosSeqComputation . B_25_2_0 . im
) ; rtb_B_63_12_0 = muDoubleScalarAtan2 ( _rtB -> PosSeqComputation .
B_25_2_0 . im , _rtB -> PosSeqComputation . B_25_2_0 . re ) ; _rtB ->
B_63_181_0 = _rtX -> integrator_CSTATE_c ; { real_T * * uBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_b . TUbufferPtrs [ 0 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_137 ; B_63_182_0 =
PV_Battery_Grid2021a_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK_g . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_g . Last , _rtDW -> TransportDelay_IWORK_g . Tail ,
_rtDW -> TransportDelay_IWORK_g . Head , _rtP -> P_138 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_g .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_g . Head ] == ssGetT ( S )
) ) ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_184_0 = _rtP -> P_139 ; } rtb_B_63_9_0 = ( ssGetT ( S ) >= _rtB ->
B_63_184_0 ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_186_0 = _rtDW -> Memory_PreviousInput_mf ; } if ( rtb_B_63_9_0 ) { _rtB
-> B_63_188_0 = ( _rtB -> B_63_181_0 - B_63_182_0 ) * _rtP -> P_11 ; } else {
_rtB -> B_63_188_0 = _rtB -> B_63_186_0 ; } _rtB -> B_63_189_0 = _rtX ->
integrator_CSTATE_fr ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_nb . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S )
; real_T tMinusDelay = simTime - _rtP -> P_142 ; B_63_190_0 =
PV_Battery_Grid2021a_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK_ck . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_ck . Last , _rtDW -> TransportDelay_IWORK_ck . Tail ,
_rtDW -> TransportDelay_IWORK_ck . Head , _rtP -> P_143 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_ck .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_ck . Head ] == ssGetT ( S )
) ) ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_192_0 = _rtP -> P_144 ; } rtb_B_63_9_0 = ( ssGetT ( S ) >= _rtB ->
B_63_192_0 ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_194_0 = _rtDW -> Memory_PreviousInput_c ; } if ( rtb_B_63_9_0 ) { _rtB
-> B_63_196_0 = ( _rtB -> B_63_189_0 - B_63_190_0 ) * _rtP -> P_10 ; } else {
_rtB -> B_63_196_0 = _rtB -> B_63_194_0 ; } rtb_B_63_0_0 = _rtP -> P_146 *
muDoubleScalarAtan2 ( _rtB -> B_63_196_0 , _rtB -> B_63_188_0 ) * _rtP ->
P_147 ; B_63_201_0 . re = muDoubleScalarHypot ( _rtB -> B_63_188_0 , _rtB ->
B_63_196_0 ) * muDoubleScalarCos ( rtb_B_63_0_0 ) ; B_63_201_0 . im =
muDoubleScalarHypot ( _rtB -> B_63_188_0 , _rtB -> B_63_196_0 ) *
muDoubleScalarSin ( rtb_B_63_0_0 ) ; _rtB -> B_63_202_0 = _rtX ->
integrator_CSTATE_pb ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_f . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_149 ; B_63_203_0 =
PV_Battery_Grid2021a_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK_l . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_l . Last , _rtDW -> TransportDelay_IWORK_l . Tail ,
_rtDW -> TransportDelay_IWORK_l . Head , _rtP -> P_150 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_l .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_l . Head ] == ssGetT ( S )
) ) ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_205_0 = _rtP -> P_151 ; } rtb_B_63_9_0 = ( ssGetT ( S ) >= _rtB ->
B_63_205_0 ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_207_0 = _rtDW -> Memory_PreviousInput_b ; } if ( rtb_B_63_9_0 ) { _rtB
-> B_63_209_0 = ( _rtB -> B_63_202_0 - B_63_203_0 ) * _rtP -> P_13 ; } else {
_rtB -> B_63_209_0 = _rtB -> B_63_207_0 ; } _rtB -> B_63_210_0 = _rtX ->
integrator_CSTATE_o ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_c . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_154 ; B_63_211_0 =
PV_Battery_Grid2021a_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK_p . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_p . Last , _rtDW -> TransportDelay_IWORK_p . Tail ,
_rtDW -> TransportDelay_IWORK_p . Head , _rtP -> P_155 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_p .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_p . Head ] == ssGetT ( S )
) ) ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_213_0 = _rtP -> P_156 ; } rtb_B_63_9_0 = ( ssGetT ( S ) >= _rtB ->
B_63_213_0 ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_215_0 = _rtDW -> Memory_PreviousInput_i ; } if ( rtb_B_63_9_0 ) { _rtB
-> B_63_217_0 = ( _rtB -> B_63_210_0 - B_63_211_0 ) * _rtP -> P_12 ; } else {
_rtB -> B_63_217_0 = _rtB -> B_63_215_0 ; } rtb_B_63_0_0 = _rtP -> P_158 *
muDoubleScalarAtan2 ( _rtB -> B_63_217_0 , _rtB -> B_63_209_0 ) * _rtP ->
P_159 ; B_63_222_0 . re = muDoubleScalarHypot ( _rtB -> B_63_209_0 , _rtB ->
B_63_217_0 ) * muDoubleScalarCos ( rtb_B_63_0_0 ) ; B_63_222_0 . im =
muDoubleScalarHypot ( _rtB -> B_63_209_0 , _rtB -> B_63_217_0 ) *
muDoubleScalarSin ( rtb_B_63_0_0 ) ; _rtB -> B_63_223_0 = _rtX ->
integrator_CSTATE_oo ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_h . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_161 ; B_63_224_0 =
PV_Battery_Grid2021a_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK_o . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_o . Last , _rtDW -> TransportDelay_IWORK_o . Tail ,
_rtDW -> TransportDelay_IWORK_o . Head , _rtP -> P_162 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_o .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_o . Head ] == ssGetT ( S )
) ) ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_226_0 = _rtP -> P_163 ; } rtb_B_63_9_0 = ( ssGetT ( S ) >= _rtB ->
B_63_226_0 ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_228_0 = _rtDW -> Memory_PreviousInput_j ; } if ( rtb_B_63_9_0 ) { _rtB
-> B_63_230_0 = ( _rtB -> B_63_223_0 - B_63_224_0 ) * _rtP -> P_15 ; } else {
_rtB -> B_63_230_0 = _rtB -> B_63_228_0 ; } _rtB -> B_63_231_0 = _rtX ->
integrator_CSTATE_ec ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_d . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_166 ; B_63_232_0 =
PV_Battery_Grid2021a_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK_b . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_b . Last , _rtDW -> TransportDelay_IWORK_b . Tail ,
_rtDW -> TransportDelay_IWORK_b . Head , _rtP -> P_167 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_b .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_b . Head ] == ssGetT ( S )
) ) ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_234_0 = _rtP -> P_168 ; } rtb_B_63_9_0 = ( ssGetT ( S ) >= _rtB ->
B_63_234_0 ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_236_0 = _rtDW -> Memory_PreviousInput_i3 ; } if ( rtb_B_63_9_0 ) { _rtB
-> B_63_238_0 = ( _rtB -> B_63_231_0 - B_63_232_0 ) * _rtP -> P_14 ; } else {
_rtB -> B_63_238_0 = _rtB -> B_63_236_0 ; } rtb_B_63_0_0 = _rtP -> P_170 *
muDoubleScalarAtan2 ( _rtB -> B_63_238_0 , _rtB -> B_63_230_0 ) * _rtP ->
P_171 ; B_63_243_0 . re = muDoubleScalarHypot ( _rtB -> B_63_230_0 , _rtB ->
B_63_238_0 ) * muDoubleScalarCos ( rtb_B_63_0_0 ) ; B_63_243_0 . im =
muDoubleScalarHypot ( _rtB -> B_63_230_0 , _rtB -> B_63_238_0 ) *
muDoubleScalarSin ( rtb_B_63_0_0 ) ; PV_Battery_Grid2021a_NegSeqComputation (
S , _rtB -> B_63_53_0 , B_63_201_0 , B_63_222_0 , B_63_243_0 , & _rtB ->
PosSeqComputation_o , & _rtDW -> PosSeqComputation_o , & _rtP ->
PosSeqComputation_o ) ; PV_Battery_Grid2021a_NegSeqComputation ( S , _rtB ->
B_63_54_0 , B_63_201_0 , B_63_222_0 , B_63_243_0 , & _rtB ->
NegSeqComputation_h , & _rtDW -> NegSeqComputation_h , & _rtP ->
NegSeqComputation_h ) ; PV_Battery_Grid2021a_ZeroSeqComputation ( S , _rtB ->
B_63_55_0 , B_63_201_0 , B_63_222_0 , B_63_243_0 , & _rtB ->
ZeroSeqComputation_f , & _rtDW -> ZeroSeqComputation_f , & _rtP ->
ZeroSeqComputation_f ) ; rtb_B_63_0_0 = rtb_B_63_41_0 * muDoubleScalarHypot (
_rtB -> PosSeqComputation_o . B_25_2_0 . re , _rtB -> PosSeqComputation_o .
B_25_2_0 . im ) * _rtP -> P_172 ; muDoubleScalarSinCos ( _rtP -> P_175 * (
_rtP -> P_173 * rtb_B_63_12_0 - _rtP -> P_174 * muDoubleScalarAtan2 ( _rtB ->
PosSeqComputation_o . B_25_2_0 . im , _rtB -> PosSeqComputation_o . B_25_2_0
. re ) ) , & rtb_B_63_27_0 , & rtb_B_63_41_0 ) ; _rtB -> B_63_255_0 =
rtb_B_63_0_0 * rtb_B_63_41_0 ; ssCallAccelRunBlock ( S , 63 , 256 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_63_257_0 = rtb_B_63_0_0 * rtb_B_63_27_0 ;
ssCallAccelRunBlock ( S , 63 , 258 , SS_CALL_MDL_OUTPUTS ) ; i =
ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_259_0 = _rtP ->
P_176 * _rtB -> B_63_46_0 [ 25 ] ; _rtB -> B_63_260_0 = _rtP -> P_177 * _rtB
-> B_63_46_0 [ 26 ] ; _rtB -> B_63_261_0 = _rtP -> P_178 * _rtB -> B_63_46_0
[ 27 ] ; _rtB -> B_63_262_0 [ 0 ] = _rtB -> B_63_259_0 ; _rtB -> B_63_262_0 [
1 ] = _rtB -> B_63_260_0 ; _rtB -> B_63_262_0 [ 2 ] = _rtB -> B_63_261_0 ;
_rtB -> B_63_266_0 [ 0 ] = _rtP -> P_179 * _rtB -> B_63_46_0 [ 13 ] * _rtP ->
P_182 ; _rtB -> B_63_266_0 [ 1 ] = _rtP -> P_180 * _rtB -> B_63_46_0 [ 14 ] *
_rtP -> P_182 ; _rtB -> B_63_266_0 [ 2 ] = _rtP -> P_181 * _rtB -> B_63_46_0
[ 15 ] * _rtP -> P_182 ; ssCallAccelRunBlock ( S , 63 , 267 ,
SS_CALL_MDL_OUTPUTS ) ; { if ( _rtDW ->
TAQSigLogging_InsertedFor_From1_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_From1_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 3 ) , ( char * ) & _rtB -> B_63_104_0 + 0 ) ; } } { if (
_rtDW -> TAQSigLogging_InsertedFor_From26_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_From26_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 3 ) , ( char * ) & _rtB -> B_63_108_0 + 0 ) ; } } } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { { if ( _rtDW ->
TAQSigLogging_InsertedFor_From7_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_From7_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_63_255_0 + 0 ) ; } } } i =
ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { { if ( _rtDW ->
TAQSigLogging_InsertedFor_Iin_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Iin_at_outport_0_PWORK . AQHandles , ssGetTaskTime
( S , 3 ) , ( char * ) & _rtB -> B_63_106_0 + 0 ) ; } } { if ( _rtDW ->
TAQSigLogging_InsertedFor_Product1_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Product1_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 3 ) , ( char * ) & _rtB -> B_63_108_0 + 0 ) ; } } { if (
_rtDW -> TAQSigLogging_InsertedFor_Product2_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Product2_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 3 ) , ( char * ) & _rtB -> B_63_110_0 + 0 ) ; } } { if (
_rtDW -> TAQSigLogging_InsertedFor_Product3_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Product3_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 3 ) , ( char * ) & _rtB -> B_63_113_0 + 0 ) ; } } { if (
_rtDW -> TAQSigLogging_InsertedFor_Vin_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Vin_at_outport_0_PWORK . AQHandles , ssGetTaskTime
( S , 3 ) , ( char * ) & _rtB -> B_63_107_0 + 0 ) ; } } for ( i = 0 ; i < 6 ;
i ++ ) { if ( _rtB -> B_63_46_1 [ i + 7 ] >= _rtP -> P_185 ) { rtb_B_63_17_0
= _rtB -> B_63_46_0 [ i + 7 ] * _rtP -> P_184 ; } else { rtb_B_63_17_0 = _rtP
-> P_183 ; } if ( rtb_B_63_17_0 > _rtP -> P_186 ) { _rtB -> B_63_279_0 [ i ]
= _rtP -> P_186 ; } else if ( rtb_B_63_17_0 < _rtP -> P_187 ) { _rtB ->
B_63_279_0 [ i ] = _rtP -> P_187 ; } else { _rtB -> B_63_279_0 [ i ] =
rtb_B_63_17_0 ; } } ssCallAccelRunBlock ( S , 63 , 287 , SS_CALL_MDL_OUTPUTS
) ; i = ssIsSampleHit ( S , 7 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_315_0 =
_rtB -> B_63_107_0 ; } } i = ssIsSampleHit ( S , 7 , 0 ) ; if ( i != 0 ) {
rtb_B_63_317_0 = _rtB -> B_63_315_0 - _rtDW -> Memory_PreviousInput_dr ;
rtb_B_63_318_0 = muDoubleScalarAbs ( rtb_B_63_317_0 ) ; } i = ssIsSampleHit (
S , 3 , 0 ) ; if ( i != 0 ) { i = ssIsSampleHit ( S , 7 , 0 ) ; if ( i != 0 )
{ _rtB -> B_63_319_0 = _rtB -> B_63_106_0 ; } } i = ssIsSampleHit ( S , 7 , 0
) ; if ( i != 0 ) { rtb_B_63_0_0 = _rtB -> B_63_319_0 - _rtDW ->
Memory1_PreviousInput ; if ( rtb_B_63_318_0 > _rtP -> P_191 ) {
rtb_B_63_317_0 = _rtB -> B_63_319_0 / _rtB -> B_63_315_0 + rtb_B_63_0_0 /
rtb_B_63_317_0 ; if ( muDoubleScalarAbs ( rtb_B_63_317_0 ) > _rtP -> P_2 ) {
rtb_B_63_317_0 = muDoubleScalarSign ( rtb_B_63_317_0 ) * _rtB -> B_63_42_0_p
; } else { rtb_B_63_317_0 = _rtB -> B_63_45_0_g ; } } else if (
muDoubleScalarAbs ( rtb_B_63_0_0 ) > _rtP -> P_3 ) { rtb_B_63_317_0 =
muDoubleScalarSign ( rtb_B_63_0_0 ) * _rtB -> B_63_43_0_c ; } else {
rtb_B_63_317_0 = _rtB -> B_63_44_0_f ; } _rtB -> B_63_327_0 = _rtDW ->
Memory2_PreviousInput_m - rtb_B_63_317_0 * _rtB -> B_63_46_0_g ; if ( _rtDW
-> Memory2_PreviousInput_m > _rtP -> P_193 ) { _rtB -> B_63_328_0 = _rtP ->
P_193 ; } else if ( _rtDW -> Memory2_PreviousInput_m < _rtP -> P_194 ) { _rtB
-> B_63_328_0 = _rtP -> P_194 ; } else { _rtB -> B_63_328_0 = _rtDW ->
Memory2_PreviousInput_m ; } ssCallAccelRunBlock ( S , 63 , 329 ,
SS_CALL_MDL_OUTPUTS ) ; i = ssIsSampleHit ( S , 8 , 0 ) ; if ( i != 0 ) {
_rtB -> B_63_330_0 = _rtB -> B_63_328_0 ; } } i = ssIsSampleHit ( S , 8 , 0 )
; if ( i != 0 ) { if ( _rtB -> B_63_330_0 > _rtP -> P_195 ) { _rtB ->
B_63_331_0 = _rtP -> P_195 ; } else if ( _rtB -> B_63_330_0 < _rtP -> P_196 )
{ _rtB -> B_63_331_0 = _rtP -> P_196 ; } else { _rtB -> B_63_331_0 = _rtB ->
B_63_330_0 ; } } _rtB -> B_63_337_0 = _rtB -> B_63_331_0 - look1_binlxpw (
muDoubleScalarRem ( ssGetT ( S ) - _rtB -> B_63_333_0 , _rtB -> B_63_49_0 ) ,
_rtP -> P_198 , _rtP -> P_197 , 1U ) ; if ( ssIsModeUpdateTimeStep ( S ) ) {
_rtDW -> Switch_Mode = ( _rtB -> B_63_337_0 > _rtP -> P_199 ) ; } if ( _rtDW
-> Switch_Mode ) { _rtB -> B_63_338_0 = _rtB -> B_63_47_0 ; } else { _rtB ->
B_63_338_0 = _rtB -> B_63_48_0 ; } _rtB -> B_63_340_0 = ( muDoubleScalarSin (
_rtP -> P_202 * ssGetTaskTime ( S , 0 ) + _rtP -> P_203 ) * _rtP -> P_200 +
_rtP -> P_201 ) * _rtB -> B_63_266_0 [ 0 ] ; _rtB -> B_63_342_0 = (
muDoubleScalarSin ( _rtP -> P_206 * ssGetTaskTime ( S , 0 ) + _rtP -> P_207 )
* _rtP -> P_204 + _rtP -> P_205 ) * _rtB -> B_63_266_0 [ 0 ] ; _rtB ->
B_63_344_0 = ( muDoubleScalarSin ( _rtP -> P_210 * ssGetTaskTime ( S , 0 ) +
_rtP -> P_211 ) * _rtP -> P_208 + _rtP -> P_209 ) * _rtB -> B_63_266_0 [ 1 ]
; _rtB -> B_63_346_0 = ( muDoubleScalarSin ( _rtP -> P_214 * ssGetTaskTime (
S , 0 ) + _rtP -> P_215 ) * _rtP -> P_212 + _rtP -> P_213 ) * _rtB ->
B_63_266_0 [ 1 ] ; _rtB -> B_63_348_0 = ( muDoubleScalarSin ( _rtP -> P_218 *
ssGetTaskTime ( S , 0 ) + _rtP -> P_219 ) * _rtP -> P_216 + _rtP -> P_217 ) *
_rtB -> B_63_266_0 [ 2 ] ; _rtB -> B_63_350_0 = ( muDoubleScalarSin ( _rtP ->
P_222 * ssGetTaskTime ( S , 0 ) + _rtP -> P_223 ) * _rtP -> P_220 + _rtP ->
P_221 ) * _rtB -> B_63_266_0 [ 2 ] ; _rtB -> B_63_352_0 = ( muDoubleScalarSin
( _rtP -> P_226 * ssGetTaskTime ( S , 0 ) + _rtP -> P_227 ) * _rtP -> P_224 +
_rtP -> P_225 ) * _rtB -> B_63_259_0 ; _rtB -> B_63_354_0 = (
muDoubleScalarSin ( _rtP -> P_230 * ssGetTaskTime ( S , 0 ) + _rtP -> P_231 )
* _rtP -> P_228 + _rtP -> P_229 ) * _rtB -> B_63_259_0 ; _rtB -> B_63_356_0 =
( muDoubleScalarSin ( _rtP -> P_234 * ssGetTaskTime ( S , 0 ) + _rtP -> P_235
) * _rtP -> P_232 + _rtP -> P_233 ) * _rtB -> B_63_260_0 ; _rtB -> B_63_358_0
= ( muDoubleScalarSin ( _rtP -> P_238 * ssGetTaskTime ( S , 0 ) + _rtP ->
P_239 ) * _rtP -> P_236 + _rtP -> P_237 ) * _rtB -> B_63_260_0 ; _rtB ->
B_63_360_0 = ( muDoubleScalarSin ( _rtP -> P_242 * ssGetTaskTime ( S , 0 ) +
_rtP -> P_243 ) * _rtP -> P_240 + _rtP -> P_241 ) * _rtB -> B_63_261_0 ; _rtB
-> B_63_362_0 = ( muDoubleScalarSin ( _rtP -> P_246 * ssGetTaskTime ( S , 0 )
+ _rtP -> P_247 ) * _rtP -> P_244 + _rtP -> P_245 ) * _rtB -> B_63_261_0 ; i
= ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { rtb_B_63_363_0 = _rtB ->
B_63_56_0 - _rtB -> B_63_104_0 ; _rtB -> B_63_366_0 = _rtP -> P_248 *
rtb_B_63_363_0 + _rtDW -> Integrator_DSTATE ; ssCallAccelRunBlock ( S , 63 ,
367 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 63 , 368 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_63_369_0 = _rtDW -> UnitDelay_DSTATE_o ; }
i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { rtb_B_63_370_0 = ( _rtB ->
B_63_369_0 != PV_Battery_Grid2021a_rtC ( S ) -> B_63_3_0 ) ; } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_372_0 = _rtP ->
P_252 ; } rtb_B_63_317_0 = ssGetT ( S ) + _rtB -> B_63_372_0 ; i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_374_0 = _rtP ->
P_253 ; } _rtB -> B_63_376_0 = _rtP -> P_254 * muDoubleScalarRem (
rtb_B_63_317_0 , _rtB -> B_63_374_0 ) ; _rtB -> B_63_378_0 = ( ( 2.0 * _rtB
-> B_63_376_0 - 1.0 ) + 1.0 ) * 0.5 ; i = ssIsSampleHit ( S , 1 , 0 ) ; if (
i != 0 ) { if ( ssIsModeUpdateTimeStep ( S ) ) { _rtDW ->
RelationalOperator_Mode = ( _rtB -> B_63_369_0 >= _rtB -> B_63_378_0 ) ; }
_rtB -> B_63_381_0 = ( rtb_B_63_370_0 && _rtDW -> RelationalOperator_Mode ) ;
ssCallAccelRunBlock ( S , 63 , 382 , SS_CALL_MDL_OUTPUTS ) ; } i =
ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { ssCallAccelRunBlock ( S , 63 ,
383 , SS_CALL_MDL_OUTPUTS ) ; rtb_B_63_384_0 = _rtB -> B_63_366_0 -
rtb_B_63_112_0 ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_385_0 = _rtB -> B_63_57_0 - _rtB -> B_63_381_0 ; } i = ssIsSampleHit ( S
, 3 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_386_0 = _rtP -> P_255 *
rtb_B_63_363_0 ; _rtB -> B_63_387_0 = _rtP -> P_256 * rtb_B_63_384_0 ;
rtb_B_63_0_0 = _rtP -> P_259 * rtb_B_63_384_0 + _rtDW -> Integrator_DSTATE_p
; if ( rtb_B_63_0_0 > _rtP -> P_260 ) { _rtB -> B_63_391_0 = _rtP -> P_260 ;
} else if ( rtb_B_63_0_0 < _rtP -> P_261 ) { _rtB -> B_63_391_0 = _rtP ->
P_261 ; } else { _rtB -> B_63_391_0 = rtb_B_63_0_0 ; } } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { zcEvent = rt_ZCFcn ( ANY_ZERO_CROSSING , &
_rtZCE -> HitCrossing_Input_ZCE , _rtB -> B_63_376_0 - _rtP -> P_262 ) ; if (
_rtDW -> HitCrossing_MODE == 0 ) { if ( zcEvent != NO_ZCEVENT ) { _rtB ->
B_63_392_0 = ! _rtB -> B_63_392_0 ; _rtDW -> HitCrossing_MODE = 1 ; } else if
( _rtB -> B_63_392_0 ) { _rtB -> B_63_392_0 = ( ( ! ( _rtB -> B_63_376_0 !=
_rtP -> P_262 ) ) && _rtB -> B_63_392_0 ) ; } else { _rtB -> B_63_392_0 = ( (
_rtB -> B_63_376_0 == _rtP -> P_262 ) || _rtB -> B_63_392_0 ) ; } } else {
_rtB -> B_63_392_0 = ( ( ! ( _rtB -> B_63_376_0 != _rtP -> P_262 ) ) && _rtB
-> B_63_392_0 ) ; _rtDW -> HitCrossing_MODE = 0 ; } } i = ssIsSampleHit ( S ,
4 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_393_0 = _rtDW -> UnitDelay_DSTATE_ev ;
} i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_394_0 =
_rtDW -> Memory_PreviousInput_e ; } ssCallAccelRunBlock ( S , 63 , 395 ,
SS_CALL_MDL_OUTPUTS ) ; i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { for
( i = 0 ; i < 3 ; i ++ ) { _rtB -> B_63_397_0 [ i ] = ( ( _rtP -> P_265 [ i +
3 ] * _rtB -> B_63_262_0 [ 1 ] + _rtP -> P_265 [ i ] * _rtB -> B_63_262_0 [ 0
] ) + _rtP -> P_265 [ i + 6 ] * _rtB -> B_63_262_0 [ 2 ] ) * _rtP -> P_266 ;
} } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Step3_MODE = (
ssGetTaskTime ( S , 1 ) >= _rtP -> P_267 ) ; if ( _rtDW -> Step3_MODE == 1 )
{ _rtB -> B_63_398_0 = _rtP -> P_269 ; } else { _rtB -> B_63_398_0 = _rtP ->
P_268 ; } } _rtB -> B_63_399_0 = _rtX -> Integrator_CSTATE ; _rtB ->
B_63_400_0 = _rtB -> B_63_398_0 * _rtB -> B_63_399_0 ;
PV_Battery_Grid2021a_Subsystem1 ( S , _rtB -> B_63_111_0 , & _rtB ->
B_63_397_0 [ 0 ] , _rtB -> B_63_400_0 , & _rtB -> Subsystem1_j , & _rtDW ->
Subsystem1_j ) ; PV_Battery_Grid2021a_Subsystempi2delay ( S , _rtB ->
B_63_113_0_b , & _rtB -> B_63_397_0 [ 0 ] , _rtB -> B_63_400_0 , & _rtB ->
Subsystempi2delay_a , & _rtDW -> Subsystempi2delay_a ) ; if ( _rtB ->
B_63_111_0 != 0 ) { _rtB -> B_63_403_0 [ 0 ] = _rtB -> Subsystem1_j .
B_14_0_0 ; _rtB -> B_63_403_0 [ 1 ] = _rtB -> Subsystem1_j . B_14_1_0 ; }
else { _rtB -> B_63_403_0 [ 0 ] = _rtB -> Subsystempi2delay_a . B_13_0_0 ;
_rtB -> B_63_403_0 [ 1 ] = _rtB -> Subsystempi2delay_a . B_13_1_0 ; }
ssCallAccelRunBlock ( S , 63 , 404 , SS_CALL_MDL_OUTPUTS ) ; i =
ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { for ( i = 0 ; i < 3 ; i ++ ) {
_rtB -> B_63_406_0 [ i ] = ( ( _rtP -> P_271 [ i + 3 ] * _rtB -> B_63_266_0 [
1 ] + _rtP -> P_271 [ i ] * _rtB -> B_63_266_0 [ 0 ] ) + _rtP -> P_271 [ i +
6 ] * _rtB -> B_63_266_0 [ 2 ] ) * _rtP -> P_272 ; } }
PV_Battery_Grid2021a_Subsystem1 ( S , _rtB -> B_63_60_0 , & _rtB ->
B_63_406_0 [ 0 ] , _rtB -> B_63_399_0 , & _rtB -> Subsystem1_i , & _rtDW ->
Subsystem1_i ) ; PV_Battery_Grid2021a_Subsystempi2delay ( S , _rtB ->
B_63_62_0 , & _rtB -> B_63_406_0 [ 0 ] , _rtB -> B_63_399_0 , & _rtB ->
Subsystempi2delay_n , & _rtDW -> Subsystempi2delay_n ) ; if ( _rtB ->
B_63_60_0 != 0 ) { _rtB -> B_63_409_0 [ 0 ] = _rtB -> Subsystem1_i . B_14_0_0
; _rtB -> B_63_409_0 [ 1 ] = _rtB -> Subsystem1_i . B_14_1_0 ; } else { _rtB
-> B_63_409_0 [ 0 ] = _rtB -> Subsystempi2delay_n . B_13_0_0 ; _rtB ->
B_63_409_0 [ 1 ] = _rtB -> Subsystempi2delay_n . B_13_1_0 ; }
ssCallAccelRunBlock ( S , 63 , 410 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 63 , 411 , SS_CALL_MDL_OUTPUTS ) ; i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_412_0 [ 0 ] =
_rtDW -> UnitDelay_DSTATE_b [ 0 ] ; _rtB -> B_63_412_0 [ 1 ] = _rtDW ->
UnitDelay_DSTATE_b [ 1 ] ; _rtB -> B_63_412_0 [ 2 ] = _rtDW ->
UnitDelay_DSTATE_b [ 2 ] ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 )
{ _rtB -> B_63_414_0 = _rtP -> P_274 ; } rtb_B_63_112_0 = ssGetT ( S ) + _rtB
-> B_63_414_0 ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_416_0 = _rtP -> P_275 ; } rtb_B_63_112_0 = look1_pbinlxpw ( _rtP ->
P_276 * muDoubleScalarRem ( rtb_B_63_112_0 , _rtB -> B_63_416_0 ) , _rtP ->
P_278 , _rtP -> P_277 , & _rtDW -> m_bpIndex , 2U ) ; i = ssIsSampleHit ( S ,
1 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_420_0 = _rtP -> P_279 ; } _rtB ->
B_63_423_0 = ( ( rtb_B_63_112_0 - _rtB -> B_63_420_0 ) * _rtB -> B_63_93_0 +
_rtB -> B_63_91_0 [ 0 ] ) + _rtB -> B_63_93_0 ; i = ssIsSampleHit ( S , 1 , 0
) ; if ( i != 0 ) { if ( ssIsModeUpdateTimeStep ( S ) ) { _rtDW ->
RelationalOperator2_Mode [ 0 ] = ( _rtB -> B_63_412_0 [ 0 ] >= _rtB ->
B_63_423_0 ) ; _rtDW -> RelationalOperator2_Mode [ 1 ] = ( _rtB -> B_63_412_0
[ 1 ] >= _rtB -> B_63_423_0 ) ; _rtDW -> RelationalOperator2_Mode [ 2 ] = (
_rtB -> B_63_412_0 [ 2 ] >= _rtB -> B_63_423_0 ) ; } _rtB -> B_63_424_0 [ 0 ]
= _rtDW -> RelationalOperator2_Mode [ 0 ] ; _rtB -> B_63_424_0 [ 1 ] = _rtDW
-> RelationalOperator2_Mode [ 1 ] ; _rtB -> B_63_424_0 [ 2 ] = _rtDW ->
RelationalOperator2_Mode [ 2 ] ; } rtb_B_63_426_0 [ 0 ] = _rtB -> B_63_424_0
[ 0 ] ; rtb_B_63_426_0 [ 1 ] = ! _rtB -> B_63_424_0 [ 0 ] ; rtb_B_63_426_0 [
2 ] = _rtB -> B_63_424_0 [ 1 ] ; rtb_B_63_426_0 [ 3 ] = ! _rtB -> B_63_424_0
[ 1 ] ; rtb_B_63_426_0 [ 4 ] = _rtB -> B_63_424_0 [ 2 ] ; rtb_B_63_426_0 [ 5
] = ! _rtB -> B_63_424_0 [ 2 ] ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i !=
0 ) { _rtDW -> Step1_MODE = ( ssGetTaskTime ( S , 1 ) >= _rtP -> P_280 ) ; if
( _rtDW -> Step1_MODE == 1 ) { _rtB -> B_63_427_0 = _rtP -> P_282 ; } else {
_rtB -> B_63_427_0 = _rtP -> P_281 ; } } for ( i = 0 ; i < 6 ; i ++ ) {
rtb_B_63_426_0 [ i ] *= _rtB -> B_63_427_0 ; } i = ssIsSampleHit ( S , 1 , 0
) ; if ( i != 0 ) { _rtDW -> Step2_MODE = ( ssGetTaskTime ( S , 1 ) >= _rtP
-> P_283 ) ; if ( _rtDW -> Step2_MODE == 1 ) { _rtB -> B_63_429_0 = _rtP ->
P_285 ; } else { _rtB -> B_63_429_0 = _rtP -> P_284 ; } } i = ssIsSampleHit (
S , 3 , 0 ) ; if ( i != 0 ) { for ( i = 0 ; i < 3 ; i ++ ) { B_63_431_0 [ i ]
= ( ( _rtP -> P_286 [ i + 3 ] * _rtB -> B_63_266_0 [ 1 ] + _rtP -> P_286 [ i
] * _rtB -> B_63_266_0 [ 0 ] ) + _rtP -> P_286 [ i + 6 ] * _rtB -> B_63_266_0
[ 2 ] ) * _rtP -> P_287 ; } B_63_435_0 = muDoubleScalarMod ( _rtB ->
B_63_69_0_p * ssGetTaskTime ( S , 3 ) * _rtB -> B_63_70_0 + _rtB ->
B_63_68_0_n , _rtB -> B_63_66_0_m ) ; PV_Battery_Grid2021a_Subsystem1_h ( S ,
_rtB -> B_63_116_0 , & B_63_431_0 [ 0 ] , B_63_435_0 , & _rtB -> Subsystem1_h
, & _rtDW -> Subsystem1_h ) ; PV_Battery_Grid2021a_Subsystempi2delay_d ( S ,
_rtB -> B_63_118_0 , & B_63_431_0 [ 0 ] , B_63_435_0 , & _rtB ->
Subsystempi2delay_d , & _rtDW -> Subsystempi2delay_d ) ; if ( _rtB ->
B_63_116_0 != 0 ) { _rtB -> B_63_438_0 [ 0 ] = _rtB -> Subsystem1_h .
B_54_0_0 ; _rtB -> B_63_438_0 [ 1 ] = _rtB -> Subsystem1_h . B_54_1_0 ; }
else { _rtB -> B_63_438_0 [ 0 ] = _rtB -> Subsystempi2delay_d . B_53_0_0 ;
_rtB -> B_63_438_0 [ 1 ] = _rtB -> Subsystempi2delay_d . B_53_1_0 ; }
rtb_B_63_439_0 = _rtB -> B_63_64_0 - _rtB -> B_63_438_0 [ 0 ] ;
rtb_B_63_442_0 = _rtP -> P_288 * rtb_B_63_439_0 + _rtDW ->
Integrator_DSTATE_l ; rtb_B_63_443_0 = _rtB -> B_63_100_0_l * _rtB ->
B_63_438_0 [ 1 ] ; rtb_B_63_112_0 = _rtP -> P_291 * _rtB -> B_63_46_0 [ 31 ]
* _rtP -> P_294 ; rtb_B_63_0_0 = _rtP -> P_292 * _rtB -> B_63_46_0 [ 32 ] *
_rtP -> P_294 ; B_63_453_0_idx_1 = _rtP -> P_293 * _rtB -> B_63_46_0 [ 33 ] *
_rtP -> P_294 ; for ( i = 0 ; i < 3 ; i ++ ) { B_63_450_0 [ i ] = ( ( _rtP ->
P_295 [ i + 3 ] * rtb_B_63_0_0 + _rtP -> P_295 [ i ] * rtb_B_63_112_0 ) +
_rtP -> P_295 [ i + 6 ] * B_63_453_0_idx_1 ) * _rtP -> P_296 ; }
PV_Battery_Grid2021a_Subsystem1_h ( S , _rtB -> B_63_121_0_n , & B_63_450_0 [
0 ] , B_63_435_0 , & _rtB -> Subsystem1_m , & _rtDW -> Subsystem1_m ) ;
PV_Battery_Grid2021a_Subsystempi2delay_d ( S , _rtB -> B_63_123_0 , &
B_63_450_0 [ 0 ] , B_63_435_0 , & _rtB -> Subsystempi2delay_f , & _rtDW ->
Subsystempi2delay_f ) ; if ( _rtB -> B_63_121_0_n != 0 ) { rtb_B_63_0_0 =
_rtB -> Subsystem1_m . B_54_0_0 ; B_63_453_0_idx_1 = _rtB -> Subsystem1_m .
B_54_1_0 ; } else { rtb_B_63_0_0 = _rtB -> Subsystempi2delay_f . B_53_0_0 ;
B_63_453_0_idx_1 = _rtB -> Subsystempi2delay_f . B_53_1_0 ; } rtb_B_63_442_0
= ( rtb_B_63_442_0 - rtb_B_63_443_0 ) - rtb_B_63_0_0 ; rtb_B_63_112_0 = ( (
_rtP -> P_297 * rtb_B_63_442_0 + _rtDW -> Integrator_DSTATE_d ) + _rtB ->
B_63_438_0 [ 0 ] ) - _rtB -> B_63_108_0_j * B_63_453_0_idx_1 ; rtb_B_63_443_0
= _rtB -> B_63_65_0 - _rtB -> B_63_438_0 [ 1 ] ; B_63_453_0_idx_1 = ( ( _rtP
-> P_300 * rtb_B_63_443_0 + _rtDW -> Integrator_DSTATE_pi ) + _rtB ->
B_63_438_0 [ 0 ] * _rtB -> B_63_100_0_l ) - B_63_453_0_idx_1 ; rtb_B_63_363_0
= ( ( _rtP -> P_303 * B_63_453_0_idx_1 + _rtDW -> Integrator_DSTATE_pe ) +
rtb_B_63_0_0 * _rtB -> B_63_108_0_j ) + _rtB -> B_63_438_0 [ 1 ] ; if ( _rtB
-> B_63_103_0 > 0 ) { _rtB -> B_50_0_0 = rtb_B_63_112_0 * muDoubleScalarCos (
B_63_435_0 ) - rtb_B_63_363_0 * muDoubleScalarSin ( B_63_435_0 ) ; _rtB ->
B_50_1_0 = rtb_B_63_112_0 * muDoubleScalarSin ( B_63_435_0 ) + rtb_B_63_363_0
* muDoubleScalarCos ( B_63_435_0 ) ; if ( ssIsModeUpdateTimeStep ( S ) ) {
srUpdateBC ( _rtDW -> Subsystem1_SubsysRanBC ) ; } } if ( _rtB -> B_63_105_0
> 0 ) { _rtB -> B_49_0_0 = rtb_B_63_112_0 * muDoubleScalarSin ( B_63_435_0 )
+ rtb_B_63_363_0 * muDoubleScalarCos ( B_63_435_0 ) ; _rtB -> B_49_1_0 = -
rtb_B_63_112_0 * muDoubleScalarCos ( B_63_435_0 ) + rtb_B_63_363_0 *
muDoubleScalarSin ( B_63_435_0 ) ; if ( ssIsModeUpdateTimeStep ( S ) ) {
srUpdateBC ( _rtDW -> Subsystempi2delay_SubsysRanBC ) ; } } if ( _rtB ->
B_63_103_0 != 0 ) { rtb_B_63_112_0 = _rtB -> B_50_0_0 ; rtb_B_63_0_0 = _rtB
-> B_50_1_0 ; } else { rtb_B_63_112_0 = _rtB -> B_49_0_0 ; rtb_B_63_0_0 =
_rtB -> B_49_1_0 ; } for ( i = 0 ; i < 3 ; i ++ ) { _rtB -> B_63_476_0 [ i ]
= 0.0 ; _rtB -> B_63_476_0 [ i ] += _rtP -> P_306 [ i ] * rtb_B_63_112_0 ;
_rtB -> B_63_476_0 [ i ] += _rtP -> P_306 [ i + 3 ] * rtb_B_63_0_0 ; _rtB ->
B_63_476_0 [ i ] += _rtP -> P_306 [ i + 6 ] * _rtB -> B_63_94_0 ; } } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_478_0 = _rtP ->
P_307 ; } rtb_B_63_112_0 = ssGetT ( S ) + _rtB -> B_63_478_0 ; i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_480_0 = _rtP ->
P_308 ; } rtb_B_63_112_0 = look1_pbinlxpw ( _rtP -> P_309 * muDoubleScalarRem
( rtb_B_63_112_0 , _rtB -> B_63_480_0 ) , _rtP -> P_311 , _rtP -> P_310 , &
_rtDW -> m_bpIndex_i , 2U ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 )
{ _rtB -> B_63_484_0 = _rtP -> P_312 ; } _rtB -> B_63_487_0 = ( (
rtb_B_63_112_0 - _rtB -> B_63_484_0 ) * _rtB -> B_63_97_0 + _rtB -> B_63_95_0
[ 0 ] ) + _rtB -> B_63_97_0 ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 )
{ if ( ssIsModeUpdateTimeStep ( S ) ) { _rtDW -> RelationalOperator2_Mode_k [
0 ] = ( _rtB -> B_63_476_0 [ 0 ] >= _rtB -> B_63_487_0 ) ; _rtDW ->
RelationalOperator2_Mode_k [ 1 ] = ( _rtB -> B_63_476_0 [ 1 ] >= _rtB ->
B_63_487_0 ) ; _rtDW -> RelationalOperator2_Mode_k [ 2 ] = ( _rtB ->
B_63_476_0 [ 2 ] >= _rtB -> B_63_487_0 ) ; } _rtB -> B_63_488_0 [ 0 ] = _rtDW
-> RelationalOperator2_Mode_k [ 0 ] ; _rtB -> B_63_488_0 [ 1 ] = _rtDW ->
RelationalOperator2_Mode_k [ 1 ] ; _rtB -> B_63_488_0 [ 2 ] = _rtDW ->
RelationalOperator2_Mode_k [ 2 ] ; } _rtB -> B_63_492_0 [ 0 ] = _rtB ->
B_63_429_0 * ( real_T ) _rtB -> B_63_488_0 [ 0 ] + rtb_B_63_426_0 [ 0 ] ;
_rtB -> B_63_492_0 [ 1 ] = _rtB -> B_63_429_0 * ( real_T ) ! _rtB ->
B_63_488_0 [ 0 ] + rtb_B_63_426_0 [ 1 ] ; _rtB -> B_63_492_0 [ 2 ] = _rtB ->
B_63_429_0 * ( real_T ) _rtB -> B_63_488_0 [ 1 ] + rtb_B_63_426_0 [ 2 ] ;
_rtB -> B_63_492_0 [ 3 ] = _rtB -> B_63_429_0 * ( real_T ) ! _rtB ->
B_63_488_0 [ 1 ] + rtb_B_63_426_0 [ 3 ] ; _rtB -> B_63_492_0 [ 4 ] = _rtB ->
B_63_429_0 * ( real_T ) _rtB -> B_63_488_0 [ 2 ] + rtb_B_63_426_0 [ 4 ] ;
_rtB -> B_63_492_0 [ 5 ] = _rtB -> B_63_429_0 * ( real_T ) ! _rtB ->
B_63_488_0 [ 2 ] + rtb_B_63_426_0 [ 5 ] ; rtb_B_63_112_0 = _rtP -> P_313 *
_rtB -> B_63_394_0 ; _rtB -> B_63_494_0 = _rtB -> B_63_63_0 - _rtB ->
B_63_409_0 [ 1 ] ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW ->
Step4_MODE = ( ssGetTaskTime ( S , 1 ) >= _rtP -> P_314 ) ; if ( _rtDW ->
Step4_MODE == 1 ) { _rtB -> B_63_495_0 = _rtP -> P_316 ; } else { _rtB ->
B_63_495_0 = _rtP -> P_315 ; } } _rtB -> B_63_496_0 = _rtB -> B_63_494_0 *
_rtB -> B_63_495_0 ; _rtB -> B_63_497_0 = _rtP -> P_317 * _rtB -> B_63_496_0
; _rtB -> B_63_498_0 = _rtX -> Integrator_CSTATE_b ; _rtB -> B_63_499_0 =
_rtP -> P_319 * _rtB -> B_63_496_0 ; _rtB -> B_63_500_0 = _rtB -> B_63_499_0
+ _rtB -> B_63_498_0 ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if (
ssIsModeUpdateTimeStep ( S ) ) { _rtDW -> RelationalOperator_Mode_c = ( _rtX
-> Integrator_CSTATE_o > _rtB -> B_63_73_0 ) ; } _rtB -> B_63_501_0 = _rtDW
-> RelationalOperator_Mode_c ; } _rtB -> B_63_502_0 = _rtX ->
Integrator_CSTATE_o - _rtB -> B_63_73_0 ; if ( ( _rtDW ->
Initial_FirstOutputTime == ( rtMinusInf ) ) || ( _rtDW ->
Initial_FirstOutputTime == ssGetTaskTime ( S , 0 ) ) ) { _rtDW ->
Initial_FirstOutputTime = ssGetTaskTime ( S , 0 ) ; _rtB -> B_63_503_0 = _rtP
-> P_320 ; } else { _rtB -> B_63_503_0 = _rtB -> B_63_502_0 ; } if (
ssIsModeUpdateTimeStep ( S ) ) { rtb_B_63_370_0 = ( _rtB -> B_63_501_0 && (
_rtZCE -> Integrator_Reset_ZCE != POS_ZCSIG ) ) ; _rtZCE ->
Integrator_Reset_ZCE = _rtB -> B_63_501_0 ; if ( rtb_B_63_370_0 || ( _rtDW ->
Integrator_IWORK != 0 ) ) { rtb_B_63_370_0 = ( _rtX -> Integrator_CSTATE_o !=
_rtB -> B_63_503_0 ) ; _rtX -> Integrator_CSTATE_o = _rtB -> B_63_503_0 ; if
( rtb_B_63_370_0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; } } if ( _rtX ->
Integrator_CSTATE_o >= _rtP -> P_321 ) { if ( _rtX -> Integrator_CSTATE_o !=
_rtP -> P_321 ) { _rtX -> Integrator_CSTATE_o = _rtP -> P_321 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; } _rtDW ->
Integrator_MODE = 3 ; } else if ( _rtX -> Integrator_CSTATE_o <= _rtP ->
P_322 ) { if ( _rtX -> Integrator_CSTATE_o != _rtP -> P_322 ) { _rtX ->
Integrator_CSTATE_o = _rtP -> P_322 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; } _rtDW ->
Integrator_MODE = 4 ; } else { if ( _rtDW -> Integrator_MODE != 0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtDW -> Integrator_MODE
= 0 ; } _rtB -> B_63_504_0 = _rtX -> Integrator_CSTATE_o ; } else { _rtB ->
B_63_504_0 = _rtX -> Integrator_CSTATE_o ; }
PV_Battery_Grid2021a_AutomaticGainControl ( S , _rtB -> B_63_72_0 , _rtB ->
B_63_394_0 , _rtB -> B_63_504_0 , _rtB -> B_63_266_0 , & _rtB ->
AutomaticGainControl_p , & _rtDW -> AutomaticGainControl_p , & _rtP ->
AutomaticGainControl_p , & _rtX -> AutomaticGainControl_p , & ( (
XDis_PV_Battery_Grid2021a_T * ) ssGetContStateDisabled ( S ) ) ->
AutomaticGainControl_p ) ; if ( ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX ->
Integrator_CSTATE_c >= _rtP -> P_325 ) { if ( _rtX -> Integrator_CSTATE_c !=
_rtP -> P_325 ) { _rtX -> Integrator_CSTATE_c = _rtP -> P_325 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtDW ->
Integrator_MODE_a = 3 ; } else if ( _rtX -> Integrator_CSTATE_c <= _rtP ->
P_326 ) { if ( _rtX -> Integrator_CSTATE_c != _rtP -> P_326 ) { _rtX ->
Integrator_CSTATE_c = _rtP -> P_326 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtDW ->
Integrator_MODE_a = 4 ; } else { if ( _rtDW -> Integrator_MODE_a != 0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtDW ->
Integrator_MODE_a = 0 ; } _rtB -> B_63_507_0 = _rtX -> Integrator_CSTATE_c ;
} else { _rtB -> B_63_507_0 = _rtX -> Integrator_CSTATE_c ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T appliedDelay ;
_rtB -> B_63_508_0 = PV_Battery_Grid2021a_acc_rt_VTDelayfindtDInterpolate ( (
( X_PV_Battery_Grid2021a_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE , * uBuffer , _rtDW ->
VariableTransportDelay_IWORK . CircularBufSize , _rtDW ->
VariableTransportDelay_IWORK . Head , _rtDW -> VariableTransportDelay_IWORK .
Tail , & _rtDW -> VariableTransportDelay_IWORK . Last , simTime , 0.0 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW ->
VariableTransportDelay_IWORK . CircularBufSize ) [ _rtDW ->
VariableTransportDelay_IWORK . Head ] == ssGetT ( S ) ) ) , _rtP -> P_328 , &
appliedDelay ) ; } _rtB -> B_63_509_0 = _rtX -> integrator_CSTATE_i ; _rtB ->
B_63_510_0 = ssGetT ( S ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) {
if ( ssIsModeUpdateTimeStep ( S ) ) { _rtDW -> RelationalOperator_Mode_n = (
_rtB -> B_63_510_0 >= _rtB -> B_63_74_0 ) ; } _rtB -> B_63_511_0 = _rtDW ->
RelationalOperator_Mode_n ; } if ( _rtB -> B_63_511_0 ) { _rtB -> B_44_0_0 =
_rtB -> B_63_509_0 - _rtB -> B_63_508_0 ; _rtB -> B_44_1_0 = _rtB -> B_44_0_0
* _rtB -> B_63_394_0 ; _rtB -> B_63_513_0 = _rtB -> B_44_1_0 ; } else { _rtB
-> B_63_513_0 = _rtB -> B_63_75_0 ; } _rtB -> B_63_514_0 = _rtB -> B_63_513_0
* _rtB -> AutomaticGainControl_p . B_15_15_0 ; _rtB -> B_63_515_0 = _rtP ->
P_330 * _rtB -> B_63_514_0 ; _rtB -> B_63_516_0 = _rtP -> P_331 * _rtB ->
B_63_514_0 ; _rtB -> B_63_517_0 = _rtP -> P_332 * _rtB -> B_63_514_0 ; _rtB
-> B_63_518_0 = 0.0 ; _rtB -> B_63_518_0 += _rtP -> P_334 * _rtX ->
TransferFcn_CSTATE ; _rtB -> B_63_518_0 += _rtP -> P_335 * _rtB -> B_63_517_0
; _rtB -> B_63_519_0 = ( _rtB -> B_63_515_0 + _rtB -> B_63_507_0 ) + _rtB ->
B_63_518_0 ; if ( ssIsModeUpdateTimeStep ( S ) ) { _rtDW -> Saturation2_MODE
= _rtB -> B_63_519_0 >= _rtP -> P_336 ? 1 : _rtB -> B_63_519_0 > _rtP ->
P_337 ? 0 : - 1 ; } _rtB -> B_63_520_0 = _rtDW -> Saturation2_MODE == 1 ?
_rtP -> P_336 : _rtDW -> Saturation2_MODE == - 1 ? _rtP -> P_337 : _rtB ->
B_63_519_0 ; _rtB -> B_63_521_0 = _rtP -> P_338 * _rtB -> B_63_520_0 ; if ( (
_rtDW -> LastMajorTimeA >= ssGetTaskTime ( S , 0 ) ) && ( _rtDW ->
LastMajorTimeB >= ssGetTaskTime ( S , 0 ) ) ) { _rtB -> B_63_522_0 = _rtB ->
B_63_521_0 ; } else { if ( ( ( _rtDW -> LastMajorTimeA < _rtDW ->
LastMajorTimeB ) && ( _rtDW -> LastMajorTimeB < ssGetTaskTime ( S , 0 ) ) )
|| ( ( _rtDW -> LastMajorTimeA >= _rtDW -> LastMajorTimeB ) && ( _rtDW ->
LastMajorTimeA >= ssGetTaskTime ( S , 0 ) ) ) ) { rtb_B_63_317_0 =
ssGetTaskTime ( S , 0 ) - _rtDW -> LastMajorTimeB ; rtb_B_63_363_0 = _rtDW ->
PrevYB ; } else { rtb_B_63_317_0 = ssGetTaskTime ( S , 0 ) - _rtDW ->
LastMajorTimeA ; rtb_B_63_363_0 = _rtDW -> PrevYA ; } rtb_B_63_318_0 =
rtb_B_63_317_0 * _rtP -> P_339 ; rtb_B_63_384_0 = _rtB -> B_63_521_0 -
rtb_B_63_363_0 ; if ( rtb_B_63_384_0 > rtb_B_63_318_0 ) { _rtB -> B_63_522_0
= rtb_B_63_363_0 + rtb_B_63_318_0 ; } else { rtb_B_63_317_0 *= _rtP -> P_340
; if ( rtb_B_63_384_0 < rtb_B_63_317_0 ) { _rtB -> B_63_522_0 =
rtb_B_63_363_0 + rtb_B_63_317_0 ; } else { _rtB -> B_63_522_0 = _rtB ->
B_63_521_0 ; } } } _rtB -> B_63_523_0 = _rtX -> Integrator_x1_CSTATE ; _rtB
-> B_63_524_0 = _rtP -> P_342 * _rtB -> B_63_523_0 ; _rtB -> B_63_525_0 =
_rtX -> Integrator_x2_CSTATE ; _rtB -> B_63_526_0 = _rtP -> P_344 * _rtB ->
B_63_525_0 ; _rtB -> B_63_527_0 = _rtP -> P_345 * _rtB -> B_63_523_0 ; _rtB
-> B_63_528_0 = _rtP -> P_346 * _rtB -> B_63_525_0 ; _rtB -> B_63_529_0 =
_rtB -> B_63_524_0 + _rtB -> B_63_526_0 ; _rtB -> B_63_530_0 = _rtB ->
B_63_527_0 + _rtB -> B_63_528_0 ; _rtB -> B_63_531_0 = _rtP -> P_347 * _rtB
-> B_63_522_0 ; _rtB -> B_63_532_0 = _rtB -> B_63_529_0 + _rtB -> B_63_531_0
; _rtB -> B_63_533_0 = _rtP -> P_348 * _rtB -> B_63_522_0 ; _rtB ->
B_63_534_0 = _rtB -> B_63_530_0 + _rtB -> B_63_533_0 ; _rtB -> B_63_539_0 = (
_rtP -> P_349 * _rtB -> B_63_523_0 + _rtP -> P_350 * _rtB -> B_63_525_0 ) +
_rtP -> P_351 * _rtB -> B_63_522_0 ; if ( _rtB -> B_63_394_0 > _rtP -> P_352
) { _rtB -> B_63_540_0 = _rtP -> P_352 ; } else if ( _rtB -> B_63_394_0 <
_rtP -> P_353 ) { _rtB -> B_63_540_0 = _rtP -> P_353 ; } else { _rtB ->
B_63_540_0 = _rtB -> B_63_394_0 ; } _rtB -> B_63_541_0 = 1.0 / _rtB ->
B_63_540_0 ; for ( i = 0 ; i < 3 ; i ++ ) { _rtB -> B_63_542_0 [ i ] = 0.0 ;
_rtB -> B_63_542_0 [ i ] += _rtP -> P_354 [ i ] * _rtB -> B_63_266_0 [ 0 ] ;
_rtB -> B_63_542_0 [ i ] += _rtP -> P_354 [ i + 3 ] * _rtB -> B_63_266_0 [ 1
] ; _rtB -> B_63_542_0 [ i ] += _rtP -> P_354 [ i + 6 ] * _rtB -> B_63_266_0
[ 2 ] ; _rtB -> B_63_543_0 [ i ] = _rtP -> P_355 * _rtB -> B_63_542_0 [ i ] ;
} PV_Battery_Grid2021a_Subsystempi2delay ( S , _rtB -> B_63_80_0_o , & _rtB
-> B_63_543_0 [ 0 ] , _rtB -> B_63_504_0 , & _rtB -> Subsystempi2delay_e , &
_rtDW -> Subsystempi2delay_e ) ; PV_Battery_Grid2021a_Subsystem1 ( S , _rtB
-> B_63_78_0_l , & _rtB -> B_63_543_0 [ 0 ] , _rtB -> B_63_504_0 , & _rtB ->
Subsystem1_b , & _rtDW -> Subsystem1_b ) ; if ( _rtB -> B_63_78_0_l != 0 ) {
_rtB -> B_63_546_0 [ 0 ] = _rtB -> Subsystem1_b . B_14_0_0 ; _rtB ->
B_63_546_0 [ 1 ] = _rtB -> Subsystem1_b . B_14_1_0 ; } else { _rtB ->
B_63_546_0 [ 0 ] = _rtB -> Subsystempi2delay_e . B_13_0_0 ; _rtB ->
B_63_546_0 [ 1 ] = _rtB -> Subsystempi2delay_e . B_13_1_0 ; } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Qref_MODE = (
ssGetTaskTime ( S , 1 ) >= _rtP -> P_356 ) ; if ( _rtDW -> Qref_MODE == 1 ) {
rtb_B_63_17_0 = _rtP -> P_358 ; } else { rtb_B_63_17_0 = _rtP -> P_357 ; }
_rtB -> B_63_548_0 = _rtP -> P_359 * rtb_B_63_17_0 ; } _rtB -> B_63_549_0 =
_rtB -> B_63_548_0 / _rtB -> B_63_409_0 [ 0 ] ; _rtB -> B_63_550_0 = _rtB ->
B_63_549_0 - _rtB -> B_63_403_0 [ 1 ] ; _rtB -> B_63_552_0 = _rtX ->
Integrator_CSTATE_a ; _rtB -> B_63_553_0 = _rtP -> P_360 * _rtB -> B_63_550_0
+ _rtB -> B_63_552_0 ; ssCallAccelRunBlock ( S , 63 , 554 ,
SS_CALL_MDL_OUTPUTS ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) {
_rtDW -> Pref_MODE = ( ssGetTaskTime ( S , 1 ) >= _rtP -> P_362 ) ; if (
_rtDW -> Pref_MODE == 1 ) { rtb_B_63_17_0 = _rtP -> P_364 ; } else {
rtb_B_63_17_0 = _rtP -> P_363 ; } _rtB -> B_63_556_0 = _rtP -> P_365 *
rtb_B_63_17_0 ; } _rtB -> B_63_557_0 = _rtB -> B_63_556_0 / _rtB ->
B_63_409_0 [ 0 ] ; _rtB -> B_63_558_0 = _rtB -> B_63_557_0 - _rtB ->
B_63_403_0 [ 0 ] ; _rtB -> B_63_560_0 = _rtX -> Integrator_CSTATE_p ; _rtB ->
B_63_561_0 = _rtP -> P_366 * _rtB -> B_63_558_0 + _rtB -> B_63_560_0 ;
ssCallAccelRunBlock ( S , 63 , 562 , SS_CALL_MDL_OUTPUTS ) ; rtb_B_63_112_0
*= _rtB -> B_63_83_0 ; _rtB -> B_63_566_0 = ( _rtB -> B_63_561_0 -
rtb_B_63_112_0 * _rtB -> B_63_403_0 [ 1 ] ) + _rtB -> B_63_409_0 [ 0 ] ;
ssCallAccelRunBlock ( S , 63 , 567 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 63 , 568 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_63_570_0 = rtb_B_63_112_0 * _rtB -> B_63_403_0 [ 0 ] + _rtB -> B_63_553_0 ;
ssCallAccelRunBlock ( S , 63 , 571 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 63 , 572 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_63_573_0 = _rtB -> B_63_570_0 + _rtB -> B_63_84_0 ; ssCallAccelRunBlock ( S
, 63 , 574 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_63_575_0 = _rtP -> P_368 *
_rtB -> B_63_558_0 ; _rtB -> B_63_576_0 = _rtP -> P_369 * _rtB -> B_63_550_0
; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( i != 0 ) && ssIsModeUpdateTimeStep
( S ) ) { if ( _rtB -> B_63_88_0 > 0 ) { if ( ! _rtDW -> Subsystem1_MODE ) {
if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtDW -> Subsystem1_MODE
= true ; } } else if ( _rtDW -> Subsystem1_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Subsystem1_MODE =
false ; } } if ( _rtDW -> Subsystem1_MODE ) { _rtB -> B_48_0_0 = _rtB ->
B_63_566_0 * muDoubleScalarCos ( _rtB -> B_63_400_0 ) - _rtB -> B_63_573_0 *
muDoubleScalarSin ( _rtB -> B_63_400_0 ) ; _rtB -> B_48_1_0 = _rtB ->
B_63_566_0 * muDoubleScalarSin ( _rtB -> B_63_400_0 ) + _rtB -> B_63_573_0 *
muDoubleScalarCos ( _rtB -> B_63_400_0 ) ; if ( ssIsModeUpdateTimeStep ( S )
) { srUpdateBC ( _rtDW -> Subsystem1_SubsysRanBC_g ) ; } } i = ssIsSampleHit
( S , 1 , 0 ) ; if ( ( i != 0 ) && ssIsModeUpdateTimeStep ( S ) ) { if ( _rtB
-> B_63_90_0 > 0 ) { if ( ! _rtDW -> Subsystempi2delay_MODE ) { if (
ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtDW ->
Subsystempi2delay_MODE = true ; } } else if ( _rtDW -> Subsystempi2delay_MODE
) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW ->
Subsystempi2delay_MODE = false ; } } if ( _rtDW -> Subsystempi2delay_MODE ) {
_rtB -> B_47_0_0 = _rtB -> B_63_566_0 * muDoubleScalarSin ( _rtB ->
B_63_400_0 ) + _rtB -> B_63_573_0 * muDoubleScalarCos ( _rtB -> B_63_400_0 )
; _rtB -> B_47_1_0 = - _rtB -> B_63_566_0 * muDoubleScalarCos ( _rtB ->
B_63_400_0 ) + _rtB -> B_63_573_0 * muDoubleScalarSin ( _rtB -> B_63_400_0 )
; if ( ssIsModeUpdateTimeStep ( S ) ) { srUpdateBC ( _rtDW ->
Subsystempi2delay_SubsysRanBC_a ) ; } } if ( _rtB -> B_63_88_0 != 0 ) {
rtb_B_63_112_0 = _rtB -> B_48_0_0 ; rtb_B_63_0_0 = _rtB -> B_48_1_0 ; } else
{ rtb_B_63_112_0 = _rtB -> B_47_0_0 ; rtb_B_63_0_0 = _rtB -> B_47_1_0 ; } for
( i = 0 ; i < 3 ; i ++ ) { _rtB -> B_63_581_0 [ i ] = 0.0 ; _rtB ->
B_63_581_0 [ i ] += _rtP -> P_370 [ i ] * rtb_B_63_112_0 ; _rtB -> B_63_581_0
[ i ] += _rtP -> P_370 [ i + 3 ] * rtb_B_63_0_0 ; _rtB -> B_63_581_0 [ i ] +=
_rtP -> P_370 [ i + 6 ] * _rtB -> B_63_85_0 ; } i = ssIsSampleHit ( S , 5 , 0
) ; if ( i != 0 ) { _rtB -> B_63_582_0 = _rtDW -> UnitDelay_DSTATE_p ; } i =
ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { ssCallAccelRunBlock ( S , 63 ,
583 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 63 , 584 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_63_585_0 = _rtP -> P_372 * rtb_B_63_439_0 ;
_rtB -> B_63_586_0 = _rtP -> P_373 * rtb_B_63_442_0 ; _rtB -> B_63_587_0 =
_rtP -> P_374 * B_63_453_0_idx_1 ; _rtB -> B_63_588_0 = _rtP -> P_375 *
rtb_B_63_443_0 ; } i = ssIsSampleHit ( S , 6 , 0 ) ; if ( i != 0 ) { _rtB ->
B_63_589_0 = _rtDW -> UnitDelay_DSTATE_j ; } i = ssIsSampleHit ( S , 3 , 0 )
; if ( i != 0 ) { ssCallAccelRunBlock ( S , 63 , 590 , SS_CALL_MDL_OUTPUTS )
; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_63_603_0 =
_rtDW -> Memory_PreviousInput_at ; } ssCallAccelRunBlock ( S , 63 , 604 ,
SS_CALL_MDL_OUTPUTS ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { {
if ( _rtDW -> TAQSigLogging_InsertedFor_PLL3ph_at_outport_0_PWORK . AQHandles
&& ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_PLL3ph_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_63_603_0 + 0 ) ; } } if (
ssIsModeUpdateTimeStep ( S ) ) { _rtDW -> RelationalOperator_Mode_nz = ( _rtX
-> Integrator_CSTATE_n > _rtB -> B_63_127_0_d ) ; } _rtB -> B_63_606_0 =
_rtDW -> RelationalOperator_Mode_nz ; } _rtB -> B_63_607_0 = _rtX ->
Integrator_CSTATE_n - _rtB -> B_63_127_0_d ; if ( ( _rtDW ->
Initial_FirstOutputTime_a == ( rtMinusInf ) ) || ( _rtDW ->
Initial_FirstOutputTime_a == ssGetTaskTime ( S , 0 ) ) ) { _rtDW ->
Initial_FirstOutputTime_a = ssGetTaskTime ( S , 0 ) ; _rtB -> B_63_608_0 =
_rtP -> P_378 ; } else { _rtB -> B_63_608_0 = _rtB -> B_63_607_0 ; } if (
ssIsModeUpdateTimeStep ( S ) ) { rtb_B_63_370_0 = ( _rtB -> B_63_606_0 && (
_rtZCE -> Integrator_Reset_ZCE_p != POS_ZCSIG ) ) ; _rtZCE ->
Integrator_Reset_ZCE_p = _rtB -> B_63_606_0 ; if ( rtb_B_63_370_0 || ( _rtDW
-> Integrator_IWORK_b != 0 ) ) { rtb_B_63_370_0 = ( _rtX ->
Integrator_CSTATE_n != _rtB -> B_63_608_0 ) ; _rtX -> Integrator_CSTATE_n =
_rtB -> B_63_608_0 ; if ( rtb_B_63_370_0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; } } if ( _rtX ->
Integrator_CSTATE_n >= _rtP -> P_379 ) { if ( _rtX -> Integrator_CSTATE_n !=
_rtP -> P_379 ) { _rtX -> Integrator_CSTATE_n = _rtP -> P_379 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; } _rtDW ->
Integrator_MODE_at = 3 ; } else if ( _rtX -> Integrator_CSTATE_n <= _rtP ->
P_380 ) { if ( _rtX -> Integrator_CSTATE_n != _rtP -> P_380 ) { _rtX ->
Integrator_CSTATE_n = _rtP -> P_380 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; } _rtDW ->
Integrator_MODE_at = 4 ; } else { if ( _rtDW -> Integrator_MODE_at != 0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtDW ->
Integrator_MODE_at = 0 ; } _rtB -> B_63_609_0 = _rtX -> Integrator_CSTATE_n ;
} else { _rtB -> B_63_609_0 = _rtX -> Integrator_CSTATE_n ; }
PV_Battery_Grid2021a_AutomaticGainControl ( S , _rtB -> B_63_126_0 , _rtB ->
B_63_603_0 , _rtB -> B_63_609_0 , _rtB -> B_63_266_0 , & _rtB ->
AutomaticGainControl , & _rtDW -> AutomaticGainControl , & _rtP ->
AutomaticGainControl , & _rtX -> AutomaticGainControl , & ( (
XDis_PV_Battery_Grid2021a_T * ) ssGetContStateDisabled ( S ) ) ->
AutomaticGainControl ) ; if ( ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX ->
Integrator_CSTATE_f >= _rtP -> P_383 ) { if ( _rtX -> Integrator_CSTATE_f !=
_rtP -> P_383 ) { _rtX -> Integrator_CSTATE_f = _rtP -> P_383 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtDW ->
Integrator_MODE_az = 3 ; } else if ( _rtX -> Integrator_CSTATE_f <= _rtP ->
P_384 ) { if ( _rtX -> Integrator_CSTATE_f != _rtP -> P_384 ) { _rtX ->
Integrator_CSTATE_f = _rtP -> P_384 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtDW ->
Integrator_MODE_az = 4 ; } else { if ( _rtDW -> Integrator_MODE_az != 0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtDW ->
Integrator_MODE_az = 0 ; } _rtB -> B_63_612_0 = _rtX -> Integrator_CSTATE_f ;
} else { _rtB -> B_63_612_0 = _rtX -> Integrator_CSTATE_f ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_m .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T appliedDelay ;
_rtB -> B_63_613_0 = PV_Battery_Grid2021a_acc_rt_VTDelayfindtDInterpolate ( (
( X_PV_Battery_Grid2021a_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE_h , * uBuffer , _rtDW ->
VariableTransportDelay_IWORK_c . CircularBufSize , _rtDW ->
VariableTransportDelay_IWORK_c . Head , _rtDW ->
VariableTransportDelay_IWORK_c . Tail , & _rtDW ->
VariableTransportDelay_IWORK_c . Last , simTime , 0.0 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW ->
VariableTransportDelay_IWORK_c . CircularBufSize ) [ _rtDW ->
VariableTransportDelay_IWORK_c . Head ] == ssGetT ( S ) ) ) , _rtP -> P_386 ,
& appliedDelay ) ; } _rtB -> B_63_614_0 = _rtX -> integrator_CSTATE_n ; _rtB
-> B_63_615_0 = ssGetT ( S ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0
) { if ( ssIsModeUpdateTimeStep ( S ) ) { _rtDW -> RelationalOperator_Mode_i
= ( _rtB -> B_63_615_0 >= _rtB -> B_63_128_0 ) ; } _rtB -> B_63_616_0 = _rtDW
-> RelationalOperator_Mode_i ; } if ( _rtB -> B_63_616_0 ) { _rtB -> B_16_0_0
= _rtB -> B_63_614_0 - _rtB -> B_63_613_0 ; _rtB -> B_16_1_0 = _rtB ->
B_16_0_0 * _rtB -> B_63_603_0 ; _rtB -> B_63_618_0 = _rtB -> B_16_1_0 ; }
else { _rtB -> B_63_618_0 = _rtB -> B_63_129_0_g ; } _rtB -> B_63_619_0 =
_rtB -> B_63_618_0 * _rtB -> AutomaticGainControl . B_15_15_0 ; _rtB ->
B_63_620_0 = _rtP -> P_388 * _rtB -> B_63_619_0 ; _rtB -> B_63_621_0 = _rtP
-> P_389 * _rtB -> B_63_619_0 ; _rtB -> B_63_622_0 = _rtP -> P_390 * _rtB ->
B_63_619_0 ; _rtB -> B_63_623_0 = 0.0 ; _rtB -> B_63_623_0 += _rtP -> P_392 *
_rtX -> TransferFcn_CSTATE_k ; _rtB -> B_63_623_0 += _rtP -> P_393 * _rtB ->
B_63_622_0 ; _rtB -> B_63_624_0 = ( _rtB -> B_63_620_0 + _rtB -> B_63_612_0 )
+ _rtB -> B_63_623_0 ; if ( ssIsModeUpdateTimeStep ( S ) ) { _rtDW ->
Saturation2_MODE_e = _rtB -> B_63_624_0 >= _rtP -> P_394 ? 1 : _rtB ->
B_63_624_0 > _rtP -> P_395 ? 0 : - 1 ; } _rtB -> B_63_625_0 = _rtDW ->
Saturation2_MODE_e == 1 ? _rtP -> P_394 : _rtDW -> Saturation2_MODE_e == - 1
? _rtP -> P_395 : _rtB -> B_63_624_0 ; _rtB -> B_63_626_0 = _rtP -> P_396 *
_rtB -> B_63_625_0 ; if ( ( _rtDW -> LastMajorTimeA_l >= ssGetTaskTime ( S ,
0 ) ) && ( _rtDW -> LastMajorTimeB_c >= ssGetTaskTime ( S , 0 ) ) ) { _rtB ->
B_63_627_0 = _rtB -> B_63_626_0 ; } else { if ( ( ( _rtDW -> LastMajorTimeA_l
< _rtDW -> LastMajorTimeB_c ) && ( _rtDW -> LastMajorTimeB_c < ssGetTaskTime
( S , 0 ) ) ) || ( ( _rtDW -> LastMajorTimeA_l >= _rtDW -> LastMajorTimeB_c )
&& ( _rtDW -> LastMajorTimeA_l >= ssGetTaskTime ( S , 0 ) ) ) ) {
rtb_B_63_317_0 = ssGetTaskTime ( S , 0 ) - _rtDW -> LastMajorTimeB_c ;
rtb_B_63_363_0 = _rtDW -> PrevYB_l ; } else { rtb_B_63_317_0 = ssGetTaskTime
( S , 0 ) - _rtDW -> LastMajorTimeA_l ; rtb_B_63_363_0 = _rtDW -> PrevYA_l ;
} rtb_B_63_318_0 = rtb_B_63_317_0 * _rtP -> P_397 ; rtb_B_63_384_0 = _rtB ->
B_63_626_0 - rtb_B_63_363_0 ; if ( rtb_B_63_384_0 > rtb_B_63_318_0 ) { _rtB
-> B_63_627_0 = rtb_B_63_363_0 + rtb_B_63_318_0 ; } else { rtb_B_63_317_0 *=
_rtP -> P_398 ; if ( rtb_B_63_384_0 < rtb_B_63_317_0 ) { _rtB -> B_63_627_0 =
rtb_B_63_363_0 + rtb_B_63_317_0 ; } else { _rtB -> B_63_627_0 = _rtB ->
B_63_626_0 ; } } } _rtB -> B_63_628_0 = _rtX -> Integrator_x1_CSTATE_p ; _rtB
-> B_63_629_0 = _rtP -> P_400 * _rtB -> B_63_628_0 ; _rtB -> B_63_630_0 =
_rtX -> Integrator_x2_CSTATE_k ; _rtB -> B_63_631_0 = _rtP -> P_402 * _rtB ->
B_63_630_0 ; _rtB -> B_63_632_0 = _rtP -> P_403 * _rtB -> B_63_628_0 ; _rtB
-> B_63_633_0 = _rtP -> P_404 * _rtB -> B_63_630_0 ; _rtB -> B_63_634_0 =
_rtB -> B_63_629_0 + _rtB -> B_63_631_0 ; _rtB -> B_63_635_0 = _rtB ->
B_63_632_0 + _rtB -> B_63_633_0 ; _rtB -> B_63_636_0 = _rtP -> P_405 * _rtB
-> B_63_627_0 ; _rtB -> B_63_637_0 = _rtB -> B_63_634_0 + _rtB -> B_63_636_0
; _rtB -> B_63_638_0 = _rtP -> P_406 * _rtB -> B_63_627_0 ; _rtB ->
B_63_639_0 = _rtB -> B_63_635_0 + _rtB -> B_63_638_0 ; _rtB -> B_63_644_0 = (
_rtP -> P_407 * _rtB -> B_63_628_0 + _rtP -> P_408 * _rtB -> B_63_630_0 ) +
_rtP -> P_409 * _rtB -> B_63_627_0 ; if ( _rtB -> B_63_603_0 > _rtP -> P_410
) { _rtB -> B_63_645_0 = _rtP -> P_410 ; } else if ( _rtB -> B_63_603_0 <
_rtP -> P_411 ) { _rtB -> B_63_645_0 = _rtP -> P_411 ; } else { _rtB ->
B_63_645_0 = _rtB -> B_63_603_0 ; } _rtB -> B_63_646_0 = 1.0 / _rtB ->
B_63_645_0 ; for ( i = 0 ; i < 3 ; i ++ ) { _rtB -> B_63_647_0 [ i ] = 0.0 ;
_rtB -> B_63_647_0 [ i ] += _rtP -> P_412 [ i ] * _rtB -> B_63_266_0 [ 0 ] ;
_rtB -> B_63_647_0 [ i ] += _rtP -> P_412 [ i + 3 ] * _rtB -> B_63_266_0 [ 1
] ; _rtB -> B_63_647_0 [ i ] += _rtP -> P_412 [ i + 6 ] * _rtB -> B_63_266_0
[ 2 ] ; _rtB -> B_63_648_0 [ i ] = _rtP -> P_413 * _rtB -> B_63_647_0 [ i ] ;
} PV_Battery_Grid2021a_Subsystempi2delay ( S , _rtB -> B_63_134_0 , & _rtB ->
B_63_648_0 [ 0 ] , _rtB -> B_63_609_0 , & _rtB -> Subsystempi2delay , & _rtDW
-> Subsystempi2delay ) ; PV_Battery_Grid2021a_Subsystem1 ( S , _rtB ->
B_63_132_0 , & _rtB -> B_63_648_0 [ 0 ] , _rtB -> B_63_609_0 , & _rtB ->
Subsystem1 , & _rtDW -> Subsystem1 ) ; if ( _rtB -> B_63_132_0 != 0 ) { _rtB
-> B_63_651_0 [ 0 ] = _rtB -> Subsystem1 . B_14_0_0 ; _rtB -> B_63_651_0 [ 1
] = _rtB -> Subsystem1 . B_14_1_0 ; } else { _rtB -> B_63_651_0 [ 0 ] = _rtB
-> Subsystempi2delay . B_13_0_0 ; _rtB -> B_63_651_0 [ 1 ] = _rtB ->
Subsystempi2delay . B_13_1_0 ; } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i !=
0 ) { _rtB -> B_63_652_0 = _rtP -> P_414 * _rtB -> B_63_46_0 [ 23 ] ;
ssCallAccelRunBlock ( S , 63 , 653 , SS_CALL_MDL_OUTPUTS ) ; if ( _rtB ->
B_63_135_0_l >= _rtP -> P_418 ) { rtb_B_63_439_0 = _rtB -> B_63_140_0_d ; }
else { rtb_B_63_439_0 = rt_Lookup ( & _rtP -> P_416 [ 0 ] , 6 , ssGetTaskTime
( S , 3 ) , & _rtP -> P_417 [ 0 ] ) ; } if ( _rtP -> P_415 >= _rtP -> P_419 )
{ if ( _rtB -> B_63_136_0 >= _rtP -> P_20 ) { _rtB -> B_63_731_0 =
rtb_B_63_439_0 ; } else { _rtB -> B_63_731_0 = _rtB -> B_63_139_0 ; } } else
{ _rtB -> B_63_731_0 = rt_Lookup ( & _rtP -> P_21 [ 0 ] , 5 , ssGetTaskTime (
S , 3 ) , & _rtP -> P_22 [ 0 ] ) ; } if ( _rtP -> P_420 >= _rtP -> P_421 ) {
if ( _rtB -> B_63_137_0 >= _rtP -> P_23 ) { _rtB -> B_63_739_0 =
rtb_B_63_439_0 ; } else { _rtB -> B_63_739_0 = _rtB -> B_63_139_0 ; } } else
{ _rtB -> B_63_739_0 = rt_Lookup ( & _rtP -> P_24 [ 0 ] , 5 , ssGetTaskTime (
S , 3 ) , & _rtP -> P_25 [ 0 ] ) ; } if ( _rtP -> P_422 >= _rtP -> P_423 ) {
if ( _rtB -> B_63_138_0_d >= _rtP -> P_26 ) { _rtB -> B_63_747_0 =
rtb_B_63_439_0 ; } else { _rtB -> B_63_747_0 = _rtB -> B_63_139_0 ; } } else
{ _rtB -> B_63_747_0 = rt_Lookup ( & _rtP -> P_27 [ 0 ] , 5 , ssGetTaskTime (
S , 3 ) , & _rtP -> P_28 [ 0 ] ) ; } } UNUSED_PARAMETER ( tid ) ; } static
void mdlOutputsTID9 ( SimStruct * S , int_T tid ) { B_PV_Battery_Grid2021a_T
* _rtB ; DW_PV_Battery_Grid2021a_T * _rtDW ; P_PV_Battery_Grid2021a_T * _rtP
; real_T rtb_B_63_19_0 ; real_T rtb_B_63_4_0 ; int32_T i ; _rtDW = ( (
DW_PV_Battery_Grid2021a_T * ) ssGetRootDWork ( S ) ) ; _rtP = ( (
P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_PV_Battery_Grid2021a_T * ) _ssGetModelBlockIO ( S ) ) ; for ( i = 0 ; i < 7
; i ++ ) { _rtB -> B_63_0_0 [ i ] = _rtP -> P_424 [ i ] ; } _rtB -> B_63_2_0
= _rtP -> P_426 ; rtb_B_63_4_0 = _rtP -> P_425 + _rtP -> P_427 ; _rtB ->
B_63_10_0 = ( ( rtb_B_63_4_0 - _rtP -> P_428 ) * _rtP -> P_429 + _rtP ->
P_430 ) * _rtP -> P_431 ; _rtB -> B_63_11_0_m = _rtP -> P_432 ; rtb_B_63_19_0
= rtb_B_63_4_0 / _rtP -> P_437 ; _rtB -> B_63_27_0 = muDoubleScalarExp ( _rtP
-> P_439 - ( ( rtb_B_63_4_0 - _rtP -> P_434 ) * _rtP -> P_435 + _rtP -> P_433
) * _rtP -> P_436 / ( _rtP -> P_440 * rtb_B_63_4_0 ) ) * ( _rtP -> P_438 *
muDoubleScalarPower ( rtb_B_63_19_0 , 3.0 ) ) ; _rtB -> B_63_28_0_c = _rtP ->
P_441 ; _rtB -> B_63_29_0 = _rtP -> P_442 ; _rtB -> B_63_30_0 = _rtP -> P_443
* rtb_B_63_19_0 ; _rtB -> B_63_31_0 = _rtP -> P_444 ; _rtB -> B_63_32_0 =
_rtP -> P_445 ; _rtB -> B_63_33_0 = _rtP -> P_446 ; _rtB -> B_63_34_0 = _rtP
-> P_447 ; _rtB -> B_63_35_0_k = _rtP -> P_448 ; _rtB -> B_63_36_0 = _rtP ->
P_449 ; _rtB -> B_63_37_0_c = _rtP -> P_450 ; _rtB -> B_63_38_0_b = _rtP ->
P_451 ; _rtB -> B_63_39_0 = _rtP -> P_452 ; _rtB -> B_63_40_0 = _rtP -> P_453
; _rtB -> B_63_41_0 = _rtP -> P_454 ; _rtB -> B_63_42_0_p = _rtP -> P_455 ;
_rtB -> B_63_43_0_c = _rtP -> P_456 ; _rtB -> B_63_44_0_f = _rtP -> P_457 ;
_rtB -> B_63_45_0_g = _rtP -> P_458 ; _rtB -> B_63_46_0_g = _rtP -> P_459 ;
_rtB -> B_63_47_0 = _rtP -> P_460 ; _rtB -> B_63_48_0 = _rtP -> P_461 ; _rtB
-> B_63_49_0 = _rtP -> P_462 ; _rtB -> B_63_50_0 = _rtP -> P_463 ; _rtB ->
B_63_51_0 = _rtP -> P_464 ; _rtB -> B_63_52_0 = _rtP -> P_465 ; _rtB ->
B_63_53_0 = _rtP -> P_466 ; _rtB -> B_63_54_0 = _rtP -> P_467 ; _rtB ->
B_63_55_0 = _rtP -> P_468 ; _rtB -> B_63_56_0 = _rtP -> P_469 ; _rtB ->
B_63_57_0 = _rtP -> P_470 ; _rtB -> B_63_60_0 = ( uint8_T ) ( _rtP -> P_471
== _rtP -> P_472 ) ; _rtB -> B_63_62_0 = ( uint8_T ) ( _rtP -> P_471 == _rtP
-> P_473 ) ; _rtB -> B_63_63_0 = _rtP -> P_474 ; _rtB -> B_63_64_0 = _rtP ->
P_475 ; _rtB -> B_63_65_0 = _rtP -> P_476 ; _rtB -> B_63_66_0_m = _rtP ->
P_477 ; _rtB -> B_63_68_0_n = _rtP -> P_479 * _rtP -> P_478 ; _rtB ->
B_63_69_0_p = _rtP -> P_480 ; _rtB -> B_63_70_0 = _rtP -> P_481 ;
PV_Battery_Grid2021a_AutomaticGainControlTID9 ( S , & _rtB ->
AutomaticGainControl_p , & _rtDW -> AutomaticGainControl_p , & _rtP ->
AutomaticGainControl_p ) ; _rtB -> B_63_72_0 = _rtP -> P_482 ; _rtB ->
B_63_73_0 = _rtP -> P_483 ; _rtB -> B_63_74_0 = _rtP -> P_484 ; _rtB ->
B_63_75_0 = _rtP -> P_485 ; _rtB -> B_63_78_0_l = ( uint8_T ) ( _rtP -> P_486
== _rtP -> P_487 ) ; _rtB -> B_63_80_0_o = ( uint8_T ) ( _rtP -> P_486 ==
_rtP -> P_488 ) ; _rtB -> B_63_83_0 = _rtP -> P_489 + _rtP -> P_490 ; _rtB ->
B_63_84_0 = _rtP -> P_491 ; _rtB -> B_63_85_0 = _rtP -> P_492 ; _rtB ->
B_63_88_0 = ( uint8_T ) ( _rtP -> P_493 == _rtP -> P_494 ) ; _rtB ->
B_63_90_0 = ( uint8_T ) ( _rtP -> P_493 == _rtP -> P_495 ) ; _rtB ->
B_63_94_0 = _rtP -> P_498 ; _rtB -> B_63_91_0 [ 0 ] = _rtP -> P_496 [ 0 ] ;
_rtB -> B_63_95_0 [ 0 ] = _rtP -> P_499 [ 0 ] ; _rtB -> B_63_91_0 [ 1 ] =
_rtP -> P_496 [ 1 ] ; _rtB -> B_63_95_0 [ 1 ] = _rtP -> P_499 [ 1 ] ; _rtB ->
B_63_93_0 = ( _rtB -> B_63_91_0 [ 1 ] - _rtB -> B_63_91_0 [ 0 ] ) * _rtP ->
P_497 ; _rtB -> B_63_97_0 = ( _rtB -> B_63_95_0 [ 1 ] - _rtB -> B_63_95_0 [ 0
] ) * _rtP -> P_500 ; _rtB -> B_63_100_0_l = _rtP -> P_502 * _rtB ->
B_63_69_0_p * _rtP -> P_501 ; _rtB -> B_63_103_0 = ( uint8_T ) ( _rtP ->
P_503 == _rtP -> P_504 ) ; _rtB -> B_63_105_0 = ( uint8_T ) ( _rtP -> P_503
== _rtP -> P_505 ) ; _rtB -> B_63_108_0_j = _rtP -> P_507 * _rtB ->
B_63_69_0_p * _rtP -> P_506 ; _rtB -> B_63_111_0 = ( uint8_T ) ( _rtP ->
P_508 == _rtP -> P_509 ) ; _rtB -> B_63_113_0_b = ( uint8_T ) ( _rtP -> P_508
== _rtP -> P_510 ) ; _rtB -> B_63_116_0 = ( uint8_T ) ( _rtP -> P_511 == _rtP
-> P_512 ) ; _rtB -> B_63_118_0 = ( uint8_T ) ( _rtP -> P_511 == _rtP ->
P_513 ) ; _rtB -> B_63_121_0_n = ( uint8_T ) ( _rtP -> P_514 == _rtP -> P_515
) ; _rtB -> B_63_123_0 = ( uint8_T ) ( _rtP -> P_514 == _rtP -> P_516 ) ;
_rtB -> B_63_124_0 = _rtP -> P_517 ;
PV_Battery_Grid2021a_AutomaticGainControlTID9 ( S , & _rtB ->
AutomaticGainControl , & _rtDW -> AutomaticGainControl , & _rtP ->
AutomaticGainControl ) ; _rtB -> B_63_126_0 = _rtP -> P_518 ; _rtB ->
B_63_127_0_d = _rtP -> P_519 ; _rtB -> B_63_128_0 = _rtP -> P_520 ; _rtB ->
B_63_129_0_g = _rtP -> P_521 ; _rtB -> B_63_132_0 = ( uint8_T ) ( _rtP ->
P_522 == _rtP -> P_523 ) ; _rtB -> B_63_134_0 = ( uint8_T ) ( _rtP -> P_522
== _rtP -> P_524 ) ; _rtB -> B_63_135_0_l = _rtP -> P_525 ; _rtB ->
B_63_136_0 = _rtP -> P_526 ; _rtB -> B_63_137_0 = _rtP -> P_527 ; _rtB ->
B_63_138_0_d = _rtP -> P_528 ; _rtB -> B_63_139_0 = _rtP -> P_529 ; _rtB ->
B_63_140_0_d = _rtP -> P_530 ; UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) {
B_PV_Battery_Grid2021a_T * _rtB ; DW_PV_Battery_Grid2021a_T * _rtDW ;
P_PV_Battery_Grid2021a_T * _rtP ; XDis_PV_Battery_Grid2021a_T * _rtXdis ;
real_T HoldSine ; int32_T i ; _rtDW = ( ( DW_PV_Battery_Grid2021a_T * )
ssGetRootDWork ( S ) ) ; _rtXdis = ( ( XDis_PV_Battery_Grid2021a_T * )
ssGetContStateDisabled ( S ) ) ; _rtP = ( ( P_PV_Battery_Grid2021a_T * )
ssGetModelRtp ( S ) ) ; _rtB = ( ( B_PV_Battery_Grid2021a_T * )
_ssGetModelBlockIO ( S ) ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 )
{ _rtDW -> itinit1_PreviousInput = _rtB -> B_63_34_0 ; } i = ssIsSampleHit (
S , 3 , 0 ) ; if ( i != 0 ) { _rtDW -> Currentfilter_states = ( _rtB ->
B_63_79_0 - _rtP -> P_36 [ 1 ] * _rtDW -> Currentfilter_states ) / _rtP ->
P_36 [ 0 ] ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW ->
itinit_PreviousInput = _rtB -> B_63_99_0 ; } i = ssIsSampleHit ( S , 3 , 0 )
; if ( i != 0 ) { _rtDW -> inti_IC_LOADING = 0U ; _rtDW -> inti_DSTATE +=
_rtP -> P_39 * _rtB -> B_63_79_0 ; if ( _rtDW -> inti_DSTATE >= _rtP -> P_40
) { _rtDW -> inti_DSTATE = _rtP -> P_40 ; } else if ( _rtDW -> inti_DSTATE <=
_rtP -> P_41 ) { _rtDW -> inti_DSTATE = _rtP -> P_41 ; } if ( _rtB ->
B_63_5_0 > 0.0 ) { _rtDW -> inti_PrevResetState = 1 ; } else if ( _rtB ->
B_63_5_0 < 0.0 ) { _rtDW -> inti_PrevResetState = - 1 ; } else if ( _rtB ->
B_63_5_0 == 0.0 ) { _rtDW -> inti_PrevResetState = 0 ; } else { _rtDW ->
inti_PrevResetState = 2 ; } _rtDW -> DiscreteTimeIntegrator_DSTATE += _rtP ->
P_44 * _rtB -> B_63_98_0 ; _rtDW -> Memory2_PreviousInput = _rtB ->
B_63_100_0 ; _rtDW -> UnitDelay_DSTATE = _rtB -> B_63_69_0 ; HoldSine = _rtDW
-> lastSin ; _rtDW -> lastSin = _rtDW -> lastSin * _rtP -> P_56 + _rtDW ->
lastCos * _rtP -> P_55 ; _rtDW -> lastCos = _rtDW -> lastCos * _rtP -> P_56 -
HoldSine * _rtP -> P_55 ; HoldSine = _rtDW -> lastSin_d ; _rtDW -> lastSin_d
= _rtDW -> lastSin_d * _rtP -> P_63 + _rtDW -> lastCos_b * _rtP -> P_62 ;
_rtDW -> lastCos_b = _rtDW -> lastCos_b * _rtP -> P_63 - HoldSine * _rtP ->
P_62 ; HoldSine = _rtDW -> lastSin_b ; _rtDW -> lastSin_b = _rtDW ->
lastSin_b * _rtP -> P_70 + _rtDW -> lastCos_j * _rtP -> P_69 ; _rtDW ->
lastCos_j = _rtDW -> lastCos_j * _rtP -> P_70 - HoldSine * _rtP -> P_69 ;
ssCallAccelRunBlock ( S , 63 , 46 , SS_CALL_MDL_UPDATE ) ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK . TUbufferPtrs [ 0 ]
; real_T simTime = ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK . Head = ( (
_rtDW -> TransportDelay_IWORK . Head < ( _rtDW -> TransportDelay_IWORK .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK . Head + 1 ) : 0 )
; if ( _rtDW -> TransportDelay_IWORK . Head == _rtDW -> TransportDelay_IWORK
. Tail ) { if ( ! PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( &
_rtDW -> TransportDelay_IWORK . CircularBufSize , & _rtDW ->
TransportDelay_IWORK . Tail , & _rtDW -> TransportDelay_IWORK . Head , &
_rtDW -> TransportDelay_IWORK . Last , simTime - _rtP -> P_101 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
uBuffer + _rtDW -> TransportDelay_IWORK . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK . Head ] = _rtB -> B_63_114_0 ; } i = ssIsSampleHit ( S
, 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput = _rtB ->
B_63_121_0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_p . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK_c . Head = ( ( _rtDW -> TransportDelay_IWORK_c
. Head < ( _rtDW -> TransportDelay_IWORK_c . CircularBufSize - 1 ) ) ? (
_rtDW -> TransportDelay_IWORK_c . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_c . Head == _rtDW -> TransportDelay_IWORK_c . Tail ) {
if ( ! PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_c . CircularBufSize , & _rtDW -> TransportDelay_IWORK_c
. Tail , & _rtDW -> TransportDelay_IWORK_c . Head , & _rtDW ->
TransportDelay_IWORK_c . Last , simTime - _rtP -> P_106 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_c . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } (
* uBuffer + _rtDW -> TransportDelay_IWORK_c . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_c . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_c . Head ] = _rtB -> B_63_122_0 ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_a = _rtB ->
B_63_129_0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_l . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK_h . Head = ( ( _rtDW -> TransportDelay_IWORK_h
. Head < ( _rtDW -> TransportDelay_IWORK_h . CircularBufSize - 1 ) ) ? (
_rtDW -> TransportDelay_IWORK_h . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_h . Head == _rtDW -> TransportDelay_IWORK_h . Tail ) {
if ( ! PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_h . CircularBufSize , & _rtDW -> TransportDelay_IWORK_h
. Tail , & _rtDW -> TransportDelay_IWORK_h . Head , & _rtDW ->
TransportDelay_IWORK_h . Last , simTime - _rtP -> P_113 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_h . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } (
* uBuffer + _rtDW -> TransportDelay_IWORK_h . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_h . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_h . Head ] = _rtB -> B_63_135_0 ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_p = _rtB ->
B_63_142_0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_o . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK_f . Head = ( ( _rtDW -> TransportDelay_IWORK_f
. Head < ( _rtDW -> TransportDelay_IWORK_f . CircularBufSize - 1 ) ) ? (
_rtDW -> TransportDelay_IWORK_f . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_f . Head == _rtDW -> TransportDelay_IWORK_f . Tail ) {
if ( ! PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_f . CircularBufSize , & _rtDW -> TransportDelay_IWORK_f
. Tail , & _rtDW -> TransportDelay_IWORK_f . Head , & _rtDW ->
TransportDelay_IWORK_f . Last , simTime - _rtP -> P_118 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_f . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } (
* uBuffer + _rtDW -> TransportDelay_IWORK_f . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_f . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_f . Head ] = _rtB -> B_63_143_0 ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_d = _rtB ->
B_63_150_0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_n . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK_ff . Head = ( ( _rtDW ->
TransportDelay_IWORK_ff . Head < ( _rtDW -> TransportDelay_IWORK_ff .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_ff . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay_IWORK_ff . Head == _rtDW ->
TransportDelay_IWORK_ff . Tail ) { if ( !
PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_ff . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_ff . Tail , & _rtDW -> TransportDelay_IWORK_ff . Head ,
& _rtDW -> TransportDelay_IWORK_ff . Last , simTime - _rtP -> P_125 , uBuffer
, ( boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_ff .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "tdelay memory allocation error" )
; return ; } } ( * uBuffer + _rtDW -> TransportDelay_IWORK_ff .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_ff . Head ] = simTime ; ( *
uBuffer ) [ _rtDW -> TransportDelay_IWORK_ff . Head ] = _rtB -> B_63_156_0 ;
} i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW ->
Memory_PreviousInput_d0 = _rtB -> B_63_163_0 ; } { real_T * * uBuffer = (
real_T * * ) & _rtDW -> TransportDelay_PWORK_m . TUbufferPtrs [ 0 ] ; real_T
simTime = ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK_c2 . Head = ( ( _rtDW
-> TransportDelay_IWORK_c2 . Head < ( _rtDW -> TransportDelay_IWORK_c2 .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_c2 . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay_IWORK_c2 . Head == _rtDW ->
TransportDelay_IWORK_c2 . Tail ) { if ( !
PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_c2 . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_c2 . Tail , & _rtDW -> TransportDelay_IWORK_c2 . Head ,
& _rtDW -> TransportDelay_IWORK_c2 . Last , simTime - _rtP -> P_130 , uBuffer
, ( boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_c2 .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "tdelay memory allocation error" )
; return ; } } ( * uBuffer + _rtDW -> TransportDelay_IWORK_c2 .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_c2 . Head ] = simTime ; ( *
uBuffer ) [ _rtDW -> TransportDelay_IWORK_c2 . Head ] = _rtB -> B_63_164_0 ;
} i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW ->
Memory_PreviousInput_m = _rtB -> B_63_171_0 ; } { real_T * * uBuffer = (
real_T * * ) & _rtDW -> TransportDelay_PWORK_b . TUbufferPtrs [ 0 ] ; real_T
simTime = ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK_g . Head = ( ( _rtDW
-> TransportDelay_IWORK_g . Head < ( _rtDW -> TransportDelay_IWORK_g .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_g . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay_IWORK_g . Head == _rtDW ->
TransportDelay_IWORK_g . Tail ) { if ( !
PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_g . CircularBufSize , & _rtDW -> TransportDelay_IWORK_g
. Tail , & _rtDW -> TransportDelay_IWORK_g . Head , & _rtDW ->
TransportDelay_IWORK_g . Last , simTime - _rtP -> P_137 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_g . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } (
* uBuffer + _rtDW -> TransportDelay_IWORK_g . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_g . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_g . Head ] = _rtB -> B_63_181_0 ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_mf = _rtB ->
B_63_188_0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_nb . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S )
; _rtDW -> TransportDelay_IWORK_ck . Head = ( ( _rtDW ->
TransportDelay_IWORK_ck . Head < ( _rtDW -> TransportDelay_IWORK_ck .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_ck . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay_IWORK_ck . Head == _rtDW ->
TransportDelay_IWORK_ck . Tail ) { if ( !
PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_ck . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_ck . Tail , & _rtDW -> TransportDelay_IWORK_ck . Head ,
& _rtDW -> TransportDelay_IWORK_ck . Last , simTime - _rtP -> P_142 , uBuffer
, ( boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_ck .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "tdelay memory allocation error" )
; return ; } } ( * uBuffer + _rtDW -> TransportDelay_IWORK_ck .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_ck . Head ] = simTime ; ( *
uBuffer ) [ _rtDW -> TransportDelay_IWORK_ck . Head ] = _rtB -> B_63_189_0 ;
} i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW ->
Memory_PreviousInput_c = _rtB -> B_63_196_0 ; } { real_T * * uBuffer = (
real_T * * ) & _rtDW -> TransportDelay_PWORK_f . TUbufferPtrs [ 0 ] ; real_T
simTime = ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK_l . Head = ( ( _rtDW
-> TransportDelay_IWORK_l . Head < ( _rtDW -> TransportDelay_IWORK_l .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_l . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay_IWORK_l . Head == _rtDW ->
TransportDelay_IWORK_l . Tail ) { if ( !
PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_l . CircularBufSize , & _rtDW -> TransportDelay_IWORK_l
. Tail , & _rtDW -> TransportDelay_IWORK_l . Head , & _rtDW ->
TransportDelay_IWORK_l . Last , simTime - _rtP -> P_149 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_l . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } (
* uBuffer + _rtDW -> TransportDelay_IWORK_l . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_l . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_l . Head ] = _rtB -> B_63_202_0 ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_b = _rtB ->
B_63_209_0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_c . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK_p . Head = ( ( _rtDW -> TransportDelay_IWORK_p
. Head < ( _rtDW -> TransportDelay_IWORK_p . CircularBufSize - 1 ) ) ? (
_rtDW -> TransportDelay_IWORK_p . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_p . Head == _rtDW -> TransportDelay_IWORK_p . Tail ) {
if ( ! PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_p . CircularBufSize , & _rtDW -> TransportDelay_IWORK_p
. Tail , & _rtDW -> TransportDelay_IWORK_p . Head , & _rtDW ->
TransportDelay_IWORK_p . Last , simTime - _rtP -> P_154 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_p . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } (
* uBuffer + _rtDW -> TransportDelay_IWORK_p . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_p . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_p . Head ] = _rtB -> B_63_210_0 ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_i = _rtB ->
B_63_217_0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_h . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK_o . Head = ( ( _rtDW -> TransportDelay_IWORK_o
. Head < ( _rtDW -> TransportDelay_IWORK_o . CircularBufSize - 1 ) ) ? (
_rtDW -> TransportDelay_IWORK_o . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_o . Head == _rtDW -> TransportDelay_IWORK_o . Tail ) {
if ( ! PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_o . CircularBufSize , & _rtDW -> TransportDelay_IWORK_o
. Tail , & _rtDW -> TransportDelay_IWORK_o . Head , & _rtDW ->
TransportDelay_IWORK_o . Last , simTime - _rtP -> P_161 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_o . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } (
* uBuffer + _rtDW -> TransportDelay_IWORK_o . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_o . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_o . Head ] = _rtB -> B_63_223_0 ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_j = _rtB ->
B_63_230_0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_d . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK_b . Head = ( ( _rtDW -> TransportDelay_IWORK_b
. Head < ( _rtDW -> TransportDelay_IWORK_b . CircularBufSize - 1 ) ) ? (
_rtDW -> TransportDelay_IWORK_b . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_b . Head == _rtDW -> TransportDelay_IWORK_b . Tail ) {
if ( ! PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_b . CircularBufSize , & _rtDW -> TransportDelay_IWORK_b
. Tail , & _rtDW -> TransportDelay_IWORK_b . Head , & _rtDW ->
TransportDelay_IWORK_b . Last , simTime - _rtP -> P_166 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_b . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } (
* uBuffer + _rtDW -> TransportDelay_IWORK_b . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_b . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_b . Head ] = _rtB -> B_63_231_0 ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_i3 = _rtB ->
B_63_238_0 ; } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { for ( i = 0
; i < 6 ; i ++ ) { _rtDW -> UnitDelay_DSTATE_e [ i ] = _rtB -> B_63_279_0 [ i
] ; } } i = ssIsSampleHit ( S , 7 , 0 ) ; if ( i != 0 ) { _rtDW ->
Memory_PreviousInput_dr = _rtB -> B_63_315_0 ; _rtDW -> Memory1_PreviousInput
= _rtB -> B_63_319_0 ; _rtDW -> Memory2_PreviousInput_m = _rtB -> B_63_327_0
; } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { _rtDW ->
Integrator_DSTATE += _rtP -> P_249 * _rtB -> B_63_386_0 ; _rtDW ->
UnitDelay_DSTATE_o = _rtB -> B_63_391_0 ; _rtDW -> Integrator_DSTATE_p +=
_rtP -> P_257 * _rtB -> B_63_387_0 ; } i = ssIsSampleHit ( S , 4 , 0 ) ; if (
i != 0 ) { _rtDW -> UnitDelay_DSTATE_ev = _rtB -> B_63_393_0 ; } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_e
= _rtB -> B_63_539_0 ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
_rtDW -> UnitDelay_DSTATE_b [ 0 ] = _rtB -> B_63_581_0 [ 0 ] ; _rtDW ->
UnitDelay_DSTATE_b [ 1 ] = _rtB -> B_63_581_0 [ 1 ] ; _rtDW ->
UnitDelay_DSTATE_b [ 2 ] = _rtB -> B_63_581_0 [ 2 ] ; } i = ssIsSampleHit ( S
, 3 , 0 ) ; if ( i != 0 ) { _rtDW -> Integrator_DSTATE_l += _rtP -> P_289 *
_rtB -> B_63_585_0 ; _rtDW -> Integrator_DSTATE_d += _rtP -> P_298 * _rtB ->
B_63_586_0 ; _rtDW -> Integrator_DSTATE_pi += _rtP -> P_301 * _rtB ->
B_63_588_0 ; _rtDW -> Integrator_DSTATE_pe += _rtP -> P_304 * _rtB ->
B_63_587_0 ; } _rtDW -> Integrator_IWORK = 0 ; switch ( _rtDW ->
Integrator_MODE ) { case 3 : if ( _rtB -> B_63_520_0 < 0.0 ) { _rtDW ->
Integrator_MODE = 1 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; case 4 : if ( _rtB -> B_63_520_0 > 0.0 ) { _rtDW -> Integrator_MODE =
2 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; } _rtXdis ->
Integrator_CSTATE_o = ( ( _rtDW -> Integrator_MODE == 3 ) || ( _rtDW ->
Integrator_MODE == 4 ) ) ; PV_Battery_Grid2021a_AutomaticGainControl_Update (
S , & _rtB -> AutomaticGainControl_p , & _rtDW -> AutomaticGainControl_p , &
_rtP -> AutomaticGainControl_p , & ( ( X_PV_Battery_Grid2021a_T * )
ssGetContStates ( S ) ) -> AutomaticGainControl_p ) ; switch ( _rtDW ->
Integrator_MODE_a ) { case 3 : if ( _rtB -> B_63_516_0 < 0.0 ) { _rtDW ->
Integrator_MODE_a = 1 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; case 4 : if ( _rtB -> B_63_516_0 > 0.0 ) { _rtDW -> Integrator_MODE_a
= 2 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; } _rtXdis
-> Integrator_CSTATE_c = ( ( _rtDW -> Integrator_MODE_a == 3 ) || ( _rtDW ->
Integrator_MODE_a == 4 ) ) ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT (
S ) ; _rtDW -> VariableTransportDelay_IWORK . Head = ( ( _rtDW ->
VariableTransportDelay_IWORK . Head < ( _rtDW -> VariableTransportDelay_IWORK
. CircularBufSize - 1 ) ) ? ( _rtDW -> VariableTransportDelay_IWORK . Head +
1 ) : 0 ) ; if ( _rtDW -> VariableTransportDelay_IWORK . Head == _rtDW ->
VariableTransportDelay_IWORK . Tail ) { if ( !
PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
VariableTransportDelay_IWORK . CircularBufSize , & _rtDW ->
VariableTransportDelay_IWORK . Tail , & _rtDW -> VariableTransportDelay_IWORK
. Head , & _rtDW -> VariableTransportDelay_IWORK . Last , simTime - _rtP ->
P_327 , uBuffer , ( boolean_T ) 0 , ( boolean_T ) 1 , & _rtDW ->
VariableTransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * uBuffer + _rtDW ->
VariableTransportDelay_IWORK . CircularBufSize ) [ _rtDW ->
VariableTransportDelay_IWORK . Head ] = simTime ; ( * uBuffer + 2 * _rtDW ->
VariableTransportDelay_IWORK . CircularBufSize ) [ _rtDW ->
VariableTransportDelay_IWORK . Head ] = ( ( X_PV_Battery_Grid2021a_T * )
ssGetContStates ( S ) ) -> VariableTransportDelay_CSTATE ; ( * uBuffer ) [
_rtDW -> VariableTransportDelay_IWORK . Head ] = _rtB -> B_63_509_0 ; } if (
_rtDW -> LastMajorTimeA == ( rtInf ) ) { _rtDW -> LastMajorTimeA =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA = _rtB -> B_63_522_0 ; } else if (
_rtDW -> LastMajorTimeB == ( rtInf ) ) { _rtDW -> LastMajorTimeB =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYB = _rtB -> B_63_522_0 ; } else if (
_rtDW -> LastMajorTimeA < _rtDW -> LastMajorTimeB ) { _rtDW -> LastMajorTimeA
= ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA = _rtB -> B_63_522_0 ; } else {
_rtDW -> LastMajorTimeB = ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYB = _rtB ->
B_63_522_0 ; } i = ssIsSampleHit ( S , 5 , 0 ) ; if ( i != 0 ) { _rtDW ->
UnitDelay_DSTATE_p = _rtB -> B_63_582_0 ; } i = ssIsSampleHit ( S , 6 , 0 ) ;
if ( i != 0 ) { _rtDW -> UnitDelay_DSTATE_j = _rtB -> B_63_589_0 ; } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW ->
Memory_PreviousInput_at = _rtB -> B_63_644_0 ; } _rtDW -> Integrator_IWORK_b
= 0 ; switch ( _rtDW -> Integrator_MODE_at ) { case 3 : if ( _rtB ->
B_63_625_0 < 0.0 ) { _rtDW -> Integrator_MODE_at = 1 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; case 4 : if (
_rtB -> B_63_625_0 > 0.0 ) { _rtDW -> Integrator_MODE_at = 2 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; } _rtXdis ->
Integrator_CSTATE_n = ( ( _rtDW -> Integrator_MODE_at == 3 ) || ( _rtDW ->
Integrator_MODE_at == 4 ) ) ;
PV_Battery_Grid2021a_AutomaticGainControl_Update ( S , & _rtB ->
AutomaticGainControl , & _rtDW -> AutomaticGainControl , & _rtP ->
AutomaticGainControl , & ( ( X_PV_Battery_Grid2021a_T * ) ssGetContStates ( S
) ) -> AutomaticGainControl ) ; switch ( _rtDW -> Integrator_MODE_az ) { case
3 : if ( _rtB -> B_63_621_0 < 0.0 ) { _rtDW -> Integrator_MODE_az = 1 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; case 4 : if (
_rtB -> B_63_621_0 > 0.0 ) { _rtDW -> Integrator_MODE_az = 2 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; } _rtXdis ->
Integrator_CSTATE_f = ( ( _rtDW -> Integrator_MODE_az == 3 ) || ( _rtDW ->
Integrator_MODE_az == 4 ) ) ; { real_T * * uBuffer = ( real_T * * ) & _rtDW
-> VariableTransportDelay_PWORK_m . TUbufferPtrs [ 0 ] ; real_T simTime =
ssGetT ( S ) ; _rtDW -> VariableTransportDelay_IWORK_c . Head = ( ( _rtDW ->
VariableTransportDelay_IWORK_c . Head < ( _rtDW ->
VariableTransportDelay_IWORK_c . CircularBufSize - 1 ) ) ? ( _rtDW ->
VariableTransportDelay_IWORK_c . Head + 1 ) : 0 ) ; if ( _rtDW ->
VariableTransportDelay_IWORK_c . Head == _rtDW ->
VariableTransportDelay_IWORK_c . Tail ) { if ( !
PV_Battery_Grid2021a_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
VariableTransportDelay_IWORK_c . CircularBufSize , & _rtDW ->
VariableTransportDelay_IWORK_c . Tail , & _rtDW ->
VariableTransportDelay_IWORK_c . Head , & _rtDW ->
VariableTransportDelay_IWORK_c . Last , simTime - _rtP -> P_385 , uBuffer , (
boolean_T ) 0 , ( boolean_T ) 1 , & _rtDW -> VariableTransportDelay_IWORK_c .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "vtdelay memory allocation error"
) ; return ; } } ( * uBuffer + _rtDW -> VariableTransportDelay_IWORK_c .
CircularBufSize ) [ _rtDW -> VariableTransportDelay_IWORK_c . Head ] =
simTime ; ( * uBuffer + 2 * _rtDW -> VariableTransportDelay_IWORK_c .
CircularBufSize ) [ _rtDW -> VariableTransportDelay_IWORK_c . Head ] = ( (
X_PV_Battery_Grid2021a_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE_h ; ( * uBuffer ) [ _rtDW ->
VariableTransportDelay_IWORK_c . Head ] = _rtB -> B_63_614_0 ; } if ( _rtDW
-> LastMajorTimeA_l == ( rtInf ) ) { _rtDW -> LastMajorTimeA_l =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA_l = _rtB -> B_63_627_0 ; } else if
( _rtDW -> LastMajorTimeB_c == ( rtInf ) ) { _rtDW -> LastMajorTimeB_c =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYB_l = _rtB -> B_63_627_0 ; } else if
( _rtDW -> LastMajorTimeA_l < _rtDW -> LastMajorTimeB_c ) { _rtDW ->
LastMajorTimeA_l = ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA_l = _rtB ->
B_63_627_0 ; } else { _rtDW -> LastMajorTimeB_c = ssGetTaskTime ( S , 0 ) ;
_rtDW -> PrevYB_l = _rtB -> B_63_627_0 ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdateTID9 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { B_PV_Battery_Grid2021a_T *
_rtB ; DW_PV_Battery_Grid2021a_T * _rtDW ; P_PV_Battery_Grid2021a_T * _rtP ;
XDis_PV_Battery_Grid2021a_T * _rtXdis ; XDot_PV_Battery_Grid2021a_T * _rtXdot
; X_PV_Battery_Grid2021a_T * _rtX ; _rtDW = ( ( DW_PV_Battery_Grid2021a_T * )
ssGetRootDWork ( S ) ) ; _rtXdis = ( ( XDis_PV_Battery_Grid2021a_T * )
ssGetContStateDisabled ( S ) ) ; _rtXdot = ( ( XDot_PV_Battery_Grid2021a_T *
) ssGetdX ( S ) ) ; _rtX = ( ( X_PV_Battery_Grid2021a_T * ) ssGetContStates (
S ) ) ; _rtP = ( ( P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) ; _rtB
= ( ( B_PV_Battery_Grid2021a_T * ) _ssGetModelBlockIO ( S ) ) ; _rtXdot ->
integrator_CSTATE = _rtB -> B_63_340_0 ; _rtXdot -> integrator_CSTATE_a =
_rtB -> B_63_342_0 ; _rtXdot -> integrator_CSTATE_af = _rtB -> B_63_344_0 ;
_rtXdot -> integrator_CSTATE_f = _rtB -> B_63_346_0 ; _rtXdot ->
integrator_CSTATE_e = _rtB -> B_63_348_0 ; _rtXdot -> integrator_CSTATE_p =
_rtB -> B_63_350_0 ; _rtXdot -> integrator_CSTATE_c = _rtB -> B_63_352_0 ;
_rtXdot -> integrator_CSTATE_fr = _rtB -> B_63_354_0 ; _rtXdot ->
integrator_CSTATE_pb = _rtB -> B_63_356_0 ; _rtXdot -> integrator_CSTATE_o =
_rtB -> B_63_358_0 ; _rtXdot -> integrator_CSTATE_oo = _rtB -> B_63_360_0 ;
_rtXdot -> integrator_CSTATE_ec = _rtB -> B_63_362_0 ; _rtXdot ->
Integrator_CSTATE = _rtB -> B_63_500_0 ; _rtXdot -> Integrator_CSTATE_b =
_rtB -> B_63_497_0 ; if ( _rtXdis -> Integrator_CSTATE_o ) { _rtXdot ->
Integrator_CSTATE_o = 0.0 ; } else { _rtXdot -> Integrator_CSTATE_o = _rtB ->
B_63_520_0 ; } PV_Battery_Grid2021a_AutomaticGainControl_Deriv ( S , & _rtB
-> AutomaticGainControl_p , & _rtDW -> AutomaticGainControl_p , & _rtP ->
AutomaticGainControl_p , & _rtXdot -> AutomaticGainControl_p ) ; if ( _rtXdis
-> Integrator_CSTATE_c ) { _rtXdot -> Integrator_CSTATE_c = 0.0 ; } else {
_rtXdot -> Integrator_CSTATE_c = _rtB -> B_63_516_0 ; } { real_T instantDelay
; instantDelay = _rtB -> B_63_541_0 ; if ( instantDelay > _rtP -> P_327 ) {
instantDelay = _rtP -> P_327 ; } if ( instantDelay < 0.0 ) { ( (
XDot_PV_Battery_Grid2021a_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE = 0 ; } else { ( ( XDot_PV_Battery_Grid2021a_T
* ) ssGetdX ( S ) ) -> VariableTransportDelay_CSTATE = 1.0 / instantDelay ; }
} _rtXdot -> integrator_CSTATE_i = _rtB -> B_63_546_0 [ 1 ] ; _rtXdot ->
TransferFcn_CSTATE = 0.0 ; _rtXdot -> TransferFcn_CSTATE += _rtP -> P_333 *
_rtX -> TransferFcn_CSTATE ; _rtXdot -> TransferFcn_CSTATE += _rtB ->
B_63_517_0 ; _rtXdot -> Integrator_x1_CSTATE = _rtB -> B_63_532_0 ; _rtXdot
-> Integrator_x2_CSTATE = _rtB -> B_63_534_0 ; _rtXdot -> Integrator_CSTATE_a
= _rtB -> B_63_576_0 ; _rtXdot -> Integrator_CSTATE_p = _rtB -> B_63_575_0 ;
if ( _rtXdis -> Integrator_CSTATE_n ) { _rtXdot -> Integrator_CSTATE_n = 0.0
; } else { _rtXdot -> Integrator_CSTATE_n = _rtB -> B_63_625_0 ; }
PV_Battery_Grid2021a_AutomaticGainControl_Deriv ( S , & _rtB ->
AutomaticGainControl , & _rtDW -> AutomaticGainControl , & _rtP ->
AutomaticGainControl , & _rtXdot -> AutomaticGainControl ) ; if ( _rtXdis ->
Integrator_CSTATE_f ) { _rtXdot -> Integrator_CSTATE_f = 0.0 ; } else {
_rtXdot -> Integrator_CSTATE_f = _rtB -> B_63_621_0 ; } { real_T instantDelay
; instantDelay = _rtB -> B_63_646_0 ; if ( instantDelay > _rtP -> P_385 ) {
instantDelay = _rtP -> P_385 ; } if ( instantDelay < 0.0 ) { ( (
XDot_PV_Battery_Grid2021a_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE_h = 0 ; } else { ( (
XDot_PV_Battery_Grid2021a_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE_h = 1.0 / instantDelay ; } } _rtXdot ->
integrator_CSTATE_n = _rtB -> B_63_651_0 [ 1 ] ; _rtXdot ->
TransferFcn_CSTATE_k = 0.0 ; _rtXdot -> TransferFcn_CSTATE_k += _rtP -> P_391
* _rtX -> TransferFcn_CSTATE_k ; _rtXdot -> TransferFcn_CSTATE_k += _rtB ->
B_63_622_0 ; _rtXdot -> Integrator_x1_CSTATE_p = _rtB -> B_63_637_0 ; _rtXdot
-> Integrator_x2_CSTATE_k = _rtB -> B_63_639_0 ; }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { B_PV_Battery_Grid2021a_T *
_rtB ; DW_PV_Battery_Grid2021a_T * _rtDW ; P_PV_Battery_Grid2021a_T * _rtP ;
X_PV_Battery_Grid2021a_T * _rtX ; ZCV_PV_Battery_Grid2021a_T * _rtZCSV ;
_rtDW = ( ( DW_PV_Battery_Grid2021a_T * ) ssGetRootDWork ( S ) ) ; _rtZCSV =
( ( ZCV_PV_Battery_Grid2021a_T * ) ssGetSolverZcSignalVector ( S ) ) ; _rtX =
( ( X_PV_Battery_Grid2021a_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_PV_Battery_Grid2021a_T * ) _ssGetModelBlockIO ( S ) ) ; _rtZCSV ->
Step_StepTime_ZC = ssGetT ( S ) - _rtP -> P_48 ; _rtZCSV ->
Switch_SwitchCond_ZC = _rtB -> B_63_337_0 - _rtP -> P_199 ; _rtZCSV ->
RelationalOperator_RelopInput_ZC = _rtB -> B_63_369_0 - _rtB -> B_63_378_0 ;
_rtZCSV -> HitCrossing_Input_ZC = _rtB -> B_63_376_0 - _rtP -> P_262 ; if (
_rtDW -> HitCrossing_MODE == 0 ) { _rtZCSV -> HitCrossing_Output_ZC = ( (
_rtB -> B_63_376_0 - _rtP -> P_262 ) * ( real_T ) _rtB -> B_63_392_0 != 0.0 )
; } else { _rtZCSV -> HitCrossing_Output_ZC = ssGetT ( S ) -
ssGetTimeOfLastOutput ( S ) ; } _rtZCSV -> Step3_StepTime_ZC = ssGetT ( S ) -
_rtP -> P_267 ; _rtZCSV -> Step1_StepTime_ZC = ssGetT ( S ) - _rtP -> P_280 ;
_rtZCSV -> Step2_StepTime_ZC = ssGetT ( S ) - _rtP -> P_283 ; _rtZCSV ->
RelationalOperator2_RelopInput_ZC [ 0 ] = _rtB -> B_63_412_0 [ 0 ] - _rtB ->
B_63_423_0 ; _rtZCSV -> RelationalOperator2_RelopInput_ZC_j [ 0 ] = _rtB ->
B_63_476_0 [ 0 ] - _rtB -> B_63_487_0 ; _rtZCSV ->
RelationalOperator2_RelopInput_ZC [ 1 ] = _rtB -> B_63_412_0 [ 1 ] - _rtB ->
B_63_423_0 ; _rtZCSV -> RelationalOperator2_RelopInput_ZC_j [ 1 ] = _rtB ->
B_63_476_0 [ 1 ] - _rtB -> B_63_487_0 ; _rtZCSV ->
RelationalOperator2_RelopInput_ZC [ 2 ] = _rtB -> B_63_412_0 [ 2 ] - _rtB ->
B_63_423_0 ; _rtZCSV -> RelationalOperator2_RelopInput_ZC_j [ 2 ] = _rtB ->
B_63_476_0 [ 2 ] - _rtB -> B_63_487_0 ; _rtZCSV -> Step4_StepTime_ZC = ssGetT
( S ) - _rtP -> P_314 ; _rtZCSV -> RelationalOperator_RelopInput_ZC_e = _rtX
-> Integrator_CSTATE_o - _rtB -> B_63_73_0 ; switch ( _rtDW ->
Integrator_MODE ) { case 1 : _rtZCSV -> Integrator_IntgUpLimit_ZC = 0.0 ;
_rtZCSV -> Integrator_IntgLoLimit_ZC = _rtP -> P_321 - _rtP -> P_322 ; break
; case 2 : _rtZCSV -> Integrator_IntgUpLimit_ZC = _rtP -> P_322 - _rtP ->
P_321 ; _rtZCSV -> Integrator_IntgLoLimit_ZC = 0.0 ; break ; default :
_rtZCSV -> Integrator_IntgUpLimit_ZC = _rtX -> Integrator_CSTATE_o - _rtP ->
P_321 ; _rtZCSV -> Integrator_IntgLoLimit_ZC = _rtX -> Integrator_CSTATE_o -
_rtP -> P_322 ; break ; } if ( ( _rtDW -> Integrator_MODE == 3 ) || ( _rtDW
-> Integrator_MODE == 4 ) ) { _rtZCSV -> Integrator_LeaveSaturate_ZC = _rtB
-> B_63_520_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC = 0.0 ; }
_rtZCSV -> HitCrossing_HitNoOutput_ZC = _rtB -> B_63_504_0 - _rtP -> P_323 ;
PV_Battery_Grid2021a_AutomaticGainControl_ZC ( S , & _rtB ->
AutomaticGainControl_p , & _rtDW -> AutomaticGainControl_p , & _rtP ->
AutomaticGainControl_p , & _rtZCSV -> AutomaticGainControl_p ) ; switch (
_rtDW -> Integrator_MODE_a ) { case 1 : _rtZCSV ->
Integrator_IntgUpLimit_ZC_n = 0.0 ; _rtZCSV -> Integrator_IntgLoLimit_ZC_n =
_rtP -> P_325 - _rtP -> P_326 ; break ; case 2 : _rtZCSV ->
Integrator_IntgUpLimit_ZC_n = _rtP -> P_326 - _rtP -> P_325 ; _rtZCSV ->
Integrator_IntgLoLimit_ZC_n = 0.0 ; break ; default : _rtZCSV ->
Integrator_IntgUpLimit_ZC_n = _rtX -> Integrator_CSTATE_c - _rtP -> P_325 ;
_rtZCSV -> Integrator_IntgLoLimit_ZC_n = _rtX -> Integrator_CSTATE_c - _rtP
-> P_326 ; break ; } if ( ( _rtDW -> Integrator_MODE_a == 3 ) || ( _rtDW ->
Integrator_MODE_a == 4 ) ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_n = _rtB
-> B_63_516_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_n = 0.0 ; }
_rtZCSV -> RelationalOperator_RelopInput_ZC_m = _rtB -> B_63_510_0 - _rtB ->
B_63_74_0 ; _rtZCSV -> Saturation2_UprLim_ZC = _rtB -> B_63_519_0 - _rtP ->
P_336 ; _rtZCSV -> Saturation2_LwrLim_ZC = _rtB -> B_63_519_0 - _rtP -> P_337
; _rtZCSV -> Qref_StepTime_ZC = ssGetT ( S ) - _rtP -> P_356 ; _rtZCSV ->
Pref_StepTime_ZC = ssGetT ( S ) - _rtP -> P_362 ; _rtZCSV ->
RelationalOperator_RelopInput_ZC_l = _rtX -> Integrator_CSTATE_n - _rtB ->
B_63_127_0_d ; switch ( _rtDW -> Integrator_MODE_at ) { case 1 : _rtZCSV ->
Integrator_IntgUpLimit_ZC_e = 0.0 ; _rtZCSV -> Integrator_IntgLoLimit_ZC_m =
_rtP -> P_379 - _rtP -> P_380 ; break ; case 2 : _rtZCSV ->
Integrator_IntgUpLimit_ZC_e = _rtP -> P_380 - _rtP -> P_379 ; _rtZCSV ->
Integrator_IntgLoLimit_ZC_m = 0.0 ; break ; default : _rtZCSV ->
Integrator_IntgUpLimit_ZC_e = _rtX -> Integrator_CSTATE_n - _rtP -> P_379 ;
_rtZCSV -> Integrator_IntgLoLimit_ZC_m = _rtX -> Integrator_CSTATE_n - _rtP
-> P_380 ; break ; } if ( ( _rtDW -> Integrator_MODE_at == 3 ) || ( _rtDW ->
Integrator_MODE_at == 4 ) ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_nd =
_rtB -> B_63_625_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_nd = 0.0
; } _rtZCSV -> HitCrossing_HitNoOutput_ZC_p = _rtB -> B_63_609_0 - _rtP ->
P_381 ; PV_Battery_Grid2021a_AutomaticGainControl_ZC ( S , & _rtB ->
AutomaticGainControl , & _rtDW -> AutomaticGainControl , & _rtP ->
AutomaticGainControl , & _rtZCSV -> AutomaticGainControl ) ; switch ( _rtDW
-> Integrator_MODE_az ) { case 1 : _rtZCSV -> Integrator_IntgUpLimit_ZC_f =
0.0 ; _rtZCSV -> Integrator_IntgLoLimit_ZC_o = _rtP -> P_383 - _rtP -> P_384
; break ; case 2 : _rtZCSV -> Integrator_IntgUpLimit_ZC_f = _rtP -> P_384 -
_rtP -> P_383 ; _rtZCSV -> Integrator_IntgLoLimit_ZC_o = 0.0 ; break ;
default : _rtZCSV -> Integrator_IntgUpLimit_ZC_f = _rtX ->
Integrator_CSTATE_f - _rtP -> P_383 ; _rtZCSV -> Integrator_IntgLoLimit_ZC_o
= _rtX -> Integrator_CSTATE_f - _rtP -> P_384 ; break ; } if ( ( _rtDW ->
Integrator_MODE_az == 3 ) || ( _rtDW -> Integrator_MODE_az == 4 ) ) { _rtZCSV
-> Integrator_LeaveSaturate_ZC_e = _rtB -> B_63_621_0 ; } else { _rtZCSV ->
Integrator_LeaveSaturate_ZC_e = 0.0 ; } _rtZCSV ->
RelationalOperator_RelopInput_ZC_lw = _rtB -> B_63_615_0 - _rtB -> B_63_128_0
; _rtZCSV -> Saturation2_UprLim_ZC_o = _rtB -> B_63_624_0 - _rtP -> P_394 ;
_rtZCSV -> Saturation2_LwrLim_ZC_h = _rtB -> B_63_624_0 - _rtP -> P_395 ; }
static void mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S , 0 ,
1414914073U ) ; ssSetChecksumVal ( S , 1 , 2230396534U ) ; ssSetChecksumVal (
S , 2 , 1303948731U ) ; ssSetChecksumVal ( S , 3 , 4062312776U ) ; { mxArray
* slVerStructMat = ( NULL ) ; mxArray * slStrMat = mxCreateString (
"simulink" ) ; char slVerChar [ 10 ] ; int status = mexCallMATLAB ( 1 , &
slVerStructMat , 1 , & slStrMat , "ver" ) ; if ( status == 0 ) { mxArray *
slVerMat = mxGetField ( slVerStructMat , 0 , "Version" ) ; if ( slVerMat == (
NULL ) ) { status = 1 ; } else { status = mxGetString ( slVerMat , slVerChar
, 10 ) ; } } mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat )
; if ( ( status == 1 ) || ( strcmp ( slVerChar , "10.6" ) != 0 ) ) { return ;
} } ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if (
ssGetSizeofDWork ( S ) != ( SLSize ) sizeof ( DW_PV_Battery_Grid2021a_T ) ) {
static char msg [ 256 ] ; sprintf ( msg ,
"Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file (%ld vs %lu)." , ( signed long )
ssGetSizeofDWork ( S ) , ( unsigned long ) sizeof ( DW_PV_Battery_Grid2021a_T
) ) ; ssSetErrorStatus ( S , msg ) ; } if ( ssGetSizeofGlobalBlockIO ( S ) !=
( SLSize ) sizeof ( B_PV_Battery_Grid2021a_T ) ) { static char msg [ 256 ] ;
sprintf ( msg , "Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file (%ld vs %lu)." , ( signed long )
ssGetSizeofGlobalBlockIO ( S ) , ( unsigned long ) sizeof (
B_PV_Battery_Grid2021a_T ) ) ; ssSetErrorStatus ( S , msg ) ; } { int
ssSizeofParams ; ssGetSizeofParams ( S , & ssSizeofParams ) ; if (
ssSizeofParams != sizeof ( P_PV_Battery_Grid2021a_T ) ) { static char msg [
256 ] ; sprintf ( msg , "Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file (%d vs %lu)." , ssSizeofParams , (
unsigned long ) sizeof ( P_PV_Battery_Grid2021a_T ) ) ; ssSetErrorStatus ( S
, msg ) ; } } _ssSetModelRtp ( S , ( real_T * ) &
PV_Battery_Grid2021a_rtDefaultP ) ; _ssSetConstBlockIO ( S , &
PV_Battery_Grid2021a_rtInvariant ) ; rt_InitInfAndNaN ( sizeof ( real_T ) ) ;
( ( P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) -> P_1 = rtMinusInf ;
( ( P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) -> P_41 = rtMinusInf ;
( ( P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) -> P_186 = rtInf ; ( (
P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) -> P_321 = rtInf ; ( (
P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) -> P_322 = rtMinusInf ; (
( P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) -> P_325 = rtInf ; ( (
P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) -> P_336 = rtInf ; ( (
P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) -> P_379 = rtInf ; ( (
P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) -> P_380 = rtMinusInf ; (
( P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) -> P_383 = rtInf ; ( (
P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) -> P_394 = rtInf ; ( (
P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) -> AutomaticGainControl_p
. P_7 = rtInf ; ( ( P_PV_Battery_Grid2021a_T * ) ssGetModelRtp ( S ) ) ->
AutomaticGainControl . P_7 = rtInf ; } static void mdlInitializeSampleTimes (
SimStruct * S ) { slAccRegPrmChangeFcn ( S , mdlOutputsTID9 ) ; } static void
mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
