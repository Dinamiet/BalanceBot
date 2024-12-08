#ifndef _CMDS_H_
#define _CMDS_H_

#include "cli.h"

void                     imu_Cmd(const CLI* cli, const size_t argc, char* const argv[]);
extern const char* const imu_Help[];

void                     gyro_Cmd(const CLI* cli, const size_t argc, char* const argv[]);
extern const char* const gyro_Help[];

void                     accel_Cmd(const CLI* cli, const size_t argc, char* const argv[]);
extern const char* const accel_Help[];

void                     control_Cmd(const CLI* cli, const size_t argc, char* const argv[]);
extern const char* const control_Help[];

void                     motor_Cmd(const CLI* cli, const size_t argc, char* const argv[]);
extern const char* const motor_Help[];

void                     heartbeat_Cmd(const CLI* cli, const size_t argc, char* const argv[]);
extern const char* const heartbeat_Help[];

void                     exit_Cmd(const CLI* cli, const size_t argc, char* const argv[]);
extern const char* const exit_Help[];

#endif
