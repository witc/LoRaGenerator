# LoRaGenerator
Universal  LoRa packet generator with user settings.

## Commands:
### Radio
Every Radio packet sets radio to Standby mode!
- SetTxFreq
- SetRxFreq
- SetTxPower
- SetSF
- Set BW
- SetIQ
### Packet
- SetPacketToSend(data, size)
- SetRepeating (true/false, period)
- SetRxCRCCheck(true/false)
- SetHeaderMode(true/false) - for both TX and RX

### Actions:
- Sendpacket(data,size)
- SendLastPacketAgain()
- StartRX(freq,SF,BQ,IQ,timeout, HeaderMode,crcCheck)
- StartTxCW()


# USART Packet:

| 4B  |  4B  |  n |  2B |
|---|---|---|---|
| Sync Word  |Header|  payload |  crc - pocitano z celeho paketu |

**Header**
| 1B  |  1B | 2B  | 
|---|---|---|
| size of payload  |RFU| crc Header  

**Payload**
| 1B  |   | 
|---|---|
| Command  |Value|   

**Table of commands for radio**
| **cmd**  | **opCode**  | **parameters**| 
|---|---|--|
| setTxFreq  | 0x1  |   Frequency (4B)|
| setRxFreq  | 0x2  |   Frequency (4B)|
| setTxPower  | 0x3  |   Power (1B)|
| setTxSF  | 0x4  |  Spreading factor SF5-SF12 (1B)|
| setRxSF  | 0x5  |  Spreading factor SF5-SF12 (1B)|
| setTxBW  | 0x6  |   BandWidth 7.81 - 500 kHz (2B)|
| setRxBW  | 0x7  |   BandWidth 7.81 - 500 kHz (2B)|
| setTxIQ  | 0x8  |   IQ invert true/false (1B)|
| setRxIQ  | 0x9  |   IQ invert true/false (1B)|
| setTxCR  | 10  |   CodeRate 4/5-4/8 (1B)|
| setRxCR  | 11  |   CodeRate 4/5-4/8 (1B)|
| setStandby  | 12  |  |
| startTXCW  | 13  |  |
| preparePacket  | 14  | data[0] - nasledna velikost,  data[N] (N B)|
| setAutoRepeating  | 15  |   true/false (1 B)|
| setRepeatingPeriod  | 16  |   Period (ms) (4 B)|
| setRxCRCCheck  | 17  |   Crc check true/false (1 B)|
| setHeaderMode Rx & TX | 18  |   Enable header mode true/false (1 B)|
| sendPacket  | 19 |   empty |
| sendLastPacketAgain  | 20  |   empty |
| startRx  | 21  |   Timeout (ms) (4 B) |

**Table of commands for packet**
| **cmd**  | **opCode**  | **parameters**| 
|---|---|--|
| preparePacket  | 0x21  |   data[N] (N B)|
| setAutoRepeating  | 0x22  |   true/false (1 B)|
| setRepeatingPeriod  | 0x23  |   Period (ms) (4 B)|
| setRxCRCCheck  | 0x24  |   Crc check true/false (1 B)|
| setHeaderMode Rx & TX | 0x25  |   Enable header mode true/false (1 B)|

**Table of commands for actions**
| **cmd**  | **opCode**  | **parameters**| 
|---|---|--|
| sendPacket  | 0x41  |   empty |
| sendLastPacketAgain  | 0x42  |   empty |
| startRx  | 0x43  |   Timeout (ms) (4 B) |

