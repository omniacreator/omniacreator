/***************************************************************************//**
* @file
* Interface Library Main
*
* @version @n 1.0
* @date @n 3/16/2014
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2014 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 3/16/2014
*******************************************************************************/

#ifndef ILCORE_H
#define ILCORE_H

#include "ildependencies.h"

#define HEX 16
#define DEC 10
#define OCT 8
#define BIN 2

typedef uint32 argb;

/// @cond PRIVATE

class ILNode;

class ILData
{

protected:

    ILData()
    {
        m_masterNode = 0;

        m_fifoHead = 0;
        m_fifoTail = 0;

        m_XBufferSize = 0;

        m_receiveBufferSize = 0;
        m_sendBufferSize = 0;

        m_L0StateMachine = L0_WAIT_FOR_ESCAPE;
        m_L0Size = 0;
        m_L0Checksum = 0;

        m_L1StateMachine = L1_WAIT_FOR_PACKET_FUNCTION_0;
        m_L1Function = 0;
        m_L1Size = 0;

        m_L2StateMachine = L2_WAIT_FOR_HANDLE_0;
        m_L2Size = 0;
        m_L2Node = 0;

        m_DCEFatalError = false;
        m_DTEFatalError = false;
        m_L2Response = false;

        m_writeCredits = DEF_CREDITS;
        m_readCredit = 0;

        m_responseBuffer = 0;
        m_responseBufferSize = 0;

        m_checksumMode = CRC_16_CCITT;
    }

    ILNode *m_masterNode;

    uint8 m_fifoHead, m_fifoTail,
    m_fifoBuffer[FIFO_BUFFER_SIZE];

    uint8 m_XBufferSize,
    m_XBuffer[MAX_CREDITS];

    uint8 m_receiveBufferSize,
    m_receiveBuffer[LAYER_0_HEAD_SIZE + MAX_SET_LAYER_0_DATA_SIZE];

    uint8 m_sendBufferSize,
    m_sendBuffer[MAX_SET_LAYER_0_DATA_SIZE];

    enum
    {
        L0_WAIT_FOR_ESCAPE,
        L0_WAIT_FOR_NULL,
        L0_WAIT_FOR_PACKET_SIZE_0,
        L0_WAIT_FOR_PACKET_SIZE_1,
        L0_WAIT_FOR_PACKET_DATA,
        L0_WAIT_FOR_CHECKSUM_0,
        L0_WAIT_FOR_CHECKSUM_1,
        L0_ERROR_STATE
    }
    m_L0StateMachine; uint8 m_L0Size, m_L0Checksum;

    enum
    {
        L1_WAIT_FOR_PACKET_FUNCTION_0,
        L1_WAIT_FOR_PACKET_FUNCTION_1,
        L1_WAIT_FOR_PACKET_SIZE_0,
        L1_WAIT_FOR_PACKET_SIZE_1,
        L1_WAIT_FOR_PACKET_DATA,
        L1_ERROR_STATE
    }
    m_L1StateMachine; uint16 m_L1Function, m_L1Size;

    enum
    {
        L2_WAIT_FOR_HANDLE_0,
        L2_WAIT_FOR_HANDLE_1,
        L2_WAIT_FOR_HANDLE_2,
        L2_WAIT_FOR_PACKET_DATA,
        L2_NON_FATAL_ERROR_STATE,
    }
    m_L2StateMachine; uint16 m_L2Size; ILNode *m_L2Node;

    bool m_DCEFatalError, m_DTEFatalError, m_L2Response;

    int8 m_readCredit, m_writeCredits;

    uint8 *m_responseBuffer; uint16 m_responseBufferSize;

    enum { CRC_16_CCITT, MOD_16_SUM } m_checksumMode;

private:

    ILData(const ILData &);
    ILData &operator=(const ILData &);

    friend class ILNode;
};

/// @endcond PRIVATE

class ILNode
{

public:

    ILNode()
    {
        m_data = 0;

        m_handle = m_handles[0] = m_handles[1] = m_handles[2] = 0;

        m_signalBuffer = 0; m_signalBufferSize = 0;

        m_parent = 0;
        m_children = 0;
        m_next = 0;

        m_enabled = false;
    }

    virtual ~ILNode()
    {
        // See construct()
        // See deconstruct()
    }

    uint8 handle() const
    {
        return m_handle;
    }

    ILNode *parent() const
    {
        return m_parent;
    }

    ILNode *children() const
    {
        return m_children;
    }

    ILNode *next() const
    {
        return m_next;
    }

    bool enabled() const
    {
        return m_enabled;
    }

    enum ILNodeType
    {
        NT_NONE,
        NT_LIBRARY,
        NT_WINDOW_TERMINAL,
        NT_WINDOW_OSCILLOSCOPE,
        NT_WINDOW_TABLE,
        NT_WINDOW_TREE,
        NT_WINDOW_GRAPHICS,
        NT_WINDOW_INTERFACE,
        NT_WINDOW_OSCILLOSCOPE_PLOT_BAR,
        NT_WINDOW_OSCILLOSCOPE_PLOT_CURVE,
        NT_WINDOW_OSCILLOSCOPE_PLOT_GRAPH,
        NT_WINDOW_OSCILLOSCOPE_PLOT_BOX,
        NT_WINDOW_INTERFACE_GBOX_GROUP_BOX,
        NT_WINDOW_INTERFACE_IBOX_PUSH_BUTTON,
        NT_WINDOW_INTERFACE_IBOX_RADIO_BUTTON,
        NT_WINDOW_INTERFACE_IBOX_CHECK_BOX,
        NT_WINDOW_INTERFACE_IBOX_LINE_EDIT,
        NT_WINDOW_INTERFACE_IBOX_INT_SPIN_BOX,
        NT_WINDOW_INTERFACE_IBOX_BIN_SPIN_BOX,
        NT_WINDOW_INTERFACE_IBOX_HEX_SPIN_BOX,
        NT_WINDOW_INTERFACE_IBOX_DOUBLE_SPIN_BOX,
        NT_WINDOW_INTERFACE_IBOX_TIME_EDIT,
        NT_WINDOW_INTERFACE_IBOX_DATE_EDIT,
        NT_WINDOW_INTERFACE_IBOX_DATE_TIME_EDIT,
        NT_WINDOW_INTERFACE_IBOX_SLIDER,
        NT_WINDOW_INTERFACE_IBOX_LABEL,
        NT_WINDOW_INTERFACE_IBOX_PROGRESS_BAR,
        NT_WINDOW_INTERFACE_IBOX_SEPERATOR,
    };

    virtual ILNodeType type() const
    {
        return NT_NONE;
    }

    ILNode *findChild(uint8 handle)
    {
        return findChild(this, handle);
    }

    ILNode *findChild(ILNode *node, uint8 handle)
    {
        if(node && (node = node->children()))
        {
            while(node && (node->handle() != handle))
            {
                node = node->next();
            }
        }

        return node;
    }

    ILNode *findSibling(uint8 handle)
    {
        return findSibling(this, handle);
    }

    ILNode *findSibling(ILNode *node, uint8 handle)
    {
        if(node && (node = node->parent()) && (node = node->children()))
        {
            while(node && (node->handle() != handle))
            {
                node = node->next();
            }
        }

        return node;
    }

protected:

    ILData *m_data; // pointer to data struct

    // Handles Stack //////////////////////////////////////////////////////////

    uint8 m_handle, m_handles[3];

    virtual uint16 handlesSize() const
    {
        return 0;
    }

    // Signal Handling ////////////////////////////////////////////////////////

    uint8 *m_signalBuffer; uint16 m_signalBufferSize;

    virtual void signalBufferCallBack()
    {
        return;
    }

    // Constructor Stuff //////////////////////////////////////////////////////

