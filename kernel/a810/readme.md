#  2018-4-10

+ 1.机制的实现  4-10
* 2.效果的实现
* 3.扩展性的实现
* 4.整体优化和测试

---
### 1.机制的实现
重新整理led_effect结构体定义,
要不要使用stack
<http://example.com/>
https://www.appinn.com/markdown/

`具体代码如下`

    static int ad193x_mute(struct snd_soc_dai *dai, int mute)
    {
      struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(dai->codec);

      if (mute)
        regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL2,
                AD193X_DAC_MASTER_MUTE,
                AD193X_DAC_MASTER_MUTE);
      else
        regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL2,
                AD193X_DAC_MASTER_MUTE, 0);

      return 0;
    }


> This is a blockquote with two paragraphs. Lorem ipsum dolor sit amet,
 consectetuer adipiscing elit. Aliquam hendrerit mi posuere lectus.
 Vestibulum enim wisi, viverra nec, fringilla in, laoreet vitae, risus.

> Donec sit amet nisl. Aliquam semper ipsum sit amet velit. Suspendisse
> id sem consectetuer libero luctus adipiscing.


> This is a blockquote with two paragraphs. Lorem ipsum dolor sit amet,
consectetuer adipiscing elit. Aliquam hendrerit mi posuere lectus.
Vestibulum enim wisi, viverra nec, fringilla in, laoreet vitae, risus.
Donec sit amet nisl. Aliquam semper ipsum sit amet velit. Suspendisse
id sem consectetuer libero luctus adipiscing.


### 2.效果的实现
+ **Question-1**
内核中实现：
每次ioctl传入cmd=LEDS_EFFECT_WAKE_UP和参数arg=direction1
会唤醒wake_up一个阻塞的进程wait_event_interruptible(),
然后根据direction去实现当前角度的灯光闪烁5次
期间如果有其他的事件如又ioctl传入另外一个方向
direction2就打断当前的闪烁并重现根据direction2去实现灯光闪烁5次
+ **Question-2**
todolist:  kthread_should_stop是什么 ?
+ **Question-3**
当前设计：

![@good](/home/jiangxj/github/ArrayZhangWork/wiki/img/1495765358637.png)
------------
