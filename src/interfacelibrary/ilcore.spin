''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' Interface Library Core
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

OBJ serial: "ilfullduplexserialdriver.spin"

CON BASE_SIZE = 64

CON ' SerialBase

    ESCAPE_BASE
    ESCAPE_FILE_BASE
    ESCAPE_FOLDER_BASE
    ESCAPE_INPUT_DIALOG_BASE
    ESCAPE_OUTPUT_DIALOG_BASE
    ESCAPE_BASE_SPACER_0 ' spacer
    ESCAPE_BASE_SPACER_1 ' spacer
    ESCAPE_BASE_SPACER_2 ' spacer
    ESCAPE_BASE_SPACER_3 ' spacer

    WINDOW_BASE

    TERMINAL_BASE

    OSCILLOSCOPE_BASE
    OSCILLOSCOPE_XAXIS_BASE
    OSCILLOSCOPE_YAXIS_BASE
    OSCILLOSCOPE_FFTXAXIS_BASE
    OSCILLOSCOPE_FFTYAXIS_BASE
    OSCILLOSCOPE_BAR_BASE
    OSCILLOSCOPE_BAR_BASE_EXTENSION
    OSCILLOSCOPE_CURVE_BASE
    OSCILLOSCOPE_CURVE_BASE_EXTENSION
    OSCILLOSCOPE_GRAPH_BASE
    OSCILLOSCOPE_GRAPH_BASE_EXTENSION
    OSCILLOSCOPE_BOX_BASE
    OSCILLOSCOPE_BOX_BASE_EXTNESION
    OSCILLOSCOPE_BASE_SPACER_0 ' spacer
    OSCILLOSCOPE_BASE_SPACER_1 ' spacer
    OSCILLOSCOPE_BASE_SPACER_2 ' spacer
    OSCILLOSCOPE_BASE_SPACER_3 ' spacer

    TABLE_BASE
    TABLE_ROW_BASE
    TABLE_COLUMN_BASE

    TREE_BASE

    GRAPHICS_BASE
    GRAPHICS_BASE_EXTENSION_0
    GRAPHICS_BASE_EXTENSION_1
    GRAPHICS_BASE_SPACER_0 ' spacer
    GRAPHICS_BASE_SPACER_1 ' spacer
    GRAPHICS_BASE_SPACER_2 ' spacer
    GRAPHICS_BASE_SPACER_3 ' spacer

    INTERFACE_BASE
    INTERFACE_GROUP_BOX_BASE
    INTERFACE_PUSH_BUTTON_BASE
    INTERFACE_RADIO_BUTTON_BASE
    INTERFACE_CHECK_BOX_BASE
    INTERFACE_LINE_EDIT_BASE
    INTERFACE_INT_SPIN_BOX_BASE
    INTERFACE_BIN_SPIN_BOX_BASE
    INTERFACE_HEX_SPIN_BOX_BASE
    INTERFACE_DOUBLE_SPIN_BOX_BASE
    INTERFACE_TIME_EDIT_BASE
    INTERFACE_DATE_EDIT_BASE
    INTERFACE_DATE_TIME_EDIT_BASE
    INTERFACE_SLIDER_BASE
    INTERFACE_LABEL_BASE
    INTERFACE_PROGRESS_BAR_BASE
    INTERFACE_SEPERATOR_BASE
    INTERFACE_BASE_SPACER_0 ' spacer
    INTERFACE_BASE_SPACER_1 ' spacer
    INTERFACE_BASE_SPACER_2 ' spacer
    INTERFACE_BASE_SPACER_3 ' spacer

