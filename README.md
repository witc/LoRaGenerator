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


# USART Packet:

| 4B  |  4B  |  n |  2B |
|---|---|---|---|
| Sync Word  |Header|  payload |  crc - pocitano z celeho paketu |

**Header**
| 1B  |  1B | 2B  | 
|---|---|---|
| size of payload  |RFU| crc Header  
