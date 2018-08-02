/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_socket.c
  * @brief:     The driver of socket by W5500.
  * @version:   V1.0.1 
  * @author:    Lexun
  * @date:      2017-12-06
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */
#ifndef __DRV_SOCKET_H__
#define __DRV_SOCKET_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include "drv_w5500_conf.h"


/* DEFINE --------------------------------------------------------------------*/
#define W5500_SOCK_ANY_PORT_NUM     0xC000;
#define W5500_SOCKET                uint8_t  ///< SOCKET type define for legacy driver

#define W5500_SOCK_OK               1        ///< Result is OK about socket process.
#define W5500_SOCK_BUSY             0        ///< Socket is busy on processing the operation. Valid only Non-block IO Mode.
#define W5500_SOCK_FATAL            -1000    ///< Result is fatal error about socket process.

#define W5500_SOCK_ERROR            0        
#define W5500_SOCKERR_SOCKNUM       (W5500_SOCK_ERROR - 1)     ///< Invalid socket number
#define W5500_SOCKERR_SOCKOPT       (W5500_SOCK_ERROR - 2)     ///< Invalid socket option
#define W5500_SOCKERR_SOCKINIT      (W5500_SOCK_ERROR - 3)     ///< Socket is not initialized
#define W5500_SOCKERR_SOCKCLOSED    (W5500_SOCK_ERROR - 4)     ///< Socket unexpectedly closed.
#define W5500_SOCKERR_SOCKMODE      (W5500_SOCK_ERROR - 5)     ///< Invalid socket mode for socket operation.
#define W5500_SOCKERR_SOCKFLAG      (W5500_SOCK_ERROR - 6)     ///< Invalid socket flag
#define W5500_SOCKERR_SOCKSTATUS    (W5500_SOCK_ERROR - 7)     ///< Invalid socket status for socket operation.
#define W5500_SOCKERR_ARG           (W5500_SOCK_ERROR - 10)    ///< Invalid argrument.
#define W5500_SOCKERR_PORTZERO      (W5500_SOCK_ERROR - 11)    ///< Port number is zero
#define W5500_SOCKERR_IPINVALID     (W5500_SOCK_ERROR - 12)    ///< Invalid IP address
#define W5500_SOCKERR_TIMEOUT       (W5500_SOCK_ERROR - 13)    ///< Timeout occurred
#define W5500_SOCKERR_DATALEN       (W5500_SOCK_ERROR - 14)    ///< Data length is zero or greater than buffer max size.
#define W5500_SOCKERR_BUFFER        (W5500_SOCK_ERROR - 15)    ///< Socket buffer is not enough for data communication.

#define W5500_SOCKFATAL_PACKLEN     (W5500_SOCK_FATAL - 1)     ///< Invalid packet length. Fatal Error.

/*
 * SOCKET FLAG
 */
#define W5500_SF_ETHER_OWN           (Sn_MR_MFEN)        ///< In \ref Sn_MR_MACRAW, Receive only the packet as broadcast, multicast and own packet
#define W5500_SF_IGMP_VER2           (Sn_MR_MC)          ///< In \ref Sn_MR_UDP with \ref SF_MULTI_ENABLE, Select IGMP version 2.   
#define W5500_SF_TCP_NODELAY         (Sn_MR_ND)          ///< In \ref Sn_MR_TCP, Use to nodelayed ack.
#define W5500_SF_MULTI_ENABLE        (Sn_MR_MULTI)       ///< In \ref Sn_MR_UDP, Enable multicast mode.

#if _WIZCHIP_ == 5500
   #define W5500_SF_BROAD_BLOCK         (Sn_MR_BCASTB)   ///< In \ref Sn_MR_UDP or \ref Sn_MR_MACRAW, Block broadcast packet. Valid only in W5500
   #define W5500_SF_MULTI_BLOCK         (Sn_MR_MMB)      ///< In \ref Sn_MR_MACRAW, Block multicast packet. Valid only in W5500
   #define W5500_SF_IPv6_BLOCK          (Sn_MR_MIP6B)    ///< In \ref Sn_MR_MACRAW, Block IPv6 packet. Valid only in W5500
   #define W5500_SF_UNI_BLOCK           (Sn_MR_UCASTB)   ///< In \ref Sn_MR_UDP with \ref SF_MULTI_ENABLE. Valid only in W5500
#endif /* _WIZCHIP_ == 5500 */

#define W5500_SF_IO_NONBLOCK           0x01              ///< Socket nonblock io mode. It used parameter in \ref socket().

