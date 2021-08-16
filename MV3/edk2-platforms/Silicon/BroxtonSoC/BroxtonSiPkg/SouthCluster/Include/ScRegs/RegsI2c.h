/** @file
  Register names for I2C device.

  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _REGS_I2C_H_
#define _REGS_I2C_H_

#define    DEFAULT_PCI_BUS_NUMBER_SC         0
#define    PCI_DEVICE_NUMBER_LPSS_I2C        30

#define    R_LPSS_I2C_STSCMD                 0x04  ///< Status & Command
#define    B_LPSS_I2C_STSCMD_RMA             BIT29 ///< RMA
#define    B_LPSS_I2C_STSCMD_RCA             BIT28 ///< RCA
#define    B_LPSS_I2C_STSCMD_CAPLIST         BIT20 ///< Capability List
#define    B_LPSS_I2C_STSCMD_INTRSTS         BIT19 ///< Interrupt Status
#define    B_LPSS_I2C_STSCMD_INTRDIS         BIT10 ///< Interrupt Disable
#define    B_LPSS_I2C_STSCMD_SERREN          BIT8  ///< SERR# Enable
#define    B_LPSS_I2C_STSCMD_BME             BIT2  ///< Bus Master Enable
#define    B_LPSS_I2C_STSCMD_MSE             BIT1  ///< Memory Space Enable

#define    R_LPSS_I2C_BAR                    0x10  ///< BAR0 Low
#define    B_LPSS_I2C_BAR_BA                 0xFFFFF000 ///< Base Address
#define    B_LPSS_I2C_BAR_SI                 0x00000FF0 ///< Size Indicator
#define    B_LPSS_I2C_BAR_PF                 BIT3  ///< Prefetchable
#define    B_LPSS_I2C_BAR_TYPE               (BIT2 | BIT1) ///< Type
#define    B_LPSS_I2C_BAR_MS                 BIT0  ///< Message Space

#define    R_LPSS_I2C_BAR1                   0x18  ///< BAR1 Low
#define    B_LPSS_I2C_BAR1_BA                0xFFFFF000 ///< Base Address
#define    B_LPSS_I2C_BAR1_SI                0x00000FF0 ///< Size Indicator
#define    B_LPSS_I2C_BAR1_PF                BIT3  ///< Prefetchable
#define    B_LPSS_I2C_BAR1_TYPE              (BIT2 | BIT1) ///< Type
#define    B_LPSS_I2C_BAR1_MS                BIT0  ///< Message Space

#define    NUM_RETRIES                       0xFFFF

#define    bit(a)                            1 << (a)

///
/// MMI/O Register Definitions
///
#define    I2C0_REG_SPACE_ADDR_BASE          0xFF138000  ///< 01K

#define    R_IC_CON                          (0x00) ///< I2C Control
#define    B_IC_RESTART_EN                   BIT5
#define    B_IC_SLAVE_DISABLE                BIT6
#define    V_SPEED_STANDARD                  0x02
#define    V_SPEED_FAST                      0x04
#define    V_SPEED_HIGH                      0x06
#define    B_MASTER_MODE                     BIT0

#define    R_IC_TAR                          (0x04) ///< I2C Target Address
#define    IC_TAR_10BITADDR_MASTER           BIT12

#define    R_IC_SAR                          (0x08) ///< I2C Slave Address
#define    R_IC_HS_MADDR                     (0x0C) ///< I2C HS MasterMode Code Address
#define    R_IC_DATA_CMD                     (0x10) ///< I2C Rx/Tx Data Buffer and Command

#define    B_READ_CMD                         BIT8  ///< 1 = read, 0 = write
#define    B_CMD_STOP                         BIT9  ///< 1 = STOP
#define    B_CMD_RESTART                      BIT10 ///< 1 = IC_RESTART_EN

#define    V_WRITE_CMD_MASK                  (0xFF)

#define    R_IC_SS_SCL_HCNT                  (0x14) ///< Standard Speed I2C Clock SCL High Count
#define    R_IC_SS_SCL_LCNT                  (0x18) ///< Standard Speed I2C Clock SCL Low Count
#define    R_IC_FS_SCL_HCNT                  (0x1C) ///< Full Speed I2C Clock SCL High Count
#define    R_IC_FS_SCL_LCNT                  (0x20) ///< Full Speed I2C Clock SCL Low Count
#define    R_IC_HS_SCL_HCNT                  (0x24) ///< High Speed I2C Clock SCL High Count
#define    R_IC_HS_SCL_LCNT                  (0x28) ///< High Speed I2C Clock SCL Low Count
#define    R_IC_INTR_STAT                    (0x2C) ///< I2C Inetrrupt Status
#define    R_IC_INTR_MASK                    (0x30) ///< I2C Interrupt Mask
#define    I2C_INTR_GEN_CALL                 BIT11 ///< General call received
#define    I2C_INTR_START_DET                BIT10
#define    I2C_INTR_STOP_DET                 BIT9
#define    I2C_INTR_ACTIVITY                 BIT8
#define    I2C_INTR_TX_ABRT                  BIT6  ///< Set on NACK
#define    I2C_INTR_TX_EMPTY                 BIT4
#define    I2C_INTR_TX_OVER                  BIT3
#define    I2C_INTR_RX_FULL                  BIT2  ///< Data bytes in RX FIFO over threshold
#define    I2C_INTR_RX_OVER                  BIT1
#define    I2C_INTR_RX_UNDER                 BIT0
#define    R_IC_RAW_INTR_STAT                (0x34) ///< I2C Raw Interrupt Status
#define    R_IC_RX_TL                        (0x38) ///< I2C Receive FIFO Threshold
#define    R_IC_TX_TL                        (0x3C) ///< I2C Transmit FIFO Threshold
#define    R_IC_CLR_INTR                     (0x40) ///< Clear Combined and Individual Interrupts
#define    R_IC_CLR_RX_UNDER                 (0x44) ///< Clear RX_UNDER Interrupt
#define    R_IC_CLR_RX_OVER                  (0x48) ///< Clear RX_OVERinterrupt
#define    R_IC_CLR_TX_OVER                  (0x4C) ///< Clear TX_OVER interrupt
#define    R_IC_CLR_RD_REQ                   (0x50) ///< Clear RD_REQ interrupt
#define    R_IC_CLR_TX_ABRT                  (0x54) ///< Clear TX_ABRT interrupt
#define    R_IC_CLR_RX_DONE                  (0x58) ///< Clear RX_DONE interrupt
#define    R_IC_CLR_ACTIVITY                 (0x5C) ///< Clear ACTIVITY interrupt
#define    R_IC_CLR_STOP_DET                 (0x60) ///< Clear STOP_DET interrupt
#define    R_IC_CLR_START_DET                (0x64) ///< Clear START_DET interrupt
#define    R_IC_CLR_GEN_CALL                 (0x68) ///< Clear GEN_CALL interrupt
#define    R_IC_ENABLE                       (0x6C) ///< I2C Enable
#define    I2C_ENABLE_ABORT                  BIT1
#define    I2C_ENABLE_ENABLE                 BIT0
#define    R_IC_STATUS                       (0x70) ///< I2C Status

#define    STAT_MST_ACTIVITY                 BIT5  ///< Master FSM Activity Status.
#define    STAT_RFF                          BIT4  ///< RX FIFO is completely full
#define    STAT_RFNE                         BIT3  ///< RX FIFO is not empty
#define    STAT_TFE                          BIT2  ///< TX FIFO is completely empty
#define    STAT_TFNF                         BIT1  ///< TX FIFO is not full

#define    R_IC_TXFLR                        (0x74) ///< Transmit FIFO Level Register
#define    R_IC_RXFLR                        (0x78) ///< Receive FIFO Level Register
#define    R_IC_SDA_HOLD                     (0x7C) ///< I2C IC_DEFAULT_SDA_HOLD//16bits
#define    R_IC_TX_ABRT_SOURCE               (0x80) ///< I2C Transmit Abort Status Register
#define    I2C_ABRT_SLVRD_INTX               BIT15
#define    I2C_ABRT_SLV_ARBLOST              BIT14
#define    I2C_ABRT_SLVFLUSH_TXFIFO          BIT13
#define    I2C_ARB_LOST                      BIT12
#define    I2C_ABRT_MASTER_DIS               BIT11
#define    I2C_ABRT_10B_RD_NORSTRT           BIT10
#define    I2C_ABRT_SBYTE_NORSTRT            BIT9
#define    I2C_ABRT_HS_NORSTRT               BIT8
#define    I2C_ABRT_SBYTE_ACKDET             BIT7
#define    I2C_ABRT_HS_ACKDET                BIT6
#define    I2C_ABRT_GCALL_READ               BIT5
#define    I2C_ABRT_GCALL_NOACK              BIT4
#define    I2C_ABRT_TXDATA_NOACK             BIT3
#define    I2C_ABRT_10ADDR2_NOACK            BIT2
#define    I2C_ABRT_10ADDR1_NOACK            BIT1
#define    I2C_ABRT_7B_ADDR_NOACK            BIT0
#define    R_IC_SLV_DATA_NACK_ONLY           (0x84) ///< Generate SLV_DATA_NACK Register
#define    R_IC_DMA_CR                       (0x88) ///< DMA Control Register
#define    R_IC_DMA_TDLR                     (0x8C) ///< DMA Transmit Data Level
#define    R_IC_DMA_RDLR                     (0x90) ///< DMA Receive Data Level
#define    R_IC_SDA_SETUP                    (0x94) ///< I2C SDA Setup Register
#define    R_IC_ACK_GENERAL_CALL             (0x98) ///< I2C ACK General Call Register
#define    R_IC_ENABLE_STATUS                (0x9C) ///< I2C Enable Status Register
#define    R_IC_COMP_PARAM                   (0xF4) ///< Component Parameter Register
#define    R_IC_COMP_VERSION                 (0xF8) ///< Component Version ID
#define    R_IC_COMP_TYPE                    (0xFC) ///< Component Type

#define    R_IC_RESET_CONTROL                (0x204) ///< Reset control
#define    I2C_RESET_CONTROLLER              (BIT0 | BIT1)
#define    I2C_RESET_IDMA                    (BIT2)

#define    R_IC_CLK_GATE                     (0x238) ///< Clock Gate
#define    I2C_FORCE_CLOCK_ON                (BIT0 | BIT1)
#define    I2C_FORCE_CLOCK_OFF               (BIT1)
#define    I2C_FORCE_IDMA_CLOCK_ON           (BIT2 | BIT3)
#define    I2C_FORCE_IDMA_CLOCK_OFF          (BIT3)

#define    I2C_SS_SCL_HCNT_VALUE_100M         0x1DD
#define    I2C_SS_SCL_LCNT_VALUE_100M         0x1E4
#define    I2C_FS_SCL_HCNT_VALUE_100M         0x54
#define    I2C_FS_SCL_LCNT_VALUE_100M         0x9a
#define    I2C_HS_SCL_HCNT_VALUE_100M         0x7
#define    I2C_HS_SCL_LCNT_VALUE_100M         0xE

#endif