CON ' Serial Function

    #(ESCAPE_BASE * BASE_SIZE)

    ESCAPE_NAK
    ESCAPE_NAK_SPACER ' spacer
    ESCAPE_ACK
    ESCAPE_ACK_SPACER ' spacer
    ESCAPE_RESET_DCE
    ESCAPE_RESET_DCE_SPACER ' spacer
    ESCAPE_RESET_DTE
    ESCAPE_RESET_DTE_SPACER ' spacer
    ESCAPE_GET_VERSION_INT
    ESCAPE_GET_VERSION_INT_SPACER ' spacer
    ESCAPE_GET_VERSION_STR
    ESCAPE_GET_VERSION_STR_SPACER ' spacer
    ESCAPE_OPEN_PROCESS
    ESCAPE_OPEN_PROCESS_SPACER ' spacer
    ESCAPE_OPEN_URL
    ESCAPE_OPEN_URL_SPACER ' spacer
    ESCAPE_SPACER_00 ' spacer
    ESCAPE_SPACER_01 ' spacer
    ESCAPE_SPACER_02 ' spacer
    ESCAPE_SPACER_03 ' spacer
    ESCAPE_SPACER_04 ' spacer
    ESCAPE_SPACER_05 ' spacer
    ESCAPE_SPACER_06 ' spacer
    ESCAPE_SPACER_07 ' spacer
    ESCAPE_SPACER_08 ' spacer
    ESCAPE_SPACER_09 ' spacer
    ESCAPE_SPACER_10 ' spacer
    ESCAPE_SPACER_11 ' spacer
    ESCAPE_SPACER_12 ' spacer
    ESCAPE_SPACER_13 ' spacer
    ESCAPE_SPACER_14 ' spacer
    ESCAPE_SPACER_15 ' spacer
    ESCAPE_SET_m_CHECKSUM_MODE
    ESCAPE_GET_m_CHECKSUM_MODE
    ESCAPE_SET_MAX_CREDITS
    ESCAPE_GET_MAX_CREDITS
    ESCAPE_SET_MAX_LAYER_0_DATA_SIZE
    ESCAPE_GET_MAX_LAYER_0_DATA_SIZE
    ESCAPE_SET_BAUD_RATE
    ESCAPE_GET_BAUD_RATE

    #(WINDOW_BASE * BASE_SIZE)

    WINDOW_CLOSE_WINDOW
    WINDOW_CLOSE_WINDOWS
    WINDOW_CLOSE_WINDOW_SPACER_0 ' spacer
    WINDOW_CLOSE_WINDOW_SPACER_1 ' spacer
    WINDOW_CLOSE_WINDOW_SPACER_2 ' spacer
    WINDOW_CLOSE_WINDOW_SPACER_3 ' spacer
    WINDOW_CLOSE_WINDOW_SPACER_4 ' spacer
    WINDOW_CLOSE_WINDOW_SPACER_5 ' spacer
    WINDOW_DELETE_WINDOW
    WINDOW_DELETE_WINDOWS
    WINDOW_DELETE_WINDOW_SPACER_0 ' spacer
    WINDOW_DELETE_WINDOW_SPACER_1 ' spacer
    WINDOW_DELETE_WINDOW_SPACER_2 ' spacer
    WINDOW_DELETE_WINDOW_SPACER_3 ' spacer
    WINDOW_DELETE_WINDOW_SPACER_4 ' spacer
    WINDOW_DELETE_WINDOW_SPACER_5 ' spacer
    WINDOW_SET_WINDOW_TITLE
    WINDOW_GET_WINDOW_TITLE
    WINDOW_SET_WINDOW_MESSAGE
    WINDOW_GET_WINDOW_MESSAGE
    WINDOW_SET_WINDOW_VISIBLE
    WINDOW_GET_WINDOW_VISIBLE
    WINDOW_SET_WINDOW_MINIMIZED
    WINDOW_GET_WINDOW_MINIMIZED
    WINDOW_SET_WINDOW_MAXIMIZED
    WINDOW_GET_WINDOW_MAXIMIZED
    WINDOW_SET_WINDOW_FULL_SCREEN
    WINDOW_GET_WINDOW_FULL_SCREEN

CON ' SerialTerminalControlCharacter

    CC_NULL ' = 0
    CC_HOME_CURSOR
    CC_END_CURSOR
    CC_MOVE_CURSOR_LEFT
    CC_MOVE_CURSOR_RIGHT
    CC_MOVE_CURSOR_UP
    CC_MOVE_CURSOR_DOWN
    CC_BEEP_SPEAKER
    CC_BACKSPACE
    CC_TAB
    CC_LINE_FEED
    CC_CLEAR_TO_END_OF_LINE
    CC_CLEAR_LINES_BELOW
    CC_CARRIAGE_RETURN
    CC_CLEAR_TO_START_OF_LINE
    CC_CLEAR_LINES_ABOVE
    CC_CLEAR_SCREEN
    CC_XON
    CC_SET_FOREGROUND_TO_GRAY
    CC_XOFF
    CC_SET_FOREGROUND_TO_BLACK
    CC_SET_FOREGROUND_TO_BLUE
    CC_SET_FOREGROUND_TO_RED
    CC_SET_FOREGROUND_TO_GREEN
    CC_SET_FOREGROUND_TO_MAGENTA
    CC_SET_FOREGROUND_TO_CYAN
    CC_SET_FOREGROUND_TO_YELLOW
    CC_ESCAPE
    CC_MOVE_TO_START_OF_LINE
    CC_MOVE_TO_END_OF_LINE
    CC_MOVE_TO_PREVIOUS_START_OF_LINE
    CC_MOVE_TO_NEXT_START_OF_LINE
    CC_DELETE = 127

