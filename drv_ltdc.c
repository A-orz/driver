/*
 * LCD-TFT display controller (LTDC)
 *
 */
#include "board.h"
#include "drv_ltdc.h"
#include "stm32h7xx.h"

void ltdc_low_init(void)
{
    /* clock and pin  */
    GPIO_InitTypeDef GPIO_Initure;

    SET_BIT(RCC->APB3ENR, 0x1 << 3);
//    __HAL_RCC_LTDC_CLK_ENABLE();
//    __HAL_RCC_DMA2D_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    //��ʼ��PB5����������
    GPIO_Initure.Pin=GPIO_PIN_5;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull=GPIO_PULLUP;
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    //��ʼ��PF10
    GPIO_Initure.Pin=GPIO_PIN_10;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;
    GPIO_Initure.Pull=GPIO_NOPULL;
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Initure.Alternate=GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);

    //��ʼ��PG6,7,11
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);

    //��ʼ��PH9,10,11,12,13,14,15
    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);

    //��ʼ��PI0,1,2,4,5,6,7,9,10
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|\
                     GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10;
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);
}

int ltdc_init(void)
{
    ltdc_low_init();
    //�������Ĳ���
    struct stm32_ltdc_reg * lcd_base;
    lcd_base = (struct stm32_ltdc_reg *)(0x50001000);

    // �������Ŀ�Ⱥ͸߶�
    lcd_base->sscr = ((1-1) << 16 | (1-1));
    // ����ǰ���ȳ���
    lcd_base->bpcr = ((46 + 1 - 1) << 16 | (23 + 1 - 1));
    // �����ܵĳ���
    lcd_base->awcr = ((800 + 46 + 1 - 1) << 16 | (480 + 23 + 1 - 1));
    // �����ܵĳ���
    lcd_base->twcr = ((210 + 800 + 46 + 1 - 1) << 16 | (22 + 480 + 23 + 1 - 1));
    // ���ñ�����ɫ
    lcd_base->bccr = 0xFFFF;

    // layer1����

    // layer1
    lcd_base->l1whpcr = ((800 + 46 + 1 - 1) << 16 | (46 + 1));
    lcd_base->l1wvpcr = ((480 + 23 + 1 - 1) << 16 | (23 + 1));
    lcd_base->l1pfcr = 0x2;
    lcd_base->l1cfbar = 0xC0000000;
    lcd_base->l1cfblr = ((800 *2) << 16 | ((800 *2) + 7));
    lcd_base->l1cfblnr = 480;
    lcd_base->l1dccr = 0xFFFFFFFF;

    // layer1ʹ��
    lcd_base->l1cr |= 0x01;

    lcd_base->srcr |= 0x3;
    // ʹ������
    lcd_base->gcr |= 0x1;
    // �򿪱����
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);

    return 0;
}

void draw_point(rt_uint16_t y, rt_uint16_t x, rt_uint16_t color)
{
    rt_uint32_t temp;

    temp = (x * 800)+ y;

    *(rt_uint16_t *)(0xC0000000 + temp * 2) = color;
}