    ILNode *m_parent; // pointer to parent node
    ILNode *m_children; // pointer to child node
    ILNode *m_next; // pointer to next sibling

    bool m_enabled;

    virtual void construct(ILNode *parent)
    {
        if((m_parent = parent))
        {
            if((m_data = m_parent->m_data))
            {
                if(!m_parent->m_children)
                {
                    m_parent->m_children = this;
                }
                else
                {
                    ILNode *node = m_parent->m_children;

                    if(node->m_handle == 0)
                    {
                        while(node->m_next
                        && (node->m_handle == (node->m_next->m_handle - 1)))
                        {
                            node = node->m_next;
                        }

                        m_next = node->m_next;
                        m_handle = node->m_handle + 1;

                        node->m_next = this;
                    }
                    else
                    {
                        m_next = node;

                        m_parent->m_children = this;
                    }
                }
            }
            else
            {
                m_parent = 0;
            }
        }

        m_enabled = m_data;
    }

    virtual void deconstruct()
    {
        deconstructChildren();

        if(m_parent)
        {
            if(m_parent->m_children == this)
            {
                m_parent->m_children = m_next;
            }
            else
            {
                ILNode *node = m_parent->m_children;

                while(node && (node->m_next != this))
                {
                    node = node->m_next;
                }

                if(node->m_next == this)
                {
                    node->m_next = m_next;
                }
            }
        }

        if(type() != NT_LIBRARY)
        {
            m_data = 0;
        }

        m_handle = m_handles[0] = m_handles[1] = m_handles[2] = 0;

        // m_signalBuffer = m_signalBuffer; // do not zero
        // m_signalBufferSize = m_signalBufferSize; // do not zero

        m_parent = 0;
        m_children = 0;
        m_next = 0;

        m_enabled = false;
    }

    void deconstructChildren()
    {
        ILNode *node = m_children;

        while(node)
        {
            ILNode *temp = node; node = node->m_next; temp->deconstruct();
        }
    }

    // Send Packet ////////////////////////////////////////////////////////////

    bool sendPacket(uint16 function, // Sends a function
                    bool waitForResponse)
    {
        if(!m_enabled)
        {
            return false;
        }

        uint32 sum = handlesSize() * sizeof(uint8);

        if(sum > MAX_LAYER_1_DATA_SIZE)
        {
            return false;
        }

        beginPacket(function, sum);

        send8(m_handles, handlesSize());

        return endPacket(waitForResponse);
    }

    // string stuff //