CON ' StandardColors

    SC_BLACK = $FF000000
    SC_WHITE = $FFFFFFFF
    SC_DARK_GRAY = $FF808080
    SC_GRAY = $FFA0A0A0
    SC_LIGHT_GRAY = $FFC0C0C0
    SC_RED = $FFFF0000
    SC_GREEN = $FF00FF00
    SC_BLUE = $FF0000FF
    SC_CYAN = $FF00FFFF
    SC_MAGENTA = $FFFF00FF
    SC_YELLOW = $FFFFFF00
    SC_DARK_RED = $FF800000
    SC_DARK_GREEN = $FF008000
    SC_DARK_BLUE = $FF000080
    SC_DARK_CYAN = $FF008080
    SC_DARK_MAGENTA = $FF800080
    SC_DARK_YELLOW = $FF808000
    SC_TRANSPARENT = $00000000

CON ' CoordinateSystem

    CS_CARTESIAN
    CS_POLAR

CON ' AngleUnits

    AU_DEGREES
    AU_RADIANS

CON ' LineStyle

    LS_NO_PEN
    LS_SOLID_LINE
    LS_DASH_LINE
    LS_DOT_LINE
    LS_DASH_DOT_LINE
    LS_DASH_DOT_DOT_LINE

CON ' FillStyle

    FS_NO_BRUSH
    FS_SOLID_PATTERN
    FS_DENSE_1_PATTERN ' not supported
    FS_DENSE_2_PATTERN ' not supported
    FS_DENSE_3_PATTERN ' not supported
    FS_DENSE_4_PATTERN ' not supported
    FS_DENSE_5_PATTERN ' not supported
    FS_DENSE_6_PATTERN ' not supported
    FS_DENSE_7_PATTERN ' not supported
    FS_HOR_PATTERN
    FS_VER_PATTERN
    FS_CROSS_PATTERN
    FS_BACKWARD_DIAGONAL_PATTERN
    FS_FORWARD_DIAGONAL_PATTERN
    FS_DIAGONAL_PATTERN
    FS_LINEAR_GRADIENT_PATTERN ' not supported
    FS_RADIAL_GRADIENT_PATTERN ' not supported
    FS_CONICAL_GRADIENT_PATTERN ' not supported

CON ' ScaleType

    ST_LINEAR
    ST_LOGARITHMIC

CON ' ScatterShape

    SS_NONE
    SS_DOT
    SS_CROSS
    SS_PLUS
    SS_CIRCLE
    SS_DISC
    SS_SQUARE
    SS_DIAMOND
    SS_STAR
    SS_TRIANGLE
    SS_TRIANGLE_INVERTED
    SS_CROSS_SQUARE
    SS_PLUS_SQUARE
    SS_CROSS_CIRCLE
    SS_PLUS_CIRCLE
    SS_PEACE

CON ' CurveFFT

    CFFT_NONE
    CFFT_FORWARD
    CFFT_BACKWARD

CON ' CurveFFTWF

    CFFTWF_RECTANGLE
    CFFTWF_TRIANGLE
    CFFTWF_WELCH
    CFFTWF_HANNING
    CFFTWF_HAMMING
    CFFTWF_BLACKMAN
    CFFTWF_BLACKMAN_HARRIS
    CFFTWF_BLACKMAN_NUTTAL

CON ' CurveLineStyle

    CLS_NONE
    CLS_LINE

CON ' GraphFFT

    GFFT_NONE
    GFFT_MAGNITUDE
    GFFT_POWER
    GFFT_MAGNITUDE_DB
    GFFT_POWER_DB
    GFFT_PHASE_RADIANS
    GFFT_PHASE_DEGREES

CON ' GraphFFTWF

    GFFTWF_RECTANGLE
    GFFTWF_TRIANGLE
    GFFTWF_WELCH
    GFFTWF_HANNING
    GFFTWF_HAMMING
    GFFTWF_BLACKMAN
    GFFTWF_BLACKMAN_HARRIS
    GFFTWF_BLACKMAN_NUTTAL

CON ' GraphLineStyle

    GLS_NONE
    GLS_LINE
    GLS_STEP_LEFT
    GLS_STEP_RIGHT
    GLS_STEP_CENTER
    GLS_IMPULSE

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

