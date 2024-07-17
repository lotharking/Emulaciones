/*
 * File: prueba.c
 *
 * Code generated for Simulink model 'prueba'.
 *
 * Model version                  : 1.21
 * Simulink Coder version         : 8.13 (R2017b) 24-Jul-2017
 * C/C++ source code generated on : Mon Feb 17 11:16:36 2020
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: AMD->x86-64 (Windows64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "prueba.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmIsMajorTimeStep
# define rtmIsMajorTimeStep(rtm)       (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
# define rtmIsMinorTimeStep(rtm)       (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               ((rtm)->Timing.t)
#endif

#ifndef rtmSetTPtr
# define rtmSetTPtr(rtm, val)          ((rtm)->Timing.t = (val))
#endif

/* Block signals and states (auto storage) */
DW rtDW;

/* External inputs (root inport signals with auto storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with auto storage) */
ExtY rtY;

/* Real-time model */
RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;


//Funciones creadas por el programador set y get
//-------Funciones para asignar entradas-------------

void set_R(double Res){
	rtU.Rs=Res;
}


//-----Funciones para obtener valores de las salidas-------
	
double get_Vd(void){
	return rtY.Vd;
	 
	}
	
double get_Vq(void){
	return rtY.Vq;
	
}

double get_Md(void){
	return rtY.Md;
	
}

double get_Mq(void) {
    return rtY.Mq; 
}

double get_Mabc(void) {
    return rtY.Mabc; 
}

