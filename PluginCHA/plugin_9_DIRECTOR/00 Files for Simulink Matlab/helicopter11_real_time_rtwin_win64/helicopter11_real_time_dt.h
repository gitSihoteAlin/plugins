/*
 * helicopter11_real_time_dt.h
 *
 * Code generation for model "helicopter11_real_time".
 *
 * Model version              : 1.30
 * Simulink Coder version : 8.6 (R2014a) 27-Dec-2013
 * C source code generated on : Wed Oct 15 00:16:58 2014
 *
 * Target selection: rtwin.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Generic->32-bit x86 compatible
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "ext_types.h"

/* data type size table */
static uint_T rtDataTypeSizes[] = {
  sizeof(real_T),
  sizeof(real32_T),
  sizeof(int8_T),
  sizeof(uint8_T),
  sizeof(int16_T),
  sizeof(uint16_T),
  sizeof(int32_T),
  sizeof(uint32_T),
  sizeof(boolean_T),
  sizeof(fcn_call_T),
  sizeof(int_T),
  sizeof(pointer_T),
  sizeof(action_T),
  2*sizeof(uint32_T)
};

/* data type name table */
static const char_T * rtDataTypeNames[] = {
  "real_T",
  "real32_T",
  "int8_T",
  "uint8_T",
  "int16_T",
  "uint16_T",
  "int32_T",
  "uint32_T",
  "boolean_T",
  "fcn_call_T",
  "int_T",
  "pointer_T",
  "action_T",
  "timer_uint32_pair_T"
};

/* data type transitions for block I/O structure */
static DataTypeTransition rtBTransitions[] = {
  { (char_T *)(&helicopter11_real_time_B.D1), 0, 0, 31 },

  { (char_T *)(&helicopter11_real_time_B.ByteUnpack_o3[0]), 1, 0, 74 },

  { (char_T *)(&helicopter11_real_time_B.ByteUnpack_o2), 6, 0, 8 },

  { (char_T *)(&helicopter11_real_time_B.ByteUnpack_o1[0]), 2, 0, 545 },

  { (char_T *)(&helicopter11_real_time_B.PacketInput[0]), 3, 0, 834 }
  ,

  { (char_T *)(&helicopter11_real_time_DW.DiscreteTransferFcn1_states), 0, 0, 2
  },

  { (char_T *)(&helicopter11_real_time_DW.PacketOutput_PWORK[0]), 11, 0, 5 },

  { (char_T *)(&helicopter11_real_time_DW.clockTickCounter), 6, 0, 1 }
};

/* data type transition table for block I/O structure */
static DataTypeTransitionTable rtBTransTable = {
  8U,
  rtBTransitions
};

/* data type transitions for Parameters structure */
static DataTypeTransition rtPTransitions[] = {
  { (char_T *)(&helicopter11_real_time_P.PacketOutput_MaxMissedTicks), 0, 0, 5 },

  { (char_T *)(&helicopter11_real_time_P.PacketOutput_PacketID), 6, 0, 2 },

  { (char_T *)(&helicopter11_real_time_P.Kren_zad_Value), 0, 0, 105 },

  { (char_T *)(&helicopter11_real_time_P.Gain2_Gain_f), 1, 0, 7 },

  { (char_T *)(&helicopter11_real_time_P.Constant10_Value), 6, 0, 3 },

  { (char_T *)(&helicopter11_real_time_P.Constant_Value_i), 2, 0, 5 },

  { (char_T *)(&helicopter11_real_time_P.ManualSwitch3_CurrentSetting), 3, 0, 4
  }
};

/* data type transition table for Parameters structure */
static DataTypeTransitionTable rtPTransTable = {
  7U,
  rtPTransitions
};

/* [EOF] helicopter11_real_time_dt.h */