CON RX_BUFFER_SIZE_EXPONENT = 8 ' 256 bytes

    RX_BUFFER_SIZE = 1 << RX_BUFFER_SIZE_EXPONENT
    RX_BUFFER_SIZE_MASK = RX_BUFFER_SIZE - 1

    RX_BUFFER_SIZE_2 = 2 << RX_BUFFER_SIZE_EXPONENT
    RX_BUFFER_SIZE_MASK_2 = RX_BUFFER_SIZE_2 - 1

DAT

    m_rxHead long 0
    m_rxTail long 0

    m_rxBuffer byte 0[RX_BUFFER_SIZE]

PUB flush
    m_rxTail := m_rxHead

PUB available
    return ((m_rxHead - m_rxTail) & RX_BUFFER_SIZE_MASK_2)

PUB free
    return RX_BUFFER_SIZE - available

PUB full
    return not free

PUB empty
    return not available

PUB peek : b
    b := -1
    if available
        b := m_rxBuffer[m_rxTail & RX_BUFFER_SIZE_MASK]

PUB read : b
    b := peek
    if b <> -1
        m_rxTail := (m_rxTail + 1) & RX_BUFFER_SIZE_MASK_2

PUB writeChar(c)
    serial.tx(c)

PUB writeString(s)
    repeat strsize(s)
        writeChar(byte[s++])

PRI receive(b)
    ifnot full
        m_rxBuffer[m_rxHead & RX_BUFFER_SIZE_MASK] := b
        m_rxHead := (m_rxHead + 1) & RX_BUFFER_SIZE_MASK_2

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

CON MAX_LAYER_0_DATA_SIZE = 2 + 2

CON RESPONSE_BUFFER_SIZE_EXPONENT = >|MAX_LAYER_0_DATA_SIZE - 1 ' 4 bytes

    RESPONSE_BUFFER_SIZE = 1 << RESPONSE_BUFFER_SIZE_EXPONENT
    RESPONSE_BUFFER_SIZE_MASK = RESPONSE_BUFFER_SIZE - 1

    RESPONSE_BUFFER_SIZE_2 = 2 << RESPONSE_BUFFER_SIZE_EXPONENT
    RESPONSE_BUFFER_SIZE_MASK_2 = RESPONSE_BUFFER_SIZE_2 - 1

DAT

    m_responseHead long 0
    m_responseTail long 0

    m_responseBuffer byte 0[RESPONSE_BUFFER_SIZE]

PRI responseFlush
    m_responseTail := m_responseHead

PRI responseAvailable
    return ((m_responseHead - m_responseTail) & RESPONSE_BUFFER_SIZE_MASK_2)

PRI responseGet : b
    b := m_responseBuffer[m_responseTail & RESPONSE_BUFFER_SIZE_MASK]
    m_responseTail := (m_responseTail + 1) & RESPONSE_BUFFER_SIZE_MASK_2

PRI responsePut(b)
    m_responseBuffer[m_responseHead & RESPONSE_BUFFER_SIZE_MASK] := b
    m_responseHead := (m_responseHead + 1) & RESPONSE_BUFFER_SIZE_MASK_2

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

DAT

    m_rxChecksum word 0
    m_txChecksum word 0

    m_checksumCRCTable word $0000, $1081, $2102, $3183
                       word $4204, $5285, $6306, $7387
                       word $8408, $9489, $A50A, $B58B
                       word $C60C, $D68D, $E70E, $F78F

PRI updateChecksum(checksum, b) | temp
    temp := word[checksum]
    temp := ((temp >> 4) & $FFF) ^ m_checksumCRCTable[(temp ^ (b >> 0)) & $F]
    temp := ((temp >> 4) & $FFF) ^ m_checksumCRCTable[(temp ^ (b >> 4)) & $F]
    word[checksum] := temp

PRI initRXChecksum
    m_rxChecksum~~

PRI updateRXChecksum(b)
    updateChecksum(@m_rxChecksum, b)

PRI initTXChecksum
    m_txChecksum~~

PRI updateTXChecksum(b)
    updateChecksum(@m_txChecksum, b)

PRI sendTXChecksum
    !m_txChecksum
    writeChar(m_txChecksum.byte[0])
    writeChar(m_txChecksum.byte[1])

PRI sendByte(b)
    writeChar(b)
    updateTXChecksum(b)

PRI sendWord(w)
    sendByte(w.byte[0])
    sendByte(w.byte[1])

PRI sendLong(l)
    sendWord(l.word[0])
    sendWord(l.word[1])

PRI sendData(d, size)
    repeat size
        sendByte(byte[d++])

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

DAT m_coreEnabled long false

PRI disableCore
    serial.end
    m_coreEnabled := false