/* Model step function */
void prueba_step(void)
{
  real_T rtb_Memory2;
  real_T rtb_Memory3;
  real_T rtb_Memory;
  real_T rtb_DiscreteTransferFcn1;
  real_T rtb_Add;
  real_T rtb_sinwt2pi3_kv;
  real_T Divide_a;

  /* Memory: '<Root>/Memory2' */
  rtb_Memory2 = rtDW.Memory2_PreviousInput;

  /* Outport: '<Root>/Vd' */
  rtY.Vd = rtb_Memory2;

  /* Memory: '<Root>/Memory3' */
  rtb_Memory3 = rtDW.Memory3_PreviousInput;

  /* Outport: '<Root>/Vq' */
  rtY.Vq = rtb_Memory3;

  /* DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' incorporates:
   *  DiscreteTransferFcn: '<Root>/Discrete Transfer Fcn'
   *  Sum: '<S3>/Add'
   */
  rtDW.DiscreteTransferFcn_tmp = (0.0198 * rtDW.DiscreteTransferFcn_states -
    rtb_Memory2) - (-rtDW.DiscreteTransferFcn_states_d);

  /* Memory: '<Root>/Memory' */
  rtb_Memory = rtDW.Memory_PreviousInput;

  /* DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn' incorporates:
   *  DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn'
   *  Gain: '<Root>/ABC2ab1'
   *  Product: '<Root>/Product3'
   *  Sum: '<Root>/Sum'
   *  Sum: '<S1>/Add'
   */
  rtDW.DiscreteTransferFcn_tmp_j = (((0.5 * rtDW.DiscreteTransferFcn_tmp +
    -0.495 * rtDW.DiscreteTransferFcn_states_d) - rtb_Memory3 *
    376.99111843077515 * 1.0E-5) - rtb_Memory) -
    (-rtDW.DiscreteTransferFcn_states_f);

  /* Memory: '<Root>/Memory1' */
  rtb_Add = rtDW.Memory1_PreviousInput;

  /* Gain: '<S5>/ABC2ab7' incorporates:
   *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
   *  Gain: '<S5>/ABC2ab4'
   *  Product: '<S5>/Product4'
   *  Sum: '<S5>/Sum2'
   *  Sum: '<S5>/Sum4'
   */
  rtb_DiscreteTransferFcn1 = (((5.0 * rtDW.DiscreteTransferFcn_tmp_j + -4.99 *
    rtDW.DiscreteTransferFcn_states_f) - rtb_Add * 376.99111843077515 * 0.025) +
    rtb_Memory2) * 0.013333333333333334;

  /* Saturate: '<S5>/Saturation' */
  if (rtb_DiscreteTransferFcn1 > 1.0) {
    /* Outport: '<Root>/Md' */
    rtY.Md = 1.0;
  } else if (rtb_DiscreteTransferFcn1 < -1.0) {
    /* Outport: '<Root>/Md' */
    rtY.Md = -1.0;
  } else {
    /* Outport: '<Root>/Md' */
    rtY.Md = rtb_DiscreteTransferFcn1;
  }

  /* End of Saturate: '<S5>/Saturation' */

  /* DiscreteTransferFcn: '<S4>/Discrete Transfer Fcn' incorporates:
   *  Constant: '<Root>/Ref Voq'
   *  Sum: '<S4>/Add'
   */
  rtDW.DiscreteTransferFcn_tmp_b = (0.0 - rtb_Memory3) -
    (-rtDW.DiscreteTransferFcn_states_n);

  /* Sum: '<S2>/Add' incorporates:
   *  DiscreteTransferFcn: '<S4>/Discrete Transfer Fcn'
   *  Gain: '<Root>/ABC2ab2'
   *  Product: '<Root>/Product1'
   *  Sum: '<Root>/Sum1'
   */
  rtb_Memory2 = (376.99111843077515 * rtb_Memory2 * 1.0E-5 + (0.5 *
    rtDW.DiscreteTransferFcn_tmp_b + -0.495 * rtDW.DiscreteTransferFcn_states_n))
    - rtb_Add;

  /* DiscreteTransferFcn: '<S2>/Discrete Transfer Fcn' */
  rtDW.DiscreteTransferFcn_tmp_c = rtb_Memory2 -
    (-rtDW.DiscreteTransferFcn_states_o);

  /* Gain: '<S5>/ABC2ab8' incorporates:
   *  DiscreteTransferFcn: '<S2>/Discrete Transfer Fcn'
   *  Gain: '<S5>/ABC2ab3'
   *  Product: '<S5>/Product2'
   *  Sum: '<S5>/Sum3'
   *  Sum: '<S5>/Sum5'
   */
  rtb_Add = ((376.99111843077515 * rtb_Memory * 0.025 + (5.0 *
    rtDW.DiscreteTransferFcn_tmp_c + -4.99 * rtDW.DiscreteTransferFcn_states_o))
             + rtb_Memory3) * 0.013333333333333334;

  /* Saturate: '<S5>/Saturation1' */
  if (rtb_Add > 1.0) {
    /* Outport: '<Root>/Mq' */
    rtY.Mq = 1.0;
  } else if (rtb_Add < -1.0) {
    /* Outport: '<Root>/Mq' */
    rtY.Mq = -1.0;
  } else {
    /* Outport: '<Root>/Mq' */
    rtY.Mq = rtb_Add;
  }

  /* End of Saturate: '<S5>/Saturation1' */

  /* Product: '<S6>/Product' incorporates:
   *  Clock: '<S6>/Clock1'
   */
  rtb_Memory3 = rtM->Timing.t[0] * 376.99111843077515;

  /* Trigonometry: '<S6>/Trigonometric Function' */
  rtb_Memory = sin(rtb_Memory3);

  /* Trigonometry: '<S6>/Trigonometric Function1' */
  rtb_Memory3 = cos(rtb_Memory3);

  /* Sum: '<S12>/Sum' incorporates:
   *  Constant: '<S12>/K1'
   *  Constant: '<S12>/K2'
   *  Product: '<S12>/Product'
   *  Product: '<S12>/Product1'
   */
  rtb_Add = (0.0 - rtb_Memory * 0.5) - rtb_Memory3 * 0.8660254037844386;

  /* Sum: '<S12>/Sum1' incorporates:
   *  Constant: '<S12>/K1'
   *  Constant: '<S12>/K2'
   *  Product: '<S12>/Product2'
   *  Product: '<S12>/Product3'
   */
  rtb_DiscreteTransferFcn1 = rtb_Memory * 0.8660254037844386 - rtb_Memory3 * 0.5;

  /* Sum: '<S12>/Sum2' */
  rtb_sinwt2pi3_kv = (0.0 - rtb_Add) - rtb_Memory;

  /* Fcn: '<S12>/Fcn2' incorporates:
   *  Outport: '<Root>/Md'
   *  Outport: '<Root>/Mq'
   */
  rtb_Memory2 = rtY.Md * rtb_Memory + rtY.Mq * rtb_Memory3;

  /* Fcn: '<S12>/Fcn4' incorporates:
   *  Outport: '<Root>/Md'
   *  Outport: '<Root>/Mq'
   */
  rtb_Add = rtY.Md * rtb_Add + rtY.Mq * rtb_DiscreteTransferFcn1;

  /* Fcn: '<S12>/Fcn5' incorporates:
   *  Outport: '<Root>/Md'
   *  Outport: '<Root>/Mq'
   *  Sum: '<S12>/Sum3'
   */
  rtb_sinwt2pi3_kv = ((0.0 - rtb_DiscreteTransferFcn1) - rtb_Memory3) * rtY.Mq +
    rtY.Md * rtb_sinwt2pi3_kv;

  /* Outport: '<Root>/Mabc' */
  rtY.Mabc[0] = rtb_Memory2;
  rtY.Mabc[1] = rtb_Add;
  rtY.Mabc[2] = rtb_sinwt2pi3_kv;

  /* DiscreteTransferFcn: '<S7>/Discrete Transfer Fcn' incorporates:
   *  Constant: '<Root>/Ref Voq2'
   *  Product: '<Root>/Divide'
   */
  rtDW.DiscreteTransferFcn_tmp_d = (rtb_Memory2 * 300.0 - -1.96 *
    rtDW.DiscreteTransferFcn_states_i[0]) - rtDW.DiscreteTransferFcn_states_i[1];

  /* Product: '<S7>/Divide' incorporates:
   *  Constant: '<Root>/Ref Voq6'
   *  Inport: '<Root>/Rs'
   *  Memory: '<S7>/Memory'
   *  Sum: '<Root>/Sum2'
   */
  rtb_DiscreteTransferFcn1 = rtDW.Memory_PreviousInput_m / (rtU.Rs + 0.001);

  /* DiscreteTransferFcn: '<S7>/Discrete Transfer Fcn1' */
  rtDW.DiscreteTransferFcn1_tmp = (rtb_DiscreteTransferFcn1 - -1.96 *
    rtDW.DiscreteTransferFcn1_states[0]) - rtDW.DiscreteTransferFcn1_states[1];

  /* Sum: '<S7>/Sum2' incorporates:
   *  DiscreteTransferFcn: '<S7>/Discrete Transfer Fcn'
   *  DiscreteTransferFcn: '<S7>/Discrete Transfer Fcn1'
   */
  rtDW.Sum2 = ((0.009901 * rtDW.DiscreteTransferFcn_tmp_d + 0.0198 *
                rtDW.DiscreteTransferFcn_states_i[0]) + 0.009901 *
               rtDW.DiscreteTransferFcn_states_i[1]) + ((-4.95 *
    rtDW.DiscreteTransferFcn1_tmp + 0.0 * rtDW.DiscreteTransferFcn1_states[0]) +
    4.95 * rtDW.DiscreteTransferFcn1_states[1]);

  /* DiscreteTransferFcn: '<S8>/Discrete Transfer Fcn' incorporates:
   *  Constant: '<Root>/Ref Voq2'
   *  Product: '<Root>/Divide'
   */
  rtDW.DiscreteTransferFcn_tmp_dp = (rtb_Add * 300.0 - -1.96 *
    rtDW.DiscreteTransferFcn_states_h[0]) - rtDW.DiscreteTransferFcn_states_h[1];

  /* Product: '<S8>/Divide' incorporates:
   *  Constant: '<Root>/Ref Voq6'
   *  Inport: '<Root>/Rs'
   *  Memory: '<S8>/Memory1'
   *  Sum: '<Root>/Sum2'
   */
  rtb_Memory2 = rtDW.Memory1_PreviousInput_g / (rtU.Rs + 0.001);

  /* DiscreteTransferFcn: '<S8>/Discrete Transfer Fcn1' */
  rtDW.DiscreteTransferFcn1_tmp_j = (rtb_Memory2 - -1.96 *
    rtDW.DiscreteTransferFcn1_states_p[0]) - rtDW.DiscreteTransferFcn1_states_p
    [1];

  /* Sum: '<S8>/Sum2' incorporates:
   *  DiscreteTransferFcn: '<S8>/Discrete Transfer Fcn'
   *  DiscreteTransferFcn: '<S8>/Discrete Transfer Fcn1'
   */
  rtDW.Sum2_n = ((0.009901 * rtDW.DiscreteTransferFcn_tmp_dp + 0.0198 *
                  rtDW.DiscreteTransferFcn_states_h[0]) + 0.009901 *
                 rtDW.DiscreteTransferFcn_states_h[1]) + ((-4.95 *
    rtDW.DiscreteTransferFcn1_tmp_j + 0.0 * rtDW.DiscreteTransferFcn1_states_p[0])
    + 4.95 * rtDW.DiscreteTransferFcn1_states_p[1]);

  /* DiscreteTransferFcn: '<S9>/Discrete Transfer Fcn' incorporates:
   *  Constant: '<Root>/Ref Voq2'
   *  Product: '<Root>/Divide'
   */
  rtDW.DiscreteTransferFcn_tmp_ca = (rtb_sinwt2pi3_kv * 300.0 - -1.96 *
    rtDW.DiscreteTransferFcn_states_b[0]) - rtDW.DiscreteTransferFcn_states_b[1];

  /* Product: '<S9>/Divide' incorporates:
   *  Constant: '<Root>/Ref Voq6'
   *  Inport: '<Root>/Rs'
   *  Memory: '<S9>/Memory1'
   *  Sum: '<Root>/Sum2'
   */
  Divide_a = rtDW.Memory1_PreviousInput_a / (rtU.Rs + 0.001);

  /* DiscreteTransferFcn: '<S9>/Discrete Transfer Fcn1' */
  rtDW.DiscreteTransferFcn1_tmp_ju = (Divide_a - -1.96 *
    rtDW.DiscreteTransferFcn1_states_g[0]) - rtDW.DiscreteTransferFcn1_states_g
    [1];

  /* Sum: '<S9>/Sum2' incorporates:
   *  DiscreteTransferFcn: '<S9>/Discrete Transfer Fcn'
   *  DiscreteTransferFcn: '<S9>/Discrete Transfer Fcn1'
   */
  rtDW.Sum2_l = ((0.009901 * rtDW.DiscreteTransferFcn_tmp_ca + 0.0198 *
                  rtDW.DiscreteTransferFcn_states_b[0]) + 0.009901 *
                 rtDW.DiscreteTransferFcn_states_b[1]) + ((-4.95 *
    rtDW.DiscreteTransferFcn1_tmp_ju + 0.0 * rtDW.DiscreteTransferFcn1_states_g
    [0]) + 4.95 * rtDW.DiscreteTransferFcn1_states_g[1]);

  /* Sum: '<S13>/Sum' incorporates:
   *  Constant: '<S13>/K1'
   *  Constant: '<S13>/K2'
   *  Product: '<S13>/Product'
   *  Product: '<S13>/Product1'
   */
  rtb_sinwt2pi3_kv = (0.0 - rtb_Memory * 0.5) - rtb_Memory3 * 0.8660254037844386;

  /* Sum: '<S13>/Sum1' incorporates:
   *  Constant: '<S13>/K1'
   *  Constant: '<S13>/K2'
   *  Product: '<S13>/Product2'
   *  Product: '<S13>/Product3'
   */
  rtb_Add = rtb_Memory * 0.8660254037844386 - rtb_Memory3 * 0.5;

  /* Fcn: '<S13>/Fcn1' incorporates:
   *  Sum: '<S13>/Sum3'
   */
  rtDW.Vq = (((0.0 - rtb_Add) - rtb_Memory3) * Divide_a +
             (rtb_DiscreteTransferFcn1 * rtb_Memory3 + rtb_Memory2 * rtb_Add)) *
    0.66666666666666663;

  /* Fcn: '<S13>/Fcn2' incorporates:
   *  Sum: '<S13>/Sum2'
   */
  rtDW.Vd = (((0.0 - rtb_sinwt2pi3_kv) - rtb_Memory) * Divide_a +
             (rtb_DiscreteTransferFcn1 * rtb_Memory + rtb_Memory2 *
              rtb_sinwt2pi3_kv)) * 0.66666666666666663;

  /* Sum: '<S14>/Sum' incorporates:
   *  Constant: '<S14>/K1'
   *  Constant: '<S14>/K2'
   *  Product: '<S14>/Product'
   *  Product: '<S14>/Product1'
   */
  rtb_sinwt2pi3_kv = (0.0 - rtb_Memory * 0.5) - rtb_Memory3 * 0.8660254037844386;

  /* Sum: '<S14>/Sum1' incorporates:
   *  Constant: '<S14>/K1'
   *  Constant: '<S14>/K2'
   *  Product: '<S14>/Product2'
   *  Product: '<S14>/Product3'
   */
  rtb_Add = rtb_Memory * 0.8660254037844386 - rtb_Memory3 * 0.5;

  /* Fcn: '<S14>/Fcn1' incorporates:
   *  Sum: '<S14>/Sum3'
   */
  rtDW.Vq_f = (((0.0 - rtb_Add) - rtb_Memory3) * rtDW.Sum2_l + (rtDW.Sum2 *
    rtb_Memory3 + rtDW.Sum2_n * rtb_Add)) * 0.66666666666666663;

  /* Fcn: '<S14>/Fcn2' incorporates:
   *  Sum: '<S14>/Sum2'
   */
  rtDW.Vd_p = (((0.0 - rtb_sinwt2pi3_kv) - rtb_Memory) * rtDW.Sum2_l +
               (rtDW.Sum2 * rtb_Memory + rtDW.Sum2_n * rtb_sinwt2pi3_kv)) *
    0.66666666666666663;

  /* Update for Memory: '<Root>/Memory2' */
  rtDW.Memory2_PreviousInput = rtDW.Vd_p;

  /* Update for Memory: '<Root>/Memory3' */
  rtDW.Memory3_PreviousInput = rtDW.Vq_f;

  /* Update for DiscreteTransferFcn: '<Root>/Discrete Transfer Fcn' */
  rtDW.DiscreteTransferFcn_states = 180.0 - -0.9802 *
    rtDW.DiscreteTransferFcn_states;

  /* Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' */
  rtDW.DiscreteTransferFcn_states_d = rtDW.DiscreteTransferFcn_tmp;

  /* Update for Memory: '<Root>/Memory' */
  rtDW.Memory_PreviousInput = rtDW.Vd;

  /* Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn' */
  rtDW.DiscreteTransferFcn_states_f = rtDW.DiscreteTransferFcn_tmp_j;

  /* Update for Memory: '<Root>/Memory1' */
  rtDW.Memory1_PreviousInput = rtDW.Vq;

  /* Update for DiscreteTransferFcn: '<S4>/Discrete Transfer Fcn' */
  rtDW.DiscreteTransferFcn_states_n = rtDW.DiscreteTransferFcn_tmp_b;

  /* Update for DiscreteTransferFcn: '<S2>/Discrete Transfer Fcn' */
  rtDW.DiscreteTransferFcn_states_o = rtDW.DiscreteTransferFcn_tmp_c;

  /* Update for DiscreteTransferFcn: '<S7>/Discrete Transfer Fcn' */
  rtDW.DiscreteTransferFcn_states_i[1] = rtDW.DiscreteTransferFcn_states_i[0];
  rtDW.DiscreteTransferFcn_states_i[0] = rtDW.DiscreteTransferFcn_tmp_d;

  /* Update for Memory: '<S7>/Memory' */
  rtDW.Memory_PreviousInput_m = rtDW.Sum2;

  /* Update for DiscreteTransferFcn: '<S7>/Discrete Transfer Fcn1' */
  rtDW.DiscreteTransferFcn1_states[1] = rtDW.DiscreteTransferFcn1_states[0];
  rtDW.DiscreteTransferFcn1_states[0] = rtDW.DiscreteTransferFcn1_tmp;

  /* Update for DiscreteTransferFcn: '<S8>/Discrete Transfer Fcn' */
  rtDW.DiscreteTransferFcn_states_h[1] = rtDW.DiscreteTransferFcn_states_h[0];
  rtDW.DiscreteTransferFcn_states_h[0] = rtDW.DiscreteTransferFcn_tmp_dp;

  /* Update for Memory: '<S8>/Memory1' */
  rtDW.Memory1_PreviousInput_g = rtDW.Sum2_n;

  /* Update for DiscreteTransferFcn: '<S8>/Discrete Transfer Fcn1' */
  rtDW.DiscreteTransferFcn1_states_p[1] = rtDW.DiscreteTransferFcn1_states_p[0];
  rtDW.DiscreteTransferFcn1_states_p[0] = rtDW.DiscreteTransferFcn1_tmp_j;

  /* Update for DiscreteTransferFcn: '<S9>/Discrete Transfer Fcn' */
  rtDW.DiscreteTransferFcn_states_b[1] = rtDW.DiscreteTransferFcn_states_b[0];
  rtDW.DiscreteTransferFcn_states_b[0] = rtDW.DiscreteTransferFcn_tmp_ca;

  /* Update for Memory: '<S9>/Memory1' */
  rtDW.Memory1_PreviousInput_a = rtDW.Sum2_l;

  /* Update for DiscreteTransferFcn: '<S9>/Discrete Transfer Fcn1' */
  rtDW.DiscreteTransferFcn1_states_g[1] = rtDW.DiscreteTransferFcn1_states_g[0];
  rtDW.DiscreteTransferFcn1_states_g[0] = rtDW.DiscreteTransferFcn1_tmp_ju;

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  rtM->Timing.t[0] =
    (++rtM->Timing.clockTick0) * rtM->Timing.stepSize0;

  {
    /* Update absolute timer for sample time: [0.0001s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 0.0001, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     */
    rtM->Timing.clockTick1++;
  }
}

/* Model initialize function */
void prueba_initialize(void)
{
  /* Registration code */
  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&rtM->solverInfo, &rtM->Timing.simTimeStep);
    rtsiSetTPtr(&rtM->solverInfo, &rtmGetTPtr(rtM));
    rtsiSetStepSizePtr(&rtM->solverInfo, &rtM->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&rtM->solverInfo, ((const char_T **)
      (&rtmGetErrorStatus(rtM))));
    rtsiSetRTModelPtr(&rtM->solverInfo, rtM);
  }

  rtsiSetSimTimeStep(&rtM->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&rtM->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(rtM, &rtM->Timing.tArray[0]);
  rtM->Timing.stepSize0 = 0.0001;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
