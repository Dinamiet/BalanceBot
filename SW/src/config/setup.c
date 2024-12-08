#include "config.h"
#include "datapacket.h"
#include "drivers.h"
#include "messages.h"
#include "tasks.h"
#include "utilities.h"

DataPacket* config;

static DataPacket        dp;
static SchedulerTask     heartbeatTask;
static SchedulerTask     configReceiveTask;
static DataPacketMessage configMessages[] = {
		{ MESSAGE_GYRO_CALIBRATION,  gyroCalibrationRequest_Handler},
		{      MESSAGE_GYRO_OFFSET,       gyroOffsetRequest_Handler},
		{MESSAGE_ACCEL_CALIBRATION, accelCalibrationRequest_Handler},
		{     MESSAGE_ACCEL_OFFSET,      accelOffsetRequest_Handler},
		{    MESSAGE_SUBSCRIBE_IMU,     subscribeIMURequest_Handler},
		{      MESSAGE_IMU_ENABLED,              imuEnabled_Handler},
		{      MESSAGE_CONTROL_SET,              controlSet_Handler},
		{   MESSAGE_CONTROL_ENABLE,           controlEnable_Handler},
		{      MESSAGE_MOTORS_MOVE,              motorsMove_Handler},
		{  MESSAGE_MOTORS_COOLDOWN,          motorsCooldown_Handler},
		{    MESSAGE_MOTORS_ENABLE,            motorsEnable_Handler},
		{ MESSAGE_MOTORS_SMALLSTEP,         motorsSmallStep_Handler},
		{MESSAGE_HEARTBEAT_ENABLED,          heartbeatEnable_Hander},
		{						0,							NULL}
};

static size_t config_read(void* data, const size_t size);
static size_t config_write(const void* data, const size_t size);
static void   heartbeat_Function(void* _);
static void   configReceive_Function(void* _);

static size_t config_read(void* data, const size_t size) { return UART_Read(uart, data, size); }

static size_t config_write(const void* data, const size_t size) { return UART_Write(uart, data, size); }

static void heartbeat_Function(void* _)
{
	(void)_;

	DataPacket_Send(config, MESSAGE_HEARTBEAT, NULL, 0);
}

static void configReceive_Function(void* _)
{
	(void)_;

	DataPacket_Receive(config);
}

void Setup_Config()
{
	config = &dp;
	DataPacket_Init(config, configMessages, config_read, config_write);
	Scheduler_CreateRecurringTask(taskScheduler, &heartbeatTask, TASK_HEARTBEAT, heartbeat_Function, NULL, TASK_HEARTBEAT_PERIOD);
	Scheduler_CreateRecurringTask(taskScheduler, &configReceiveTask, TASK_CONFIG_RECEIVE, configReceive_Function, NULL, TASK_CONFIG_RECEIVE_PERIOD);
}

void Heartbeat_Enabled(bool enable)
{
	if (enable)
		Scheduler_Activate(&heartbeatTask);
	else
		Scheduler_Deactivate(&heartbeatTask);
}