PRI disableCoreXOFF
    serial.tx(CC_XOFF)
    disableCore

DAT m_receivedResponse long false

CON MAX_L0_CREDITS = 16
DAT m_txL0Credits long MAX_L0_CREDITS

CON

    L0_WAIT_FOR_ESCAPE
    L0_WAIT_FOR_NULL
    L0_WAIT_FOR_PACKET_SIZE_0
    L0_WAIT_FOR_PACKET_SIZE_1
    L0_WAIT_FOR_PACKET_DATA
    L0_WAIT_FOR_CHECKSUM_0
    L0_WAIT_FOR_CHECKSUM_1
    L0_ERROR_STATE

DAT

    m_receiveLayer0Temp0 long 0
    m_receiveLayer0Temp1 long 0
    m_receiveLayer0State long L0_WAIT_FOR_ESCAPE

PRI receiveLayer0 | b
    repeat serial.rxAvailable
        b := serial.rx
        case m_receiveLayer0State

            L0_WAIT_FOR_ESCAPE:
                case b

                    CC_XON:
                        m_txL0Credits := (m_txL0Credits+1)<#MAX_L0_CREDITS

                    CC_XOFF:
                        m_txL0Credits := (m_txL0Credits+1)<#MAX_L0_CREDITS
                        m_receiveLayer0State := L0_ERROR_STATE
                        disableCore
                        return

                    CC_ESCAPE:
                        initRXChecksum
                        updateRXChecksum(b)
                        responseFlush
                        m_receiveLayer0State := L0_WAIT_FOR_NULL

                    other:
                        receive(b)

            L0_WAIT_FOR_NULL:
                updateRXChecksum(b)
                m_receiveLayer0State := L0_WAIT_FOR_PACKET_SIZE_0

            L0_WAIT_FOR_PACKET_SIZE_0:
                updateRXChecksum(b)
                m_receiveLayer0Temp0 := b
                m_receiveLayer0State := L0_WAIT_FOR_PACKET_SIZE_1

                if m_receiveLayer0Temp0 > MAX_LAYER_0_DATA_SIZE
                    m_receiveLayer0State := L0_ERROR_STATE
                    disableCoreXOFF
                    return

            L0_WAIT_FOR_PACKET_SIZE_1:
                updateRXChecksum(b)
                m_receiveLayer0State := L0_WAIT_FOR_PACKET_DATA

                ifnot m_receiveLayer0Temp0
                    m_receiveLayer0State := L0_WAIT_FOR_CHECKSUM_0

                if m_receiveLayer0Temp0 <> (b ^ $FF)
                    m_receiveLayer0State := L0_ERROR_STATE
                    disableCoreXOFF
                    return

            L0_WAIT_FOR_PACKET_DATA:
                updateRXChecksum(b)
                responsePut(b)

                ifnot --m_receiveLayer0Temp0
                    m_receiveLayer0State := L0_WAIT_FOR_CHECKSUM_0

            L0_WAIT_FOR_CHECKSUM_0:
                m_receiveLayer0Temp1.byte[0] := b
                m_receiveLayer0State := L0_WAIT_FOR_CHECKSUM_1

            L0_WAIT_FOR_CHECKSUM_1:
                m_receiveLayer0Temp1.byte[1] := b
                m_receiveLayer0State := L0_WAIT_FOR_ESCAPE

                if m_receiveLayer0Temp1 <> (m_rxChecksum ^ $FFFF)
                    m_receiveLayer0State := L0_ERROR_STATE
                    disableCoreXOFF
                    return

                else
                    writeChar(CC_XON)
                    receiveLayer1

            L0_ERROR_STATE: ' wait here until reset

CON MAX_L1_CREDITS = 16
DAT m_txL1Credits long MAX_L1_CREDITS

CON

    L1_WAIT_FOR_PACKET_FUNCTION_0
    L1_WAIT_FOR_PACKET_FUNCTION_1
    L1_WAIT_FOR_PACKET_SIZE_0
    L1_WAIT_FOR_PACKET_SIZE_1
    L1_WAIT_FOR_PACKET_DATA
    L1_ERROR_STATE

DAT

    m_receiveLayer1Temp0 long 0
    m_receiveLayer1Temp1 long 0
    m_receiveLayer1State long L1_WAIT_FOR_PACKET_FUNCTION_0