    bool sendPacket(uint16 function, // Sends multiple strings
                    const char **stringArray, uint16 stringArraySize,
                    bool waitForResponse)
    {
        if((!m_enabled)
        || (!stringArray)
        || (!stringArraySize))
        {
            return false;
        }

        uint32 sum = handlesSize() * sizeof(uint8);

        for(uint16 i = 0; i < stringArraySize; i++)
        {
            if(!stringArray[i])
            {
                return false;
            }

            sum += stringLength(stringArray[i]) + sizeof('\0');
        }

        if(sum > MAX_LAYER_1_DATA_SIZE)
        {
            return false;
        }

        beginPacket(function, sum);

        send8(m_handles, handlesSize());

        for(uint16 i = 0; i < stringArraySize; i++)
        {
            send8(reinterpret_cast<const uint8 *>(stringArray[i]),
                  stringLength(stringArray[i]) + sizeof('\0'));
        }

        return endPacket(waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends a single string
                    const char *stringValue,
                    bool waitForResponse)
    {
        return sendPacket(function,
        &stringValue, sizeof(stringValue) / sizeof(const char *),
        waitForResponse);
    }

    // float struct stuff //

    bool sendPacket(uint16 function, // Sends multiple float32 structs w/ size
                    const float32 *fStructArray, uint16 fStructArraySize,
                    uint16 structSize, // sizeof each struct in longs
                    const char **stringArray, bool waitForResponse)
    {
        return sendPacket(function,
        reinterpret_cast<const int32 *>(fStructArray), fStructArraySize,
        structSize, stringArray, waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends multiple float32 structs w/ size
                    const float32 *fStructArray, uint16 fStructArraySize,
                    uint16 structSize, // sizeof each struct in longs
                    bool waitForResponse)
    {
        return sendPacket(function,
        reinterpret_cast<const int32 *>(fStructArray), fStructArraySize,
        structSize, waitForResponse);
    }

    // int struct stuff //

    bool sendPacket(uint16 function, // Sends multiple int32 structs w/ size
                    const int32 *iStructArray, uint16 iStructArraySize,
                    uint16 structSize, // sizeof each struct in longs
                    const char **stringArray, bool waitForResponse)
    {
        if((!m_enabled)
        || (!iStructArray)
        || (!iStructArraySize)
        || (iStructArraySize > 256)
        || (!structSize)
        || (!stringArray))
        {
            return false;
        }

        uint32 sum = (handlesSize() * sizeof(uint8)) + sizeof(uint8) +
                     (iStructArraySize * structSize * sizeof(int32));

        for(uint16 i = 0; i < iStructArraySize; i++)
        {
            if(!stringArray[i])
            {
                return false;
            }

            sum += stringLength(stringArray[i]) + sizeof('\0');
        }

        if(sum > MAX_LAYER_1_DATA_SIZE)
        {
            return false;
        }

        beginPacket(function, sum);

        send8(m_handles, handlesSize());
        send8(iStructArraySize - 1);

        for(uint16 i = 0; i < iStructArraySize; i++)
        {
            send32(reinterpret_cast<const uint32 *>(iStructArray +
                   (i * structSize)), structSize);

            send8(reinterpret_cast<const uint8 *>(stringArray[i]),
                  stringLength(stringArray[i]) + sizeof('\0'));
        }

        return endPacket(waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends multiple int32 structs w/ size
                    const int32 *iStructArray, uint16 iStructArraySize,
                    uint16 structSize, // sizeof each struct in longs
                    bool waitForResponse)
    {
        if((!m_enabled)
        || (!iStructArray)
        || (!iStructArraySize)
        || (iStructArraySize > 256)
        || (!structSize))
        {
            return false;
        }

        uint32 sum = (handlesSize() * sizeof(uint8)) + sizeof(uint8) +
                     (iStructArraySize * structSize * sizeof(int32));

        if(sum > MAX_LAYER_1_DATA_SIZE)
        {
            return false;
        }

        beginPacket(function, sum);

        send8(m_handles, handlesSize());
        send8(iStructArraySize - 1);

        send32(reinterpret_cast<const uint32 *>(iStructArray),
               iStructArraySize * structSize);

        return endPacket(waitForResponse);
    }

    // float stuff //

    bool sendPacket(uint16 function, // Sends multiple float32s w/ a string
                    const float32 *floatArray, uint16 floatArraySize,
                    const char *stringValue, bool waitForResponse)
    {
        return sendPacket(function,
        reinterpret_cast<const int32 *>(floatArray), floatArraySize,
        stringValue, waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends a single float32 w/ a string
                    float32 floatValue,
                    const char *stringValue, bool waitForResponse)
    {
        return sendPacket(function,
        &floatValue, sizeof(floatValue) / sizeof(float32),
        stringValue, waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends multiple float32s
                    const float32 *floatArray, uint16 floatArraySize,
                    bool waitForResponse)
    {
        return sendPacket(function,
        reinterpret_cast<const int32 *>(floatArray), floatArraySize,
        waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends a single float32
                    float32 floatValue,
                    bool waitForResponse)
    {
        return sendPacket(function,
        &floatValue, sizeof(floatValue) / sizeof(float32),
        waitForResponse);
    }

    // int stuff //

    bool sendPacket(uint16 function, // Sends multiple int32s w/ a string
                    const int32 *intArray, uint16 intArraySize,
                    const char *stringValue, bool waitForResponse)
    {
        if((!m_enabled)
        || (!intArray)
        || (!intArraySize)
        || (!stringValue))
        {
            return false;
        }

        uint32 stringSize = stringLength(stringValue) + sizeof('\0');

        uint32 sum = (handlesSize() * sizeof(uint8)) +
                     (intArraySize * sizeof(int32)) +
                     stringSize;

        if(sum > MAX_LAYER_1_DATA_SIZE)
        {
            return false;
        }

        beginPacket(function, sum);

        send8(m_handles, handlesSize());
        send32(reinterpret_cast<const uint32 *>(intArray), intArraySize);
        send8(reinterpret_cast<const uint8 *>(stringValue), stringSize);

        return endPacket(waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends a single int32 w/ a string
                    int32 intValue,
                    const char *stringValue, bool waitForResponse)
    {
        return sendPacket(function,
        &intValue, sizeof(intValue) / sizeof(int32),
        stringValue, waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends multiple int32s
                    const int32 *intArray, uint16 intArraySize,
                    bool waitForResponse)
    {
        if((!m_enabled)
        || (!intArray)
        || (!intArraySize))
        {
            return false;
        }

        uint32 sum = (handlesSize() * sizeof(uint8)) +
                     (intArraySize * sizeof(int32));

        if(sum > MAX_LAYER_1_DATA_SIZE)
        {
            return false;
        }

        beginPacket(function, sum);

        send8(m_handles, handlesSize());
        send32(reinterpret_cast<const uint32 *>(intArray), intArraySize);

        return endPacket(waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends a single int32
                    int32 intValue,
                    bool waitForResponse)
    {
        return sendPacket(function,
        &intValue, sizeof(intValue) / sizeof(int32),
        waitForResponse);
    }

    // uint16 stuff //

    bool sendPacket(uint16 function, // Sends multiple uint16s w/ a string
                    const uint16 *wordArray, uint16 wordArraySize,
                    const char *stringValue, bool waitForResponse)
    {
        if((!m_enabled)
        || (!wordArray)
        || (!wordArraySize)
        || (!stringValue))
        {
            return false;
        }

        uint32 stringSize = stringLength(stringValue) + sizeof('\0');

        uint32 sum = (handlesSize() * sizeof(uint8)) +
                     (wordArraySize * sizeof(uint16)) +
                     stringSize;

        if(sum > MAX_LAYER_1_DATA_SIZE)
        {
            return false;
        }

        beginPacket(function, sum);

        send8(m_handles, handlesSize());
        send16(wordArray, wordArraySize);
        send8(reinterpret_cast<const uint8 *>(stringValue), stringSize);

        return endPacket(waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends a single uint16 w/ a string
                    uint16 wordValue,
                    const char *stringValue, bool waitForResponse)
    {
        return sendPacket(function,
        &wordValue, sizeof(wordValue) / sizeof(uint16),
        stringValue, waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends multiple uint16s
                    const uint16 *wordArray, uint16 wordArraySize,
                    bool waitForResponse)
    {
        if((!m_enabled)
        || (!wordArray)
        || (!wordArraySize))
        {
            return false;
        }

        uint32 sum = (handlesSize() * sizeof(uint8)) +
                     (wordArraySize * sizeof(uint16));

        if(sum > MAX_LAYER_1_DATA_SIZE)
        {
            return false;
        }

        beginPacket(function, sum);

        send8(m_handles, handlesSize());
        send16(wordArray, wordArraySize);

        return endPacket(waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends a single uint16
                    uint16 wordValue,
                    bool waitForResponse)
    {
        return sendPacket(function,
        &wordValue, sizeof(wordValue) / sizeof(uint16),
        waitForResponse);
    }

    // uint8 stuff //

    bool sendPacket(uint16 function, // Sends multiple uint8s w/ a string
                    const uint8 *byteArray, uint16 byteArraySize,
                    const char *stringValue, bool waitForResponse)
    {
        if((!m_enabled)
        || (!byteArray)
        || (!byteArraySize)
        || (!stringValue))
        {
            return false;
        }

        uint32 stringSize = stringLength(stringValue) + sizeof('\0');

        uint32 sum = (handlesSize() * sizeof(uint8)) +
                     (byteArraySize * sizeof(uint8)) +
                     stringSize;

        if(sum > MAX_LAYER_1_DATA_SIZE)
        {
            return false;
        }

        beginPacket(function, sum);

        send8(m_handles, handlesSize());
        send8(byteArray, byteArraySize);
        send8(reinterpret_cast<const uint8 *>(stringValue), stringSize);

        return endPacket(waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends a single uint8 w/ a string
                    uint8 byteValue,
                    const char *stringValue, bool waitForResponse)
    {
        return sendPacket(function,
        &byteValue, sizeof(byteValue) / sizeof(uint8),
        stringValue, waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends multiple uint8s
                    const uint8 *byteArray, uint16 byteArraySize,
                    bool waitForResponse)
    {
        if((!m_enabled)
        || (!byteArray)
        || (!byteArraySize))
        {
            return false;
        }

        uint32 sum = (handlesSize() * sizeof(uint8)) +
                     (byteArraySize * sizeof(uint8));

        if(sum > MAX_LAYER_1_DATA_SIZE)
        {
            return false;
        }

        beginPacket(function, sum);

        send8(m_handles, handlesSize());
        send8(byteArray, byteArraySize);

        return endPacket(waitForResponse);
    }

    bool sendPacket(uint16 function, // Sends a single uint8
                    uint8 byteValue,
                    bool waitForResponse)
    {
        return sendPacket(function,
        &byteValue, sizeof(byteValue) / sizeof(uint8),
        waitForResponse);
    }

    // Receive Packet /////////////////////////////////////////////////////////

    bool receivePacket(uint16 function, // Sends a function
                       char *response, uint16 responseSize)
    {
        if((!m_enabled)
        || (!response)
        || (!responseSize))
        {
            return false;
        }

        setReceive(response, responseSize);
        bool result = sendPacket(function, true);
        clearReceive();

        return result;
    }

    // string stuff //

    bool receivePacket(uint16 function, // Sends multiple strings
                       const char **stringArray, uint16 stringArraySize,
                       char *response, uint16 responseSize)
    {
        if((!m_enabled)
        || (!response)
        || (!responseSize))
        {
            return false;
        }

        setReceive(response, responseSize);
        bool result = sendPacket(function, stringArray, stringArraySize, true);
        clearReceive();

        return result;
    }

    bool receivePacket(uint16 function, // Sends a single string
                       const char *stringValue,
                       char *response, uint16 responseSize)
    {
        return receivePacket(function,
        &stringValue, sizeof(stringValue) / sizeof(const char *),
        response, responseSize);
    }

    // float stuff //

    bool receivePacket(uint16 function, // Sends multiple strings
                       const char **stringArray, uint16 stringArraySize,
                       float32 *response)
    {
        return receivePacket(function,
        stringArray, stringArraySize, reinterpret_cast<int32 *>(response));
    }

    bool receivePacket(uint16 function, // Sends a single string
                       const char *stringValue,
                       float32 *response)
    {
        return receivePacket(function,
        stringValue, reinterpret_cast<int32 *>(response));
    }

    bool receivePacket(uint16 function, // Sends a function
                       float32 *response)
    {
        return receivePacket(function,
        reinterpret_cast<int32 *>(response));
    }

    // int stuff //

    bool receivePacket(uint16 function, // Sends multiple strings
                       const char **stringArray, uint16 stringArraySize,
                       int32 *response)
    {
        if((!m_enabled)
        || (!response))
        {
            return false;
        }

        setReceive(response);
        bool result = sendPacket(function, stringArray, stringArraySize, true);
        clearReceive();

        return result;
    }

    bool receivePacket(uint16 function, // Sends a single string
                       const char *stringValue,
                       int32 *response)
    {
        return receivePacket(function,
        &stringValue, sizeof(stringValue) / sizeof(const char *),
        response);
    }

    bool receivePacket(uint16 function, // Sends a function
                       int32 *response)
    {
        if((!m_enabled)
        || (!response))
        {
            return false;
        }

        setReceive(response);
        bool result = sendPacket(function, true);
        clearReceive();

        *response = fromLittleEndian(*response); return result;
    }

    // uint16 stuff //

    bool receivePacket(uint16 function, // Sends multiple uint16s
                       const uint16 *wordArray, uint16 wordArraySize,
                       char *response, uint16 responseSize)
    {
        if((!m_enabled)
        || (!response)
        || (!responseSize))
        {
            return false;
        }

        setReceive(response, responseSize);
        bool result = sendPacket(function, wordArray, wordArraySize, true);
        clearReceive();

        return result;
    }

    bool receivePacket(uint16 function, // Sends a single uint16
                       uint16 shortValue,
                       char *response, uint16 responseSize)
    {
        return receivePacket(function,
        &shortValue, sizeof(shortValue) / sizeof(uint16),
        response, responseSize);
    }

    // uint8 stuff //

    bool receivePacket(uint16 function, // Sends multiple uint8s
                       const uint8 *byteArray, uint16 byteArraySize,
                       char *response, uint16 responseSize)
    {
        if((!m_enabled)
        || (!response)
        || (!responseSize))
        {
            return false;
        }

        setReceive(response, responseSize);
        bool result = sendPacket(function, byteArray, byteArraySize, true);
        clearReceive();

        return result;
    }

    bool receivePacket(uint16 function, // Sends a single uint8
                       uint8 byteValue,
                       char *response, uint16 responseSize)
    {
        return receivePacket(function,
        &byteValue, sizeof(byteValue) / sizeof(uint8),
        response, responseSize);
    }

    // I2A Stuff //////////////////////////////////////////////////////////////

    char *itoa(uint32 number, uint16 base, bool isSigned)
    {
        static char itoaBuffer[sizeof('-') + (sizeof(int32)*8) + sizeof('\0')];

        char *string = itoaBuffer + sizeof(itoaBuffer) - sizeof('\0');

        *string = '\0';

        if((02 <= base) && (base <= 36))
        {
            int32 signedNumber = number;

            bool negative = isSigned && (signedNumber < 0) && (base == DEC);

            if(negative)
            {
                number = -signedNumber;
            }

            do
            {
                uint32 temp = number % base; number /= base;

                *--string = (temp < 10) ? temp + '0' : temp + 'A' - 10;
            }
            while(number);

            if(negative)
            {
                *--string = '-';
            }
        }

        return string;
    }

    // F2A Stuff //////////////////////////////////////////////////////////////

private:

    ILNode(const ILNode &);
    ILNode &operator=(const ILNode &);

    friend class ILCore;

    // Receiver Stack /////////////////////////////////////////////////////////

    void queueXBuffer(char data)
    {
        // See serialWrite()

        if(m_data->m_XBufferSize < MAX_CREDITS)
        {
            m_data->m_XBuffer[m_data->m_XBufferSize++] = data;
        }
    }

    void dequeueXBuffer()
    {
        for(uint16 i = 0, j = m_data->m_XBufferSize; i < j; i++)
        {
            serialWrite(m_data->m_XBuffer[i]);
        }

        m_data->m_XBufferSize = 0;
    }

    void receiveL0()
    {
        for(uint16 i = 0,
            j = serialAvailable(); i < j; i++)
        {
            uint8 dataByte = serialRead();

            switch(m_data->m_L0StateMachine)
            {
                case ILData::L0_WAIT_FOR_ESCAPE:
                {
                    switch(dataByte)
                    {
                        case CC_XON:
                        {
                            m_data->m_writeCredits =
                            minMacro(m_data->m_writeCredits + 1, MAX_CREDITS);
                            break;
                        }

                        case CC_XOFF:
                        {
                            m_data->m_DTEFatalError = true;

                            m_data->m_writeCredits =
                            minMacro(m_data->m_writeCredits + 1, MAX_CREDITS);
                            break;
                        }

                        case CC_ESCAPE:
                        {
                            m_data->m_receiveBuffer
                            [m_data->m_receiveBufferSize++] = dataByte;

                            m_data->m_L0StateMachine =
                            ILData::L0_WAIT_FOR_NULL;

                            break;
                        }

                        default:
                        {
                            if(((m_data->m_fifoHead + 1) % FIFO_BUFFER_SIZE)
                            != m_data->m_fifoTail)
                            {
                                m_data->m_fifoBuffer
                                [m_data->m_fifoHead] = dataByte;

                                m_data->m_fifoHead =
                                (m_data->m_fifoHead + 1) %
                                FIFO_BUFFER_SIZE;
                            }

                            break;
                        }
                    }

                    break;
                }

                case ILData::L0_WAIT_FOR_NULL:
                {
                    m_data->m_receiveBuffer
                    [m_data->m_receiveBufferSize++] = dataByte;

                    m_data->m_L0StateMachine =
                    ILData::L0_WAIT_FOR_PACKET_SIZE_0;

                    break;
                }

                case ILData::L0_WAIT_FOR_PACKET_SIZE_0:
                {
                    m_data->m_receiveBuffer
                    [m_data->m_receiveBufferSize++] = dataByte;

                    m_data->m_L0Size = dataByte;

                    if(m_data->m_L0Size > MAX_SET_LAYER_0_DATA_SIZE)
                    {
                        m_data->m_DCEFatalError = true;

                        queueXBuffer(CC_XOFF);

                        m_data->m_receiveBufferSize = 0;
                        m_data->m_L0Size = 0;

                        m_data->m_L0StateMachine =
                        ILData::L0_ERROR_STATE;
                    }
                    else
                    {
                        m_data->m_L0StateMachine =
                        ILData::L0_WAIT_FOR_PACKET_SIZE_1;
                    }

                    break;
                }

                case ILData::L0_WAIT_FOR_PACKET_SIZE_1:
                {
                    m_data->m_receiveBuffer
                    [m_data->m_receiveBufferSize++] = dataByte;

                    uint8 invertedL0Size = ~dataByte;

                    if(m_data->m_L0Size != invertedL0Size)
                    {
                        m_data->m_DCEFatalError = true;

                        queueXBuffer(CC_XOFF);

                        m_data->m_receiveBufferSize = 0;
                        m_data->m_L0Size = 0;

                        m_data->m_L0StateMachine =
                        ILData::L0_ERROR_STATE;
                    }
                    else
                    {
                        m_data->m_L0StateMachine =
                        ILData::L0_WAIT_FOR_PACKET_DATA;

                        if(!m_data->m_L0Size)
                        {
                            m_data->m_L0StateMachine =
                            ILData::L0_WAIT_FOR_CHECKSUM_0;
                        }
                    }

                    break;
                }

                case ILData::L0_WAIT_FOR_PACKET_DATA:
                {
                    m_data->m_receiveBuffer
                    [m_data->m_receiveBufferSize++] = dataByte;

                    m_data->m_L0Size -= 1;

                    if(!m_data->m_L0Size)
                    {
                        m_data->m_L0StateMachine =
                        ILData::L0_WAIT_FOR_CHECKSUM_0;
                    }

                    break;
                }

                case ILData::L0_WAIT_FOR_CHECKSUM_0:
                {
                    m_data->m_L0Checksum =
                    dataByte;

                    m_data->m_L0StateMachine =
                    ILData::L0_WAIT_FOR_CHECKSUM_1;

                    break;
                }

                case ILData::L0_WAIT_FOR_CHECKSUM_1:
                {
                    union { uint16 u; char c[sizeof(uint16)]; } u;

                    u.c[0] = m_data->m_L0Checksum;
                    u.c[1] = dataByte;

                    uint16 L0Checksum = fromLittleEndian(u.u);

                    uint16 expectedL0Checksum;
                    initChecksum(&expectedL0Checksum);
                    updateChecksum(&expectedL0Checksum,
                    m_data->m_receiveBuffer, m_data->m_receiveBufferSize);
                    finiChecksum(&expectedL0Checksum);

                    if(L0Checksum == expectedL0Checksum)
                    {
                        queueXBuffer(CC_XON);

                        receiveL1();

                        m_data->m_receiveBufferSize = 0;
                        m_data->m_L0Checksum = 0;

                        m_data->m_L0StateMachine =
                        ILData::L0_WAIT_FOR_ESCAPE;
                    }
                    else
                    {
                        m_data->m_DCEFatalError = true;

                        queueXBuffer(CC_XOFF);

                        m_data->m_receiveBufferSize = 0;
                        m_data->m_L0Checksum = 0;

                        m_data->m_L0StateMachine =
                        ILData::L0_ERROR_STATE;
                    }

                    break;
                }

                case ILData::L0_ERROR_STATE:
                {
                    break; // wait here until reset
                }
            }
        }
    }

    void receiveL1()
    {
        for(uint16 i = LAYER_0_HEAD_SIZE,
            j = m_data->m_receiveBufferSize; i < j; i++)
        {
            uint8 dataByte = m_data->m_receiveBuffer[i];

            switch(m_data->m_L1StateMachine)
            {
                case ILData::L1_WAIT_FOR_PACKET_FUNCTION_0:
                {
                    m_data->m_L1Function =
                    dataByte;

                    m_data->m_L1StateMachine =
                    ILData::L1_WAIT_FOR_PACKET_FUNCTION_1;

                    break;
                }

                case ILData::L1_WAIT_FOR_PACKET_FUNCTION_1:
                {
                    union { uint16 u; char c[sizeof(uint16)]; } u;

                    u.c[0] = m_data->m_L1Function;
                    u.c[1] = dataByte;

                    m_data->m_L1Function = fromLittleEndian(u.u);

                    m_data->m_L1StateMachine =
                    ILData::L1_WAIT_FOR_PACKET_SIZE_0;

                    break;
                }

                case ILData::L1_WAIT_FOR_PACKET_SIZE_0:
                {
                    m_data->m_L1Size =
                    dataByte;

                    m_data->m_L1StateMachine =
                    ILData::L1_WAIT_FOR_PACKET_SIZE_1;

                    break;
                }

                case ILData::L1_WAIT_FOR_PACKET_SIZE_1:
                {
                    union { uint16 u; char c[sizeof(uint16)]; } u;

                    u.c[0] = m_data->m_L1Size;
                    u.c[1] = dataByte;

                    m_data->m_L1Size = fromLittleEndian(u.u);

                    if(m_data->m_L1Size > MAX_LAYER_1_DATA_SIZE)
                    {
                        m_data->m_DCEFatalError = true;

                        m_data->m_L1Function = 0;
                        m_data->m_L1Size = 0;

                        m_data->m_L1StateMachine =
                        ILData::L1_ERROR_STATE;
                    }
                    else
                    {
                        m_data->m_L1StateMachine =
                        ILData::L1_WAIT_FOR_PACKET_DATA;

                        if(!m_data->m_L1Size)
                        {
                            receiveL2(0, true);
                            m_data->m_L1Function = 0;

                            m_data->m_L1StateMachine =
                            ILData::L1_WAIT_FOR_PACKET_FUNCTION_0;
                        }
                    }

                    break;
                }

                case ILData::L1_WAIT_FOR_PACKET_DATA:
                {
                    receiveL2(dataByte, false);

                    m_data->m_L1Size -= 1;

                    if(!m_data->m_L1Size)
                    {
                        receiveL2(0, true);
                        m_data->m_L1Function = 0;

                        m_data->m_L1StateMachine =
                        ILData::L1_WAIT_FOR_PACKET_FUNCTION_0;
                    }

                    break;
                }

                case ILData::L1_ERROR_STATE:
                {
                    break; // wait here until reset
                }
            }
        }
    }

    void receiveL2(uint8 dataByte, bool resetReceiveL2)
    {
        if(resetReceiveL2)
        {
            m_data->m_L2Response = (m_data->m_L1Function == ESCAPE_ACK);

            if((m_data->m_L1Function == ESCAPE_NAK)
            || (m_data->m_L1Function == ESCAPE_ACK))
            {
                m_data->m_readCredit = maxMacro(m_data->m_readCredit - 1, 0);
            }

            if((m_data->m_L1Function == ESCAPE_RESET_DCE)
            || (m_data->m_L1Function == ESCAPE_RESET_DTE))
            {
                m_data->m_DTEFatalError = true;
            }

            if(m_data->m_L2StateMachine
            == ILData::L2_WAIT_FOR_PACKET_DATA)
            {
                m_data->m_L2Node->signalBufferCallBack();
            }

            m_data->m_L2Size = 0; m_data->m_L2Node = 0;

            m_data->m_L2StateMachine = ILData::L2_WAIT_FOR_HANDLE_0;
        }
        else
        {
            switch(m_data->m_L2StateMachine)
            {
                case ILData::L2_WAIT_FOR_HANDLE_0:
                {
                    switch(m_data->m_L1Function)
                    {
                        case ESCAPE_NAK:
                        case ESCAPE_ACK:
                        {
                            if(m_data->m_responseBuffer && (m_data->m_L2Size <
                               m_data->m_responseBufferSize))
                            {
                                m_data->m_responseBuffer[m_data->m_L2Size++] =
                                dataByte;
                            }

                            break;
                        }

                        case INTERFACE_PUSH_BUTTON_SIGNAL:
                        case INTERFACE_RADIO_BUTTON_SIGNAL:
                        case INTERFACE_CHECK_BOX_SIGNAL:
                        case INTERFACE_LINE_EDIT_SIGNAL:
                        case INTERFACE_INT_SPIN_BOX_SIGNAL:
                        case INTERFACE_BIN_SPIN_BOX_SIGNAL:
                        case INTERFACE_HEX_SPIN_BOX_SIGNAL:
                        case INTERFACE_DOUBLE_SPIN_BOX_SIGNAL:
                        case INTERFACE_TIME_EDIT_SIGNAL:
                        case INTERFACE_DATE_EDIT_SIGNAL:
                        case INTERFACE_DATE_TIME_EDIT_SIGNAL:
                        case INTERFACE_SLIDER_SIGNAL:
                        {
                            if((m_data->m_L2Node =
                                findChild(m_data->m_masterNode, dataByte))
                            && (m_data->m_L2Node->type() ==
                                NT_WINDOW_INTERFACE))
                            {
                                m_data->m_L2StateMachine =
                                ILData::L2_WAIT_FOR_HANDLE_1;
                            }
                            else
                            {
                                m_data->m_L2StateMachine =
                                ILData::L2_NON_FATAL_ERROR_STATE;
                            }

                            break;
                        }

                        default: break;
                    }

                    break;
                }

                case ILData::L2_WAIT_FOR_HANDLE_1:
                {
                    if((m_data->m_L2Node =
                        findChild(m_data->m_L2Node, dataByte))
                    && (m_data->m_L2Node->type() ==
                        NT_WINDOW_INTERFACE_GBOX_GROUP_BOX))
                    {
                        m_data->m_L2StateMachine =
                        ILData::L2_WAIT_FOR_HANDLE_2;
                    }
                    else
                    {
                        m_data->m_L2StateMachine =
                        ILData::L2_NON_FATAL_ERROR_STATE;
                    }

                    break;
                }

                case ILData::L2_WAIT_FOR_HANDLE_2:
                {
                    if((m_data->m_L2Node =
                        findChild(m_data->m_L2Node, dataByte))
                    && ((m_data->m_L2Node->type() ==
                        NT_WINDOW_INTERFACE_IBOX_PUSH_BUTTON)
                    || (m_data->m_L2Node->type() ==
                        NT_WINDOW_INTERFACE_IBOX_RADIO_BUTTON)
                    || (m_data->m_L2Node->type() ==
                        NT_WINDOW_INTERFACE_IBOX_CHECK_BOX)
                    || (m_data->m_L2Node->type() ==
                        NT_WINDOW_INTERFACE_IBOX_LINE_EDIT)
                    || (m_data->m_L2Node->type() ==
                        NT_WINDOW_INTERFACE_IBOX_INT_SPIN_BOX)
                    || (m_data->m_L2Node->type() ==
                        NT_WINDOW_INTERFACE_IBOX_BIN_SPIN_BOX)
                    || (m_data->m_L2Node->type() ==
                        NT_WINDOW_INTERFACE_IBOX_HEX_SPIN_BOX)
                    || (m_data->m_L2Node->type() ==
                        NT_WINDOW_INTERFACE_IBOX_DOUBLE_SPIN_BOX)
                    || (m_data->m_L2Node->type() ==
                        NT_WINDOW_INTERFACE_IBOX_TIME_EDIT)
                    || (m_data->m_L2Node->type() ==
                        NT_WINDOW_INTERFACE_IBOX_DATE_EDIT)
                    || (m_data->m_L2Node->type() ==
                        NT_WINDOW_INTERFACE_IBOX_DATE_TIME_EDIT)
                    || (m_data->m_L2Node->type() ==
                        NT_WINDOW_INTERFACE_IBOX_SLIDER)))
                    {
                        m_data->m_L2StateMachine =
                        ILData::L2_WAIT_FOR_PACKET_DATA;
                    }
                    else
                    {
                        m_data->m_L2StateMachine =
                        ILData::L2_NON_FATAL_ERROR_STATE;
                    }

                    break;
                }

                case ILData::L2_WAIT_FOR_PACKET_DATA:
                {
                    if(m_data->m_L2Node->m_signalBuffer && (m_data->m_L2Size <
                       m_data->m_L2Node->m_signalBufferSize))
                    {
                        m_data->m_L2Node->m_signalBuffer[m_data->m_L2Size++] =
                        dataByte;
                    }

                    break;
                }

                case ILData::L2_NON_FATAL_ERROR_STATE:
                {
                    break;
                }
            }
        }
    }

    // Sender Stack ///////////////////////////////////////////////////////////

    void beginPacket(uint16 function, uint16 size)
    {
        // Start Outstanding Packet Limiter ///////////////////////////////////

        uint32 timeStamp = timeMilliseconds();

        while(m_data->m_readCredit == MAX_OUTSTANDING)
        {
            if((timeMilliseconds() - timeStamp) > READ_CREDIT_TIMEOUT)
            {
                break;
            }
            else
            {
                receiveL0(); dequeueXBuffer();
            }
        }

        // End Outstanding Packet Limiter /////////////////////////////////////

        send16(function); send16(size);
    }

    bool endPacket(bool waitForResponse)
    {
        flushPacket();

        m_data->m_readCredit=minMacro(m_data->m_readCredit+1, MAX_OUTSTANDING);

        #ifdef HALF_DUPLEX_MODE
        {
            waitForResponse = waitForResponse; return waitResponse();
        }
        #else
        {
            return (!waitForResponse) || waitResponse();
        }
        #endif
    }

    void flushPacket()
    {
        if(m_data->m_sendBufferSize)
        {
            union
            {
                struct
                {
                    char c0;
                    char c1;
                    uint8 c2;
                    uint8 c3;
                }
                s;

                uint8 c[LAYER_0_HEAD_SIZE];
            }
            s;

            s.s.c0 = '\e';
            s.s.c1 = '\0';
            s.s.c2 = m_data->m_sendBufferSize;
            s.s.c3 = ~m_data->m_sendBufferSize;

            uint16 checksum; initChecksum(&checksum);

            updateChecksum(&checksum,
                           s.c, sizeof(s.c));

            updateChecksum(&checksum,
                           m_data->m_sendBuffer, m_data->m_sendBufferSize);

            finiChecksum(&checksum); checksum = toLittleEndian(checksum);

            uint32 timeStamp = timeMilliseconds();

            do
            {
                if((timeMilliseconds() - timeStamp) > WRITE_CREDITS_TIMEOUT)
                {
                    m_data->m_writeCredits += 1; // regenerate credits
                }
                else
                {
                    receiveL0(); dequeueXBuffer();
                }
            }
            while(!m_data->m_writeCredits);

            m_data->m_writeCredits -= 1;

            serialWrite(s.c[0]);
            receiveL0();
            serialWrite(s.c[1]);
            receiveL0();
            serialWrite(s.c[2]);
            receiveL0();
            serialWrite(s.c[3]);
            receiveL0();

            for(uint16 i = 0, j = m_data->m_sendBufferSize; i < j; i++)
            {
                serialWrite(m_data->m_sendBuffer[i]);
                receiveL0();
            }

            m_data->m_sendBufferSize = 0;

            union { uint16 u; char c[sizeof(uint16)]; } u; u.u = checksum;

            serialWrite(u.c[0]);
            receiveL0();
            serialWrite(u.c[1]);
            dequeueXBuffer();
        }
    }

    void send8(uint8 data)
    {
        m_data->m_sendBuffer[m_data->m_sendBufferSize++] = data;

        if(m_data->m_sendBufferSize >= MAX_SET_LAYER_0_DATA_SIZE)
        {
            flushPacket();
        }
    }

    void send8(const uint8 *data, uint16 size)
    {
        for(uint16 i = 0; i < size; i++)
        {
            send8(data[i]);
        }
    }

    void send16(uint16 data)
    {
        data = toLittleEndian(data);

        send8(reinterpret_cast<const uint8 *>(&data), sizeof(data));
    }

    void send16(const uint16 *data, uint16 size)
    {
        for(uint16 i = 0; i < size; i++)
        {
            send16(data[i]);
        }
    }

    void send32(uint32 data)
    {
        data = toLittleEndian(data);

        send8(reinterpret_cast<const uint8 *>(&data), sizeof(data));
    }

    void send32(const uint32 *data, uint16 size)
    {
        for(uint16 i = 0; i < size; i++)
        {
            send32(data[i]);
        }
    }

    void send64(uint64 data)
    {
        data = toLittleEndian(data);

        send8(reinterpret_cast<const uint8 *>(&data), sizeof(data));
    }

    void send64(const uint64 *data, uint16 size)
    {
        for(uint16 i = 0; i < size; i++)
        {
            send64(data[i]);
        }
    }

    // Receive Setup //////////////////////////////////////////////////////////

    void setReceive(int32 *response)
    {
        waitResponse();

        m_data->m_responseBuffer = reinterpret_cast<uint8 *>(response);
        m_data->m_responseBufferSize = sizeof(int32);
    }

    void setReceive(float32 *response)
    {
        waitResponse();

        m_data->m_responseBuffer = reinterpret_cast<uint8 *>(response);
        m_data->m_responseBufferSize = sizeof(float32);
    }

    void setReceive(char *response, uint16 responseSize)
    {
        waitResponse();

        if(response && responseSize)
        {
            response[responseSize -= 1] = 0;
        }

        m_data->m_responseBuffer = reinterpret_cast<uint8 *>(response);
        m_data->m_responseBufferSize = responseSize;
    }

    void clearReceive()
    {
        m_data->m_responseBuffer = 0;
        m_data->m_responseBufferSize = 0;
    }

    bool waitResponse()
    {
        uint32 timeStamp = timeMilliseconds();

        while(m_data->m_readCredit)
        {
            if((timeMilliseconds() - timeStamp) > READ_CREDIT_TIMEOUT)
            {
                return false;
            }
            else
            {
                receiveL0(); dequeueXBuffer();
            }
        }

        bool temp = m_data->m_L2Response;
        m_data->m_L2Response = false;
        return temp;
    }

    // Checksum Calculator ////////////////////////////////////////////////////

    void initChecksum(uint16 *checksum)
    {
        *checksum = (m_data->m_checksumMode == ILData::CRC_16_CCITT)
        ? 0xFFFF : 0x0000;
    }

    void updateChecksum(uint16 *checksum, const uint8 *data, uint16 size)
    {
        uint16 temp = *checksum;

        if(m_data->m_checksumMode == ILData::CRC_16_CCITT)
        {
            for(uint16 i = 0; i < size; i++)
            {
                // QByteArray CRC-16-CCITT //

                static const uint16 checksumCRCTable[] =
                {
                    0x0000, 0x1081, 0x2102, 0x3183,
                    0x4204, 0x5285, 0x6306, 0x7387,
                    0x8408, 0x9489, 0xA50A, 0xB58B,
                    0xC60C, 0xD68D, 0xE70E, 0xF78F
                };

                temp = ((temp >> 4) & 0xFFF) ^
                checksumCRCTable[(temp ^ (data[i] >> 0)) & 0xF];

                temp = ((temp >> 4) & 0xFFF) ^
                checksumCRCTable[(temp ^ (data[i] >> 4)) & 0xF];
            }
        }
        else
        {
            for(uint16 i = 0; i < size; i++)
            {
                temp += data[i];
            }
        }

        *checksum = temp;
    }

    void finiChecksum(uint16 *checksum)
    {
        uint16 temp = *checksum;

        *checksum = (m_data->m_checksumMode == ILData::CRC_16_CCITT)
        ? ~temp : temp;
    }
};

class ILCore;

class ILWindow : public ILNode
{

public:

    ILWindow() : ILNode()
    {
        m_visibility = bool();
        m_minimized = bool();
        m_maximized = bool();
        m_fullscreen = bool();
    }

    virtual bool initUndocked(ILCore *parent, const char *name,
                              bool waitForResponse = false) = 0;

    virtual bool initDocked(ILCore *parent, const char *name,
                            bool waitForResponse = false) = 0;

//    virtual bool initDocked(ILWindow *parent, const char *name,
//                            bool waitForResponse = false) = 0;

    virtual bool init(ILCore *parent, const char *name,
                      bool waitForResponse = false) = 0;

    bool setTitle(const char *title,
                  bool waitForResponse = false)
    {
        return sendPacket(WINDOW_SET_WINDOW_TITLE,
        title, waitForResponse);
    }

    bool getTitle(char *title,
                  uint16 size)
    {
        return receivePacket(WINDOW_GET_WINDOW_TITLE,
        title, size);
    }

    bool setMessage(const char *message,
                    bool waitForResponse = false)
    {
        return sendPacket(WINDOW_SET_WINDOW_MESSAGE,
        message, waitForResponse);
    }

    bool getMessage(char *message,
                    uint16 size)
    {
        return receivePacket(WINDOW_GET_WINDOW_MESSAGE,
        message, size);
    }

    bool setVisibility(bool visibility,
                       bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(WINDOW_SET_WINDOW_VISIBLE,
        int32(visibility), waitForResponse)))
        {
            m_visibility = visibility;
        }

        return result;
    }

    bool getVisibility(bool bypassCache = false,
                       bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(WINDOW_GET_WINDOW_VISIBLE,
            &response)))
            {
               m_visibility = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_visibility : bool();
    }

    bool setMinimized(bool minimized,
                      bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(WINDOW_SET_WINDOW_MINIMIZED,
        int32(minimized), waitForResponse)))
        {
            m_minimized = minimized;
        }

        return result;
    }

    bool getMinimized(bool bypassCache = false,
                      bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(WINDOW_GET_WINDOW_MINIMIZED,
            &response)))
            {
               m_minimized = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_minimized : bool();
    }

    bool setMaximized(bool maximized,
                      bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(WINDOW_SET_WINDOW_MAXIMIZED,
        int32(maximized), waitForResponse)))
        {
            m_maximized = maximized;
        }

        return result;
    }

    bool getMaximized(bool bypassCache = false,
                      bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(WINDOW_GET_WINDOW_MAXIMIZED,
            &response)))
            {
               m_maximized = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_maximized : bool();
    }

    bool setFullscreen(bool fullscreen,
                       bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(WINDOW_SET_WINDOW_FULL_SCREEN,
        int32(fullscreen), waitForResponse)))
        {
            m_fullscreen = fullscreen;
        }

        return result;
    }

    bool getFullscreen(bool bypassCache = false,
                       bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(WINDOW_GET_WINDOW_FULL_SCREEN,
            &response)))
            {
               m_fullscreen = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_fullscreen : bool();
    }

protected:

    virtual uint16 handlesSize() const
    {
        return 1;
    }

    virtual void construct(ILNode *parent)
    {
        if(parent)
        {
            ILNode::construct(parent);

            m_handles[0] = m_handle;
        }
    }

private:

    ILWindow(const ILWindow &);
    ILWindow &operator=(const ILWindow &);

    bool m_visibility;
    bool m_minimized;
    bool m_maximized;
    bool m_fullscreen;
};

class ILCore : public ILNode, private ILData
{

public:

    ILCore() : ILNode(), ILData()
    {
        m_data = this;
        m_masterNode = this;

        m_version = int32();
    }

    virtual ILNodeType type() const
    {
        return NT_LIBRARY;
    }

    bool init()
    {
        bool result = !m_enabled;

        if(result)
        {
            construct(0);

            serialResetEvent();
            serialBegin();

            if((result =
            sendPacket(ESCAPE_SET_CHECKSUM_MODE,
            int32(m_checksumMode), true) &&
            sendPacket(ESCAPE_SET_MAX_CREDITS,
            int32(MAX_SET_CREDITS), true) &&
            sendPacket(ESCAPE_SET_MAX_LAYER_0_DATA_SIZE,
            int32(MAX_SET_LAYER_0_DATA_SIZE), true) &&
            sendPacket(ESCAPE_SET_BAUD_RATE,
            int32(MAX_SET_BAUD_RATE), false)))
            {
                serialSetBaudRate(MAX_SET_BAUD_RATE); result = waitResponse();

                // Clear Serial FIFO Buffer

                while(available()) { read(); }
            }

            if(!result)
            {
                serialEnd(); deconstruct();
            }
        }

        return result;
    }

    bool deinit()
    {
        bool result = deinitWindows(true);

        if(result)
        {
            serialEnd(); deconstruct();
        }

        return result;
    }

    bool localError()
    {
        bool temp = m_DCEFatalError;
        m_DCEFatalError = false;
        return temp;
    }

    bool remoteError()
    {
        bool temp = m_DTEFatalError;
        m_DTEFatalError = false;
        return temp;
    }

    bool localReset()
    {
        bool result = m_enabled;

        if(result)
        {
            m_masterNode = this;

            m_fifoHead = 0;
            m_fifoTail = 0;

            m_XBufferSize = 0;

            m_receiveBufferSize = 0;
            m_sendBufferSize = 0;

            m_L0StateMachine = L0_WAIT_FOR_ESCAPE;
            m_L0Size = 0;
            m_L0Checksum = 0;

            m_L1StateMachine = L1_WAIT_FOR_PACKET_FUNCTION_0;
            m_L1Function = 0;
            m_L1Size = 0;

            m_L2StateMachine = L2_WAIT_FOR_HANDLE_0;
            m_L2Size = 0;
            m_L2Node = 0;

            m_DCEFatalError = false;
            m_DTEFatalError = false;
            m_L2Response = false;

            m_writeCredits = DEF_CREDITS;
            m_readCredit = 0;

            m_responseBuffer = 0;
            m_responseBufferSize = 0;

            m_checksumMode = CRC_16_CCITT;
        }

        return result;
    }

    bool remoteReset()
    {
        bool result = localReset();

        if(result)
        {
            serialEnd();
            serialResetEvent();
            serialBegin();

            if((result =
            sendPacket(ESCAPE_SET_CHECKSUM_MODE,
            int32(m_checksumMode), true) &&
            sendPacket(ESCAPE_SET_MAX_CREDITS,
            int32(MAX_SET_CREDITS), true) &&
            sendPacket(ESCAPE_SET_MAX_LAYER_0_DATA_SIZE,
            int32(MAX_SET_LAYER_0_DATA_SIZE), true) &&
            sendPacket(ESCAPE_SET_BAUD_RATE,
            int32(MAX_SET_BAUD_RATE), false)))
            {
                serialSetBaudRate(MAX_SET_BAUD_RATE); result = waitResponse();

                // Clear Serial FIFO Buffer

                while(available()) { read(); }
            }
        }

        return result;
    }

    int32 getVersionInt(bool bypassCache = false,
                        bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(ESCAPE_GET_VERSION_INT,
            &response)))
            {
               m_version = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_version : int32();
    }

    bool getVersionStr(char *version,
                       uint16 size)
    {
        return receivePacket(ESCAPE_GET_VERSION_STR,
        version, size);
    }

    bool openProcess(const char *process,
                     bool waitForResponse = false)
    {
        return sendPacket(ESCAPE_OPEN_PROCESS,
        process, waitForResponse);
    }

    bool openUrl(const char *url,
                 bool waitForResponse = false)
    {
        return sendPacket(ESCAPE_OPEN_URL,
        url, waitForResponse);
    }

    StandardButton showCriticalDialog(const char *title, const char *text,
                                      bool *ok = 0)
    {
        const char *strings[] = {title, text}; int32 result;

        bool okay = receivePacket(ESCAPE_SHOW_CRITICAL_DIALOG,
        strings, sizeof(strings) / sizeof(const char **), &result);

        if(*ok)
        {
            *ok = okay;
        }

        return StandardButton(result);
    }

    StandardButton showInformationDialog(const char *title, const char *text,
                                         bool *ok = 0)
    {
        const char *strings[] = {title, text}; int32 result;

        bool okay = receivePacket(ESCAPE_SHOW_INFORMATION_DIALOG,
        strings, sizeof(strings) / sizeof(const char **), &result);

        if(*ok)
        {
            *ok = okay;
        }

        return StandardButton(result);
    }

    StandardButton showQuestionDialog(const char *title, const char *text,
                                      bool *ok = 0)
    {
        const char *strings[] = {title, text}; int32 result;

        bool okay = receivePacket(ESCAPE_SHOW_QUESTION_DIALOG,
        strings, sizeof(strings) / sizeof(const char **), &result);

        if(*ok)
        {
            *ok = okay;
        }

        return StandardButton(result);
    }

    StandardButton showWarningDialog(const char *title, const char *text,
                                     bool *ok = 0)
    {
        const char *strings[] = {title, text}; int32 result;

        bool okay = receivePacket(ESCAPE_SHOW_WARNING_DIALOG,
        strings, sizeof(strings) / sizeof(const char **), &result);

        if(*ok)
        {
            *ok = okay;
        }

        return StandardButton(result);
    }

    bool hideWindow(ILWindow *window,
                    bool waitForResponse = false)
    {
        bool result = window && window->enabled() && (window->parent()==this);

        if(result)
        {
            result = sendPacket(WINDOW_CLOSE_WINDOW,
            window->handle(), waitForResponse);
        }

        return result;
    }

    bool hideWindows(bool waitForResponse = false)
    {
        return sendPacket(WINDOW_CLOSE_WINDOWS, waitForResponse);
    }

    bool deinitWindow(ILWindow *window,
                      bool waitForResponse = false)
    {
        bool result = window && window->enabled() && (window->parent()==this);

        if(result)
        {
            if((result = sendPacket(WINDOW_DELETE_WINDOW,
            window->handle(), waitForResponse)))
            {
                window->deconstruct();
            }
        }

        return result;
    }

    bool deinitWindows(bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(WINDOW_DELETE_WINDOWS,
        waitForResponse)))
        {
            deconstructChildren();
        }

