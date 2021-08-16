/** @file
  GPIO pad and offset definition.

  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.


**/

#ifndef _BXTP_GPIOTABLE_H_
#define _BXTP_GPIOTABLE_H_


typedef struct {
  CHAR16*        pad_name;
  UINT32         CommAndOffset;

} GPIO_PAD_TABLE;

//
// GPIO pad and offset definition as GPIO HAS
// North community GPIO pad definition
//
STATIC CONST GPIO_PAD_TABLE mGpioPadTable[] = {

//
// GPIO pad and offset definition as GPIO HAS
// North community GPIO pad definition
//
{L"GPIO_0", N_GPIO_0},       //GPIO_0
{L"GPIO_1", N_GPIO_1},       //GPIO_1
{L"GPIO_2", N_GPIO_2},       //GPIO_2
{L"GPIO_3", N_GPIO_3},       //GPIO_3
{L"GPIO_4", N_GPIO_4},       //GPIO_4
{L"GPIO_5", N_GPIO_5},       //GPIO_5
{L"GPIO_6", N_GPIO_6},       //GPIO_6
{L"GPIO_7", N_GPIO_7},       //GPIO_7
{L"GPIO_8", N_GPIO_8},       //GPIO_8
{L"GPIO_9", N_GPIO_9},       //GPIO_9
{L"GPIO_10", N_GPIO_10},     //GPIO_10
{L"GPIO_11", N_GPIO_11},     //GPIO_11
{L"GPIO_12", N_GPIO_12},     //GPIO_12
{L"GPIO_13", N_GPIO_13},     //GPIO_13
{L"GPIO_14", N_GPIO_14},     //GPIO_14
{L"GPIO_15", N_GPIO_15},     //GPIO_15
{L"GPIO_16", N_GPIO_16},     //GPIO_16
{L"GPIO_17", N_GPIO_17},     //GPIO_17
{L"GPIO_18", N_GPIO_18},     //GPIO_18
{L"GPIO_19", N_GPIO_19},     //GPIO_19
{L"GPIO_20", N_GPIO_20},     //GPIO_20
{L"GPIO_21", N_GPIO_21},     //GPIO_21
{L"GPIO_22", N_GPIO_22},     //GPIO_22
{L"GPIO_23", N_GPIO_23},     //GPIO_23
{L"GPIO_24", N_GPIO_24},     //GPIO_24
{L"GPIO_25", N_GPIO_25},     //GPIO_25
{L"GPIO_26", N_GPIO_26},     //GPIO_26
{L"GPIO_27", N_GPIO_27},     //GPIO_27
{L"GPIO_28", N_GPIO_28},     //GPIO_28
{L"GPIO_29", N_GPIO_29},     //GPIO_29
{L"GPIO_30", N_GPIO_30},     //GPIO_30
{L"GPIO_31", N_GPIO_31},     //GPIO_31
{L"GPIO_32", N_GPIO_32},     //GPIO_32
{L"GPIO_33", N_GPIO_33},     //GPIO_33
{L"GPIO_34", N_GPIO_34},     //PWM0
{L"GPIO_35", N_GPIO_35},     //PWM1
{L"GPIO_36", N_GPIO_36},     //PWM2
{L"GPIO_37", N_GPIO_37},     //PWM3
{L"GPIO_38", N_GPIO_38},     //LPSS_UART0_RXD
{L"GPIO_39", N_GPIO_39},     //LPSS_UART0_TXD
{L"GPIO_40", N_GPIO_40},     //LPSS_UART0_RTS_B
{L"GPIO_41", N_GPIO_41},     //LPSS_UART0_CTS_B
{L"GPIO_42", N_GPIO_42},     //LPSS_UART1_RXD
{L"GPIO_43", N_GPIO_43},     //LPSS_UART1_TXD
{L"GPIO_44", N_GPIO_44},     //LPSS_UART1_RTS_B
{L"GPIO_45", N_GPIO_45},     //LPSS_UART1_CTS_B
{L"GPIO_46", N_GPIO_46},     //LPSS_UART2_RXD
{L"GPIO_47", N_GPIO_47},     //LPSS_UART2_TXD
{L"GPIO_48", N_GPIO_48},     //LPSS_UART2_RTS_B
{L"GPIO_49", N_GPIO_49},     //LPSS_UART2_CTS_B
{L"GPIO_62", N_GPIO_62},     //GP_CAMERASB00
{L"GPIO_63", N_GPIO_63},     //GP_CAMERASB01
{L"GPIO_64", N_GPIO_64},     //GP_CAMERASB02
{L"GPIO_65", N_GPIO_65},     //GP_CAMERASB03
{L"GPIO_66", N_GPIO_66},     //GP_CAMERASB04
{L"GPIO_67", N_GPIO_67},     //GP_CAMERASB05
{L"GPIO_68", N_GPIO_68},     //GP_CAMERASB06
{L"GPIO_69", N_GPIO_69},     //GP_CAMERASB07
{L"GPIO_70", N_GPIO_70},     //GP_CAMERASB08
{L"GPIO_71", N_GPIO_71},     //GP_CAMERASB09
{L"GPIO_72", N_GPIO_72},     //GP_CAMERASB10
{L"GPIO_73", N_GPIO_73},     //GP_CAMERASB11
{L"TCK", N_TCK},             //TCK
{L"TRST_B", N_TRST_B},       //TRST_B
{L"TMS", N_TMS},             //TMS
{L"TDI", N_TDI},                  //TDI
{L"CX_PMODE", N_CX_PMODE},        //CX_PMODE
{L"CX_PREQ_B", N_CX_PREQ_B},      //CX_PREQ_B
{L"JTAGX", N_JTAGX},              //JTAGX
{L"CXPRDY_B", N_CX_PRDY_B},       //CXPRDY_B
{L"TDO", N_TDO},                  //TDO
{L"GPIO_216", N_CNV_BRI_DT},      //CNV_BRI_DT
{L"GPIO_217", N_CNV_BRI_RSP},     //CNV_BRI_RSP
{L"GPIO_218", N_CNV_RGI_DT},      //CNV_RGI_DT
{L"GPIO_219", N_CNV_RGI_RSP},     //CNV_RGI_RSP
{L"SVID0_ALERT_B", N_SVID0_ALERT_B},  //SVID0_ALERT_B
{L"SVID0_DATA", N_SVID0_DATA},        //SVID0_DATA
{L"SVID0_CLK", N_SVID0_CLK},          //SVID0_CLK

//
// Northwest community GPIO pad definition
//
{L"GPIO_187", NW_GPIO_187},    //HV_DDI0_DDC_SDA
{L"GPIO_188", NW_GPIO_188},    //HV_DDI0_DDC_SCL
{L"GPIO_189", NW_GPIO_189},    //HV_DDI1_DDC_SDA
{L"GPIO_190", NW_GPIO_190},    //HV_DDI1_DDC_SCL
{L"GPIO_191", NW_GPIO_191},    //DBI_SDA
{L"GPIO_192", NW_GPIO_192},    //DBI_SCL
{L"GPIO_193", NW_GPIO_193},    //PANEL0_VDDEN
{L"GPIO_194", NW_GPIO_194},    //PANEL0_BKLTEN
{L"GPIO_195", NW_GPIO_195},    //PANEL0_BKLTCTL
{L"GPIO_196", NW_GPIO_196},    //PANEL1_VDDEN
{L"GPIO_197", NW_GPIO_197},    //PANEL1_BKLTEN
{L"GPIO_198", NW_GPIO_198},    //PANEL1_BKLTCTL
{L"GPIO_199", NW_GPIO_199},    //DBI_CSX
{L"GPIO_200", NW_GPIO_200},    //DBI_RESX
{L"GPIO_201", NW_GPIO_201},    //GP_INTD_DSI_TE1
{L"GPIO_202", NW_GPIO_202},    //GP_INTD_DSI_TE2
{L"GPIO_203", NW_GPIO_203},    //USB_OC0_B
{L"GPIO_204", NW_GPIO_204},    //USB_OC1_B
{L"PMC_SPI_FS0", NW_PMC_SPI_FS0},      //PMC_SPI_FS0
{L"PMC_SPI_FS1", NW_PMC_SPI_FS1},      //PMC_SPI_FS1
{L"PMC_SPI_FS2", NW_PMC_SPI_FS2},      //PMC_SPI_FS2
{L"PMC_SPI_RXD", NW_PMC_SPI_RXD},      //PMC_SPI_RXD
{L"PMC_SPI_TXD", NW_PMC_SPI_TXD},      //PMC_SPI_TXD
{L"PMC_SPI_CLK", NW_PMC_SPI_CLK},      //PMC_SPI_CLK
{L"PMIC_PWRGOOD", NW_PMIC_PWRGOOD},    //PMIC_PWRGOOD
{L"PMIC_RESET_B", NW_PMIC_RESET_B},    //PMIC_RESET_B
{L"GPIO_213", NW_GPIO_213},    //PMIC_SDWN_B
{L"GPIO_214", NW_GPIO_214},    //PMIC_BCUDISW2
{L"GPIO_215", NW_GPIO_215},    //PMIC_BCUDISCRIT
{L"PMIC_THERMTRIP_B", NW_PMIC_THERMTRIP_B},    //PMIC_THERMTRIP_B
{L"PMIC_STDBY", NW_PMIC_STDBY},    //PMIC_STDBY
{L"PROCHOT_B", NW_PROCHOT_B},    //PROCHOT_B
{L"PMIC_I2C_SCL", NW_PMIC_I2C_SCL},    //PMIC_I2C_SCL
{L"PMIC_I2C_SDA", NW_PMIC_I2C_SDA},    //PMIC_I2C_SDA
{L"GPIO_74", NW_GPIO_74},    //AVS_I2S1_MCLK
{L"GPIO_75", NW_GPIO_75},    //AVS_I2S1_BCLK
{L"GPIO_76", NW_GPIO_76},    //AVS_I2S1_WS_SYNC
{L"GPIO_77", NW_GPIO_77},    //AVS_I2S1_SDI
{L"GPIO_78", NW_GPIO_78},    //AVS_I2S1_SDO
{L"GPIO_79", NW_GPIO_79},    //AVS_M_CLK_A1
{L"GPIO_80", NW_GPIO_80},    //AVS_M_CLK_B1
{L"GPIO_81", NW_GPIO_81},    //AVS_M_DATA_1
{L"GPIO_82", NW_GPIO_82},    //AVS_M_CLK_AB2
{L"GPIO_83", NW_GPIO_83},    //AVS_M_DATA_2
{L"GPIO_84", NW_GPIO_84},    //AVS_I2S2_MCLK
{L"GPIO_85", NW_GPIO_85},    //AVS_I2S2_BCLK
{L"GPIO_86", NW_GPIO_86},    //AVS_I2S2_WS_SYNC
{L"GPIO_87", NW_GPIO_87},    //AVS_I2S2_SDI
{L"GPIO_88", NW_GPIO_88},    //AVS_I2S2_SDO
{L"GPIO_89", NW_GPIO_89},    //AVS_I2S3_BCLK
{L"GPIO_90", NW_GPIO_90},    //AVS_I2S3_WS_SYNC
{L"GPIO_91", NW_GPIO_91},    //AVS_I2S3_SDI
{L"GPIO_92", NW_GPIO_92},    //AVS_I2S3_SDO
{L"GPIO_97", NW_GPIO_97},    //FST_SPI_CS0_B
{L"GPIO_98", NW_GPIO_98},    //FST_SPI_CS1_B
{L"GPIO_99", NW_GPIO_99},    //FST_SPI_MOSI_IO0
{L"GPIO_100", NW_GPIO_100},    //FST_SPI_MISO_IO1
{L"GPIO_101", NW_GPIO_101},    //FST_SPI_IO2
{L"GPIO_102", NW_GPIO_102},    //FST_SPI_IO3
{L"GPIO_103", NW_GPIO_103},    //FST_SPI_CLK
{L"FST_SPI_CLK_FB", NW_FST_SPI_CLK_FB},    //FST_SPI_CLK_FB
{L"GPIO_104", NW_GPIO_104},    //GP_SSP_0_CLK
{L"GPIO_105", NW_GPIO_105},    //GP_SSP_0_FS0
{L"GPIO_106", NW_GPIO_106},    //GP_SSP_0_FS1
{L"GPIO_109", NW_GPIO_109},    //GP_SSP_0_RXD
{L"GPIO_110", NW_GPIO_110},    //GP_SSP_0_TXD
{L"GPIO_111", NW_GPIO_111},    //GP_SSP_1_CLK
{L"GPIO_112", NW_GPIO_112},    //GP_SSP_1_FS0
{L"GPIO_113", NW_GPIO_113},    //GP_SSP_1_FS1
{L"GPIO_116", NW_GPIO_116},    //GP_SSP_1_RXD
{L"GPIO_117", NW_GPIO_117},    //GP_SSP_1_TXD
{L"GPIO_118", NW_GPIO_118},    //GP_SSP_2_CLK
{L"GPIO_119", NW_GPIO_119},    //GP_SSP_2_FS0
{L"GPIO_120", NW_GPIO_120},    //GP_SSP_2_FS1
{L"GPIO_121", NW_GPIO_121},    //GP_SSP_2_FS2
{L"GPIO_122", NW_GPIO_122},    //GP_SSP_2_RXD
{L"GPIO_123", NW_GPIO_123},    //GP_SSP_2_TXD

//
// West community GPIO pad definition
//
{L"GPIO_124", W_GPIO_124},     //LPSS_I2S0_SDA
{L"GPIO_125", W_GPIO_125},     //LPSS_I2S0_SCL
{L"GPIO_126", W_GPIO_126},     //LPSS_I2S1_SDA
{L"GPIO_127", W_GPIO_127},     //LPSS_I2S1_SCL
{L"GPIO_128", W_GPIO_128},     //LPSS_I2S2_SDA
{L"GPIO_129", W_GPIO_129},     //LPSS_I2S2_SCL
{L"GPIO_130", W_GPIO_130},     //LPSS_I2S3_SDA
{L"GPIO_131", W_GPIO_131},     //LPSS_I2S3_SCL
{L"GPIO_132", W_GPIO_132},     //LPSS_I2S4_SDA
{L"GPIO_133", W_GPIO_133},     //LPSS_I2S4_SCL
{L"GPIO_134", W_GPIO_134},     //LPSS_I2S5_SDA
{L"GPIO_135", W_GPIO_135},     //LPSS_I2S5_SCL
{L"GPIO_136", W_GPIO_136},     //LPSS_I2S6_SDA
{L"GPIO_137", W_GPIO_137},     //LPSS_I2S6_SCL
{L"GPIO_138", W_GPIO_138},     //LPSS_I2S7_SDA
{L"GPIO_139", W_GPIO_139},     //LPSS_I2S7_SCL
{L"GPIO_146", W_GPIO_146},     //ISH_GPIO_0
{L"GPIO_147", W_GPIO_147},     //ISH_GPIO_1
{L"GPIO_148", W_GPIO_148},     //ISH_GPIO_2
{L"GPIO_149", W_GPIO_149},     //ISH_GPIO_3
{L"GPIO_150", W_GPIO_150},     //ISH_GPIO_4
{L"GPIO_151", W_GPIO_151},     //ISH_GPIO_5
{L"GPIO_152", W_GPIO_152},     //ISH_GPIO_6
{L"GPIO_153", W_GPIO_153},     //ISH_GPIO_7
{L"GPIO_154", W_GPIO_154},     //ISH_GPIO_8
{L"GPIO_155", W_GPIO_155},     //ISH_GPIO_9
{L"GPIO_209", W_GPIO_209},     //PCIE_CLKREQ0_B
{L"GPIO_210", W_GPIO_210},     //PCIE_CLKREQ1_B
{L"GPIO_211", W_GPIO_211},     //PCIE_CLKREQ2_B
{L"GPIO_212", W_GPIO_212},     //PCIE_CLKREQ3_B
{L"OSC_C{LK_OUT0", W_OSC_CLK_OUT_0},     //OSC_CLK_OUT0
{L"OSC_C{LK_OUT1", W_OSC_CLK_OUT_1},     //OSC_CLK_OUT1
{L"OSC_C{LK_OUT2", W_OSC_CLK_OUT_2},     //OSC_CLK_OUT2
{L"OSC_C{LK_OUT3", W_OSC_CLK_OUT_3},     //OSC_CLK_OUT3
{L"OSC_C{LK_OUT4", W_OSC_CLK_OUT_4},     //OSC_CLK_OUT4
{L"PMU_AC_PRESENT", W_PMU_AC_PRESENT},   //PMU_AC_PRESENT
{L"PMU_BAT{LOW_B", W_PMU_BATLOW_B},      //PMU_BATLOW_B
{L"PMU_PLTRST_B", W_PMU_PLTRST_B},       //PMU_PLTRST_B
{L"PMU_PWRBTN_B", W_PMU_PWRBTN_B},       //PMU_PWRBTN_B
{L"PMU_RESETBUTTON_B", W_PMU_RESETBUTTON_B},     //PMU_RESETBUTTON_B
{L"PMU_SLP_S0_B", W_PMU_SLP_S0_B},     //PMU_SLP_S0_B
{L"PMU_SLP_S3_B", W_PMU_SLP_S3_B},     //PMU_SLP_S3_B
{L"PMU_SLP_S4_B", W_PMU_SLP_S4_B},     //PMU_SLP_S4_B
{L"PMU_SUSCLK", W_PMU_SUSCLK},     //PMU_SUSCLK
{L"PMU_WAKE_B", W_PMU_WAKE_B},     //PMU_WAKE_B
{L"SUS_STAT_B", W_SUS_STAT_B},     //SUS_STAT_B
{L"SUSPWRDNACK", W_SUSPWRDNACK},     //SUSPWRDNACK

//
// Southwest community GPIO pad definition
//
{L"GPIO_205", SW_GPIO_205},    //PCIE_WAKE0_B
{L"GPIO_206", SW_GPIO_206},    //PCIE_WAKE1_B
{L"GPIO_207", SW_GPIO_207},    //PCIE_WAKE2_B
{L"GPIO_208", SW_GPIO_208},    //PCIE_WAKE3_B
{L"GPIO_156", SW_GPIO_156},    //EMMC0_CLK
{L"GPIO_157", SW_GPIO_157},    //EMMC0_D0
{L"GPIO_158", SW_GPIO_158},    //EMMC0_D1
{L"GPIO_159", SW_GPIO_159},    //EMMC0_D2
{L"GPIO_160", SW_GPIO_160},    //EMMC0_D3
{L"GPIO_161", SW_GPIO_161},    //EMMC0_D4
{L"GPIO_162", SW_GPIO_162},    //EMMC0_D5
{L"GPIO_163", SW_GPIO_163},    //EMMC0_D6
{L"GPIO_164", SW_GPIO_164},    //EMMC0_D7
{L"GPIO_165", SW_GPIO_165},    //EMMC0_CMD0
{L"GPIO_166", SW_GPIO_166},    //SDIO_CLK
{L"GPIO_167", SW_GPIO_167},    //SDIO_D0
{L"GPIO_168", SW_GPIO_168},    //SDIO_D1
{L"GPIO_169", SW_GPIO_169},    //SDIO_D2
{L"GPIO_170", SW_GPIO_170},    //SDIO_D3
{L"GPIO_171", SW_GPIO_171},    //SDIO_CMD
{L"GPIO_172", SW_GPIO_172},    //SDCARD_CLK
{L"GPIO_179", SW_GPIO_179},    //SDCARD_CMD
{L"GPIO_173", SW_GPIO_173},    //SDCARD_CLK_FB
{L"GPIO_174", SW_GPIO_174},    //SDCARD_D0
{L"GPIO_175", SW_GPIO_175},    //SDCARD_D1
{L"GPIO_176", SW_GPIO_176},    //SDCARD_D2
{L"GPIO_177", SW_GPIO_177},    //SDCARD_D3
{L"GPIO_178", SW_GPIO_178},    //SDCARD_CD_B
{L"GPIO_186", SW_GPIO_186},    //SDCARD_LVL_WP
{L"GPIO_182", SW_GPIO_182},    //EMMC0_STROBE
{L"GPIO_183", SW_GPIO_183},    //SDIO_PWR_DOWN_B
{L"SMB_ALERTB", SW_SMB_ALERTB},    //SMB_ALERTB
{L"SMB_CLK", SW_SMB_CLK},      //SMB_CLK
{L"SMB_DATA", SW_SMB_DATA},    //SMB_DATA
{L"LPC_ILB_SERIRQ", SW_LPC_ILB_SERIRQ},    //LPC_ILB_SERIRQ
{L"LPC_CLKOUT0", SW_LPC_CLKOUT0},    //LPC_CLKOUT0
{L"LPC_CLKOUT1", SW_LPC_CLKOUT1},    //LPC_CLKOUT1
{L"LPC_AD0", SW_LPC_AD0},    //LPC_AD0
{L"LPC_AD1", SW_LPC_AD1},    //LPC_AD1
{L"LPC_AD2", SW_LPC_AD2},    //LPC_AD2
{L"LPC_AD3", SW_LPC_AD3},    //LPC_AD3
{L"LPC_CLKRUN", SW_LPC_CLKRUN},    //LPC_CLKRUN
{L"LPC_FRAMEB", SW_LPC_FRAMEB}     //LPC_FRAMEB
};

#endif

