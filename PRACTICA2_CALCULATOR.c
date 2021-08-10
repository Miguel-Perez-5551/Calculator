/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    PRACTICA2_CALCULATOR.c
 * @brief   Application entry point.
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

 //Estructuras
typedef struct{
 unsigned char bCurrentState;
 unsigned char bPreviousState;
}tStateMachine;

typedef struct{
unsigned char bFirstData;
unsigned char bSecondData;
unsigned char bOperator;
short wResult;
short waDataBuffer[5];
unsigned char bOperationCounter;
}tCalcData;

//void fnYourOwnName (tStateMachine *Apuntador_1, tCalcData *Apuntador_2);

void fnInitialState(tStateMachine *Apuntador_1, tCalcData *Apuntador_2);
void fnWaitFirstDataState(tStateMachine *Apuntador_1, tCalcData *Apuntador_2);
void fnWaitOperatorState(tStateMachine * Apuntador_1, tCalcData *Apuntador_2);
void fnWaitSecondDataState(tStateMachine *Apuntador_1, tCalcData *Apuntador_2);
void fnExecuteOperationState(tStateMachine *Apuntador_1, tCalcData *Apuntador_2);
void fnSaveResultStat(tStateMachine *Apuntador_1, tCalcData *Apuntador_2);
void fnCalculateAverageState(tStateMachine *Apuntador_1, tCalcData *Apuntador_2);
void fnResetState(tStateMachine *Apuntador_1, tCalcData *Apuntador_2);

// Variables globales

unsigned char Valor_1 = 0;
unsigned char Valor_2 = 0;
unsigned char Operacion = 0;

int main(void) {

	tStateMachine STATE;
	tCalcData CALC;

	tStateMachine *Apuntador_1;
	tCalcData *Apuntador_2;

	*Apuntador_1 = STATE;
	*Apuntador_2 = CALC;

	Valor_1 = 12;
	Valor_2 = 5;
	Operacion = '-';

	Apuntador_1 -> bCurrentState = 0;

	void (*Op[])(tStateMachine*, tCalcData*) = {fnInitialState, fnWaitFirstDataState, fnWaitOperatorState, fnWaitSecondDataState, fnExecuteOperationState, fnSaveResultStat, fnCalculateAverageState, fnResetState};

	while(1){
	(*Op[Apuntador_1->bCurrentState])(&Apuntador_1, &Apuntador_2);
	}
    return 0 ;
}


void fnInitialState(tStateMachine *Apuntador_1,tCalcData *Apuntador_2)//Apuntador_1->bCurrentState 0
{
	unsigned char i = 0;
	Apuntador_2->bFirstData = 0;
	Apuntador_2->bSecondData = 0;
	Apuntador_2->bOperator = 0;
	Apuntador_2->wResult = 0;
	Apuntador_2->bOperationCounter = 0;
	Apuntador_1->bCurrentState = 0;
	Apuntador_1->bPreviousState = 0;

	for(i = 0; i < 5; i++)
	{
		Apuntador_2->waDataBuffer[i] = 0;
	}
	Apuntador_1->bPreviousState = Apuntador_1->bCurrentState;
	Apuntador_1->bCurrentState = 1;

}
void fnWaitFirstDataState(tStateMachine *Apuntador_1, tCalcData *Apuntador_2)//Apuntador_1->bCurrentState 1
{
	do{
		Apuntador_2->bFirstData= Valor_1;
	}while(Apuntador_2->bFirstData >= 0xFF);

	Apuntador_1->bPreviousState = Apuntador_1->bCurrentState;
	Apuntador_1->bCurrentState = 2;

}
void fnWaitOperatorState(tStateMachine *Apuntador_1, tCalcData *Apuntador_2)//Apuntador_1->bCurrentState 2
{
	Apuntador_2->bOperator = Operacion;
	if(Apuntador_2->bOperator == '+'||Apuntador_2->bOperator == '-'||Apuntador_2->bOperator == '*'||Apuntador_2->bOperator == '/'||Apuntador_2->bOperator == '%')
	{
		Apuntador_1->bPreviousState = Apuntador_1->bCurrentState;
		Apuntador_1->bCurrentState = 3;
	}
	else
	{
		Apuntador_1->bPreviousState = Apuntador_1->bCurrentState;
		Apuntador_1->bCurrentState = 7;
	}
}
void fnWaitSecondDataState(tStateMachine *Apuntador_1, tCalcData *Apuntador_2)//Apuntador_1->bCurrentState 3
{
	do{
		Apuntador_2->bSecondData = Valor_2;
	}while(Apuntador_2->bSecondData >= 0xFF);

	Apuntador_1->bPreviousState = Apuntador_1->bCurrentState;
	Apuntador_1->bCurrentState = 4;

}
void fnExecuteOperationState(tStateMachine *Apuntador_1, tCalcData *Apuntador_2)//Apuntador_1->bCurrentState 4
{
	if(Apuntador_2->bOperator == '+')//Suma
	{
		Apuntador_2->wResult = 	Apuntador_2->bFirstData + Apuntador_2->bSecondData;

	}else if(Apuntador_2->bOperator == '-')//Resta
	{
		Apuntador_2->wResult = 	Apuntador_2->bFirstData - Apuntador_2->bSecondData;

	}else if(Apuntador_2->bOperator == '*')//Multiplicacion
	{
		Apuntador_2->wResult = 	Apuntador_2->bFirstData * Apuntador_2->bSecondData;

	}else if(Apuntador_2->bOperator == '/')//Division
	{
		Apuntador_2->wResult = 	Apuntador_2->bFirstData / Apuntador_2->bSecondData;

	}else if(Apuntador_2->bOperator == '%')//Modulo
	{
		Apuntador_2->wResult = 	Apuntador_2->bFirstData % Apuntador_2->bSecondData;
	}
    Apuntador_2->bOperationCounter += 1;

    Apuntador_1->bPreviousState = Apuntador_1->bCurrentState;
    Apuntador_1->bCurrentState = 5;
}
void fnSaveResultStat(tStateMachine *Apuntador_1, tCalcData *Apuntador_2)//Apuntador_1->bCurrentState 5
{
	Apuntador_2->waDataBuffer[Apuntador_2->bOperationCounter - 1] = Apuntador_2->wResult;
	if(Apuntador_2->bOperationCounter == 5)
	{
		Apuntador_1->bPreviousState = Apuntador_1->bCurrentState;
		Apuntador_1->bCurrentState = 6;
	}
	else{
		Apuntador_1->bPreviousState = Apuntador_1->bCurrentState;
		Apuntador_1->bCurrentState = 7;
	}
}
void fnCalculateAverageState(tStateMachine *Apuntador_1, tCalcData *Apuntador_2)//Apuntador_1->bCurrentState 6
{
	unsigned char i = 0;
	unsigned char Sumatoria = 0;

	for(i = 0; i < 5; i++)
	{
		Sumatoria += Apuntador_2->waDataBuffer[i];
	}

	Apuntador_2->wResult = Sumatoria / 5;

	Apuntador_1->bPreviousState = Apuntador_1->bCurrentState;
	Apuntador_1->bCurrentState = 7;

}
void fnResetState(tStateMachine *Apuntador_1, tCalcData *Apuntador_2)//Apuntador_1->bCurrentState 7
{
	Apuntador_2->bFirstData = 0xFF;
	Apuntador_2->bOperator = 0xFF;
	Apuntador_2->bSecondData = 0xFF;
	Apuntador_2->wResult = 0xFF;

	Apuntador_1->bPreviousState = Apuntador_1->bCurrentState;
	Apuntador_1->bCurrentState = 1;
}
