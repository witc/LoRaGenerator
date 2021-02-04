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
| Command| Action flags (only for way from LoRaGenerator to PC)  |Values|   

**CRC8 implementation with polynom = x7+ x6+ x4+ x2+ x0 (0xD5)**
CRC is the same for header and all packet
**Table of commands for radio**
| **cmd**  | **opCode** 1B  |**action flags** 1B |**parameters** NB|  **retVal** 1B
|---|---|--|--|--|
| TxFreq  | 0x1  | 1 = set, 2 = set&get, 3 = only get| Frequency (4B)| 1 - Succes /0 - failure|
| RxFreq  | 0x2  | 1 = set, 2 = set&get, 3 = only get |  Frequency (4B)| 1 - Succes /0 - failure|
| TxPower  | 0x3  |  1 = set, 2 = set&get, 3 = only get | Power (1B)| 1 - Succes /0 - failure|
| TxSF  | 0x4  | 1 = set, 2 = set&get, 3 = only get | Spreading factor SF5-SF12 (1B)| 1 - Succes /0 - failure|
| RxSF  | 0x5  | 1 = set, 2 = set&get, 3 = only get | Spreading factor SF5-SF12 (1B)| 1 - Succes /0 - failure|
| TxBW  | 0x6  | 1 = set, 2 = set&get, 3 = only get |  BandWidth 7.81 - 500 kHz (2B)| 1 - Succes /0 - failure|
| RxBW  | 0x7  | 1 = set, 2 = set&get, 3 = only get |  BandWidth 7.81 - 500 kHz (2B)| 1 - Succes /0 - failure|
| TxIQ  | 0x8  | 1 = set, 2 = set&get, 3 = only get |  IQ invert true/false (1B)| 1 - Succes /0 - failure|
| RxIQ  | 0x9  | 1 = set, 2 = set&get, 3 = only get |  IQ invert true/false (1B)| 1 - Succes /0 - failure|
| TxCR  | 10  | 1 = set, 2 = set&get, 3 = only get |  CodeRate 4/5-4/8 (1B)| 1 - Succes /0 - failure|
| RxCR  | 11  | 1 = set, 2 = set&get, 3 = only get |  CodeRate 4/5-4/8 (1B)| 1 - Succes /0 - failure|
| setStandby  | 12  |  || 1 - Succes /0 - failure|
| startTXCW  | 13  |  || 1 - Succes /0 - failure|
| preparePacket  | 14  |1 = set, 2 = set&get, 3 = only get | data[0] - nasledna velikost,  data[N] (N B)| 1 - Succes /0 - failure|
| AutoRepeating  | 15  | 1 = set, 2 = set&get, 3 = only get |  true/false (1 B)| 1 - Succes /0 - failure|
| RepeatingPeriod  | 16  | 1 = set, 2 = set&get, 3 = only get |  Period (ms) (4 B)| 1 - Succes /0 - failure|
| RxCRCCheck  | 17  | 1 = set, 2 = set&get, 3 = only get |  Crc check true/false (1 B)| 1 - Succes /0 - failure|
| HeaderMode Rx & TX | 18  | 1 = set, 2 = set&get, 3 = only get |   Enable header mode true/false (1 B)| 1 - Succes /0 - failure|
| sendPacket  | 19 |  |  empty | 1 - Succes /0 - failure|
| sendLastPacketAgain  | 20  || 1 = set, 2 = set&get, 3 = only get |  empty | 1 - Succes /0 - failure|
| startRx  | 21  |  |  Timeout (ms) (4 B) | 1 - Succes /0 - failure|

**Odpovedi od LoRaGeneratoru maji stejny tvar paketu az na chybejici Byte action flags**


