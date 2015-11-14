/*
 * helicopter11_real_time_data.c
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
#include "helicopter11_real_time.h"
#include "helicopter11_real_time_private.h"

/* Block parameters (auto storage) */
P_helicopter11_real_time_T helicopter11_real_time_P = {
  10.0,                                /* Mask Parameter: PacketOutput_MaxMissedTicks
                                        * Referenced by: '<S3>/Packet Output'
                                        */
  10.0,                                /* Mask Parameter: PacketInput_MaxMissedTicks
                                        * Referenced by: '<S3>/Packet Input'
                                        */
  0.0,                                 /* Mask Parameter: PacketOutput_YieldWhenWaiting
                                        * Referenced by: '<S3>/Packet Output'
                                        */
  0.0,                                 /* Mask Parameter: PacketInput_YieldWhenWaiting
                                        * Referenced by: '<S3>/Packet Input'
                                        */
  75.0,                                /* Mask Parameter: CompareToConstant_const
                                        * Referenced by: '<S1>/Constant'
                                        */
  1,                                   /* Mask Parameter: PacketOutput_PacketID
                                        * Referenced by: '<S3>/Packet Output'
                                        */
  1,                                   /* Mask Parameter: PacketInput_PacketID
                                        * Referenced by: '<S3>/Packet Input'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/Kren_zad'
                                        */
  30.0,                                /* Expression: 30
                                        * Referenced by: '<Root>/Saturation2'
                                        */
  -30.0,                               /* Expression: -30
                                        * Referenced by: '<Root>/Saturation2'
                                        */
  1.0,                                 /* Expression: 1
                                        * Referenced by: '<Root>/Gain13'
                                        */
  -1.0,                                /* Expression: -1
                                        * Referenced by: '<Root>/Gain1'
                                        */
  -0.1,                                /* Expression: -0.1
                                        * Referenced by: '<Root>/Gain4'
                                        */
  -1.0,                                /* Expression: -1
                                        * Referenced by: '<Root>/Gain'
                                        */
  -10.0,                               /* Expression: -10
                                        * Referenced by: '<Root>/Gain2'
                                        */
  -1.0,                                /* Expression: -1
                                        * Referenced by: '<Root>/Gain5'
                                        */
  0.5,                                 /* Expression: 0.5
                                        * Referenced by: '<Root>/Gain6'
                                        */
  10.0,                                /* Expression: 10
                                        * Referenced by: '<Root>/Saturation'
                                        */
  -10.0,                               /* Expression: -10
                                        * Referenced by: '<Root>/Saturation'
                                        */
  7.0,                                 /* Expression: 7
                                        * Referenced by: '<Root>/Constant'
                                        */
  18.0,                                /* Expression: 18
                                        * Referenced by: '<Root>/Saturation1'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/Saturation1'
                                        */
  10.0,                                /* Expression: 0.5*20
                                        * Referenced by: '<Root>/Gain12'
                                        */

  /*  Expression: [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
   * Referenced by: '<Root>/Constant4'
   */
  { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
  1.0,                                 /* Expression: 1
                                        * Referenced by: '<S4>/Gain'
                                        */
  2.0,                                 /* Expression: 2
                                        * Referenced by: '<S4>/Gain1'
                                        */
  3.0,                                 /* Expression: 3
                                        * Referenced by: '<S4>/Gain2'
                                        */

  /*  Expression: [0,-0.5,1,0,-0.5,0,0,0]
   * Referenced by: '<Root>/Constant3'
   */
  { 0.0, -0.5, 1.0, 0.0, -0.5, 0.0, 0.0, 0.0 },
  20.0,                                /* Expression: 20
                                        * Referenced by: '<Root>/Gain9'
                                        */
  20.0,                                /* Expression: 20
                                        * Referenced by: '<Root>/Gain10'
                                        */

  /*  Expression: [0,0,0,0.45,0,0]
   * Referenced by: '<Root>/Constant5'
   */
  { 0.0, 0.0, 0.0, 0.45, 0.0, 0.0 },
  20.0,                                /* Expression: 20
                                        * Referenced by: '<Root>/Gain8'
                                        */
  1.0,                                 /* Expression: 1
                                        * Referenced by: '<Root>/Bias'
                                        */
  -5.0,                                /* Expression: -5
                                        * Referenced by: '<Root>/Tang_zad'
                                        */
  -0.1,                                /* Expression: -0.1
                                        * Referenced by: '<Root>/Gain3'
                                        */
  1.0,                                 /* Expression: 1
                                        * Referenced by: '<Root>/Bias2'
                                        */
  179.5,                               /* Expression: 0.5*359
                                        * Referenced by: '<Root>/Gain11'
                                        */
  250.0,                               /* Expression: 0.5*500
                                        * Referenced by: '<Root>/Gain7'
                                        */
  0.005,                               /* Expression: [0.005]
                                        * Referenced by: '<Root>/Discrete Transfer Fcn1'
                                        */

  /*  Expression: [1 -0.995]
   * Referenced by: '<Root>/Discrete Transfer Fcn1'
   */
  { 1.0, -0.995 },
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/Discrete Transfer Fcn1'
                                        */
  0.1,                                 /* Expression: 0.1
                                        * Referenced by: '<Root>/Gain17'
                                        */
  0.1,                                 /* Expression: 0.1
                                        * Referenced by: '<Root>/Gain15'
                                        */
  0.01,                                /* Expression: [0.01]
                                        * Referenced by: '<Root>/Discrete Transfer Fcn'
                                        */

  /*  Expression: [1 -0.99]
   * Referenced by: '<Root>/Discrete Transfer Fcn'
   */
  { 1.0, -0.99 },
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/Discrete Transfer Fcn'
                                        */
  1.0,                                 /* Expression: 1
                                        * Referenced by: '<Root>/Sine Wave1'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/Sine Wave1'
                                        */
  6.2831853071795862,                  /* Expression: 2*pi*1
                                        * Referenced by: '<Root>/Sine Wave1'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/Sine Wave1'
                                        */
  15.0,                                /* Expression: 15
                                        * Referenced by: '<Root>/Gain19'
                                        */
  1.0,                                 /* Expression: 1
                                        * Referenced by: '<Root>/Sine Wave'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/Sine Wave'
                                        */
  0.62831853071795862,                 /* Expression: 2*pi*0.1
                                        * Referenced by: '<Root>/Sine Wave'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/Sine Wave'
                                        */
  15.0,                                /* Expression: 15
                                        * Referenced by: '<Root>/Gain18'
                                        */
  1.0,                                 /* Expression: 1
                                        * Referenced by: '<Root>/Pulse Generator'
                                        */
  500.0,                               /* Computed Parameter: PulseGenerator_Period
                                        * Referenced by: '<Root>/Pulse Generator'
                                        */
  250.0,                               /* Computed Parameter: PulseGenerator_Duty
                                        * Referenced by: '<Root>/Pulse Generator'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/Pulse Generator'
                                        */
  1.852F,                              /* Computed Parameter: Gain2_Gain_f
                                        * Referenced by: '<S3>/Gain2'
                                        */
  0.3048F,                             /* Computed Parameter: Gain1_Gain_d
                                        * Referenced by: '<S3>/Gain1'
                                        */
  0.3048F,                             /* Computed Parameter: Gain_Gain_l
                                        * Referenced by: '<S3>/Gain'
                                        */
  0.0F,                                /* Computed Parameter: Constant1_Value
                                        * Referenced by: '<S3>/Constant1'
                                        */
  0.0F,                                /* Computed Parameter: Constant11_Value
                                        * Referenced by: '<S3>/Constant11'
                                        */
  0.0F,                                /* Computed Parameter: Constant7_Value
                                        * Referenced by: '<S3>/Constant7'
                                        */
  1.852F,                              /* Computed Parameter: Gain14_Gain
                                        * Referenced by: '<Root>/Gain14'
                                        */
  39,                                  /* Computed Parameter: Constant10_Value
                                        * Referenced by: '<S3>/Constant10'
                                        */
  73,                                  /* Computed Parameter: Constant6_Value
                                        * Referenced by: '<S3>/Constant6'
                                        */
  8,                                   /* Computed Parameter: Constant8_Value
                                        * Referenced by: '<S3>/Constant8'
                                        */
  68,                                  /* Computed Parameter: Constant_Value_i
                                        * Referenced by: '<S3>/Constant'
                                        */
  65,                                  /* Computed Parameter: Constant2_Value
                                        * Referenced by: '<S3>/Constant2'
                                        */
  84,                                  /* Computed Parameter: Constant3_Value_e
                                        * Referenced by: '<S3>/Constant3'
                                        */
  65,                                  /* Computed Parameter: Constant4_Value_b
                                        * Referenced by: '<S3>/Constant4'
                                        */
  0,                                   /* Computed Parameter: Constant5_Value_b
                                        * Referenced by: '<S3>/Constant5'
                                        */
  1U,                                  /* Computed Parameter: ManualSwitch3_CurrentSetting
                                        * Referenced by: '<Root>/Manual Switch3'
                                        */
  0U,                                  /* Computed Parameter: ManualSwitch2_CurrentSetting
                                        * Referenced by: '<Root>/Manual Switch2'
                                        */
  1U,                                  /* Computed Parameter: Switch_Threshold
                                        * Referenced by: '<Root>/Switch'
                                        */
  1U                                   /* Computed Parameter: ManualSwitch_CurrentSetting
                                        * Referenced by: '<Root>/Manual Switch'
                                        */
};