/*
 * UDP & MACRAW Packet Infomation
 */
#define W5500_PACK_FIRST               0x80              ///< In Non-TCP packet, It indicates to start receiving a packet.
#define W5500_PACK_REMAINED            0x01              ///< In Non-TCP packet, It indicates to remain a packet to be received.
#define W5500_PACK_COMPLETED           0x00              ///< In Non-TCP packet, It indicates to complete to receive a packet.

/**
 * @ingroup WIZnet_socket_APIs
 * @brief Open a socket.
 * @details Initializes the socket with 'sn' passed as parameter and open.
 *
 * @param sn Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b>.
 * @param protocol Protocol type to operate such as TCP, UDP and MACRAW.
 * @param port Port number to be bined.
 * @param flag Socket flags as \ref SF_ETHER_OWN, \ref SF_IGMP_VER2, \ref SF_TCP_NODELAY, \ref SF_MULTI_ENABLE, \ref SF_IO_NONBLOCK and so on.\n
 *             Valid flags only in W5500 : @ref SF_BROAD_BLOCK, @ref SF_MULTI_BLOCK, @ref SF_IPv6_BLOCK, and @ref SF_UNI_BLOCK.
 * @sa Sn_MR
 *
 * @return @b Success : The socket number @b 'sn' passed as parameter\n
 *         @b Fail    :\n @ref SOCKERR_SOCKNUM     - Invalid socket number\n
 *                        @ref SOCKERR_SOCKMODE    - Not support socket mode as TCP, UDP, and so on. \n
 *                        @ref SOCKERR_SOCKFLAG    - Invaild socket flag.
 */
int8_t  w5500_socket(uint8_t sn, uint8_t protocol, uint16_t port, uint8_t flag);

/**
 * @ingroup WIZnet_socket_APIs
 * @brief Close a socket.
 * @details It closes the socket  with @b'sn' passed as parameter.
 *
 * @param sn Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b>.
 *
 * @return @b Success : @ref SOCK_OK \n
 *         @b Fail    : @ref SOCKERR_SOCKNUM - Invalid socket number
 */
int8_t  w5500_close(uint8_t sn);

/**
 * @ingroup WIZnet_socket_APIs
 * @brief Listen to a connection request from a client.
 * @details It is listening to a connection request from a client.
 * If connection request is accepted successfully, the connection is established. Socket sn is used in passive(server) mode.
 *
 * @param sn Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b>.
 * @return @b Success : @ref SOCK_OK \n
 *         @b Fail    :\n @ref SOCKERR_SOCKINIT   - Socket is not initialized \n
 *                        @ref SOCKERR_SOCKCLOSED - Socket closed unexpectedly.
 */
int8_t  w5500_listen(uint8_t sn);

/**
 * @ingroup WIZnet_socket_APIs
 * @brief Try to connect a server.
 * @details It requests connection to the server with destination IP address and port number passed as parameter.\n
 * @note It is valid only in TCP client mode. 
 *       In block io mode, it does not return until connection is completed.
 *       In Non-block io mode, it return @ref SOCK_BUSY immediatly.
 *
 * @param sn Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b>.
 * @param addr Pointer variable of destination IP address. It should be allocated 4 bytes.
 * @param port Destination port number.
 *
 * @return @b Success : @ref SOCK_OK \n
 * @b Fail    :\n @ref SOCKERR_SOCKNUM   - Invalid socket number\n
 *                @ref SOCKERR_SOCKMODE  - Invalid socket mode\n
 *                @ref SOCKERR_SOCKINIT  - Socket is not initialized\n
 *                @ref SOCKERR_IPINVALID - Wrong server IP address\n
 *                @ref SOCKERR_PORTZERO  - Server port zero\n
 *                @ref SOCKERR_TIMEOUT   - Timeout occurred during request connection\n
 *                @ref SOCK_BUSY         - In non-block io mode, it returned immediatly\n 
 */
int8_t  w5500_connect(uint8_t sn, uint8_t * addr, uint16_t port);

