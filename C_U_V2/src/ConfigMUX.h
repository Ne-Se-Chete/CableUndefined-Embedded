#ifndef MUXCONFIG_H
#define MUXCONFIG_H

#include <Arduino.h>
// B1_1 - B1_8
const int csPin1 = PC8; // fix later PC8
const char* xPins1[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins1[8]  = {"B1_1", "B1_2", "B1_3", "B1_4", "B1_5", "B1_6", "B1_7", "B1_8"};

const int csPin2 = PC9; // fix later PC9
const char* xPins2[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins2[8]  = {"B1_1", "B1_2", "B1_3", "B1_4", "B1_5", "B1_6", "B1_7", "B1_8"};

// B1_9 - B1_16
const int csPin3 = PC10;
const char* xPins3[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins3[8]  = {"B1_9", "B1_10", "B1_11", "B1_12", "B1_13", "B1_14", "B1_15", "B1_16"};

const int csPin4 = PC11;
const char* xPins4[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins4[8]  = {"B1_9", "B1_10", "B1_11", "B1_12", "B1_13", "B1_14", "B1_15", "B1_16"};

// B1_16 - B1_24
const int csPin5 = PC12;
const char* xPins5[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins5[8]  = {"B1_17", "B1_18", "B1_19", "B1_20", "B1_21", "B1_22", "B1_23", "B1_24"};

const int csPin6 = PD0;
const char* xPins6[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins6[8]  = {"B1_17", "B1_18", "B1_19", "B1_20", "B1_21", "B1_22", "B1_23", "B1_24"};

// B1_24 - B1_32
const int csPin7 = PD1;
const char* xPins7[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins7[8]  = {"B1_25", "B1_26", "B1_27", "B1_28", "B1_29", "B1_30", "B1_31", "B1_32"};

const int csPin8 = PD2;
const char* xPins8[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins8[8]  = {"B1_25", "B1_26", "B1_27", "B1_28", "B1_29", "B1_30", "B1_31", "B1_32"};

// B1_32 - B1_40
const int csPin9 = PD3;
const char* xPins9[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins9[8]  = {"B1_33", "B1_34", "B1_35", "B1_36", "B1_37", "B1_38", "B1_39", "B1_40"};

const int csPin10 = PD4;
const char* xPins10[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins10[8]  = {"B1_33", "B1_34", "B1_35", "B1_36", "B1_37", "B1_38", "B1_39", "B1_40"};

// B1_40 - B1_48
const int csPin11 = PD5;
const char* xPins11[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins11[8]  = {"B1_41", "B1_42", "B1_43", "B1_44", "B1_45", "B1_46", "B1_47", "B1_48"};

const int csPin12 = PD6;
const char* xPins12[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins12[8]  = {"B1_41", "B1_42", "B1_43", "B1_44", "B1_45", "B1_46", "B1_47", "B1_48"};

// B1_48 - B1_56
const int csPin13 = PD7;
const char* xPins13[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins13[8]  = {"B1_49", "B1_50", "B1_51", "B1_52", "B1_53", "B1_54", "B1_55", "B1_56"};

const int csPin14 = PD10;
const char* xPins14[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins14[8]  = {"B1_49", "B1_50", "B1_51", "B1_52", "B1_53", "B1_54", "B1_55", "B1_56"};

// B1_56 - B1_64
const int csPin15 = PD11;
const char* xPins15[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins15[8]  = {"B1_57", "B1_58", "B1_59", "B1_60", "NULL", "NULL", "NULL", "NULL"};

const int csPin16 = PD12;
const char* xPins16[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins16[8]  = {"B1_57", "B1_58", "B1_59", "B1_60", "NULL", "NULL", "NULL", "NULL"};



// B2_1 - B2_8
const int csPin17 = PD13;
const char* xPins17[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins17[8]  = {"B2_1", "B2_2", "B2_3", "B2_4", "B2_5", "B2_6", "B2_7", "B2_8"};

const int csPin18 = PD14;
const char* xPins18[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins18[8]  = {"B2_1", "B2_2", "B2_3", "B2_4", "B2_5", "B2_6", "B2_7", "B2_8"};

// B2_9 - B2_16
const int csPin19 = PD15;
const char* xPins19[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins19[8]  = {"B2_9", "B2_10", "B2_11", "B2_12", "B2_13", "B2_14", "B2_15", "B2_16"};

const int csPin20 = PE0;
const char* xPins20[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins20[8]  = {"B2_9", "B2_10", "B2_11", "B2_12", "B2_13", "B2_14", "B2_15", "B2_16"};

// B2_16 - B2_24
const int csPin21 = PE1;
const char* xPins21[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins21[8]  = {"B2_17", "B2_18", "B2_19", "B2_20", "B2_21", "B2_22", "B2_23", "B2_24"};

const int csPin22 = PE2;
const char* xPins22[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins22[8]  = {"B2_17", "B2_18", "B2_19", "B2_20", "B2_21", "B2_22", "B2_23", "B2_24"};

// B2_24 - B2_32
const int csPin23 = PE3;
const char* xPins23[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins23[8]  = {"B2_25", "B2_26", "B2_27", "B2_28", "B2_29", "B2_30", "B2_31", "B2_32"};

const int csPin24 = PE4;
const char* xPins24[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins24[8]  = {"B2_25", "B2_26", "B2_27", "B2_28", "B2_29", "B2_30", "B2_31", "B2_32"};

// B2_32 - B2_40
const int csPin25 = PE5;
const char* xPins25[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins25[8]  = {"B2_33", "B2_34", "B2_35", "B2_36", "B2_37", "B2_38", "B2_39", "B2_40"};

const int csPin26 = PE6;
const char* xPins26[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins26[8]  = {"B2_33", "B2_34", "B2_35", "B2_36", "B2_37", "B2_38", "B2_39", "B2_40"};

// B2_40 - B2_48
const int csPin27 = PE7;
const char* xPins27[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins27[8]  = {"B2_41", "B2_42", "B2_43", "B2_44", "B2_45", "B2_46", "B2_47", "B2_48"};

const int csPin28 = PE8;
const char* xPins28[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins28[8]  = {"B2_41", "B2_42", "B2_43", "B2_44", "B2_45", "B2_46", "B2_47", "B2_48"};

// B2_48 - B2_56
const int csPin29 = PE9;
const char* xPins29[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins29[8]  = {"B2_49", "B2_50", "B2_51", "B2_52", "B2_53", "B2_54", "B2_55", "B2_56"};

const int csPin30 = PE10;
const char* xPins30[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins30[8]  = {"B2_49", "B2_50", "B2_51", "B2_52", "B2_53", "B2_54", "B2_55", "B2_56"};

// B2_56 - B2_64
const int csPin31 = PE11;
const char* xPins31[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins31[8]  = {"B2_57", "B2_58", "B2_59", "B2_60", "NULL", "NULL", "NULL", "NULL"};

const int csPin32 = PE12;
const char* xPins32[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins32[8]  = {"B2_57", "B2_58", "B2_59", "B2_60", "NULL", "NULL", "NULL", "NULL"};

// Analyzer
const int csPin33 = PE13;
const char* xPins33[16] = {"MT_17", "MT_18", "MT_19", "MT_20", "MT_21", "MT_22", "MT_23", "MT_24", "MT_25", "MT_26", "MT_27", "MT_28", "MT_29", "MT_30", "MT_31", "MT_32"};
const char* yPins33[8]  = {"PC_0", "PC_1", "PC_2", "PC_3", "PC_4", "PC_5", "PC_6", "PC_7"};

const int csPin34 = PE14;
const char* xPins34[16] = {"MT_1", "MT_2", "MT_3", "MT_4", "MT_5", "MT_6", "MT_7", "MT_8", "MT_9", "MT_10", "MT_11", "MT_12", "MT_13", "MT_14", "MT_15", "MT_16"};
const char* yPins34[8]  = {"PC_0", "PC_1", "PC_2", "PC_3", "PC_4", "PC_5", "PC_6", "PC_7"};


#endif // MUXCONFIG_H
