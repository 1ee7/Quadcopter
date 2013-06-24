/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : USART_Print
**�\�� : �ǰe���
**��J : *USARTx, *pWord
**��X : None
**�ϥ� : USART_Print(USART1, "Hello World\r\n");
**=====================================================================================================*/
/*=====================================================================================================*/
void USART_Print( USART_TypeDef* USARTx, u8 *pWord )
{
  while(*pWord != '\0') {
    USART_SendData(USARTx, *pWord);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    pWord++;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : NumToChar
**�\�� : �N�ƭ��ন�r��
**��J : Type, Num, *CharData, NumData
**��X : None
**�ϥ� : NumToChar(Type_D, 6, TrData, Data);
**=====================================================================================================*/
/*=====================================================================================================*/
void NumToChar( u8 Type, u8 Num, u8 *CharData, s32 NumData )
{
  u8 i = 0;
  u32 Tmp[48] = {0};
  u32 NumTmp = 1;

  switch(Type) {

    case Type_B:  // 2�i��
    case Type_O:  // 8�i��
    case Type_D:  // 10�i��
    case Type_H:  // 16�i��
      for(i=0; i<Num; i++) {
        Tmp[i] = ((u32)NumData) / NumTmp;
        NumTmp = NumTmp * Type;
      }
      for(i=0; i<Num; i++) {
        CharData[Num-i-1] = Tmp[i] - Tmp[i+1]*Type;
        if(CharData[Num-i-1]>9)
          CharData[Num-i-1] += 55;  // 65-10
        else
          CharData[Num-i-1] += 48;
      }
      CharData[Num] = '\0';
      break;

    case Type_I:  // 10�i��, �����t��
      if(NumData<0) {
        CharData[0] = '-';
        NumData = (~NumData) + 1;
        for(i=1; i<Num+1; i++) {
        Tmp[i-1] = ((u32)NumData) / NumTmp;
          NumTmp = NumTmp * 10;
        }
        for(i=1; i<Num+1; i++) {
        CharData[Num-i+1] = Tmp[i-1] - Tmp[i]*10;
          CharData[Num-i+1] += 48;
        }
        CharData[Num+1] = '\0';
      }
      else {
        CharData[0] = '+';
        //NumData = (~NumData) + 1;
        for(i=1; i<Num+1; i++) {
          Tmp[i-1] = ((u32)NumData) / NumTmp;
          NumTmp = NumTmp * 10;
        }
        for(i=1; i<Num+1; i++) {
          CharData[Num-i+1] = Tmp[i-1] - Tmp[i]*10;
          CharData[Num-i+1] += 48;
        }
        CharData[Num+1] = '\0';
      }
      break;

//     case Type_F:  // 10�i��, �B�I��
//         
//       break;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