/**
 * @ingroup WIZnet_socket_APIs
 * @brief Try to disconnect a connection socket.
 * @details It sends request message to disconnect the TCP socket 'sn' passed as parameter to the server or client.
 * @note It is valid only in TCP server or client mode. \n
 *       In block io mode, it does not return until disconnection is completed. \n
 *       In Non-block io mode, it return @ref SOCK_BUSY immediatly. \n

 * @param sn Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b>.
 * @return @b Success :   @ref SOCK_OK \n
 *         @b Fail    :\n @ref SOCKERR_SOCKNUM  - Invalid socket number \n
 *                        @ref SOCKERR_SOCKMODE - Invalid operation in the socket \n
 *                        @ref SOCKERR_TIMEOUT  - Timeout occurred \n
 *                        @ref SOCK_BUSY        - Socket is busy.
 */
int8_t  w5500_disconnect(uint8_t sn);

/**
 * @ingroup WIZnet_socket_APIs
 * @brief	Send data to the connected peer in TCP socket.
 * @details It is used to send outgoing data to the connected socket.
 * @note    It is valid only in TCP server or client mode. It can't send data greater than socket buffer size. \n
 *          In block io mode, It doesn't return until data send is completed - socket buffer size is greater than data. \n
 *          In non-block io mode, It return @ref SOCK_BUSY immediatly when socket buffer is not enough. \n
 * @param sn Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b>.
 * @param buf Pointer buffer containing data to be sent.
 * @param len The byte length of data in buf.
 * @return	@b Success : The sent data size \n
 *          @b Fail    : \n @ref SOCKERR_SOCKSTATUS - Invalid socket status for socket operation \n
 *                          @ref SOCKERR_TIMEOUT    - Timeout occurred \n
 *                          @ref SOCKERR_SOCKMODE 	- Invalid operation in the socket \n
 *                          @ref SOCKERR_SOCKNUM    - Invalid socket number \n
 *                          @ref SOCKERR_DATALEN    - zero data length \n
 *                          @ref SOCK_BUSY          - Socket is busy.
 */
int32_t w5500_send(uint8_t sn, uint8_t * buf, uint16_t len);

/**
 * @ingroup WIZnet_socket_APIs
 * @brief	Receive data from the connected peer.
 * @details It is used to read incoming data from the connected socket.\n
 *          It waits for data as much as the application wants to receive.
 * @note    It is valid only in TCP server or client mode. It can't receive data greater than socket buffer size. \n
 *          In block io mode, it doesn't return until data reception is completed - data is filled as <I>len</I> in socket buffer. \n
 *          In non-block io mode, it return @ref SOCK_BUSY immediatly when <I>len</I> is greater than data size in socket buffer. \n
 *
 * @param sn  Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b>.
 * @param buf Pointer buffer to read incoming data.
 * @param len The max data length of data in buf.
 * @return	@b Success : The real received data size \n
 *          @b Fail    :\n
 *                     @ref SOCKERR_SOCKSTATUS - Invalid socket status for socket operation \n
 *                     @ref SOCKERR_SOCKMODE   - Invalid operation in the socket \n
 *                     @ref SOCKERR_SOCKNUM    - Invalid socket number \n
 *                     @ref SOCKERR_DATALEN    - zero data length \n
 *                     @ref SOCK_BUSY          - Socket is busy.
 */
int32_t w5500_recv(uint8_t sn, uint8_t * buf, uint16_t len);

/**
 * @ingroup WIZnet_socket_APIs
 * @brief	Sends datagram to the peer with destination IP address and port number passed as parameter.
 * @details It sends datagram of UDP or MACRAW to the peer with destination IP address and port number passed as parameter.\n
 *          Even if the connectionless socket has been previously connected to a specific address,
 *          the address and port number parameters override the destination address for that particular datagram only.
 * @note    In block io mode, It doesn't return until data send is completed - socket buffer size is greater than <I>len</I>.
 *          In non-block io mode, It return @ref SOCK_BUSY immediatly when socket buffer is not enough.
 *
 * @param sn    Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b>.
 * @param buf   Pointer buffer to send outgoing data.
 * @param len   The byte length of data in buf.
 * @param addr  Pointer variable of destination IP address. It should be allocated 4 bytes.
 * @param port  Destination port number.
 *
 * @return @b Success : The sent data size \n
 *         @b Fail    :\n @ref SOCKERR_SOCKNUM     - Invalid socket number \n
 *                        @ref SOCKERR_SOCKMODE    - Invalid operation in the socket \n
 *                        @ref SOCKERR_SOCKSTATUS  - Invalid socket status for socket operation \n
 *                        @ref SOCKERR_DATALEN     - zero data length \n
 *                        @ref SOCKERR_IPINVALID   - Wrong server IP address\n
 *                        @ref SOCKERR_PORTZERO    - Server port zero\n
 *                        @ref SOCKERR_SOCKCLOSED  - Socket unexpectedly closed \n
 *                        @ref SOCKERR_TIMEOUT     - Timeout occurred \n
 *                        @ref SOCK_BUSY           - Socket is busy. 
 */
