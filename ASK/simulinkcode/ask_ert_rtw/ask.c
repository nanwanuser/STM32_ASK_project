/*
 * File: ask.c
 *
 * Code generated for Simulink model 'ask'.
 *
 * Model version                  : 1.0
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Tue Dec 24 21:25:07 2024
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "ask.h"
#include "ask_private.h"

/* External outputs (root outports fed by signals with default storage) */
ExtY_ask_T ask_Y;

/* Real-time model */
static RT_MODEL_ask_T ask_M_;
RT_MODEL_ask_T *const ask_M = &ask_M_;

/* Model step function */
void ask_step(void)
{
  /* Outport: '<Root>/Out1' incorporates:
   *  Sin: '<Root>/Sine Wave'
   */
  ask_Y.Out1 = sin(ask_M->Timing.t[0]);

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  ask_M->Timing.t[0] =
    ((time_T)(++ask_M->Timing.clockTick0)) * ask_M->Timing.stepSize0;

  {
    /* Update absolute timer for sample time: [0.001s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 0.001, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     */
    ask_M->Timing.clockTick1++;
  }
}

/* Model initialize function */
void ask_initialize(void)
{
  /* Registration code */
  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&ask_M->solverInfo, &ask_M->Timing.simTimeStep);
    rtsiSetTPtr(&ask_M->solverInfo, &rtmGetTPtr(ask_M));
    rtsiSetStepSizePtr(&ask_M->solverInfo, &ask_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&ask_M->solverInfo, (&rtmGetErrorStatus(ask_M)));
    rtsiSetRTModelPtr(&ask_M->solverInfo, ask_M);
  }

  rtsiSetSimTimeStep(&ask_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&ask_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(ask_M, &ask_M->Timing.tArray[0]);
  ask_M->Timing.stepSize0 = 0.001;
}

/* Model terminate function */
void ask_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
