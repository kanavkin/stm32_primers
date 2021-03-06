/*********************************
 * stm32 timer led example main.c
 ********************************/

#include "stm32_libs/stm32f10x.h"
#include "stm32_libs/boctok/stm32_gpio.h"
#include "stm32_libs/boctok/boctok_types.h"


/* User defined function prototypes */
void led_toggle(void);


int main(void)
{
  /* Enable PORT C clock */
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

  /* Initialize GPIOC PIN13 */
  GPIO_configure(GPIOC, 13, GPIO_OUT_OD_2MHZ);

  /* Turn off LED to start with */
  GPIOC->BSRR= (uint32_t) (1 << 13);
  
  /* Enable TIM2 clock */
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  /* Enable TIM2 global interrupt */
    NVIC->ISER[0] |= 0x10000000;

  /* Frequency after prescaler = 72MHz/(7199+1) = 10KHz.
   * Compare register = 5000 so a compare match occurs every 0.5s -> T= 0,1 ms
   */
      TIM2->PSC = (uint16_t) 7199;
      TIM2->CCR1 = (uint16_t)5000;

      /* Enable Capture/Compare 1 interrupt */
      TIM2->DIER |= (uint16_t)0x0002;

  /* Enable TIM2 counter (in upcount mode) */
  TIM2->CR1 |= (uint16_t)0x0001;


  while(1)
    {
      /* Do nothing, all happens in ISR */
    }
}

/* Toggle LED */
void led_toggle(void)
{
  /* If PORTC BIT 13 is set, led is off */
  if(GPIOC->ODR & (uint32_t)(1<<13))
    {
      GPIOC->BRR= (uint32_t) (1 << 13);
    }
  else
    {
      GPIOC->BSRR= (uint32_t) (1 << 13);
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

