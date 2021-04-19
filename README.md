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

| 2B  |  4B  |  n |  1B |
|---|---|---|---|
| Sync Word  - 0x2DD4  |Header|  payload |  crc - from whole packet |

**Header**
| 1B  |  2B | 1B  | 
|---|---|---|
| size of payload  |RFU| crc Header  

**Payload**
| 1B  | 1B   | NB|
|---|---|--|
| Command| Action flags (only for way from PC to Generator)  |data|   

**CRC8 implementation with polynom = x7+ x6+ x4+ x2+ x0 (0xD5)**
CRC is the same for header and whole packet

**Table of commands for radio**
| **cmd**  | **opCode** 1B  |**action flags** 1B |**parameters** NB|  **retVal** 1B
|---|---|--|--|--|
| TxFreq  | 1  | yes | Frequency (4B)| 1 - Succes /0 - failure|
| RxFreq  | 2  | yes |  Frequency (4B)| 1 - Succes /0 - failure|
| TxPower  | 3  | yes | Power (1B)| 1 - Succes /0 - failure|
| TxSF  | 4  | yes | Spreading factor SF5-SF12 (1B)| 1 - Succes /0 - failure|
| RxSF  | 5  |yes  | Spreading factor SF5-SF12 (1B)| 1 - Succes /0 - failure|
| TxBW  | 6  |yes |  BandWidth 7.81 - 500 kHz (4B)| 1 - Succes /0 - failure|
| RxBW  | 7  |yes  |  BandWidth 7.81 - 500 kHz (4B)| 1 - Succes /0 - failure|
| TxIQ  | 8  |yes |  IQ invert true/false (1B)| 1 - Succes /0 - failure|
| RxIQ  | 9  |yes |  IQ invert true/false (1B)| 1 - Succes /0 - failure|
| TxCR  | 10  |yes |  CodeRate 4/5-4/8 (1B)| 1 - Succes /0 - failure|
| RxCR  | 11  |yes  |  CodeRate 4/5-4/8 (1B)| 1 - Succes /0 - failure|
| HeaderMode  TX | 12  | yes|   Enable header mode true/false (1 B)| 1 - Succes /0 - failure|
| HeaderMode  RX | 13  | yes|   Enable header mode true/false (1 B)| 1 - Succes /0 - failure|
| CRC TX  | 14  | yes |  Crc check true/false (1 B)| 1 - Succes /0 - failure|
| CRC RX  | 15  | yes |  Crc check true/false (1 B)| 1 - Succes /0 - failure|
| preparePacket  | 16  |yes | data[0] - nasledna velikost,  data[N] (N B)| 1 - Succes /0 - failure|
| AutoRepeating  | 17  | yes |  true/false (4 B) (max = never repeat)| 1 - Succes /0 - failure|


**Table of short (GET) commands for radio**
| **cmd**  | **opCode** 1B  |**action flags** 1B |**parameters** NB|  **retVal** 1B
|---|---|--|--|--|
| GetTxFreq  | 1+40  | yes | Frequency (4B)| 1 - Succes /0 - failure|
| GetRxFreq  | 2+40  | yes |  Frequency (4B)| 1 - Succes /0 - failure|
| GetTxPower  | 3+40  | yes | Power (1B)| 1 - Succes /0 - failure|
| GetTxSF  | 4+40  | yes | Spreading factor SF5-SF12 (1B)| 1 - Succes /0 - failure|
| GetRxSF  | 5+40  |yes  | Spreading factor SF5-SF12 (1B)| 1 - Succes /0 - failure|
| GetTxBW  | 6+40  |yes |  BandWidth 7.81 - 500 kHz (4B)| 1 - Succes /0 - failure|
| GetRxBW  | 7+40  |yes  |  BandWidth 7.81 - 500 kHz (4B)| 1 - Succes /0 - failure|
| GetTxIQ  | 8+40  |yes |  IQ invert true/false (1B)| 1 - Succes /0 - failure|
| GetRxIQ  | 9+40  |yes |  IQ invert true/false (1B)| 1 - Succes /0 - failure|
| GetTxCR  | 10+40  |yes |  CodeRate 4/5-4/8 (1B)| 1 - Succes /0 - failure|
| GetRxCR  | 11+40  |yes  |  CodeRate 4/5-4/8 (1B)| 1 - Succes /0 - failure|
| GetHeaderMode  TX | 12+40  | yes|   Enable header mode true/false (1 B)| 1 - Succes /0 - failure|
| GetHeaderMode  RX | 13+40  | yes|   Enable header mode true/false (1 B)| 1 - Succes /0 - failure|
| GetTxCRC  | 14+40  | yes |  Crc check true/false (1 B)| 1 - Succes /0 - failure|
| GetRxCRC  | 15+40  | yes |  Crc check true/false (1 B)| 1 - Succes /0 - failure|
| GetpreparePacket  | 16+40  |yes | data[0] - nasledna velikost,  data[N] (N B)| 1 - Succes /0 - failure|
| GetAutoRepeating  | 17+40  | yes |  true/false (4 B) (max = never repeat)| 1 - Succes /0 - failure|


**Action Flags**
1 = only set the value
2 = set and get answer back
3 = only get value from LoRaGenerator

**Table of commands with no actions flag**
| **cmd**  | **opCode** 1B  |**action flags** 1B |**parameters** NB|  **retVal** 1B
|---|---|--|--|--|
| WhoAreYou  | 254  | no | type for generator 2 B +systemInfo (NB)| 1 - Succes /0 - failure|
| WhatIsYourName  | 253  | no | String with name| 1 - Succes /0 - failure|
| sendPacket  | 252 | no |  empty | 1 - Succes /0 - failure|
| startTXCW  | 251  | no || 1 - Succes /0 - failure|
| setStandby  | 250  | no || 1 - Succes /0 - failure|
| readRxPacket  | 249  | no || 1 - Succes /0 - failure|
| startRx  | 248  |  no|  singleRX true/false (1 B) | 1 - Succes /0 - failure|


**system info**
-MCU, radio chip, max output power, min output power,

**Odpovedi od LoRaGeneratoru maji stejny tvar paketu az na chybejici Byte action flags**