PRI receiveLayer1 | b
    repeat responseAvailable
        b := responseGet
        case m_receiveLayer1State

            L1_WAIT_FOR_PACKET_FUNCTION_0:
                m_receiveLayer1Temp0.byte[0] := b
                m_receiveLayer1State := L1_WAIT_FOR_PACKET_FUNCTION_1

            L1_WAIT_FOR_PACKET_FUNCTION_1:
                m_receiveLayer1Temp0.byte[1] := b
                m_receiveLayer1State := L1_WAIT_FOR_PACKET_SIZE_0

            L1_WAIT_FOR_PACKET_SIZE_0:
                m_receiveLayer1Temp1.byte[0] := b
                m_receiveLayer1State := L1_WAIT_FOR_PACKET_SIZE_1

            L1_WAIT_FOR_PACKET_SIZE_1:
                m_receiveLayer1Temp1.byte[1] := b
                m_receiveLayer1State := L1_WAIT_FOR_PACKET_FUNCTION_0

                if m_receiveLayer1Temp1
                    m_receiveLayer1State := L1_ERROR_STATE
                    disableCore
                    return

                else
                    case m_receiveLayer1Temp0

                        ESCAPE_NAK:
                            m_txL1Credits := (m_txL1Credits+1)<#MAX_L1_CREDITS
                            m_receivedResponse := false

                        ESCAPE_ACK:
                            m_txL1Credits := (m_txL1Credits+1)<#MAX_L1_CREDITS
                            m_receivedResponse := true

                        other:
                            m_receiveLayer1State := L1_ERROR_STATE
                            disableCore
                            return

            L1_WAIT_FOR_PACKET_DATA:

                ifnot --m_receiveLayer1Temp1
                    m_receiveLayer1State := L1_WAIT_FOR_PACKET_FUNCTION_0

            L1_ERROR_STATE: ' wait here until reset

CON CREDIT_TIMEOUT = 5 ' seconds
PRI waitForCredits | t
    if m_coreEnabled

        t := cnt
        repeat
            receiveLayer0

            ifnot m_coreEnabled
                return false

            ifnot (cnt - t) < (clkfreq * CREDIT_TIMEOUT)
                disableCore
                return false

        until m_txL0Credits and m_txL1Credits

        m_txL0Credits--
        m_txL1Credits--

        return true

CON RESPONSE_TIMEOUT = 5 ' seconds
PUB waitForResponse | t
    if m_coreEnabled

        m_receivedResponse := false

        t := cnt
        repeat
            receiveLayer0

            ifnot m_coreEnabled
                return false

            ifnot (cnt - t) < (clkfreq * RESPONSE_TIMEOUT)
                disableCore
                return false

        until (m_txL0Credits==MAX_L0_CREDITS)and(m_txL1Credits==MAX_L1_CREDITS)

        return m_receivedResponse

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

CON L0_HEADER_SIZE = 1 + 1 + 1 + 1 + 2
CON L1_HEADER_SIZE = 2 + 2
CON MAX_PKT_SIZE = 256
CON MAX_PKT_D_SIZE = MAX_PKT_SIZE - L0_HEADER_SIZE + L1_HEADER_SIZE

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND1_PS = 0 ' payload size
PUB send1(function)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND1_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND1_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND1_PS)
        ' Payload
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND1BYTE_PS = 0 + 1 ' payload size
PUB send1Byte(function, b0)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND1BYTE_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND1BYTE_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND1BYTE_PS)
        ' Payload
        sendByte(b0)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND1WORD_PS = 0 + 2 ' payload size
PUB send1Word(function, w0)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND1WORD_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND1WORD_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND1WORD_PS)
        ' Payload
        sendWord(w0)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND1LONG_PS = 0 + 4 ' payload size
PUB send1Long(function, l0)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND1LONG_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND1LONG_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND1LONG_PS)
        ' Payload
        sendLong(l0)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND1STRING_PS = 0 ' payload size
PUB send1String(function, s0) | size
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        size := strsize(s0) <# constant(MAX_PKT_D_SIZE - SEND1STRING_PS)
        sendByte(constant(L1_HEADER_SIZE + SEND1STRING_PS) + size)
        sendByte(!(constant(L1_HEADER_SIZE + SEND1STRING_PS) + size))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND1STRING_PS + size)
        ' Payload
        sendData(s0, size)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND1HANDLE_PS = 1 ' payload size
PUB send1Handle(function, h0)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND1HANDLE_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND1HANDLE_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND1HANDLE_PS)
        ' Payload
        sendByte(h0)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND1HANDLE1BYTE_PS = 1 + 1 ' payload size
PUB send1Handle1Byte(function, h0, b0)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND1HANDLE1BYTE_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND1HANDLE1BYTE_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND1HANDLE1BYTE_PS)
        ' Payload
        sendByte(h0)
        sendByte(b0)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND1HANDLE1WORD_PS = 1 + 2 ' payload size
