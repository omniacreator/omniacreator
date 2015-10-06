''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' Interface Library Full Duplex Serial Driver
'
' @version @n 1.0
' @date @n 11/30/2014
'
' @author @n Kwabena W. Agyeman
' @copyright @n (c) 2014 Kwabena W. Agyeman
' @n All rights reserved - Please see the end of the file for the terms of use
'
' @par Update History:
' @n v1.0 - Original release - 11/30/2014
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

CON RX_BUFFER_SIZE_EXPONENT = 8 ' 256 bytes

    RX_BUFFER_SIZE = 1 << RX_BUFFER_SIZE_EXPONENT
    RX_BUFFER_SIZE_MASK = RX_BUFFER_SIZE - 1

    RX_BUFFER_SIZE_2 = 2 << RX_BUFFER_SIZE_EXPONENT
    RX_BUFFER_SIZE_MASK_2 = RX_BUFFER_SIZE_2 - 1

CON TX_BUFFER_SIZE_EXPONENT = 8 ' 256 bytes

    TX_BUFFER_SIZE = 1 << TX_BUFFER_SIZE_EXPONENT
    TX_BUFFER_SIZE_MASK = TX_BUFFER_SIZE - 1

    TX_BUFFER_SIZE_2 = 2 << TX_BUFFER_SIZE_EXPONENT
    TX_BUFFER_SIZE_MASK_2 = TX_BUFFER_SIZE_2 - 1

DAT

    m_cogID long 0
    m_cogTX long 0

    m_rxHead long 0
    m_rxTail long 0
    m_txHead long 0
    m_txTail long 0

    m_rxBuffer byte 0[RX_BUFFER_SIZE]
    m_txBuffer byte 0[TX_BUFFER_SIZE]

PUB rxFlush
    m_rxTail := m_rxHead

PUB rxAvailable ' used space
    return ((m_rxHead - m_rxTail) & RX_BUFFER_SIZE_MASK_2)

PUB rx : rxByte ' may block
    repeat until rxAvailable
    rxByte := m_rxBuffer[m_rxTail & RX_BUFFER_SIZE_MASK]
    m_rxTail := (m_rxTail + 1) & RX_BUFFER_SIZE_MASK_2

PUB txFlush
    repeat until (txAvailable == TX_BUFFER_SIZE) and not m_cogTX

PUB txAvailable ' free space
    return TX_BUFFER_SIZE - ((m_txHead - m_txTail) & TX_BUFFER_SIZE_MASK_2)

PUB tx(txByte) ' may block
    repeat until txAvailable
    m_txBuffer[m_txHead & TX_BUFFER_SIZE_MASK] := txByte
    m_txHead := (m_txHead + 1) & TX_BUFFER_SIZE_MASK_2

PUB begin(rxPin, txPin, baudRate)
    end

    m_rxHeadPtr := @m_rxHead
    m_rxTailPtr := @m_rxTail
    m_txHeadPtr := @m_txHead
    m_txTailPtr := @m_txTail

    m_rxBufferPtr := @m_rxBuffer
    m_txBufferPtr := @m_txBuffer

    m_rxPin := |<rxPin
    m_txPin := |<txPin
    m_ctraMode := constant(%00100 << 26) + (txPin & %11111)
    m_baudRate := (clkfreq / baudRate) / 4

    m_cogID := cognew(@init, @m_cogTX) + 1

PUB end
    if m_cogID
        txFlush
        cogstop(m_cogID~ - 1)

DAT

                org 0

init            neg phsa, #1
                mov ctra, m_ctraMode
                mov dira, m_txPin

                rdlong rxHead, m_rxHeadPtr
                mov rxPC, #rxBegin

                rdlong txTail, m_txTailPtr
                mov txPC, #txBegin

                mov syncCNT, m_baudRate
                add syncCNT, cnt

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

sync            waitcnt syncCNT, m_baudRate
                jmpret syncPC, rxPC
                waitcnt syncCNT, m_baudRate
                jmpret syncPC, rxPC
                waitcnt syncCNT, m_baudRate
                jmpret syncPC, rxPC
                waitcnt syncCNT, m_baudRate
                jmpret syncPC, txPC

                jmp #sync

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

                ' Start Bit
