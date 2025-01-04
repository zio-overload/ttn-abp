/**
 * Full code testing for ABP
 * Mergin RUI3-LowPower-Example & Custom Code
 * 
*/
#include <Arduino.h>
// Debug, set to 0 to disable
#ifndef MY_DEBUG
#define MY_DEBUG 1
#endif

#if MY_DEBUG > 0
#define MYLOG(tag, ...)                  \
	do                                   \
	{                                    \
		if (tag)                         \
			Serial.printf("[%s] ", tag); \
		Serial.printf(__VA_ARGS__);      \
		Serial.printf("\n");             \
	} while (0);                         \
	delay(100)
#else
#define MYLOG(...)
#endif

/*************************************

   LoRaWAN band setting:
     RAK_REGION_EU433
     RAK_REGION_CN470
     RAK_REGION_RU864
     RAK_REGION_IN865
     RAK_REGION_EU868
     RAK_REGION_US915
     RAK_REGION_AU915
     RAK_REGION_KR920
     RAK_REGION_AS923

 *************************************/

// TTN credentials
#define ABP_BAND     (RAK_REGION_AU915)
#define ABP_DEVADDR  {0x26, 0x0B, 0x58, 0xDA}
#define ABP_APPSKEY  {0xE8, 0xFB, 0xCB, 0x62, 0x1F, 0x69, 0x7D, 0x82, 0xFC, 0x77, 0x98, 0x1A, 0xF6, 0x94, 0xFC, 0x68}
#define ABP_NWKSKEY  {0x7F, 0xBA, 0x4F, 0xFD, 0x07, 0x33, 0x8A, 0x2E, 0x0E, 0x12, 0x5B, 0x60, 0x10, 0x34, 0xA6, 0xA8}

// Foward declarations
void send_packet(void);

/** Mask channel value for AUS915-SubChannel-2 by zz*/
uint16_t maskBuff = 0x0002;
/** Packet is confirmed/unconfirmed (Set with AT commands) */
bool g_confirmed_mode = false;
/** If confirmed packet, number or retries (Set with AT commands) */
uint8_t g_confirmed_retry = 0;
/** Data rate  (Set with AT commands) */
uint8_t g_data_rate = 5;
/**  Delay rx1 rx2 windows by zz*/
uint8_t g_rx1_delay = 5;
uint16_t g_rx2_delay = 6000;

/** Rx2 window data rate*/
//uint8_t g_rx2_dr = 8;

/** Tx Power Max Value*/
//uint8_t g_txp = 14;

/** Time interval to send packets in milliseconds */
uint32_t g_send_repeat_time = 20000;


/** Flag if transmit is active, used by some sensors */
volatile bool tx_active = false;

/** fPort to send packages */
uint8_t set_fPort = 2;

/** Payload buffer */
uint8_t g_solution_data[64];



void receiveCallback(SERVICE_LORA_RECEIVE_T *data)
{
	MYLOG("RX-CB", "RX, port %d, DR %d, RSSI %d, SNR %d", data->Port, data->RxDatarate, data->Rssi, data->Snr);
	for (int i = 0; i < data->BufferSize; i++)
	{
		Serial.printf("%02X", data->Buffer[i]);
	}
	Serial.print("\r\n");
	tx_active = false;
}

void sendCallback(int32_t status)
{
	MYLOG("TX-CB", "TX status %d", status);	
	tx_active = false;
}