PUB send1Handle1WORD(function, h0, w0)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND1HANDLE1WORD_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND1HANDLE1WORD_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND1HANDLE1WORD_PS)
        ' Payload
        sendByte(h0)
        sendWord(w0)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND1HANDLE1LONG_PS = 1 + 4 ' payload size
PUB send1Handle1Long(function, h0, l0)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND1HANDLE1LONG_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND1HANDLE1LONG_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND1HANDLE1LONG_PS)
        ' Payload
        sendByte(h0)
        sendLong(l0)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND1HANDLE1STRING_PS = 1 ' payload size
PUB send1Handle1String(function, h0, s0) | size
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        size := strsize(s0) <# constant(MAX_PKT_D_SIZE - SEND1HANDLE1STRING_PS)
        sendByte(constant(L1_HEADER_SIZE + SEND1HANDLE1STRING_PS) + size)
        sendByte(!(constant(L1_HEADER_SIZE + SEND1HANDLE1STRING_PS) + size))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND1HANDLE1STRING_PS + size)
        ' Payload
        sendByte(h0)
        sendData(s0, size)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND2HANDLE_PS = 1 + 1 ' payload size
PUB send2Handle(function, h0, h1)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND2HANDLE_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND2HANDLE_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND2HANDLE_PS)
        ' Payload
        sendByte(h0)
        sendByte(h1)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND2HANDLE1BYTE_PS = 1 + 1 + 1 ' payload size
PUB send2Handle1Byte(function, h0, h1, b0)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND2HANDLE1BYTE_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND2HANDLE1BYTE_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND2HANDLE1BYTE_PS)
        ' Payload
        sendByte(h0)
        sendByte(h1)
        sendByte(b0)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND2HANDLE1WORD_PS = 1 + 1 + 2 ' payload size
PUB send2Handle1Word(function, h0, h1, w0)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND2HANDLE1WORD_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND2HANDLE1WORD_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND2HANDLE1WORD_PS)
        ' Payload
        sendByte(h0)
        sendByte(h1)
        sendWord(w0)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND2HANDLE1LONG_PS = 1 + 1 + 4 ' payload size
PUB send2Handle1Long(function, h0, h1, l0)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND2HANDLE1LONG_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND2HANDLE1LONG_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND2HANDLE1LONG_PS)
        ' Payload
        sendByte(h0)
        sendByte(h1)
        sendLong(l0)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND2HANDLE1STRING_PS = 1 + 1 ' payload size
PUB send2Handle1String(function, h0, h1, s0) | size
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        size := strsize(s0) <# constant(MAX_PKT_D_SIZE - SEND2HANDLE1STRING_PS)
        sendByte(constant(L1_HEADER_SIZE + SEND2HANDLE1STRING_PS) + size)
        sendByte(!(constant(L1_HEADER_SIZE + SEND2HANDLE1STRING_PS) + size))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND2HANDLE1STRING_PS + size)
        ' Payload
        sendByte(h0)
        sendByte(h1)
        sendData(s0, size)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND2HANDLE2LONG_PS = 1 + 1 + 4 + 4 ' payload size
PUB send2Handle2Long(function, h0, h1, l0, l1)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND2HANDLE2LONG_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND2HANDLE2LONG_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND2HANDLE2LONG_PS)
        ' Payload
        sendByte(h0)
        sendByte(h1)
        sendLong(l0)
        sendLong(l1)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND2HANDLE3LONG_PS = 1 + 1 + 4 + 4 + 4 ' payload size
PUB send2Handle3Long(function, h0, h1, l0, l1, l2)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND2HANDLE3LONG_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND2HANDLE3LONG_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND2HANDLE3LONG_PS)
        ' Payload
        sendByte(h0)
        sendByte(h1)
        sendLong(l0)
        sendLong(l1)
        sendLong(l2)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND2HANDLE4LONG_PS = 1 + 1 + 4 + 4 + 4 + 4 ' payload size
PUB send2Handle4Long(function, h0, h1, l0, l1, l2, l3)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND2HANDLE4LONG_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND2HANDLE4LONG_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND2HANDLE4LONG_PS)
        ' Payload
        sendByte(h0)
        sendByte(h1)
        sendLong(l0)
        sendLong(l1)
        sendLong(l2)
        sendLong(l3)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND2HANDLE5LONG_PS = 1 + 1 + 4 + 4 + 4 + 4 + 4 ' payload size