int32_t w5500_sendto(uint8_t sn, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t port);

/**
 * @ingroup WIZnet_socket_APIs
 * @brief Receive datagram of UDP or MACRAW
 * @details This function is an application I/F function which is used to receive the data in other then TCP mode. \n
 *          This function is used to receive UDP and MAC_RAW mode, and handle the header as well. 
 *          This function can divide to received the packet data.
 *          On the MACRAW SOCKET, the addr and port parameters are ignored.
 * @note    In block io mode, it doesn't return until data reception is completed - data is filled as <I>len</I> in socket buffer
 *          In non-block io mode, it return @ref SOCK_BUSY immediatly when <I>len</I> is greater than data size in socket buffer.
 *
 * @param sn   Socket number. It should be <b>0 ~ @ref \_WIZCHIP_SOCK_NUM_</b>.
 * @param buf  Pointer buffer to read incoming data.
 * @param len  The max data length of data in buf. 
 *             When the received packet size <= len, receives data as packet sized.
 *             When others, receives data as len.
 * @param addr Pointer variable of destination IP address. It should be allocated 4 bytes.
 *             It is valid only when the first call recvfrom for receiving the packet.
 *             When it is valid, @ref  packinfo[7] should be set as '1' after call @ref getsockopt(sn, SO_PACKINFO, &packinfo).
 * @param port Pointer variable of destination port number.
 *             It is valid only when the first call recvform for receiving the packet.
*             When it is valid, @ref  packinfo[7] should be set as '1' after call @ref getsockopt(sn, SO_PACKINFO, &packinfo).
 *
 * @return	@b Success : This function return real received data size for success.\n
 *          @b Fail    : @ref SOCKERR_DATALEN    - zero data length \n
 *                       @ref SOCKERR_SOCKMODE   - Invalid operation in the socket \n
 *                       @ref SOCKERR_SOCKNUM    - Invalid socket number \n
 *                       @ref SOCKBUSY           - Socket is busy.
 */
int32_t w5500_recvfrom(uint8_t sn, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t *port);


/////////////////////////////
// SOCKET CONTROL & OPTION //
/////////////////////////////
#define W5500_SOCK_IO_BLOCK         0  ///< Socket Block IO Mode in @ref setsockopt().
#define W5500_SOCK_IO_NONBLOCK      1  ///< Socket Non-block IO Mode in @ref setsockopt().

/**
 * @defgroup DATA_TYPE DATA TYPE
 */

/**
 * @ingroup DATA_TYPE
 * @brief The kind of Socket Interrupt.
 * @sa Sn_IR, Sn_IMR, setSn_IR(), getSn_IR(), setSn_IMR(), getSn_IMR()
 */
typedef enum
{
   W5500_SIK_CONNECTED     = (1 << 0),    ///< conntected
   W5500_SIK_DISCONNECTED  = (1 << 1),    ///< disconnected
   W5500_SIK_RECEIVED      = (1 << 2),    ///< data received
   W5500_SIK_TIMEOUT       = (1 << 3),    ///< timeout occured
   W5500_SIK_SENT          = (1 << 4),    ///< send ok
   W5500_SIK_ALL           = 0x1F,        ///< all interrupt
}sockint_kind;

/**
 * @ingroup DATA_TYPE
 * @brief The type of @ref ctlsocket().
 */
typedef enum
{
   W5500_CS_SET_IOMODE,          ///< set socket IO mode with @ref SOCK_IO_BLOCK or @ref SOCK_IO_NONBLOCK
   W5500_CS_GET_IOMODE,          ///< get socket IO mode
   W5500_CS_GET_MAXTXBUF,        ///< get the size of socket buffer allocated in TX memory
   W5500_CS_GET_MAXRXBUF,        ///< get the size of socket buffer allocated in RX memory
   W5500_CS_CLR_INTERRUPT,       ///< clear the interrupt of socket with @ref sockint_kind
   W5500_CS_GET_INTERRUPT,       ///< get the socket interrupt. refer to @ref sockint_kind
   W5500_CS_SET_INTMASK,         ///< set the interrupt mask of socket with @ref sockint_kind
   W5500_CS_GET_INTMASK          ///< get the masked interrupt of socket. refer to @ref sockint_kind
}ctlsock_type;


