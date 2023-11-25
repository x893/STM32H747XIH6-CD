# Marvell WiFi
**Marvell WiFi** is an sdio wifi driver software running on the RT-Thread real-time operating system.

# Hardware Requirements
ROM: 512KB or above
RAM: 128KB or above

# WiFi Chips Support
1. 88w8782
2. 88w8801
3. 88w8797 (not tested yet)
![88w8782.png][1]
![88w8801.png][2]

# Features
1. STA, UAP mode (can coexist, but no routing)
2. Authentication method: OPEN/WPA-PSK/WPA2-PSK
3. Automatically reconnect
4. Low power mode
5. High speed (stm32f407 driver can reach 2MB/s)

# Compiler
1.Can be compiled using GCC
2.Or add it to the MDK5 project and compile it
(Note: Since the driver contains a large number of gcc features, please add --gnu to Misc Controls under the compiler C/C++ options):
![mdk5(--gnu).png][3]
If you want to compile through scons, please specify toolchains as armcc in rtconfig.py and add it in CFLAGS --gnu):
![armcc(--gnu).png][4]
# Components Dependence
1. sdio driver framework (RT_USING_SDIO)
2. Lwip protocol stack (RT_USING_LWIP)
3. dfs virtual file system (RT_USING_DFS)
4. libc library (RT_USING_LIBC)
5. rt_hw_us_delay(请在bsp中自行实现)
6. sdio host driver (please implement it yourself in bsp)
7. dhcpd protocol (LWIP_USING_DHCPD optional, used in UAP mode)

# Adding Method
Use the env tool officially provided by RT-Thread to obtain the package and generate the project
Or manually download the package and add it to the existing project directory, enable the following macros in rt_config.h, and use the scons tool to regenerate the mdk project:

    #define PKG_USING_WLANMARVELL
    #define MARVELLWIFI_USING_STA

# Initialize
Step 1: hard reset the wifi chip, which can be achieved by connecting to the MCU reset circuit or through GPIO control.
Step 2: If you use component initialization, you only need to enable the following macro:

    #define RT_USING_COMPONENTS_INIT

Otherwise, please manually initialize other components on which this driver depends before calling

    mwifi_system_init();

Note, before using it for the first time, please create a new directory in the target board file system: '/mrvl', and put the firmware in the FwImage folder in the package into this directory.
![firmware.png][5]

When loading the driver, you need to program the firmware for the chip. If the driver is loaded successfully, you can see the following information in the terminal command line:
![initialize.png][6]

# Usage
Provide the command under msh and type

    mwifi

View usage:
![usage.png][7]

## Example

    mwifi mlan0 connect SSID -k PASSWORD    // Connect SSID, password PASSWORD
    mwifi mlan0 disconnect                  // Disconnect
    mwifi mlan0 reassoc -e                  // Enable automatic reconnection
    mwifi mlan0 pwrsave -e                  // Enter low power mode
    mwifi mlan0 scan                        // Scan for nearby hotspots

For more usage, please refer to mwifi.c

# Attention
1. Pay attention to adjusting the task priority when using: tcpip > MOAL_WORKQ > sdio_irq = etx
2. The automatic reconnection function (MARVELLWIFI_STA_REASSOCIATION) is still in the testing stage, please do not use this function in actual projects.

***

If you have any questions during use, you are welcome to initiate issues or email to this address: `jianb1995@hotmail.com`


  [1]: image/88w8782.png "88w8782.png"
  [2]: image/88w8801.png "88w8801.png"
  [3]: image/mdk5(--gnu).png "mdk5(--gnu).png"
  [4]: image/armcc(--gnu).png "armcc(--gnu).png"
  [5]: image/firmware.png "firmware.png"
  [6]: image/initialize.png "initialize.png"
  [7]: image/usage.png "usage.png"