        return result;
    }

    inline bool clearAll(bool waitForResponse = false)
    {
        return deinitWindows(waitForResponse);
    }

    inline void idleTask()
    {
        receiveL0(); dequeueXBuffer();
    }

    // Serial Code ////////////////////////////////////////////////////////////

    int available()
    {
        int temp = m_fifoHead - m_fifoTail;

        if(temp < 0)
        {
            temp += FIFO_BUFFER_SIZE;
        }

        return temp;
    }

    bool full()
    {
        return available() == (FIFO_BUFFER_SIZE - 1);
    }

    bool empty()
    {
        return !available();
    }

    int peek()
    {
        int c = -1;

        if(!empty())
        {
            c = m_fifoBuffer[m_fifoTail];
        }

        return c;
    }

    int read()
    {
        int c = peek();

        if(c != -1)
        {
            m_fifoTail =
            (m_fifoTail + 1) %
            FIFO_BUFFER_SIZE;
        }

        return c;
    }

    size_t write(char val)
    {
        serialWrite(val); return sizeof(val);
    }

    size_t write(const char *str)
    {
        size_t temp = stringLength(str);

        serialWrite(str, temp); return temp;
    }

    size_t write(const char *buf, size_t len)
    {
        serialWrite(buf, len); return len;
    }

    size_t print(char character)
    {
        return write(character);
    }

    size_t print(const char *text)
    {
        return write(text);
    }