rxLoop          jmpret rxPC, syncPC
rxBegin         test m_rxPin, ina wc
if_c            jmp #rxLoop
                jmpret rxPC, syncPC

                ' Rx Byte
                mov rxTemp2, #8
rxLoop2         jmpret rxPC, syncPC
                jmpret rxPC, syncPC
                jmpret rxPC, syncPC
                test m_rxPin, ina wc
                rcr rxTemp, #1
                djnz rxTemp2, #rxLoop2
                shr rxTemp, #24

                ' Test Full
                rdlong rxTail, m_rxTailPtr
                neg rxTail, rxTail
                add rxTail, rxHead
                and rxTail, m_rxMask2

                ' 1/2 Stop Bit
                jmpret rxPC, syncPC

                ' Write Byte
                cmp rxTail, m_rxSize wz
if_nz           mov rxBuffer, rxHead
if_nz           and rxBuffer, m_rxMask
if_nz           add rxBuffer, m_rxBufferPtr
if_nz           wrbyte rxTemp, rxBuffer

                ' Inc Head
if_nz           add rxHead, #1
if_nz           and rxHead, m_rxMask2
if_nz           wrlong rxHead, m_rxHeadPtr

                ' 1/2 Stop Bit
                jmpret rxPC, syncPC

                jmp #rxLoop

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

                ' Test Empty
txLoop          jmpret txPC, syncPC
txBegin         rdlong txHead, m_txHeadPtr
                sub txHead, txTail
                and txHead, m_txMask2 wz
if_z            jmpret txPC, syncPC
                wrlong txHead, par
                tjz txHead, #txLoop

                ' Read Byte
                mov txBuffer, txTail
                and txBuffer, m_txMask
                add txBuffer, m_txBufferPtr

                ' Start Bit
                jmpret txPC, syncPC
                mov phsa, #$0
                rdbyte phsa, txBuffer

                ' Inc Tail
                add txTail, #1
                and txTail, m_txMask2
                wrlong txTail, m_txTailPtr

                ' Tx Byte
                mov txTemp2, #9
txLoop2         or phsa, #$100
                jmpret txPC, syncPC
                ror phsa, #1
                djnz txTemp2, #txLoop2

                jmp #txBegin

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

m_rxSize        long RX_BUFFER_SIZE
m_rxMask        long RX_BUFFER_SIZE_MASK
m_rxMask2       long RX_BUFFER_SIZE_MASK_2
m_txSize        long TX_BUFFER_SIZE
m_txMask        long TX_BUFFER_SIZE_MASK
m_txMask2       long TX_BUFFER_SIZE_MASK_2

m_rxHeadPtr     long 0
m_rxTailPtr     long 0
m_txHeadPtr     long 0
m_txTailPtr     long 0

m_rxBufferPtr   long 0
m_txBufferPtr   long 0

m_rxPin         long 0
m_txPin         long 0
m_ctraMode      long 0
m_baudRate      long 0

rxHead          res 1
rxTail          res 1
txHead          res 1
txTail          res 1

rxBuffer        res 1
txBuffer        res 1

syncCNT         res 1
syncPC          res 1
rxPC            res 1
txPC            res 1

rxTemp          res 1
rxTemp2         res 1
txTemp          res 1
txTemp2         res 1

                fit 496

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' @par MIT License - TERMS OF USE:
' @n Permission is hereby granted, free of charge, to any person obtaining a
' copy of this software and associated documentation files (the "Software"), to
' deal in the Software without restriction, including without limitation the
' rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
' sell copies of the Software, and to permit persons to whom the Software is
' furnished to do so, subject to the following conditions:
' @n
' @n The above copyright notice and this permission notice shall be included in
' all copies or substantial portions of the Software.
' @n
' @n THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
' IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
' FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
' AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
' LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
' OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
' SOFTWARE.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
