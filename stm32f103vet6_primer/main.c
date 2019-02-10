/*********************************
 * stm32 timer led example main.c
 ********************************/

#include "stm32_libs/stm32f10x/spl/stm32f10x.h"
#include "stm32_libs/stm32f10x/boctok/stm32f10x_gpio_boctok.h"
#include "stm32_libs/boctok_types.h"


/* User defined function prototypes */
void led_toggle(void);


int main(void)
{
  /* Enable PORT C clock */
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;

  /* Initialize GPIOC PIN13 */
  GPIO_configure(GPIOC, 13, GPIO_OUT_OD_2MHZ);

  /* Turn off LED to start with */
  GPIOC->BSRR= (uint32_t) (1 << 13);
  
  /* Enable TIM2 clock, PCLK1= 64 MHz */
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  /* Enable TIM2 global interrupt */
  NVIC->ISER[0] |= 0x10000000;

  /* Frequency after prescaler = 32MHz/(31999+1) = 1KHz.
   * Compare register = 500 so a compare match occurs every 0.5s -> T= 0,1 ms
   */
  TIM2->PSC = (uint16_t) 63999;
  TIM2->CCR1 = (uint16_t) 500;

  TIM2->EGR = TIM_EGR_UG;

  
  /* Enable Capture/Compare 1 interrupt */
  TIM2->DIER |= (uint16_t)0x0002;

  /* Enable TIM2 counter (in upcount mode) */
  TIM2->CR1 |= (uint16_t)0x0001;

  // DEBUG enable MCO (hse)
  //  GPIO_configure(GPIOA, 8, GPIO_AF_PP_50MHZ);
  //   RCC->CFGR |= RCC_CFGR_MCO_SYSCLK;
  
   U32 delay;

  
  while(1)
    {
       /*
	26720 gives 40 ms @ 8MHz (-> 12 cycles/ NOP) (system_stm32f10x_hsi)
	or 5,193 ms @ 8 MHz (-> 1,77 cycles / NOP) (system_stm32f10x.c)
      */
      /*
      for(delay=0; delay < 26720; delay++)
	{
	  asm volatile ("nop");
	}
        gpio_set_pin(GPIOC, 13, TOGGLE);
      */
    }
}


/* Timer 2 Interrupt Service Routine */
void TIM2_IRQHandler(void)
{
  /* Toggle LED if TIM2's Capture/Compare 1 interrupt occurred. 
  if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    {
      TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

      led_toggle();
    }
  */

  TIM2->CNT= (uint16_t) 0;
  TIM2->SR= (uint16_t) 0;

  //led_toggle();
  gpio_set_pin(GPIOC, 13, TOGGLE);
}

