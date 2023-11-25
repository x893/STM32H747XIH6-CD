# Marvell WLAN Host Driver
88W8801 WLAN basic host driver, passed debugging on STM32F4 series microcontroller. The driver runs based on callback functions and does not require an operating system.
## Interface List
- SDIO (connects wireless module, uses DMA to **simplify transfer**)
- SPI (SPI1 is connected to the onboard Flash for firmware access, SPI2 is connected to TFT\-LCD for debugging)
- GPIO and other miscellaneous
## Device List
- Microcontroller *STM32F407*
- Wireless module *88W8801*
- Onboard Flash *W25Q16DV*
- 128\*160 TFT\-LCD driver chip *ST7735S*
## Module Description
- *88W8801* For details, see [Reference / Reference](# Reference --reference)
    - *Flash* Read and write Flash, access firmware
    - *lwIP* TCP/IP protocol stack (version number 2\.1\.3), add DHCP server and NAT module, [View transplantation/modification content](88w8801/lwip/FILES)
    - *SDIO* device communication interface implementation
    - *Core* processes packets and calls related modules
    - *Wrapper* Simple interface encapsulation
- *Debug/ST7735* Output debugging information to TFT\-LCD
- *SysTime* time management (using SysTick interrupt, no overflow control)
- For other instructions, please view the README in each directory.
## Configuration files
- wireless module [88w8801\.h](88w8801/88w8801.h)
- lwIP [lwipopts\.h](88w8801/lwip/include/lwipopts.h)
- TFT\-LCD [st7735\.h](st7735/st7735.h)
- For other configurations, please check the directories at each level. *.h
## Example Project
Please use *STM32CubeMX* to open [CaptureHandler\.ioc](example/CaptureHandler.ioc), and after generating the project, separate the remaining contents in the *example* directory (excluding [STM32F407VET6\.pdf](example/STM32F407VET6.pdf))* *Overwrite** the original file, and finally **create** a *Module* directory and add all modules.
- The code is simple and for reference only
- The example project is configured with DCMI and some GPIO, and relevant modules can be added to implement image transmission.
- The software version numbers are as follows:
    - Keil uVision: *MDK\-ARM 5\.38a*

      STM32F4xx\_DFP: *2\.17\.0*
    - STM32CubeMX: *6\.8\.0*

      STM32Cube FW\_F4: *1\.27\.1*
## Reference
1. STMicroelectronics, [Datasheet DS8626](https://www.st.com/content/ccc/resource/technical/document/datasheet/ef/92/76/6d/bb/c2/4f/f7/DM00037051.pdf/files/DM00037051.pdf/jcr:content/translations/en.DM00037051.pdf)
2. STMicroelectronics, [Reference Manual RM0090](https://www.st.com/content/ccc/resource/technical/document/reference_manual/3d/6d/5a/66/b4/99/40/d4/DM00031020.pdf/files/DM00031020.pdf/jcr:content/translations/en.DM00031020.pdf)
3. IoT Wireless Link, [Marvell 88W8801 SDIO Wi\-Fi](https://github.com/sj15712795029/stm32_sdio_wifi_marvell8801_wifi)
4. SD Card Association, [SDIO Simplified Specification \(Version 3\.00\)](https://www.sdcard.org/cms/wp-content/themes/sdcard-org/dl.php?f=PartE1_SDIO_Simplified_Specification_Ver3.00.pdf)
5. Winbond, [W25Q16DV Datasheet](https://www.winbond.com/resource-files/w25q16dv%20revk%2005232016%20doc.pdf)
6. IEEE, [IEEE Std 802\.11\-2016](https://ieeexplore.ieee.org/document/7786995)
7. Swedish Institute of Computer Science, [lwIP \- A Lightweight TCP/IP Stack](https://savannah.nongnu.org/projects/lwip/)
8. RT\-Thread Development Team, [lwIP NAT Component](https://github.com/RT-Thread/rt-thread/tree/master/components/net/lwip-nat)