/**
 * @ingroup DATA_TYPE
 * @brief The type of socket option in @ref setsockopt() or @ref getsockopt()
 */ 
typedef enum
{
   W5500_SO_FLAG,           ///< Valid only in getsockopt(), For set flag of socket refer to <I>flag</I> in @ref socket().
   W5500_SO_TTL,              ///< Set/Get TTL. @ref Sn_TTL  ( @ref setSn_TTL(), @ref getSn_TTL() )
   W5500_SO_TOS,              ///< Set/Get TOS. @ref Sn_TOS  ( @ref setSn_TOS(), @ref getSn_TOS() )
   W5500_SO_MSS,              ///< Set/Get MSS. @ref Sn_MSSR ( @ref setSn_MSSR(), @ref getSn_MSSR() )
   W5500_SO_DESTIP,           ///< Set/Get the destination IP address. @ref Sn_DIPR ( @ref setSn_DIPR(), @ref getSn_DIPR() )
   W5500_SO_DESTPORT,         ///< Set/Get the destionation Port number. @ref Sn_DPORT ( @ref setSn_DPORT(), @ref getSn_DPORT() )
#if _WIZCHIP_ != 5100   
   W5500_SO_KEEPALIVESEND,    ///< Valid only in setsockopt. Manually send keep-alive packet in TCP mode
   #if _WIZCHIP_ > 5200   
      W5500_SO_KEEPALIVEAUTO, ///< Set/Get keep-alive auto transmittion timer in TCP mode
   #endif      
#endif
   W5500_SO_SENDBUF,          ///< Valid only in getsockopt. Get the free data size of Socekt TX buffer. @ref Sn_TX_FSR, @ref getSn_TX_FSR()
   W5500_SO_RECVBUF,          ///< Valid only in getsockopt. Get the received data size in socket RX buffer. @ref Sn_RX_RSR, @ref getSn_RX_RSR()
   W5500_SO_STATUS,           ///< Valid only in getsockopt. Get the socket status. @ref Sn_SR, @ref getSn_SR()
   W5500_SO_REMAINSIZE,       ///< Valid only in getsockopt. Get the remained packet size in other then TCP mode.
   W5500_SO_PACKINFO          ///< Valid only in getsockopt. Get the packet information as @ref PACK_FIRST, @ref PACK_REMAINED, and @ref PACK_COMPLETED in other then TCP mode.
}sockopt_type;

/* Public function -----------------------------------------------------------*/ 
/**
 * @ingroup WIZnet_socket_APIs
 *  @brief Control socket.
 *  @details Control IO mode, Interrupt & Mask of socket and get the socket buffer information.
 *           Refer to @ref ctlsock_type.
 *  @param sn socket number
 *  @param cstype type of control socket. refer to @ref ctlsock_type.
 *  @param arg Data type and value is determined according to @ref ctlsock_type. \n
 *             <table>
 *                  <tr> <td> @b cstype </td> <td> @b data type</td><td>@b value</td></tr>
 *                  <tr> <td> @ref CS_SET_IOMODE \n @ref CS_GET_IOMODE </td> <td> uint8_t </td><td>@ref SOCK_IO_BLOCK @ref SOCK_IO_NONBLOCK</td></tr>
 *                  <tr> <td> @ref CS_GET_MAXTXBUF \n @ref CS_GET_MAXRXBUF </td> <td> uint16_t </td><td> 0 ~ 16K </td></tr>
 *                  <tr> <td> @ref CS_CLR_INTERRUPT \n @ref CS_GET_INTERRUPT \n @ref CS_SET_INTMASK \n @ref CS_GET_INTMASK </td> <td> @ref sockint_kind </td><td> @ref SIK_CONNECTED, etc.  </td></tr> 
 *             </table>
 *  @return @b Success @ref SOCK_OK \n
 *          @b fail    @ref SOCKERR_ARG         - Invalid argument\n
 */
int8_t  w5500_ctlsocket(uint8_t sn, ctlsock_type cstype, void* arg);

