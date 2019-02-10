/*********************************
 * stm32 timer led example main.c
 ********************************/

#include "stm32_libs/boctok_types.h"
#include "stm32_libs/stm32f10x/stm32f10x.h"
#include "stm32_libs/stm32f10x/boctok/stm32f10x_gpio_boctok.h"



int main(void)
{
  // Enable PORT A clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

  // Enable TIM2 clock, PCLK1= 64 MHz
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  
  // Initialize GPIOA Pin 11
  GPIO_configure(GPIOA, 11, GPIO_OUT_PP_2MHZ);

  // Turn off LED to start with
  gpio_set_pin(GPIOA, 11, OFF);
  
  //enable timer 2 irq (prio 2)
  NVIC_SetPriority(TIM2_IRQn, 1UL);
  NVIC_ClearPendingIRQ(TIM2_IRQn);
  NVIC_EnableIRQ(TIM2_IRQn);

  /*
  Frequency after prescaler = 64MHz/(63999+1) = 1KHz.
  Compare register = 500 so a compare match occurs every 0.5s -> T= 0,1 ms
  */
  TIM2->PSC = (uint16_t) 63999;
  TIM2->CCR1 = (uint16_t) 500;

  //generate update event to write new prescaler and compare value
  TIM2->EGR = TIM_EGR_UG;

  // enable Capture/Compare 1 interrupt
  TIM2->DIER |= (uint16_t)0x0002;

  // enable TIM2 counter (in upcount mode)
  TIM2->CR1 |= (uint16_t)0x0001;
  

  while(1)
    {

    }
}


/* Timer 2 Interrupt Service Routine */
void TIM2_IRQHandler(void)
{
  if( TIM2->SR & TIM_SR_CC1IF)
    {
      //clear irq pending bit
      TIM2->SR= (U16) ~TIM_SR_CC1IF;

      //reset counter
      TIM2->CNT= (U16) 0;

      /**
      toggle led
      */
      gpio_set_pin(GPIOA, 11, TOGGLE);

    }
}

