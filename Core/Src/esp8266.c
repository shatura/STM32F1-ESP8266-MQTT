#include "esp8266.h"

static WIFI_StateTypeDef wifi_state = _OFFLINE;
static WIFI_StateTypeDef trans_state = _UNKNOWN_STATE;
static uint8_t wifi_config_step = 0;
/*********************ПРОВЕРКА ОТВЕТА ESP***********************************/
WIFI_StateTypeDef esp8266_CheckRespond(uint8_t *str) {
	u1_printf("(DBG:) usart2 ПРОВЕРКА ОТВЕТА:\r\n%s\r\n", USART2_RxBUF);
	if (strstr((const char*) USART2_RxBUF, (const char*) str) != NULL) {
		u1_printf("(DBG:) ПОДЛКЮЧЕНИЕ УСПЕШНО\r\n");
		memset(USART2_RxBUF, 0, USART2_MAX_RECVLEN);
		return _MATCHOK;
	}

	memset(USART2_RxBUF, 0, USART2_MAX_RECVLEN);
	return _MATCHERROR;
}
/*********************ПРОВЕРКА ОТВЕТА ESP***********************************/
/*********************ПРОВЕРКА  ОТПРАВКИ ESP***********************************/
WIFI_StateTypeDef esp8266_TransmitCmd(uint8_t *cmd, uint8_t *ack,
		uint32_t waitms, uint8_t newline) {
	int timeout = waitms;
	uint8_t check = 0;
	memset(USART2_RxBUF, 0, USART2_MAX_RECVLEN);
	u1_printf("\r\n(DBG:) ПОПРОБУЙТЕ ОТПРАВИТЬ cmd: %s\r\n", cmd);
	if (newline == 0)
		u2_transmit("%s", cmd);      // ОТПРАВКА ПО КОНСОЛИ ЧЕРЕЗ USART2
	else
		u2_transmit("%s\r\n", cmd); // ОТПРАВКА ПО КОНСОЛИ ЧЕРЕЗ USART2

	u1_printf("(DBG:) ОЖИДАНИЕ ОТВЕТА \r\n");
	while (timeout--) {
		// u1_printf("%d ", timeout);
		// finish dma receive
		if (USART2_RecvEndFlag == 1) {
			check = esp8266_CheckRespond(ack);
			if (check == _MATCHOK) {
				u1_printf("(DBG:)ЦИКЛ ЗАВЕРШЕН\r\n");
			}

			USART2_RxLen = 0;
			USART2_RecvEndFlag = 0;
			HAL_UART_Receive_DMA(&huart2, USART2_RxBUF, USART2_MAX_RECVLEN);

			break;
		}
		HAL_Delay(1);
	}

	if (check == _MATCHERROR) {
		u1_printf("\r\n(DBG:) Cmd ОШИБКА\r\n");
		return check;
	}
	if (timeout <= 0) {
		u1_printf("(DBG:) ЗАГРУЗКА ЗАВЕРШЕНА\r\n");
		u1_printf("\r\n(DBG:) ПЕРЕРЫВ\r\n");
		return _TIMEOUT;
	}
	u1_printf("(DBG:) УСПЕШНО\r\n");
	return _SUCCEED;
}
/*********************ПРОВЕРКА ОТПРАВКИ ESP***********************************/
/******************************АППАРАТНЫЙ СБРОС****************************************/
WIFI_StateTypeDef esp8266_HardwareReset(uint32_t waitms) {
	int timeout = waitms;
	WIFI_RST_Enable();
	HAL_Delay(500);
	WIFI_RST_Disable();
	while (timeout--) {
		if (USART2_RecvEndFlag == 1) {

			u1_printf("(DBG:) АППАРАТНЫЙ СБРОС OK!\r\n");
			HAL_Delay(100);

			USART2_RxLen = 0;
			USART2_RecvEndFlag = 0;
			HAL_UART_Receive_DMA(&huart2, USART2_RxBUF, USART2_MAX_RECVLEN);

			return _SUCCEED;
		}
		HAL_Delay(1);
	}
	if (timeout <= 0) {
		u1_printf("(DBG:) ЗАГРУЗКА ЗАВЕРШЕНА\r\n");
		u1_printf("\r\n(DBG:) ПЕРЕРЫВ\r\n");
		return _TIMEOUT;
	}

	return _UNKNOWN_ERROR;
}
/*************************АППАРАТНЫЙ СБРОС************************************/
/*************************ПОДКЛЮЧЕНИЕ К ТОЧКЕ ДОСТУПА****************************************************/
WIFI_StateTypeDef esp8266_ConnectAP() {
	uint16_t cmd_len = strlen(AP_SSID) + strlen(AP_PSWD) + 30;
	uint8_t *cmd = (uint8_t*) malloc(cmd_len * sizeof(uint8_t));
	memset(cmd, 0, cmd_len);
	sprintf((char*) cmd, "AT+CWJAP_CUR=\"%s\",\"%s\"", AP_SSID, AP_PSWD); // ПОДКЛЮЧЕНИЕ К ТОЧКЕ ДОСТУПА

	if (esp8266_TransmitCmd(cmd, (uint8_t*) "WIFI ПОДКЛЮЧЕН",
			3 * ESP8266_MAX_TIMEOUT, WITH_NEWLINE) == _SUCCEED)
		wifi_state = _ONLINE;
	else
		wifi_state = _OFFLINE;

	return wifi_state;
}
/*************************ПОДКЛЮЧЕНИЕ К ТОЧКЕ ДОСТУПА****************************************************/
/*********************************УСТАНОВКА TCP СОЕДИНЕНИЯ**************************************************/
WIFI_StateTypeDef esp8266_ConnectServer() {
	uint16_t cmd_len = strlen(IpServer) + strlen(ServerPort) + 30;
	uint8_t *cmd = (uint8_t*) malloc(cmd_len * sizeof(uint8_t));
	memset(cmd, 0, cmd_len);
	sprintf((char*) cmd, "AT+CIPSTART=\"TCP\",\"%s\",%s", IpServer, ServerPort);

	if (esp8266_TransmitCmd(cmd, (uint8_t*) "ПОДКЛЮЧЕН", 3 * ESP8266_MAX_TIMEOUT,
	WITH_NEWLINE) == _SUCCEED)
		wifi_state = _CONNECTED;
	else
		wifi_state = _DISCONNECTED;

	return wifi_state;
}