void setup (){

  Serial.begin(115200, RAK_AT_MODE);

  Serial.println("RAKwireless LoRaWan ABP Example by Ovrld");
  Serial.println("------------------------------------------------------");

  // ABP Device Address MSB first
  uint8_t node_dev_addr[4] = ABP_DEVADDR;
  // ABP Application Session Key
  uint8_t node_app_skey[16] = ABP_APPSKEY;
  // ABP Network Session Key
  uint8_t node_nwk_skey[16] = ABP_NWKSKEY;

  //if (!api.lorawan.nwm.set(1)){
  //  Serial.printf("Network working mode is incorrect! \r \n");
  //  return;
  //}  

  if (!api.lorawan.daddr.set(node_dev_addr, 4)) {
    Serial.printf("LoRaWan ABP - set device addr is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.appskey.set(node_app_skey, 16)) {
    Serial.printf
	("LoRaWan ABP - set application session key is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.nwkskey.set(node_nwk_skey, 16)) {
    Serial.printf
	("LoRaWan ABP - set network session key is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.band.set(ABP_BAND)) {
    Serial.printf("LoRaWan ABP - set band is incorrect! \r\n");
    return;
  }
  if(!api.lorawan.mask.set(&maskBuff)){
    Serial.printf("LoRaWan ABP - Channel mask is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.deviceClass.set(RAK_LORA_CLASS_A)) {
    Serial.printf("LoRaWan ABP - set device class is incorrect! \r\n");
    return;
  }
  // Set the network join mode to ABP
  if (!api.lorawan.njm.set(RAK_LORA_ABP)){
    Serial.printf("LoRaWan ABP - set network join mode is incorrect! \r\n");
    return;
  }

  delay(500);

  if (!api.lorawan.adr.set(true)) {
    Serial.printf("LoRaWan ABP - set adaptive data rate is incorrect! \r\n");
    return;
  }
  
  if (!api.lorawan.rety.set(g_confirmed_retry)) {
    Serial.printf("LoRaWan ABP - set retry times is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.cfm.set(g_confirmed_mode)) {
    Serial.printf("LoRaWan ABP - set confirm mode is incorrect! \r\n");
    return;
  }
  if(!api.lorawan.rx1dl.set(g_rx1_delay)){
    Serial.printf("Rx1 delay - not at the desired value \r \n");
    return;
  }
  if(!api.lorawan.rx2dl.set(g_rx2_delay)){
    Serial.printf("Rx2 delay - not at the desired value \r \n");
    return;
  }
  //if(!api.lorawan.rx2dr.set(g_rx2_dr)){
    //Serial.printf("Rx2 dr - not at the desired value \r \n");
    //return;
  //}

  if (!api.lorawan.dr.set(g_data_rate)) {
    Serial.printf("LoRaWan ABP - set data rate is incorrect! \r\n");
    return;
  }

  //if (!api.lorawan.dcs.set(0)){
  //  Serial.printf("Duty cycle is enabled, check if was intended!!! \r \n");
  //  return;
  //}

    /** Check LoRaWan Status*/
  Serial.printf("Duty cycle is %s\r\n", api.lorawan.dcs.get()? "ON" : "OFF");	// Check Duty Cycle status
  Serial.printf("Packet is %s\r\n", api.lorawan.cfm.get()? "CONFIRMED" : "UNCONFIRMED");	// Check Confirm status
  uint8_t assigned_dev_addr[4] = { 0 };
  api.lorawan.daddr.get(assigned_dev_addr, 4);
  Serial.printf("Device Address is %02X%02X%02X%02X\r\n", assigned_dev_addr[0], assigned_dev_addr[1], assigned_dev_addr[2], assigned_dev_addr[3]);	// Check Device Address
  Serial.printf("Uplink period is %ums\r\n", g_send_repeat_time);
  Serial.println("");

  /** Get values to print for  confirmation later on if necessary */    
  //g_confirmed_mode = api.lorawan.cfm.get();

  //g_confirmed_retry = api.lorawan.rety.get();

  //g_data_rate = api.lorawan.dr.get();


  /** Setup callbacks for recieved and send */

  api.lorawan.registerRecvCallback(receiveCallback);
  api.lorawan.registerSendCallback(sendCallback);

  /** Creater a timer */  
  api.system.timer.create(RAK_TIMER_0,daily_routine,RAK_TIMER_PERIODIC); 
  if (g_send_repeat_time != 0){

        api.system.timer.start(RAK_TIMER_0, g_send_repeat_time, NULL);
  }
}



void daily_routine(void *){

  MYLOG("UPLINK", "Start");

  	// Create payload
	char value = 0x31;
	for (int idx = 0; idx < 16; idx++)
	{
		g_solution_data[idx] = value;
		value++;
	}
	g_solution_data[64];

	// Send the packet
	send_packet();  
}

void loop(){
    api.system.sleep.all();
}

void send_packet(void){

    /** Check if it is LoRaWan*/

    if (api.lorawan.nwm.get() == 1){

        if (api.lorawan.send(16,g_solution_data, set_fPort, g_confirmed_mode, g_confirmed_retry)){

			      MYLOG("UPLINK", "Packet enqueued, size X");
			      tx_active = true;
        }
        else {
            MYLOG("UPLINK", "Send failed");
            tx_active = false;
        }

    }
}