/** 
 * @ingroup WIZnet_socket_APIs
 *  @brief set socket options
 *  @details Set socket option like as TTL, MSS, TOS, and so on. Refer to @ref sockopt_type.
 *               
 *  @param sn socket number
 *  @param sotype socket option type. refer to @ref sockopt_type
 *  @param arg Data type and value is determined according to <I>sotype</I>. \n
 *             <table>
 *                  <tr> <td> @b sotype </td> <td> @b data type</td><td>@b value</td></tr> 
 *                  <tr> <td> @ref SO_TTL </td> <td> uint8_t </td><td> 0 ~ 255 </td> </tr>
 *                  <tr> <td> @ref SO_TOS </td> <td> uint8_t </td><td> 0 ~ 255 </td> </tr>
 *                  <tr> <td> @ref SO_MSS </td> <td> uint16_t </td><td> 0 ~ 65535 </td> </tr>
 *                  <tr> <td> @ref SO_DESTIP </td> <td> uint8_t[4] </td><td>  </td></tr> 
 *                  <tr> <td> @ref SO_DESTPORT </td> <td> uint16_t </td><td> 0 ~ 65535 </td></tr> 
 *                  <tr> <td> @ref SO_KEEPALIVESEND </td> <td> null </td><td> null </td></tr> 
 *                  <tr> <td> @ref SO_KEEPALIVEAUTO </td> <td> uint8_t </td><td> 0 ~ 255 </td></tr> 
 *             </table>
 * @return 
 * - @b Success : @ref SOCK_OK \n
 * - @b Fail 
 *  - @ref SOCKERR_SOCKNUM     - Invalid Socket number \n
 *  - @ref SOCKERR_SOCKMODE    - Invalid socket mode \n
 *  - @ref SOCKERR_SOCKOPT     - Invalid socket option or its value \n
 *  - @ref SOCKERR_TIMEOUT     - Timeout occurred when sending keep-alive packet \n
 */
int8_t  w5500_setsockopt(uint8_t sn, sockopt_type sotype, void* arg);

/** 
 * @ingroup WIZnet_socket_APIs
 *  @brief get socket options
 *  @details Get socket option like as FLAG, TTL, MSS, and so on. Refer to @ref sockopt_type
 *  @param sn socket number
 *  @param sotype socket option type. refer to @ref sockopt_type
 *  @param arg Data type and value is determined according to <I>sotype</I>. \n
 *             <table>
 *                  <tr> <td> @b sotype </td> <td>@b data type</td><td>@b value</td></tr>
 *                  <tr> <td> @ref SO_FLAG </td> <td> uint8_t </td><td> @ref SF_ETHER_OWN, etc... </td> </tr>
 *                  <tr> <td> @ref SO_TOS </td> <td> uint8_t </td><td> 0 ~ 255 </td> </tr>
 *                  <tr> <td> @ref SO_MSS </td> <td> uint16_t </td><td> 0 ~ 65535 </td> </tr>
 *                  <tr> <td> @ref SO_DESTIP </td> <td> uint8_t[4] </td><td>  </td></tr> 
 *                  <tr> <td> @ref SO_DESTPORT </td> <td> uint16_t </td><td>  </td></tr> 
 *                  <tr> <td> @ref SO_KEEPALIVEAUTO </td> <td> uint8_t </td><td> 0 ~ 255 </td></tr> 
 *                  <tr> <td> @ref SO_SENDBUF </td> <td> uint16_t </td><td> 0 ~ 65535 </td></tr>  
 *                  <tr> <td> @ref SO_RECVBUF </td> <td> uint16_t </td><td> 0 ~ 65535 </td></tr>  
 *                  <tr> <td> @ref SO_STATUS </td> <td> uint8_t </td><td> @ref SOCK_ESTABLISHED, etc.. </td></tr>  
 *                  <tr> <td> @ref SO_REMAINSIZE </td> <td> uint16_t </td><td> 0~ 65535 </td></tr>
 *                  <tr> <td> @ref SO_PACKINFO </td> <td> uint8_t </td><td> @ref PACK_FIRST, etc... </td></tr>
 *             </table>
 * @return 
 * - @b Success : @ref SOCK_OK \n
 * - @b Fail 
 *  - @ref SOCKERR_SOCKNUM     - Invalid Socket number \n
 *  - @ref SOCKERR_SOCKOPT     - Invalid socket option or its value \n
 *  - @ref SOCKERR_SOCKMODE    - Invalid socket mode \n
 * @note
 *   The option as PACK_REMAINED and SO_PACKINFO is valid only in NON-TCP mode and after call @ref recvfrom(). \n
 *   When SO_PACKINFO value is PACK_FIRST and the return value of recvfrom() is zero, 
 *   This means the zero byte UDP data(UDP Header only) received.
 */
int8_t  w5500_getsockopt(uint8_t sn, sockopt_type sotype, void* arg);

#endif   // _SOCKET_H_