WIFI_StateTypeDef esp8266_SetUpTCPConnection() {
	uint8_t retry_count = 0;

	/* СБРОС esp8266 */
	u1_printf("(DBG:) ПОПЫТКА СБРОСИТЬ esp8266\r\n");
	wifi_config_step++;
	while (esp8266_HardwareReset(500) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) ПОПЫТКА СБРОСА НЕ УДАЛАСЬ\r\n");
			retry_count = 0;
			trans_state = _UNKNOWN_STATE;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(2000);		// ЖДЕМ 2 СЕКУНДУ
	retry_count = 0;		//СБРОС СЧЁТЧИК

	/* ОТКЛЮЧЕНИЕ ПРОЗРАЧНОЙ ПЕРЕДАЧИ */
	u1_printf("(DBG:) ПОПЫТКА ОТКЛЮЧЕНИЕ ПРОЗРАЧНОЙ ПЕРЕДАЧИ\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd(TRANS_QUIT_CMD, TRANS_QUIT_CMD,
	ESP8266_MAX_TIMEOUT,
	WITHOUT_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1500);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) ОТКЛЮЧЕНИЕ ПРОЗРАЧНОЙ ПЕРЕДАЧИ ОТКЛОНЕНА\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	trans_state = _TRANS_DISABLE;
	HAL_Delay(1500);
	retry_count = 0;		// СБРОС СЧЁТЧИК

	/* ЗАКРЫТИЕ echo */
	u1_printf("(DBG:) ПОПЫТКА ЗАКРЫТЬ echo\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "ATE0", OK_ACK, 500, WITH_NEWLINE)
			!= _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) ЗАКРЫТЬ echo ОТКЛЕНЕН\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		//СБРОС СЧЁТЧИК

	/* УСТАНОВКА WIFI РЕЖИМА 0:AP 1:STA 2:AP+STA */
	u1_printf("(DBG:) ПОПЫТКА УСТАНОВИТЬ WIFI РЕЖИМ\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CWMODE_CUR=1", OK_ACK, 500,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) УСТАНОВКА WIFI РЕЖИМА ОТКЛОНЕНА\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		//СБРОС СЧЁТЧИК

	/* ОТКЛЮЧЕНО АВТОПОДКЛЮЧЕНИЕ */
	u1_printf("(DBG:) ПОПЫТКА ОТКЛЮЧИТЬ АВТОПОДКЛЮЧЕНИЕ\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CWAUTOCONN=0", OK_ACK, 500,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) ОТКЛЮЧЕНИЕ АВТОПОДКЛЮЧЕНИЕ ОТКЛОНЕНО\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// СБРОС СЧЁТЧИК

	/* Connect to AP(Wifi) */
	u1_printf("(DBG:) Trying to connect to AP\r\n");
	wifi_config_step++;
	while (esp8266_ConnectAP() != _ONLINE) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) Connect to AP failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(1000);
	retry_count = 0;		// СБРОС СЧЁТЧИК

	/* ПОПЫТКА ПОЛУЧЕНИЯ AP ИНФОРМАЦИИ */
	if (wifi_state == _ONLINE) {
		while (esp8266_TransmitCmd((uint8_t*) "AT+CWJAP_CUR?", OK_ACK,
		ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED) {
			retry_count++;
			HAL_Delay(1000);
			if (retry_count > ESP8266_MAX_RETRY_TIME / 2) {
				u1_printf("(DBG:) ПОЛУЧЕНИЕ AP СООБЩЕНИЕ ОТКЛОНЕНО\r\n");
				u1_printf(
						"(DBG:) ПРОЦЕСС ПОДКЛЮЧЕНИЯ СЕРВЕРА НЕ БУДЕТ ЗАВЕРШЕН");
				retry_count = 0;
				wifi_config_step--;
				break;
			}
		}
	}

	HAL_Delay(1000);
	retry_count = 0;		// СБРОС СЧЁТЧИКА

	/* ПОПЫТКА УСТАНОВИТЬ IP ИНФОРМАЦИИ */
	if (wifi_state == _ONLINE) {
		while (esp8266_TransmitCmd((uint8_t*) "	AT+CIPSTA_CUR?", OK_ACK,
		ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED) {
			retry_count++;
			HAL_Delay(1000);
			if (retry_count > ESP8266_MAX_RETRY_TIME / 2) {
				u1_printf("(DBG:) УСТАНОВКА IP ИНФОРМАЦИИ ОТКЛОНЕНА\r\n");
				u1_printf(
						"(DBG:) ПРОЦЕСС ПОДКЛЮЧЕНИЯ СЕРВЕРА НЕ БУДЕТ ЗАВЕРШЕН");
				retry_count = 0;
				wifi_config_step--;
				break;
			}
		}
	}

	HAL_Delay(1000);
	retry_count = 0;		// СБРОС СЧЁТЧИКА

	/* УСТАНОВКА  DHCP */
	u1_printf("(DBG:) Trying to set DHCP mode\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CWDHCP_CUR=1,1", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) УСТАНОВКА DHCP МОДЕЛИ ОТКЛОНЕНА\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(1000);
	retry_count = 0;

	/* УСТАНОВКА ОДИНОЧНОГО СОЕДИНЕНИЕ */
	u1_printf("(DBG:) ПОПЫТКА УСТАНОВКИ ОДИНОЧНОГО СОЕДИНЕНИЯ\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CIPMUX=0", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) УСТАНОВКА ОДИНОЧНОГО СОЕДИНЕНИЯ ОТКЛОНЕНО\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(1000);
	retry_count = 0;

	/* УСТАНОВКА РЕЖИМА ПРОЗРАЧНОЙ ПЕРЕДАЧИ */
	u1_printf("(DBG:) ПОПЫТКА УСТАНОВИТЬ РЕЖИМ ПРОЗРАЧНОЙ ПЕРЕДАЧИ\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CIPMODE=1", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) ПРОЦЕСС ПОДКЛЮЧЕНИЯ СЕРВЕРА НЕ БУДЕТ ЗАВЕРШЕН\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(1000);
	retry_count = 0;

	/* ПОДКЛЮЧЕНИЕ К TCP СЕРВЕРУ */
	u1_printf("(DBG:) ПОПЫТКА ПОДКЛЮЧИТЬСЯ К TCP СЕРВЕРУ\r\n");
	wifi_config_step++;
	while (esp8266_ConnectServer() != _CONNECTED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) ПОДКЛЮЧЕНИЕ К TCP СЕРВЕРУ ОТКЛОНЕНО\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(1000);
	retry_count = 0;

	/* ВКЛЮЧЕНИЕ ПЕРЕДАЧИ ДАННЫХ (ПРОЗРАЧНЫЙ РЕЖИМ ПЕРЕДАЧИ) */
	u1_printf("(DBG:) ПОПЫТКА ВКЛЮЧЕНИЕЯ ПЕРЕДАЧИ ДАННЫХ\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CIPSEND", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			u1_printf("(DBG:) УСТАНОВКА ПРОЗРАЧНОГО РЕЖИМА ПЕРЕДАЧИ ОТКЛОНЕНО\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	trans_state = _TRANS_ENBALE;
	HAL_Delay(1000);
	retry_count = 0;

	/* send test msg */
//	u1_printf("Test msg is sending to TCP Server\r\n");
//	u2_transmit("This msg means TCP connection has been set up\r\n");
//	u1_printf("Test msg has been send to TCP Server\r\n");
	return _SUCCEED;
}
/*********************************УСТАНОВКА TCP СОЕДИНЕНИЯ**************************************************/