PUB send2Handle5Long(function, h0, h1, l0, l1, l2, l3, l4)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND2HANDLE5LONG_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND2HANDLE5LONG_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND2HANDLE5LONG_PS)
        ' Payload
        sendByte(h0)
        sendByte(h1)
        sendLong(l0)
        sendLong(l1)
        sendLong(l2)
        sendLong(l3)
        sendLong(l4)
        ' Checksum
        sendTXChecksum

' PROTECTED METHOD - PLEASE DO NOT CALL
CON SEND2HANDLE6LONG_PS = 1 + 1 + 4 + 4 + 4 + 4 + 4 + 4 ' payload size
PUB send2Handle6Long(function, h0, h1, l0, l1, l2, l3, l4, l5)
    if waitForCredits
        initTXChecksum
        ' Layer 0 Header
        sendByte(CC_ESCAPE)
        sendByte(CC_NULL)
        sendByte(constant(L1_HEADER_SIZE + SEND2HANDLE6LONG_PS))
        sendByte(constant(!(L1_HEADER_SIZE + SEND2HANDLE6LONG_PS)))
        ' Layer 1 Header
        sendWord(function)
        sendWord(SEND2HANDLE6LONG_PS)
        ' Payload
        sendByte(h0)
        sendByte(h1)
        sendLong(l0)
        sendLong(l1)
        sendLong(l2)
        sendLong(l3)
        sendLong(l4)
        sendLong(l5)
        ' Checksum
        sendTXChecksum

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

CON ' Checksum Mode

    CRC_16_CCITT
    MOD_16_SUM

CON

    DEF_BAUD_RATE = 2400
    MAX_BAUD_RATE = 115200

    TIME_DELAY_PAUSE_0 = (1000 / 10) ' 10 ms
    TIME_DELAY_PAUSE_1 = (1000 / 05) ' 05 ms

PUB init(rxPin, txPin)
    deinit

    serial.rxFlush

    dira[txPin] := true
    waitcnt((clkfreq/TIME_DELAY_PAUSE_0)+cnt)
    outa[txPin] := false
    waitcnt((clkfreq/TIME_DELAY_PAUSE_0)+cnt)
    outa[txPin] := true
    waitcnt((clkfreq/TIME_DELAY_PAUSE_0)+cnt)
    outa[txPin] := false
    waitcnt((clkfreq/TIME_DELAY_PAUSE_0)+cnt)
    outa[txPin] := true
    waitcnt((clkfreq/TIME_DELAY_PAUSE_0)+cnt)
    dira[txPin] := false

    serial.begin(rxPin, txPin, DEF_BAUD_RATE)

    m_coreEnabled := true
    m_txL0Credits := MAX_L0_CREDITS
    m_receiveLayer0State := L0_WAIT_FOR_ESCAPE
    m_txL1Credits := MAX_L0_CREDITS
    m_receiveLayer1State := L1_WAIT_FOR_PACKET_FUNCTION_0

    send1Long(ESCAPE_SET_m_CHECKSUM_MODE, CRC_16_CCITT)
    ifnot waitForResponse
        disableCore
        return

    send1Long(ESCAPE_SET_MAX_CREDITS, MAX_L0_CREDITS)
    ifnot waitForResponse
        disableCore
        return

    send1Long(ESCAPE_SET_MAX_LAYER_0_DATA_SIZE, MAX_LAYER_0_DATA_SIZE)
    ifnot waitForResponse
        disableCore
        return

    send1Long(ESCAPE_SET_BAUD_RATE, MAX_BAUD_RATE)

    waitcnt((clkfreq/TIME_DELAY_PAUSE_1)+cnt)
    serial.end
    waitcnt((clkfreq/TIME_DELAY_PAUSE_0)+cnt)
    serial.begin(rxPin, txPin, MAX_BAUD_RATE)
    waitcnt((clkfreq/TIME_DELAY_PAUSE_1)+cnt)

    ifnot waitForResponse
        disableCore
        return

PUB deinit
    disableCore

PUB hideWindow(windowHandle)
    send1Handle(WINDOW_CLOSE_WINDOW, windowHandle)

PUB hideWindows
    send1(WINDOW_CLOSE_WINDOWS)

PUB deinitWindow(windowHandle)
    send1Handle(WINDOW_DELETE_WINDOW, windowHandle)

PUB deinitWindows
    send1(WINDOW_DELETE_WINDOWS)

PUB clearAll
    deinitWindows

PUB idleTask
    if m_coreEnabled
        receiveLayer0

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
