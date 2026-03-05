#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#include "configblock.h"
#include "crtp.h"
#include "crtp_motor_command_raw.h"
#include "log.h"
#include "motors.h"
#include "param.h"

#define CRTP_PORT_MOTOR_RAW 0x09

static bool isInit = false;
static paramVarId_t motorPowerSetEnableParam;
static paramVarId_t m1Param;
static paramVarId_t m2Param;
static paramVarId_t m3Param;
static paramVarId_t m4Param;
static uint8_t my_id;

struct CrtpMotorRaw {
  uint16_t m1; // in m
  uint16_t m2; // in m
  uint16_t m3; // in m
  uint16_t m4;
} __attribute__((packed));

static void motorRawCallback(CRTPPacket *pk) {
  if (pk->size != sizeof(struct CrtpMotorRaw)) {
    return;
  }

  const struct CrtpMotorRaw *data = (const struct CrtpMotorRaw *)pk->data;

  paramSetInt(m1Param, data->m1);
  paramSetInt(m2Param, data->m2);
  paramSetInt(m3Param, data->m3);
  paramSetInt(m4Param, data->m4);
}

void motorRawInit(void) {
  if (isInit) {
    return;
  }

  uint64_t address = configblockGetRadioAddress();
  my_id = address & 0xFF;

  motorPowerSetEnableParam = paramGetVarId("motorPowerSet", "enable");
  m1Param = paramGetVarId("motorPowerSet", "m1");
  m2Param = paramGetVarId("motorPowerSet", "m2");
  m3Param = paramGetVarId("motorPowerSet", "m3");
  m4Param = paramGetVarId("motorPowerSet", "m4");

  crtpRegisterPortCB(CRTP_PORT_MOTOR_RAW, motorRawCallback);
  isInit = true;
}

bool motorRawTest(void) { return isInit; }
