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
| 1B  |  2B | 1B  | 
|---|---|---|
| size of payload  |RFU| crc Header  

**Payload**
| 1B  |   | 
|---|---|
| Command  |Value|   

**Table of commands for radio**
| **cmd**  | **opCode**  |**flags** |**parameters**|  **retVal**
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

**Table of reading commands for radio**
| **cmd**  | **opCode**  | **return**| 
|---|---|--|
| getTxFreq  |(0x81) 129  |   Frequency (4B)|
| getRxFreq  | 130  |   Frequency (4B)| 
| getTxPower  | 131  |   Power (1B)|
| getTxSF  |  132 |  Spreading factor SF5-SF12 (1B)| 
| getRxSF  | 133  |  Spreading factor SF5-SF12 (1B)| 
| getTxBW  | 134  |   BandWidth 7.81 - 500 kHz (2B)| 
| getRxBW  | 135  |   BandWidth 7.81 - 500 kHz (2B)| 
| getTxIQ  | 136  |   IQ invert true/false (1B)| 
| getRxIQ  | 137  |   IQ invert true/false (1B)| 
| getTxCR  | 138  |   CodeRate 4/5-4/8 (1B)| 
| getRxCR  | 139  |   CodeRate 4/5-4/8 (1B)| 
| getreparedPacket  | 140  | data[0] - nasledna velikost,  data[N] (N B)| 
| getAutoRepeating  | 141  |   true/false (1 B)| 
| getRepeatingPeriod  | 142  |   Period (ms) (4 B)| 
| getRxCRCCheck  | 143  |   Crc check true/false (1 B)|
| getHeaderMode Rx & TX | 144  |   Enable header mode true/false (1 B)| 



| **cmd**  | **Return value**  | **parameters**| 
|---|---|--|
| setTxFreq  | opCode  |  |


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

