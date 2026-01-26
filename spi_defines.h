#define CS 7
#define DUMMY 0xFF
#define WIP 0 
//EEPROM
#define WRITE 0x02
#define READ 0x03
#define WRDI 0x04
#define RDSR 0x05
#define WREN 0x06

//SxSPCR register
#define CPHA_BIT 3
#define CPOL_BIT 4
#define MASTER_MODE_BIT 5

//SxSPSR register
#define SPIF_BIT 7
