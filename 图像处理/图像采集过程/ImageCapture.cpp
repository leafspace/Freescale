/*场中断服务子程序*/
void HREF_isr(void)
{
    PORTE_PCR0 |= PORT_PCR_ISF_MASK;   //清除中断标志
    row = 0;                           //计数变量清零
    rownum = 0;
    disable_irq(91);                   //禁止场中断
    enable_irq(87);                    //使能行中断
}

/*行中断服务子程序*/
void HSYNC_ISR()
{
    PORTA_PACR16 |= POST_PCR_ISF_MASK; //清除中断标志
    if(rownum++ > 15){                 //15行的场消隐区
        if(rownum%4 == 0){             //隔三行采集一次
            delay_us(36);              //演示确保不会采集到行消隐区
            for(k = 0;k < MAXCOLUM;++k){//循环采集
                Image[i][k]=GPIOC_PDIR;//数据地址GPIOC_PDIR
            }
            row++;
            if(row==MAXLINE){          //当采集到的行数足够时
                disable_irq(87);       //禁止行中断
                enable_irq(91);        //使能场中断
                chang_signal=1;        //标识变量
            }
        }        
    }
}