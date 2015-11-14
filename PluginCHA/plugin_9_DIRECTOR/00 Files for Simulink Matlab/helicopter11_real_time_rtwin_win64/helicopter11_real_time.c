/*
 * helicopter11_real_time.c
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
#include "helicopter11_real_time_dt.h"

/* options for Real-Time Windows Target board 0 */
static double RTWinBoardOptions0[] = {
  2.130706433E+9,
  49000.0,
};

/* list of Real-Time Windows Target timers */
const int RTWinTimerCount = 1;
const double RTWinTimers[2] = {
  0.01, 0.0,
};

/* list of Real-Time Windows Target boards */
const int RTWinBoardCount = 1;
RTWINBOARD RTWinBoards[1] = {
  { "Standard_Devices/UDP_Protocol", 4444U, 2, RTWinBoardOptions0 },
};

/* Block signals (auto storage) */
B_helicopter11_real_time_T helicopter11_real_time_B;

/* Block states (auto storage) */
DW_helicopter11_real_time_T helicopter11_real_time_DW;

/* Real-time model */
RT_MODEL_helicopter11_real_time_T helicopter11_real_time_M_;
RT_MODEL_helicopter11_real_time_T *const helicopter11_real_time_M =
  &helicopter11_real_time_M_;

/* Model output function */
void helicopter11_real_time_output(void)
{
  real_T rtb_Gain3;
  real_T rtb_Bias;
  real_T u0;

  /* S-Function Block: <S3>/Packet Output */

  /* no code required */

  /* S-Function Block: <S3>/Packet Input */
  {
    uint8_T indata[833U];
    int status = RTBIO_DriverIO(0, STREAMINPUT, IOREAD, 833U,
      &helicopter11_real_time_P.PacketInput_PacketID, (double*) indata, NULL);
    if (status & 0x1) {
      RTWin_ANYTYPEPTR indp;
      indp.p_uint8_T = indata;

      {
        int_T i1;
        uint8_T *y0 = &helicopter11_real_time_B.PacketInput[0];
        for (i1=0; i1 < 833; i1++) {
          y0[i1] = *indp.p_uint8_T++;
        }
      }
    }
  }

  /* Unpack: <S3>/Byte Unpack */
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o1[0],
                &helicopter11_real_time_B.PacketInput[0],
                113);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o2,
                &helicopter11_real_time_B.PacketInput[113],
                4);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o3[0],
                &helicopter11_real_time_B.PacketInput[117],
                32);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o4,
                &helicopter11_real_time_B.PacketInput[149],
                4);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o5[0],
                &helicopter11_real_time_B.PacketInput[153],
                32);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o6[0],
                &helicopter11_real_time_B.PacketInput[185],
                432);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o7,
                &helicopter11_real_time_B.PacketInput[617],
                4);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o8[0],
                &helicopter11_real_time_B.PacketInput[621],
                32);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o9,
                &helicopter11_real_time_B.PacketInput[653],
                4);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o10[0],
                &helicopter11_real_time_B.PacketInput[657],
                32);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o11,
                &helicopter11_real_time_B.PacketInput[689],
                4);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o12[0],
                &helicopter11_real_time_B.PacketInput[693],
                32);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o13,
                &helicopter11_real_time_B.PacketInput[725],
                4);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o14[0],
                &helicopter11_real_time_B.PacketInput[729],
                32);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o15,
                &helicopter11_real_time_B.PacketInput[761],
                4);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o16[0],
                &helicopter11_real_time_B.PacketInput[765],
                32);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o17,
                &helicopter11_real_time_B.PacketInput[797],
                4);
  (void) memcpy(&helicopter11_real_time_B.ByteUnpack_o18[0],
                &helicopter11_real_time_B.PacketInput[801],
                32);

  /* DataTypeConversion: '<S3>/D1' incorporates:
   *  Gain: '<S3>/Gain2'
   */
  helicopter11_real_time_B.D1 = helicopter11_real_time_P.Gain2_Gain_f *
    helicopter11_real_time_B.ByteUnpack_o3[3];

  /* DataTypeConversion: '<S3>/D10' */
  helicopter11_real_time_B.D10 = helicopter11_real_time_B.ByteUnpack_o10[1];

  /* DataTypeConversion: '<S3>/D11' */
  helicopter11_real_time_B.D11 = helicopter11_real_time_B.ByteUnpack_o10[2];

  /* DataTypeConversion: '<S3>/D12' incorporates:
   *  Gain: '<S3>/Gain1'
   */
  helicopter11_real_time_B.D12 = helicopter11_real_time_P.Gain1_Gain_d *
    helicopter11_real_time_B.ByteUnpack_o14[2];

  /* DataTypeConversion: '<S3>/D13' incorporates:
   *  Gain: '<S3>/Gain'
   */
  helicopter11_real_time_B.D13 = helicopter11_real_time_P.Gain_Gain_l *
    helicopter11_real_time_B.ByteUnpack_o14[3];

  /* DataTypeConversion: '<S3>/D14' */
  helicopter11_real_time_B.D14 = helicopter11_real_time_B.ByteUnpack_o12[0];

  /* DataTypeConversion: '<S3>/D15' */
  helicopter11_real_time_B.D15 = helicopter11_real_time_B.ByteUnpack_o12[1];

  /* DataTypeConversion: '<S3>/D16' */
  helicopter11_real_time_B.D16 = helicopter11_real_time_B.ByteUnpack_o16[4];

  /* DataTypeConversion: '<S3>/D2' */
  helicopter11_real_time_B.D2 = helicopter11_real_time_B.ByteUnpack_o5[0];

  /* DataTypeConversion: '<S3>/D3' */
  helicopter11_real_time_B.D3 = helicopter11_real_time_B.ByteUnpack_o5[4];

  /* DataTypeConversion: '<S3>/D4' */
  helicopter11_real_time_B.D4 = helicopter11_real_time_B.ByteUnpack_o5[5];

  /* DataTypeConversion: '<S3>/D5' */
  helicopter11_real_time_B.D5 = helicopter11_real_time_B.ByteUnpack_o5[6];

  /* DataTypeConversion: '<S3>/D6' */
  helicopter11_real_time_B.D6 = helicopter11_real_time_B.ByteUnpack_o8[0];

  /* DataTypeConversion: '<S3>/D7' */
  helicopter11_real_time_B.D7 = helicopter11_real_time_B.ByteUnpack_o8[1];

  /* DataTypeConversion: '<S3>/D8' */
  helicopter11_real_time_B.D8 = helicopter11_real_time_B.ByteUnpack_o8[2];

  /* DataTypeConversion: '<S3>/D9' */
  helicopter11_real_time_B.D9 = helicopter11_real_time_B.ByteUnpack_o10[0];

  /* Gain: '<Root>/Gain9' incorporates:
   *  Constant: '<Root>/Constant3'
   */
  helicopter11_real_time_B.Gain9 = helicopter11_real_time_P.Gain9_Gain *
    helicopter11_real_time_P.Constant3_Value[1];

  /* Gain: '<Root>/Gain10' incorporates:
   *  Constant: '<Root>/Constant3'
   */
  helicopter11_real_time_B.Gain10 = helicopter11_real_time_P.Gain10_Gain *
    helicopter11_real_time_P.Constant3_Value[0];

  /* Gain: '<Root>/Gain8' incorporates:
   *  Constant: '<Root>/Constant5'
   */
  helicopter11_real_time_B.Gain8 = helicopter11_real_time_P.Gain8_Gain *
    helicopter11_real_time_P.Constant5_Value[3];

  /* Bias: '<Root>/Bias' incorporates:
   *  Constant: '<Root>/Constant3'
   */
  rtb_Bias = helicopter11_real_time_P.Constant3_Value[2] +
    helicopter11_real_time_P.Bias_Bias;

  /* ManualSwitch: '<Root>/Manual Switch2' incorporates:
   *  Constant: '<Root>/Tang_zad'
   */
  if (helicopter11_real_time_P.ManualSwitch2_CurrentSetting == 1) {
    u0 = helicopter11_real_time_B.Gain9;
  } else {
    u0 = helicopter11_real_time_P.Tang_zad_Value;
  }

  /* End of ManualSwitch: '<Root>/Manual Switch2' */

  /* Gain: '<Root>/Gain3' incorporates:
   *  Sum: '<Root>/Add4'
   */
  rtb_Gain3 = (helicopter11_real_time_B.D9 - u0) *
    helicopter11_real_time_P.Gain3_Gain;

  /* Gain: '<Root>/Gain11' incorporates:
   *  Bias: '<Root>/Bias2'
   *  Constant: '<Root>/Constant3'
   */
  helicopter11_real_time_B.Gain11 = (helicopter11_real_time_P.Constant3_Value[4]
    + helicopter11_real_time_P.Bias2_Bias) *
    helicopter11_real_time_P.Gain11_Gain;

  /* RelationalOperator: '<S1>/Compare' incorporates:
   *  Constant: '<S1>/Constant'
   */
  helicopter11_real_time_B.Compare = (uint8_T)(helicopter11_real_time_B.D1 >
    helicopter11_real_time_P.CompareToConstant_const);

  /* Switch: '<Root>/Switch' incorporates:
   *  Constant: '<Root>/Kren_zad'
   *  Gain: '<Root>/Gain13'
   *  Saturate: '<Root>/Saturation2'
   */
  if (helicopter11_real_time_B.Compare >=
      helicopter11_real_time_P.Switch_Threshold) {
    /* Sum: '<Root>/Add9' */
    u0 = helicopter11_real_time_B.Gain11 - helicopter11_real_time_B.D11;

    /* Saturate: '<Root>/Saturation2' */
    if (u0 > helicopter11_real_time_P.Saturation2_UpperSat) {
      u0 = helicopter11_real_time_P.Saturation2_UpperSat;
    } else {
      if (u0 < helicopter11_real_time_P.Saturation2_LowerSat) {
        u0 = helicopter11_real_time_P.Saturation2_LowerSat;
      }
    }

    helicopter11_real_time_B.Switch = helicopter11_real_time_P.Gain13_Gain * u0;
  } else {
    helicopter11_real_time_B.Switch = helicopter11_real_time_P.Kren_zad_Value;
  }

  /* End of Switch: '<Root>/Switch' */

  /* Gain: '<Root>/Gain7' */
  helicopter11_real_time_B.Gain7 = helicopter11_real_time_P.Gain7_Gain *
    rtb_Bias;

  /* MultiPortSwitch: '<S4>/Multiport Switch' incorporates:
   *  Constant: '<Root>/Constant4'
   *  Gain: '<Root>/Gain'
   *  Gain: '<Root>/Gain1'
   *  Gain: '<Root>/Gain12'
   *  Gain: '<Root>/Gain2'
   *  Gain: '<Root>/Gain4'
   *  Gain: '<S4>/Gain'
   *  Gain: '<S4>/Gain1'
   *  Gain: '<S4>/Gain2'
   *  Sum: '<Root>/Add'
   *  Sum: '<Root>/Add1'
   *  Sum: '<Root>/Add2'
   *  Sum: '<Root>/Add3'
   *  Sum: '<S4>/Add'
   */
  switch ((int32_T)((helicopter11_real_time_P.Gain_Gain_j *
                     helicopter11_real_time_P.Constant4_Value[27] +
                     helicopter11_real_time_P.Gain1_Gain_c *
                     helicopter11_real_time_P.Constant4_Value[28]) +
                    helicopter11_real_time_P.Gain2_Gain_e *
                    helicopter11_real_time_P.Constant4_Value[29])) {
   case 1:
    helicopter11_real_time_B.MultiportSwitch[0] = helicopter11_real_time_B.Gain9;
    helicopter11_real_time_B.MultiportSwitch[1] =
      helicopter11_real_time_B.Gain10;
    helicopter11_real_time_B.MultiportSwitch[2] = helicopter11_real_time_B.Gain8;
    helicopter11_real_time_B.MultiportSwitch[3] =
      helicopter11_real_time_P.Gain12_Gain * rtb_Bias;
    break;

   case 2:
    helicopter11_real_time_B.MultiportSwitch[0] = 0.0;
    helicopter11_real_time_B.MultiportSwitch[1] = 0.0;
    helicopter11_real_time_B.MultiportSwitch[2] = 0.0;
    helicopter11_real_time_B.MultiportSwitch[3] = 0.0;
    break;

   default:
    helicopter11_real_time_B.MultiportSwitch[0] =
      helicopter11_real_time_P.Gain1_Gain * helicopter11_real_time_B.D6 +
      rtb_Gain3;

    /* ManualSwitch: '<Root>/Manual Switch3' incorporates:
     *  Gain: '<Root>/Gain1'
     *  Sum: '<Root>/Add1'
     */
    if (helicopter11_real_time_P.ManualSwitch3_CurrentSetting == 1) {
      u0 = helicopter11_real_time_B.Switch;
    } else {
      u0 = helicopter11_real_time_B.Gain10;
    }

    /* End of ManualSwitch: '<Root>/Manual Switch3' */
    helicopter11_real_time_B.MultiportSwitch[1] = (helicopter11_real_time_B.D10
      - u0) * helicopter11_real_time_P.Gain4_Gain +
      helicopter11_real_time_P.Gain_Gain * helicopter11_real_time_B.D7;
    helicopter11_real_time_B.MultiportSwitch[2] =
      (helicopter11_real_time_P.Gain2_Gain * helicopter11_real_time_B.D8 +
       helicopter11_real_time_B.Gain8) - helicopter11_real_time_B.D15;

    /* Gain: '<Root>/Gain6' incorporates:
     *  Gain: '<Root>/Gain'
     *  Gain: '<Root>/Gain2'
     *  Gain: '<Root>/Gain4'
     *  Sum: '<Root>/Add'
     *  Sum: '<Root>/Add2'
     *  Sum: '<Root>/Add3'
     *  Sum: '<Root>/Add5'
     */
    u0 = (helicopter11_real_time_B.Gain7 - helicopter11_real_time_B.D13) *
      helicopter11_real_time_P.Gain6_Gain;

    /* Saturate: '<Root>/Saturation' */
    if (u0 > helicopter11_real_time_P.Saturation_UpperSat) {
      u0 = helicopter11_real_time_P.Saturation_UpperSat;
    } else {
      if (u0 < helicopter11_real_time_P.Saturation_LowerSat) {
        u0 = helicopter11_real_time_P.Saturation_LowerSat;
      }
    }

    /* Sum: '<Root>/Add6' incorporates:
     *  Constant: '<Root>/Constant'
     *  Gain: '<Root>/Gain5'
     *  Saturate: '<Root>/Saturation'
     *  Sum: '<Root>/Add7'
     */
    u0 = (helicopter11_real_time_P.Gain5_Gain * helicopter11_real_time_B.D16 +
          u0) + helicopter11_real_time_P.Constant_Value;

    /* Saturate: '<Root>/Saturation1' */
    if (u0 > helicopter11_real_time_P.Saturation1_UpperSat) {
      helicopter11_real_time_B.MultiportSwitch[3] =
        helicopter11_real_time_P.Saturation1_UpperSat;
    } else if (u0 < helicopter11_real_time_P.Saturation1_LowerSat) {
      helicopter11_real_time_B.MultiportSwitch[3] =
        helicopter11_real_time_P.Saturation1_LowerSat;
    } else {
      helicopter11_real_time_B.MultiportSwitch[3] = u0;
    }

    /* End of Saturate: '<Root>/Saturation1' */
    break;
  }

  /* End of MultiPortSwitch: '<S4>/Multiport Switch' */

  /* DataTypeConversion: '<S3>/S1' */
  helicopter11_real_time_B.S1 = (real32_T)
    helicopter11_real_time_B.MultiportSwitch[0];

  /* DataTypeConversion: '<S3>/S2' */
  helicopter11_real_time_B.S2 = (real32_T)
    helicopter11_real_time_B.MultiportSwitch[1];

  /* DataTypeConversion: '<S3>/S3' */
  helicopter11_real_time_B.S3 = (real32_T)
    helicopter11_real_time_B.MultiportSwitch[2];

  /* DataTypeConversion: '<S3>/S4' incorporates:
   *  DiscreteTransferFcn: '<Root>/Discrete Transfer Fcn1'
   */
  helicopter11_real_time_B.S4 = (real32_T)
    (helicopter11_real_time_P.DiscreteTransferFcn1_NumCoef *
     helicopter11_real_time_DW.DiscreteTransferFcn1_states);

  /* DataTypeConversion: '<S3>/S5' incorporates:
   *  Gain: '<Root>/Gain17'
   */
  helicopter11_real_time_B.S5 = (real32_T)(helicopter11_real_time_P.Gain17_Gain *
    rtb_Gain3);

  /* DataTypeConversion: '<S3>/S6' incorporates:
   *  Gain: '<Root>/Gain15'
   */
  helicopter11_real_time_B.S6 = (real32_T)(helicopter11_real_time_P.Gain15_Gain *
    helicopter11_real_time_B.Switch);

  /* DiscreteTransferFcn: '<Root>/Discrete Transfer Fcn' */
  helicopter11_real_time_B.DiscreteTransferFcn =
    helicopter11_real_time_P.DiscreteTransferFcn_NumCoef *
    helicopter11_real_time_DW.DiscreteTransferFcn_states;

  /* DataTypeConversion: '<S3>/S7' */
  helicopter11_real_time_B.S7 = (real32_T)
    helicopter11_real_time_B.DiscreteTransferFcn;

  /* DataTypeConversion: '<S3>/S8' incorporates:
   *  Gain: '<Root>/Gain19'
   *  Sin: '<Root>/Sine Wave1'
   */
  helicopter11_real_time_B.S8 = (real32_T)((sin
    (helicopter11_real_time_P.SineWave1_Freq *
     helicopter11_real_time_M->Timing.t[0] +
     helicopter11_real_time_P.SineWave1_Phase) *
    helicopter11_real_time_P.SineWave1_Amp +
    helicopter11_real_time_P.SineWave1_Bias) *
    helicopter11_real_time_P.Gain19_Gain);

  /* DataTypeConversion: '<S3>/S9' incorporates:
   *  Gain: '<Root>/Gain18'
   *  Sin: '<Root>/Sine Wave'
   */
  helicopter11_real_time_B.S9 = (real32_T)((sin
    (helicopter11_real_time_P.SineWave_Freq * helicopter11_real_time_M->
     Timing.t[0] + helicopter11_real_time_P.SineWave_Phase) *
    helicopter11_real_time_P.SineWave_Amp +
    helicopter11_real_time_P.SineWave_Bias) *
    helicopter11_real_time_P.Gain18_Gain);

  /* Sum: '<Root>/Add8' */
  helicopter11_real_time_B.Add8 = helicopter11_real_time_B.D12 -
    helicopter11_real_time_B.D13;

  /* ManualSwitch: '<Root>/Manual Switch' */
  if (helicopter11_real_time_P.ManualSwitch_CurrentSetting == 1) {
    helicopter11_real_time_B.ManualSwitch[0] = 0.0;
    helicopter11_real_time_B.ManualSwitch[1] = 0.0;
  } else {
    helicopter11_real_time_B.ManualSwitch[0] = helicopter11_real_time_B.D12;
    helicopter11_real_time_B.ManualSwitch[1] = helicopter11_real_time_B.Add8;
  }

  /* End of ManualSwitch: '<Root>/Manual Switch' */

  /* Gain: '<Root>/Gain14' */
  helicopter11_real_time_B.Gain14 = helicopter11_real_time_P.Gain14_Gain *
    helicopter11_real_time_B.ByteUnpack_o3[0];

  /* DiscretePulseGenerator: '<Root>/Pulse Generator' */
  helicopter11_real_time_B.PulseGenerator =
    (helicopter11_real_time_DW.clockTickCounter <
     helicopter11_real_time_P.PulseGenerator_Duty) &&
    (helicopter11_real_time_DW.clockTickCounter >= 0) ?
    helicopter11_real_time_P.PulseGenerator_Amp : 0.0;
  if (helicopter11_real_time_DW.clockTickCounter >=
      helicopter11_real_time_P.PulseGenerator_Period - 1.0) {
    helicopter11_real_time_DW.clockTickCounter = 0;
  } else {
    helicopter11_real_time_DW.clockTickCounter++;
  }

  /* End of DiscretePulseGenerator: '<Root>/Pulse Generator' */
}

