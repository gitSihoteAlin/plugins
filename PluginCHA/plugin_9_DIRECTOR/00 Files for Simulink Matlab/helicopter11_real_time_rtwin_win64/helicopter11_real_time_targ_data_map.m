  function targMap = targDataMap(),

  ;%***********************
  ;% Create Parameter Map *
  ;%***********************
      
    nTotData      = 0; %add to this count as we go
    nTotSects     = 7;
    sectIdxOffset = 0;
    
    ;%
    ;% Define dummy sections & preallocate arrays
    ;%
    dumSection.nData = -1;  
    dumSection.data  = [];
    
    dumData.logicalSrcIdx = -1;
    dumData.dtTransOffset = -1;
    
    ;%
    ;% Init/prealloc paramMap
    ;%
    paramMap.nSections           = nTotSects;
    paramMap.sectIdxOffset       = sectIdxOffset;
      paramMap.sections(nTotSects) = dumSection; %prealloc
    paramMap.nTotData            = -1;
    
    ;%
    ;% Auto data (helicopter11_real_time_P)
    ;%
      section.nData     = 5;
      section.data(5)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_P.PacketOutput_MaxMissedTicks
	  section.data(1).logicalSrcIdx = 2;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_P.PacketInput_MaxMissedTicks
	  section.data(2).logicalSrcIdx = 3;
	  section.data(2).dtTransOffset = 1;
	
	  ;% helicopter11_real_time_P.PacketOutput_YieldWhenWaiting
	  section.data(3).logicalSrcIdx = 4;
	  section.data(3).dtTransOffset = 2;
	
	  ;% helicopter11_real_time_P.PacketInput_YieldWhenWaiting
	  section.data(4).logicalSrcIdx = 5;
	  section.data(4).dtTransOffset = 3;
	
	  ;% helicopter11_real_time_P.CompareToConstant_const
	  section.data(5).logicalSrcIdx = 6;
	  section.data(5).dtTransOffset = 4;
	
      nTotData = nTotData + section.nData;
      paramMap.sections(1) = section;
      clear section
      
      section.nData     = 2;
      section.data(2)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_P.PacketOutput_PacketID
	  section.data(1).logicalSrcIdx = 7;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_P.PacketInput_PacketID
	  section.data(2).logicalSrcIdx = 8;
	  section.data(2).dtTransOffset = 1;
	
      nTotData = nTotData + section.nData;
      paramMap.sections(2) = section;
      clear section
      
      section.nData     = 53;
      section.data(53)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_P.Kren_zad_Value
	  section.data(1).logicalSrcIdx = 9;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_P.Saturation2_UpperSat
	  section.data(2).logicalSrcIdx = 10;
	  section.data(2).dtTransOffset = 1;
	
	  ;% helicopter11_real_time_P.Saturation2_LowerSat
	  section.data(3).logicalSrcIdx = 11;
	  section.data(3).dtTransOffset = 2;
	
	  ;% helicopter11_real_time_P.Gain13_Gain
	  section.data(4).logicalSrcIdx = 12;
	  section.data(4).dtTransOffset = 3;
	
	  ;% helicopter11_real_time_P.Gain1_Gain
	  section.data(5).logicalSrcIdx = 13;
	  section.data(5).dtTransOffset = 4;
	
	  ;% helicopter11_real_time_P.Gain4_Gain
	  section.data(6).logicalSrcIdx = 14;
	  section.data(6).dtTransOffset = 5;
	
	  ;% helicopter11_real_time_P.Gain_Gain
	  section.data(7).logicalSrcIdx = 15;
	  section.data(7).dtTransOffset = 6;
	
	  ;% helicopter11_real_time_P.Gain2_Gain
	  section.data(8).logicalSrcIdx = 16;
	  section.data(8).dtTransOffset = 7;
	
	  ;% helicopter11_real_time_P.Gain5_Gain
	  section.data(9).logicalSrcIdx = 17;
	  section.data(9).dtTransOffset = 8;
	
	  ;% helicopter11_real_time_P.Gain6_Gain
	  section.data(10).logicalSrcIdx = 18;
	  section.data(10).dtTransOffset = 9;
	
	  ;% helicopter11_real_time_P.Saturation_UpperSat
	  section.data(11).logicalSrcIdx = 19;
	  section.data(11).dtTransOffset = 10;
	
	  ;% helicopter11_real_time_P.Saturation_LowerSat
	  section.data(12).logicalSrcIdx = 20;
	  section.data(12).dtTransOffset = 11;
	
	  ;% helicopter11_real_time_P.Constant_Value
	  section.data(13).logicalSrcIdx = 21;
	  section.data(13).dtTransOffset = 12;
	
	  ;% helicopter11_real_time_P.Saturation1_UpperSat
	  section.data(14).logicalSrcIdx = 22;
	  section.data(14).dtTransOffset = 13;
	
	  ;% helicopter11_real_time_P.Saturation1_LowerSat
	  section.data(15).logicalSrcIdx = 23;
	  section.data(15).dtTransOffset = 14;
	
	  ;% helicopter11_real_time_P.Gain12_Gain
	  section.data(16).logicalSrcIdx = 24;
	  section.data(16).dtTransOffset = 15;
	
	  ;% helicopter11_real_time_P.Constant4_Value
	  section.data(17).logicalSrcIdx = 25;
	  section.data(17).dtTransOffset = 16;
	
	  ;% helicopter11_real_time_P.Gain_Gain_j
	  section.data(18).logicalSrcIdx = 26;
	  section.data(18).dtTransOffset = 55;
	
	  ;% helicopter11_real_time_P.Gain1_Gain_c
	  section.data(19).logicalSrcIdx = 27;
	  section.data(19).dtTransOffset = 56;
	
	  ;% helicopter11_real_time_P.Gain2_Gain_e
	  section.data(20).logicalSrcIdx = 28;
	  section.data(20).dtTransOffset = 57;
	
	  ;% helicopter11_real_time_P.Constant3_Value
	  section.data(21).logicalSrcIdx = 29;
	  section.data(21).dtTransOffset = 58;
	
	  ;% helicopter11_real_time_P.Gain9_Gain
	  section.data(22).logicalSrcIdx = 30;
	  section.data(22).dtTransOffset = 66;
	
	  ;% helicopter11_real_time_P.Gain10_Gain
	  section.data(23).logicalSrcIdx = 31;
	  section.data(23).dtTransOffset = 67;
	
	  ;% helicopter11_real_time_P.Constant5_Value
	  section.data(24).logicalSrcIdx = 32;
	  section.data(24).dtTransOffset = 68;
	
	  ;% helicopter11_real_time_P.Gain8_Gain
	  section.data(25).logicalSrcIdx = 33;
	  section.data(25).dtTransOffset = 74;
	
	  ;% helicopter11_real_time_P.Bias_Bias
	  section.data(26).logicalSrcIdx = 34;
	  section.data(26).dtTransOffset = 75;
	
	  ;% helicopter11_real_time_P.Tang_zad_Value
	  section.data(27).logicalSrcIdx = 35;
	  section.data(27).dtTransOffset = 76;
	
	  ;% helicopter11_real_time_P.Gain3_Gain
	  section.data(28).logicalSrcIdx = 36;
	  section.data(28).dtTransOffset = 77;
	
	  ;% helicopter11_real_time_P.Bias2_Bias
	  section.data(29).logicalSrcIdx = 37;
	  section.data(29).dtTransOffset = 78;
	
	  ;% helicopter11_real_time_P.Gain11_Gain
	  section.data(30).logicalSrcIdx = 38;
	  section.data(30).dtTransOffset = 79;
	
	  ;% helicopter11_real_time_P.Gain7_Gain
	  section.data(31).logicalSrcIdx = 39;
	  section.data(31).dtTransOffset = 80;
	
	  ;% helicopter11_real_time_P.DiscreteTransferFcn1_NumCoef
	  section.data(32).logicalSrcIdx = 40;
	  section.data(32).dtTransOffset = 81;
	
	  ;% helicopter11_real_time_P.DiscreteTransferFcn1_DenCoef
	  section.data(33).logicalSrcIdx = 41;
	  section.data(33).dtTransOffset = 82;
	
	  ;% helicopter11_real_time_P.DiscreteTransferFcn1_InitialStates
	  section.data(34).logicalSrcIdx = 42;
	  section.data(34).dtTransOffset = 84;
	
	  ;% helicopter11_real_time_P.Gain17_Gain
	  section.data(35).logicalSrcIdx = 43;
	  section.data(35).dtTransOffset = 85;
	
	  ;% helicopter11_real_time_P.Gain15_Gain
	  section.data(36).logicalSrcIdx = 44;
	  section.data(36).dtTransOffset = 86;
	
	  ;% helicopter11_real_time_P.DiscreteTransferFcn_NumCoef
	  section.data(37).logicalSrcIdx = 45;
	  section.data(37).dtTransOffset = 87;
	
	  ;% helicopter11_real_time_P.DiscreteTransferFcn_DenCoef
	  section.data(38).logicalSrcIdx = 46;
	  section.data(38).dtTransOffset = 88;
	
	  ;% helicopter11_real_time_P.DiscreteTransferFcn_InitialStates
	  section.data(39).logicalSrcIdx = 47;
	  section.data(39).dtTransOffset = 90;
	
	  ;% helicopter11_real_time_P.SineWave1_Amp
	  section.data(40).logicalSrcIdx = 48;
	  section.data(40).dtTransOffset = 91;
	
	  ;% helicopter11_real_time_P.SineWave1_Bias
	  section.data(41).logicalSrcIdx = 49;
	  section.data(41).dtTransOffset = 92;
	
	  ;% helicopter11_real_time_P.SineWave1_Freq
	  section.data(42).logicalSrcIdx = 50;
	  section.data(42).dtTransOffset = 93;
	
	  ;% helicopter11_real_time_P.SineWave1_Phase
	  section.data(43).logicalSrcIdx = 51;
	  section.data(43).dtTransOffset = 94;
	
	  ;% helicopter11_real_time_P.Gain19_Gain
	  section.data(44).logicalSrcIdx = 52;
	  section.data(44).dtTransOffset = 95;
	
	  ;% helicopter11_real_time_P.SineWave_Amp
	  section.data(45).logicalSrcIdx = 53;
	  section.data(45).dtTransOffset = 96;
	
	  ;% helicopter11_real_time_P.SineWave_Bias
	  section.data(46).logicalSrcIdx = 54;
	  section.data(46).dtTransOffset = 97;
	
	  ;% helicopter11_real_time_P.SineWave_Freq
	  section.data(47).logicalSrcIdx = 55;
	  section.data(47).dtTransOffset = 98;
	
	  ;% helicopter11_real_time_P.SineWave_Phase
	  section.data(48).logicalSrcIdx = 56;
	  section.data(48).dtTransOffset = 99;
	
	  ;% helicopter11_real_time_P.Gain18_Gain
	  section.data(49).logicalSrcIdx = 57;
	  section.data(49).dtTransOffset = 100;
	
	  ;% helicopter11_real_time_P.PulseGenerator_Amp
	  section.data(50).logicalSrcIdx = 58;
	  section.data(50).dtTransOffset = 101;
	
	  ;% helicopter11_real_time_P.PulseGenerator_Period
	  section.data(51).logicalSrcIdx = 59;
	  section.data(51).dtTransOffset = 102;
	
	  ;% helicopter11_real_time_P.PulseGenerator_Duty
	  section.data(52).logicalSrcIdx = 60;
	  section.data(52).dtTransOffset = 103;
	
	  ;% helicopter11_real_time_P.PulseGenerator_PhaseDelay
	  section.data(53).logicalSrcIdx = 61;
	  section.data(53).dtTransOffset = 104;
	
      nTotData = nTotData + section.nData;
      paramMap.sections(3) = section;
      clear section
      
      section.nData     = 7;
      section.data(7)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_P.Gain2_Gain_f
	  section.data(1).logicalSrcIdx = 62;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_P.Gain1_Gain_d
	  section.data(2).logicalSrcIdx = 63;
	  section.data(2).dtTransOffset = 1;
	
	  ;% helicopter11_real_time_P.Gain_Gain_l
	  section.data(3).logicalSrcIdx = 64;
	  section.data(3).dtTransOffset = 2;
	
	  ;% helicopter11_real_time_P.Constant1_Value
	  section.data(4).logicalSrcIdx = 65;
	  section.data(4).dtTransOffset = 3;
	
	  ;% helicopter11_real_time_P.Constant11_Value
	  section.data(5).logicalSrcIdx = 66;
	  section.data(5).dtTransOffset = 4;
	
	  ;% helicopter11_real_time_P.Constant7_Value
	  section.data(6).logicalSrcIdx = 67;
	  section.data(6).dtTransOffset = 5;
	
	  ;% helicopter11_real_time_P.Gain14_Gain
	  section.data(7).logicalSrcIdx = 68;
	  section.data(7).dtTransOffset = 6;
	
      nTotData = nTotData + section.nData;
      paramMap.sections(4) = section;
      clear section
      
      section.nData     = 3;
      section.data(3)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_P.Constant10_Value
	  section.data(1).logicalSrcIdx = 69;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_P.Constant6_Value
	  section.data(2).logicalSrcIdx = 70;
	  section.data(2).dtTransOffset = 1;
	
	  ;% helicopter11_real_time_P.Constant8_Value
	  section.data(3).logicalSrcIdx = 71;
	  section.data(3).dtTransOffset = 2;
	
      nTotData = nTotData + section.nData;
      paramMap.sections(5) = section;
      clear section
      
      section.nData     = 5;
      section.data(5)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_P.Constant_Value_i
	  section.data(1).logicalSrcIdx = 72;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_P.Constant2_Value
	  section.data(2).logicalSrcIdx = 73;
	  section.data(2).dtTransOffset = 1;
	
	  ;% helicopter11_real_time_P.Constant3_Value_e
	  section.data(3).logicalSrcIdx = 74;
	  section.data(3).dtTransOffset = 2;
	
	  ;% helicopter11_real_time_P.Constant4_Value_b
	  section.data(4).logicalSrcIdx = 75;
	  section.data(4).dtTransOffset = 3;
	
	  ;% helicopter11_real_time_P.Constant5_Value_b
	  section.data(5).logicalSrcIdx = 76;
	  section.data(5).dtTransOffset = 4;
	
      nTotData = nTotData + section.nData;
      paramMap.sections(6) = section;
      clear section
      
      section.nData     = 4;
      section.data(4)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_P.ManualSwitch3_CurrentSetting
	  section.data(1).logicalSrcIdx = 77;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_P.ManualSwitch2_CurrentSetting
	  section.data(2).logicalSrcIdx = 78;
	  section.data(2).dtTransOffset = 1;
	
	  ;% helicopter11_real_time_P.Switch_Threshold
	  section.data(3).logicalSrcIdx = 79;
	  section.data(3).dtTransOffset = 2;
	
	  ;% helicopter11_real_time_P.ManualSwitch_CurrentSetting
	  section.data(4).logicalSrcIdx = 80;
	  section.data(4).dtTransOffset = 3;
	
      nTotData = nTotData + section.nData;
      paramMap.sections(7) = section;
      clear section
      
    
      ;%
      ;% Non-auto Data (parameter)
      ;%
    

    ;%
    ;% Add final counts to struct.
    ;%
    paramMap.nTotData = nTotData;
    


  ;%**************************
  ;% Create Block Output Map *
  ;%**************************
      
    nTotData      = 0; %add to this count as we go
    nTotSects     = 5;
    sectIdxOffset = 0;
    
    ;%
    ;% Define dummy sections & preallocate arrays
    ;%
    dumSection.nData = -1;  
    dumSection.data  = [];
    
    dumData.logicalSrcIdx = -1;
    dumData.dtTransOffset = -1;
    
    ;%
    ;% Init/prealloc sigMap
    ;%
    sigMap.nSections           = nTotSects;
    sigMap.sectIdxOffset       = sectIdxOffset;
      sigMap.sections(nTotSects) = dumSection; %prealloc
    sigMap.nTotData            = -1;
    
    ;%
    ;% Auto data (helicopter11_real_time_B)
    ;%
      section.nData     = 27;
      section.data(27)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_B.D1
	  section.data(1).logicalSrcIdx = 0;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_B.D10
	  section.data(2).logicalSrcIdx = 1;
	  section.data(2).dtTransOffset = 1;
	
	  ;% helicopter11_real_time_B.D11
	  section.data(3).logicalSrcIdx = 2;
	  section.data(3).dtTransOffset = 2;
	
	  ;% helicopter11_real_time_B.D12
	  section.data(4).logicalSrcIdx = 3;
	  section.data(4).dtTransOffset = 3;
	
	  ;% helicopter11_real_time_B.D13
	  section.data(5).logicalSrcIdx = 4;
	  section.data(5).dtTransOffset = 4;
	
	  ;% helicopter11_real_time_B.D14
	  section.data(6).logicalSrcIdx = 5;
	  section.data(6).dtTransOffset = 5;
	
	  ;% helicopter11_real_time_B.D15
	  section.data(7).logicalSrcIdx = 6;
	  section.data(7).dtTransOffset = 6;
	
	  ;% helicopter11_real_time_B.D16
	  section.data(8).logicalSrcIdx = 7;
	  section.data(8).dtTransOffset = 7;
	
	  ;% helicopter11_real_time_B.D2
	  section.data(9).logicalSrcIdx = 8;
	  section.data(9).dtTransOffset = 8;
	
	  ;% helicopter11_real_time_B.D3
	  section.data(10).logicalSrcIdx = 9;
	  section.data(10).dtTransOffset = 9;
	
	  ;% helicopter11_real_time_B.D4
	  section.data(11).logicalSrcIdx = 10;
	  section.data(11).dtTransOffset = 10;
	
	  ;% helicopter11_real_time_B.D5
	  section.data(12).logicalSrcIdx = 11;
	  section.data(12).dtTransOffset = 11;
	
	  ;% helicopter11_real_time_B.D6
	  section.data(13).logicalSrcIdx = 12;
	  section.data(13).dtTransOffset = 12;
	
	  ;% helicopter11_real_time_B.D7
	  section.data(14).logicalSrcIdx = 13;
	  section.data(14).dtTransOffset = 13;
	
	  ;% helicopter11_real_time_B.D8
	  section.data(15).logicalSrcIdx = 14;
	  section.data(15).dtTransOffset = 14;
	
	  ;% helicopter11_real_time_B.D9
	  section.data(16).logicalSrcIdx = 15;
	  section.data(16).dtTransOffset = 15;
	
	  ;% helicopter11_real_time_B.Gain9
	  section.data(17).logicalSrcIdx = 16;
	  section.data(17).dtTransOffset = 16;
	
	  ;% helicopter11_real_time_B.Gain10
	  section.data(18).logicalSrcIdx = 17;
	  section.data(18).dtTransOffset = 17;
	
	  ;% helicopter11_real_time_B.Gain8
	  section.data(19).logicalSrcIdx = 18;
	  section.data(19).dtTransOffset = 18;
	
	  ;% helicopter11_real_time_B.Gain11
	  section.data(20).logicalSrcIdx = 19;
	  section.data(20).dtTransOffset = 19;
	
	  ;% helicopter11_real_time_B.Switch
	  section.data(21).logicalSrcIdx = 20;
	  section.data(21).dtTransOffset = 20;
	
	  ;% helicopter11_real_time_B.Gain7
	  section.data(22).logicalSrcIdx = 21;
	  section.data(22).dtTransOffset = 21;
	
	  ;% helicopter11_real_time_B.MultiportSwitch
	  section.data(23).logicalSrcIdx = 22;
	  section.data(23).dtTransOffset = 22;
	
	  ;% helicopter11_real_time_B.DiscreteTransferFcn
	  section.data(24).logicalSrcIdx = 23;
	  section.data(24).dtTransOffset = 26;
	
	  ;% helicopter11_real_time_B.Add8
	  section.data(25).logicalSrcIdx = 24;
	  section.data(25).dtTransOffset = 27;
	
	  ;% helicopter11_real_time_B.ManualSwitch
	  section.data(26).logicalSrcIdx = 25;
	  section.data(26).dtTransOffset = 28;
	
	  ;% helicopter11_real_time_B.PulseGenerator
	  section.data(27).logicalSrcIdx = 26;
	  section.data(27).dtTransOffset = 30;
	
      nTotData = nTotData + section.nData;
      sigMap.sections(1) = section;
      clear section
      
      section.nData     = 18;
      section.data(18)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_B.ByteUnpack_o3
	  section.data(1).logicalSrcIdx = 27;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o5
	  section.data(2).logicalSrcIdx = 28;
	  section.data(2).dtTransOffset = 8;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o8
	  section.data(3).logicalSrcIdx = 29;
	  section.data(3).dtTransOffset = 16;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o10
	  section.data(4).logicalSrcIdx = 30;
	  section.data(4).dtTransOffset = 24;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o12
	  section.data(5).logicalSrcIdx = 31;
	  section.data(5).dtTransOffset = 32;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o14
	  section.data(6).logicalSrcIdx = 32;
	  section.data(6).dtTransOffset = 40;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o16
	  section.data(7).logicalSrcIdx = 33;
	  section.data(7).dtTransOffset = 48;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o18
	  section.data(8).logicalSrcIdx = 34;
	  section.data(8).dtTransOffset = 56;
	
	  ;% helicopter11_real_time_B.S1
	  section.data(9).logicalSrcIdx = 35;
	  section.data(9).dtTransOffset = 64;
	
	  ;% helicopter11_real_time_B.S2
	  section.data(10).logicalSrcIdx = 36;
	  section.data(10).dtTransOffset = 65;
	
	  ;% helicopter11_real_time_B.S3
	  section.data(11).logicalSrcIdx = 37;
	  section.data(11).dtTransOffset = 66;
	
	  ;% helicopter11_real_time_B.S4
	  section.data(12).logicalSrcIdx = 38;
	  section.data(12).dtTransOffset = 67;
	
	  ;% helicopter11_real_time_B.S5
	  section.data(13).logicalSrcIdx = 39;
	  section.data(13).dtTransOffset = 68;
	
	  ;% helicopter11_real_time_B.S6
	  section.data(14).logicalSrcIdx = 40;
	  section.data(14).dtTransOffset = 69;
	
	  ;% helicopter11_real_time_B.S7
	  section.data(15).logicalSrcIdx = 41;
	  section.data(15).dtTransOffset = 70;
	
	  ;% helicopter11_real_time_B.S8
	  section.data(16).logicalSrcIdx = 42;
	  section.data(16).dtTransOffset = 71;
	
	  ;% helicopter11_real_time_B.S9
	  section.data(17).logicalSrcIdx = 43;
	  section.data(17).dtTransOffset = 72;
	
	  ;% helicopter11_real_time_B.Gain14
	  section.data(18).logicalSrcIdx = 44;
	  section.data(18).dtTransOffset = 73;
	
      nTotData = nTotData + section.nData;
      sigMap.sections(2) = section;
      clear section
      
      section.nData     = 8;
      section.data(8)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_B.ByteUnpack_o2
	  section.data(1).logicalSrcIdx = 45;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o4
	  section.data(2).logicalSrcIdx = 46;
	  section.data(2).dtTransOffset = 1;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o7
	  section.data(3).logicalSrcIdx = 47;
	  section.data(3).dtTransOffset = 2;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o9
	  section.data(4).logicalSrcIdx = 48;
	  section.data(4).dtTransOffset = 3;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o11
	  section.data(5).logicalSrcIdx = 49;
	  section.data(5).dtTransOffset = 4;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o13
	  section.data(6).logicalSrcIdx = 50;
	  section.data(6).dtTransOffset = 5;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o15
	  section.data(7).logicalSrcIdx = 51;
	  section.data(7).dtTransOffset = 6;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o17
	  section.data(8).logicalSrcIdx = 52;
	  section.data(8).dtTransOffset = 7;
	
      nTotData = nTotData + section.nData;
      sigMap.sections(3) = section;
      clear section
      
      section.nData     = 2;
      section.data(2)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_B.ByteUnpack_o1
	  section.data(1).logicalSrcIdx = 53;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_B.ByteUnpack_o6
	  section.data(2).logicalSrcIdx = 54;
	  section.data(2).dtTransOffset = 113;
	
      nTotData = nTotData + section.nData;
      sigMap.sections(4) = section;
      clear section
      
      section.nData     = 2;
      section.data(2)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_B.PacketInput
	  section.data(1).logicalSrcIdx = 55;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_B.Compare
	  section.data(2).logicalSrcIdx = 56;
	  section.data(2).dtTransOffset = 833;
	
      nTotData = nTotData + section.nData;
      sigMap.sections(5) = section;
      clear section
      
    
      ;%
      ;% Non-auto Data (signal)
      ;%
    

    ;%
    ;% Add final counts to struct.
    ;%
    sigMap.nTotData = nTotData;
    


  ;%*******************
  ;% Create DWork Map *
  ;%*******************
      
    nTotData      = 0; %add to this count as we go
    nTotSects     = 3;
    sectIdxOffset = 5;
    
    ;%
    ;% Define dummy sections & preallocate arrays
    ;%
    dumSection.nData = -1;  
    dumSection.data  = [];
    
    dumData.logicalSrcIdx = -1;
    dumData.dtTransOffset = -1;
    
    ;%
    ;% Init/prealloc dworkMap
    ;%
    dworkMap.nSections           = nTotSects;
    dworkMap.sectIdxOffset       = sectIdxOffset;
      dworkMap.sections(nTotSects) = dumSection; %prealloc
    dworkMap.nTotData            = -1;
    
    ;%
    ;% Auto data (helicopter11_real_time_DW)
    ;%
      section.nData     = 2;
      section.data(2)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_DW.DiscreteTransferFcn1_states
	  section.data(1).logicalSrcIdx = 0;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_DW.DiscreteTransferFcn_states
	  section.data(2).logicalSrcIdx = 1;
	  section.data(2).dtTransOffset = 1;
	
      nTotData = nTotData + section.nData;
      dworkMap.sections(1) = section;
      clear section
      
      section.nData     = 4;
      section.data(4)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_DW.PacketOutput_PWORK
	  section.data(1).logicalSrcIdx = 2;
	  section.data(1).dtTransOffset = 0;
	
	  ;% helicopter11_real_time_DW.PacketInput_PWORK
	  section.data(2).logicalSrcIdx = 3;
	  section.data(2).dtTransOffset = 2;
	
	  ;% helicopter11_real_time_DW.Scope_PWORK.LoggedData
	  section.data(3).logicalSrcIdx = 4;
	  section.data(3).dtTransOffset = 3;
	
	  ;% helicopter11_real_time_DW.Scope1_PWORK.LoggedData
	  section.data(4).logicalSrcIdx = 5;
	  section.data(4).dtTransOffset = 4;
	
      nTotData = nTotData + section.nData;
      dworkMap.sections(2) = section;
      clear section
      
      section.nData     = 1;
      section.data(1)  = dumData; %prealloc
      
	  ;% helicopter11_real_time_DW.clockTickCounter
	  section.data(1).logicalSrcIdx = 6;
	  section.data(1).dtTransOffset = 0;
	
      nTotData = nTotData + section.nData;
      dworkMap.sections(3) = section;
      clear section
      
    
      ;%
      ;% Non-auto Data (dwork)
      ;%
    

    ;%
    ;% Add final counts to struct.
    ;%
    dworkMap.nTotData = nTotData;
    


  ;%
  ;% Add individual maps to base struct.
  ;%

  targMap.paramMap  = paramMap;    
  targMap.signalMap = sigMap;
  targMap.dworkMap  = dworkMap;
  
  ;%
  ;% Add checksums to base struct.
  ;%


  targMap.checksum0 = 3016036482;
  targMap.checksum1 = 4069979656;
  targMap.checksum2 = 4140395925;
  targMap.checksum3 = 2069116719;

