# LoRaGenerator
Universal  LoRa packet generator with user settings.

## Commands:
### Radio
- TxFreq  
- RxFreq  
- TxPower 
- TxSF  
- RxSF  
- TxBW  
- RxBW  
- TxIQ  
- RxIQ  
- TxCR  
- RxCR  
- HeaderMode  TX 
- HeaderMode  RX 
- CRC TX  
- CRC RX  
- preparePacket 
- AutoRepeating

### Actions:
- WhoAreYou  
- WhatIsYourName 
- sendPacket  (send prepared packet)
- startTXCW  (continues wave)
- setStandby 
- readRxPacket  (read last RxPacket)
- startRx  

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

**Action Flags**
1 = only set the value
2 = set and get answer back
3 = only get value from LoRaGenerator

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


**Table of short (GET) commands for radio**
| **cmd**  | **opCode** 1B  |**action flags** 1B |**parameters** NB|  **retVal** 1B
|---|---|--|--|--|
| GetTxFreq  | 1+40  |  Frequency (4B)| 1 - Succes /0 - failure|
| GetRxFreq  | 2+40  |   Frequency (4B)| 1 - Succes /0 - failure|
| GetTxPower  | 3+40   | Power (1B)| 1 - Succes /0 - failure|
| GetTxSF  | 4+40   | Spreading factor SF5-SF12 (1B)| 1 - Succes /0 - failure|
| GetRxSF  | 5+40    | Spreading factor SF5-SF12 (1B)| 1 - Succes /0 - failure|
| GetTxBW  | 6+40   |  BandWidth 7.81 - 500 kHz (4B)| 1 - Succes /0 - failure|
| GetRxBW  | 7+40    |  BandWidth 7.81 - 500 kHz (4B)| 1 - Succes /0 - failure|
| GetTxIQ  | 8+40   |  IQ invert true/false (1B)| 1 - Succes /0 - failure|
| GetRxIQ  | 9+40   |  IQ invert true/false (1B)| 1 - Succes /0 - failure|
| GetTxCR  | 10+40   |  CodeRate 4/5-4/8 (1B)| 1 - Succes /0 - failure|
| GetRxCR  | 11+40    |  CodeRate 4/5-4/8 (1B)| 1 - Succes /0 - failure|
| GetHeaderMode  TX | 12+40  |   Enable header mode true/false (1 B)| 1 - Succes /0 - failure|
| GetHeaderMode  RX | 13+40  |   Enable header mode true/false (1 B)| 1 - Succes /0 - failure|
| GetTxCRC  | 14+40  |   Crc check true/false (1 B)| 1 - Succes /0 - failure|
| GetRxCRC  | 15+40  |   Crc check true/false (1 B)| 1 - Succes /0 - failure|
| GetpreparePacket  | 16+40  | data[0] - nasledna velikost,  data[N] (N B)| 1 - Succes /0 - failure|


**Table of commands with no actions flag**
| **cmd**  | **opCode** 1B |**parameters** NB|  **retVal** 1B
|---|---|--|--|
| WhoAreYou  | 254 | type for generator 2 B +systemInfo (NB)| 1 - Succes /0 - failure|
| WhatIsYourName  | 253 | String with name| 1 - Succes /0 - failure|
| sendPacket  | 252  | empty | 1 - Succes /0 - failure|
| startTXCW  | 251  | | 1 - Succes /0 - failure|
| setStandby  | 250   || 1 - Succes /0 - failure|
| readRxPacket  | 249  | payoad size (1B), rssi (1B), payload (NB) | 1 - Succes /0 - failure|
| startRx  | 248  | singleRX true/false (1 B), payloadSize 1 - 250 (1B) (does not care in Header Mode) | 1 - Succes /0 - failure|


**system info**
-MCU, radio chip, max output power, min output power,

**Odpovedi od LoRaGeneratoru maji stejny tvar paketu az na chybejici Byte action flags**

**CRC table**
static unsigned char crc8tab[256] = {
    0x00, 0xD5, 0x7F, 0xAA, 0xFE, 0x2B, 0x81, 0x54, 0x29, 0xFC, 0x56, 0x83, 0xD7, 0x02, 0xA8, 0x7D,
    0x52, 0x87, 0x2D, 0xF8, 0xAC, 0x79, 0xD3, 0x06, 0x7B, 0xAE, 0x04, 0xD1, 0x85, 0x50, 0xFA, 0x2F,
    0xA4, 0x71, 0xDB, 0x0E, 0x5A, 0x8F, 0x25, 0xF0, 0x8D, 0x58, 0xF2, 0x27, 0x73, 0xA6, 0x0C, 0xD9,
    0xF6, 0x23, 0x89, 0x5C, 0x08, 0xDD, 0x77, 0xA2, 0xDF, 0x0A, 0xA0, 0x75, 0x21, 0xF4, 0x5E, 0x8B,
    0x9D, 0x48, 0xE2, 0x37, 0x63, 0xB6, 0x1C, 0xC9, 0xB4, 0x61, 0xCB, 0x1E, 0x4A, 0x9F, 0x35, 0xE0,
    0xCF, 0x1A, 0xB0, 0x65, 0x31, 0xE4, 0x4E, 0x9B, 0xE6, 0x33, 0x99, 0x4C, 0x18, 0xCD, 0x67, 0xB2,
    0x39, 0xEC, 0x46, 0x93, 0xC7, 0x12, 0xB8, 0x6D, 0x10, 0xC5, 0x6F, 0xBA, 0xEE, 0x3B, 0x91, 0x44,
    0x6B, 0xBE, 0x14, 0xC1, 0x95, 0x40, 0xEA, 0x3F, 0x42, 0x97, 0x3D, 0xE8, 0xBC, 0x69, 0xC3, 0x16,
    0xEF, 0x3A, 0x90, 0x45, 0x11, 0xC4, 0x6E, 0xBB, 0xC6, 0x13, 0xB9, 0x6C, 0x38, 0xED, 0x47, 0x92,
    0xBD, 0x68, 0xC2, 0x17, 0x43, 0x96, 0x3C, 0xE9, 0x94, 0x41, 0xEB, 0x3E, 0x6A, 0xBF, 0x15, 0xC0,
    0x4B, 0x9E, 0x34, 0xE1, 0xB5, 0x60, 0xCA, 0x1F, 0x62, 0xB7, 0x1D, 0xC8, 0x9C, 0x49, 0xE3, 0x36,
    0x19, 0xCC, 0x66, 0xB3, 0xE7, 0x32, 0x98, 0x4D, 0x30, 0xE5, 0x4F, 0x9A, 0xCE, 0x1B, 0xB1, 0x64,
    0x72, 0xA7, 0x0D, 0xD8, 0x8C, 0x59, 0xF3, 0x26, 0x5B, 0x8E, 0x24, 0xF1, 0xA5, 0x70, 0xDA, 0x0F,
    0x20, 0xF5, 0x5F, 0x8A, 0xDE, 0x0B, 0xA1, 0x74, 0x09, 0xDC, 0x76, 0xA3, 0xF7, 0x22, 0x88, 0x5D,
    0xD6, 0x03, 0xA9, 0x7C, 0x28, 0xFD, 0x57, 0x82, 0xFF, 0x2A, 0x80, 0x55, 0x01, 0xD4, 0x7E, 0xAB,
    0x84, 0x51, 0xFB, 0x2E, 0x7A, 0xAF, 0x05, 0xD0, 0xAD, 0x78, 0xD2, 0x07, 0x53, 0x86, 0x2C, 0xF9};