/* Model update function */
void helicopter11_real_time_update(void)
{
  /* S-Function Block: <S3>/Packet Output */
  {
    uint8_T outdata[113U];
    RTWin_ANYTYPEPTR outdp;
    outdp.p_uint8_T = outdata;

    {
      int8_T pktout = helicopter11_real_time_P.Constant_Value_i;
      *outdp.p_int8_T++ = pktout;
    }

    {
      int8_T pktout = helicopter11_real_time_P.Constant2_Value;
      *outdp.p_int8_T++ = pktout;
    }

    {
      int8_T pktout = helicopter11_real_time_P.Constant3_Value_e;
      *outdp.p_int8_T++ = pktout;
    }

    {
      int8_T pktout = helicopter11_real_time_P.Constant4_Value_b;
      *outdp.p_int8_T++ = pktout;
    }

    {
      int8_T pktout = helicopter11_real_time_P.Constant5_Value_b;
      *outdp.p_int8_T++ = pktout;
    }

    {
      int32_T pktout = helicopter11_real_time_P.Constant8_Value;
      *outdp.p_int32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_B.S1;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_B.S2;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant11_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant11_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant11_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant11_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant11_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant11_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      int32_T pktout = helicopter11_real_time_P.Constant10_Value;
      *outdp.p_int32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_B.S4;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_B.S3;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant1_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant1_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant1_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant1_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant1_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant1_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      int32_T pktout = helicopter11_real_time_P.Constant6_Value;
      *outdp.p_int32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_B.S6;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_B.S5;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_B.S7;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_B.S8;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_B.S9;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant7_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant7_Value;
      *outdp.p_real32_T++ = pktout;
    }

    {
      real32_T pktout = helicopter11_real_time_P.Constant7_Value;
      *outdp.p_real32_T++ = pktout;
    }

    RTBIO_DriverIO(0, STREAMOUTPUT, IOWRITE, 113U,
                   &helicopter11_real_time_P.PacketOutput_PacketID, (double*)
                   outdata, NULL);
  }

  /* Update for DiscreteTransferFcn: '<Root>/Discrete Transfer Fcn1' */
  helicopter11_real_time_DW.DiscreteTransferFcn1_states =
    (helicopter11_real_time_B.MultiportSwitch[3] -
     helicopter11_real_time_P.DiscreteTransferFcn1_DenCoef[1] *
     helicopter11_real_time_DW.DiscreteTransferFcn1_states) /
    helicopter11_real_time_P.DiscreteTransferFcn1_DenCoef[0];

  /* Update for DiscreteTransferFcn: '<Root>/Discrete Transfer Fcn' */
  helicopter11_real_time_DW.DiscreteTransferFcn_states =
    (helicopter11_real_time_B.PulseGenerator -
     helicopter11_real_time_P.DiscreteTransferFcn_DenCoef[1] *
     helicopter11_real_time_DW.DiscreteTransferFcn_states) /
    helicopter11_real_time_P.DiscreteTransferFcn_DenCoef[0];

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++helicopter11_real_time_M->Timing.clockTick0)) {
    ++helicopter11_real_time_M->Timing.clockTickH0;
  }

  helicopter11_real_time_M->Timing.t[0] =
    helicopter11_real_time_M->Timing.clockTick0 *
    helicopter11_real_time_M->Timing.stepSize0 +
    helicopter11_real_time_M->Timing.clockTickH0 *
    helicopter11_real_time_M->Timing.stepSize0 * 4294967296.0;

  {
    /* Update absolute timer for sample time: [0.01s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++helicopter11_real_time_M->Timing.clockTick1)) {
      ++helicopter11_real_time_M->Timing.clockTickH1;
    }

    helicopter11_real_time_M->Timing.t[1] =
      helicopter11_real_time_M->Timing.clockTick1 *
      helicopter11_real_time_M->Timing.stepSize1 +
      helicopter11_real_time_M->Timing.clockTickH1 *
      helicopter11_real_time_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Model initialize function */
void helicopter11_real_time_initialize(void)
{
  /* S-Function Block: <S3>/Packet Output */
  /* no initial value should be set */

  /* Start for DiscretePulseGenerator: '<Root>/Pulse Generator' */
  helicopter11_real_time_DW.clockTickCounter = 0;

  /* InitializeConditions for DiscreteTransferFcn: '<Root>/Discrete Transfer Fcn1' */
  helicopter11_real_time_DW.DiscreteTransferFcn1_states =
    helicopter11_real_time_P.DiscreteTransferFcn1_InitialStates;

  /* InitializeConditions for DiscreteTransferFcn: '<Root>/Discrete Transfer Fcn' */
  helicopter11_real_time_DW.DiscreteTransferFcn_states =
    helicopter11_real_time_P.DiscreteTransferFcn_InitialStates;
}

/* Model terminate function */
void helicopter11_real_time_terminate(void)
{
  /* S-Function Block: <S3>/Packet Output */
  /* no initial value should be set */
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/
void MdlOutputs(int_T tid)
{
  helicopter11_real_time_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  helicopter11_real_time_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  helicopter11_real_time_initialize();
}

void MdlTerminate(void)
{
  helicopter11_real_time_terminate();
}

/* Registration function */
RT_MODEL_helicopter11_real_time_T *helicopter11_real_time(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)helicopter11_real_time_M, 0,
                sizeof(RT_MODEL_helicopter11_real_time_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&helicopter11_real_time_M->solverInfo,
                          &helicopter11_real_time_M->Timing.simTimeStep);
    rtsiSetTPtr(&helicopter11_real_time_M->solverInfo, &rtmGetTPtr
                (helicopter11_real_time_M));
    rtsiSetStepSizePtr(&helicopter11_real_time_M->solverInfo,
                       &helicopter11_real_time_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&helicopter11_real_time_M->solverInfo,
                          (&rtmGetErrorStatus(helicopter11_real_time_M)));
    rtsiSetRTModelPtr(&helicopter11_real_time_M->solverInfo,
                      helicopter11_real_time_M);
  }

  rtsiSetSimTimeStep(&helicopter11_real_time_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&helicopter11_real_time_M->solverInfo,"FixedStepDiscrete");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = helicopter11_real_time_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    helicopter11_real_time_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    helicopter11_real_time_M->Timing.sampleTimes =
      (&helicopter11_real_time_M->Timing.sampleTimesArray[0]);
    helicopter11_real_time_M->Timing.offsetTimes =
      (&helicopter11_real_time_M->Timing.offsetTimesArray[0]);

    /* task periods */
    helicopter11_real_time_M->Timing.sampleTimes[0] = (0.0);
    helicopter11_real_time_M->Timing.sampleTimes[1] = (0.01);

    /* task offsets */
    helicopter11_real_time_M->Timing.offsetTimes[0] = (0.0);
    helicopter11_real_time_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(helicopter11_real_time_M, &helicopter11_real_time_M->Timing.tArray
             [0]);

  {
    int_T *mdlSampleHits = helicopter11_real_time_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    helicopter11_real_time_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(helicopter11_real_time_M, -1);
  helicopter11_real_time_M->Timing.stepSize0 = 0.01;
  helicopter11_real_time_M->Timing.stepSize1 = 0.01;

  /* External mode info */
  helicopter11_real_time_M->Sizes.checksums[0] = (3016036482U);
  helicopter11_real_time_M->Sizes.checksums[1] = (4069979656U);
  helicopter11_real_time_M->Sizes.checksums[2] = (4140395925U);
  helicopter11_real_time_M->Sizes.checksums[3] = (2069116719U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[5];
    helicopter11_real_time_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    systemRan[1] = &rtAlwaysEnabled;
    systemRan[2] = &rtAlwaysEnabled;
    systemRan[3] = &rtAlwaysEnabled;
    systemRan[4] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(helicopter11_real_time_M->extModeInfo,
      &helicopter11_real_time_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(helicopter11_real_time_M->extModeInfo,
                        helicopter11_real_time_M->Sizes.checksums);
    rteiSetTPtr(helicopter11_real_time_M->extModeInfo, rtmGetTPtr
                (helicopter11_real_time_M));
  }

  helicopter11_real_time_M->solverInfoPtr =
    (&helicopter11_real_time_M->solverInfo);
  helicopter11_real_time_M->Timing.stepSize = (0.01);
  rtsiSetFixedStepSize(&helicopter11_real_time_M->solverInfo, 0.01);
  rtsiSetSolverMode(&helicopter11_real_time_M->solverInfo,
                    SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  helicopter11_real_time_M->ModelData.blockIO = ((void *)
    &helicopter11_real_time_B);
  (void) memset(((void *) &helicopter11_real_time_B), 0,
                sizeof(B_helicopter11_real_time_T));

  /* parameters */
  helicopter11_real_time_M->ModelData.defaultParam = ((real_T *)
    &helicopter11_real_time_P);

  /* states (dwork) */
  helicopter11_real_time_M->ModelData.dwork = ((void *)
    &helicopter11_real_time_DW);
  (void) memset((void *)&helicopter11_real_time_DW, 0,
                sizeof(DW_helicopter11_real_time_T));

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    helicopter11_real_time_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 14;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.B = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.P = &rtPTransTable;
  }

  /* Initialize Sizes */
  helicopter11_real_time_M->Sizes.numContStates = (0);/* Number of continuous states */
  helicopter11_real_time_M->Sizes.numY = (0);/* Number of model outputs */
  helicopter11_real_time_M->Sizes.numU = (0);/* Number of model inputs */
  helicopter11_real_time_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  helicopter11_real_time_M->Sizes.numSampTimes = (2);/* Number of sample times */
  helicopter11_real_time_M->Sizes.numBlocks = (132);/* Number of blocks */
  helicopter11_real_time_M->Sizes.numBlockIO = (57);/* Number of block outputs */
  helicopter11_real_time_M->Sizes.numBlockPrms = (131);/* Sum of parameter "widths" */
  return helicopter11_real_time_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