//    size_t print(uint8 value, uint16 base)
//    {
//        return print(itoa(value, base, false));
//    }

//    size_t print(int8 value, uint16 base)
//    {
//        return print(itoa(value, base, true));
//    }

//    size_t print(uint16 value, uint16 base = DEC)
//    {
//        return print(itoa(value, base, false));
//    }

//    size_t print(int16 value, uint16 base = DEC)
//    {
//        return print(itoa(value, base, true));
//    }

//    size_t print(uint32 value, uint16 base = DEC)
//    {
//        return print(itoa(value, base, false));
//    }

    size_t print(int32 value, uint16 base = DEC)
    {
        return print(itoa(value, base, true));
    }

    // TODO: Real float support...
    size_t printF(float32 value)
    {
        return print(int32(value), DEC);
    }

    size_t println(char character)
    {
        return write(character) + write('\n');
    }

    size_t println(const char *text)
    {
        return write(text) + write('\n');
    }

//    size_t println(uint8 value, uint16 base)
//    {
//        return println(itoa(value, base, false));
//    }

//    size_t println(int8 value, uint16 base)
//    {
//        return println(itoa(value, base, true));
//    }

//    size_t println(uint16 value, uint16 base = DEC)
//    {
//        return println(itoa(value, base, false));
//    }

//    size_t println(int16 value, uint16 base = DEC)
//    {
//        return println(itoa(value, base, true));
//    }

//    size_t println(uint32 value, uint16 base = DEC)
//    {
//        return println(itoa(value, base, false));
//    }

    size_t println(int32 value, uint16 base = DEC)
    {
        return println(itoa(value, base, true));
    }

    // TODO: Real float support...
    size_t printlnF(float32 value)
    {
        return println(int32(value), DEC);
    }

private:

    ILCore(const ILCore &);
    ILCore &operator=(const ILCore &);

    int32 m_version;
};

#endif // ILCORE_H

/***************************************************************************//**
* @file
* @par MIT License - TERMS OF USE:
* @n Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* @n
* @n The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* @n
* @n THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*******************************************************************************/
