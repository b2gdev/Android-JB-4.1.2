/*
 * omap3beagle.c  --  SoC audio for OMAP3 Beagle
 *
 * Author: Steve Sakoman <steve@sakoman.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <linux/clk.h>
#include <linux/platform_device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/jack.h>

#include <asm/mach-types.h>
#include <mach/hardware.h>
#include <mach/gpio.h>
#include <plat/mcbsp.h>

#include "omap-mcbsp.h"
#include "omap-pcm.h"

#include "../codecs/wm8994.h"		/* {PS} */


static int omap3beagle_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	unsigned int fmt,pll_out;
	int ret;			

	//printk("%s - %s\n",__FILE__, __FUNCTION__);
	#ifdef SND_OMAP_SOC_OMAP3_BEAGLE_DEBUG
		printk("[%08u] - %s - %s\n", (unsigned int)jiffies, __FILE__, __FUNCTION__); /* {PS} */
	#endif	

// {RD} BEGIN:
	/*switch (params_channels(params)) {
	case 2: // Stereo I2S mode 
		fmt =	SND_SOC_DAIFMT_I2S |
			SND_SOC_DAIFMT_NB_NF |
			SND_SOC_DAIFMT_CBM_CFM;
		break;
	case 4: // Four channel TDM mode 
		fmt =	SND_SOC_DAIFMT_DSP_A |
			SND_SOC_DAIFMT_IB_NF |
			SND_SOC_DAIFMT_CBM_CFM;
		break;
	default:
		return -EINVAL;
	}*/
	fmt =	SND_SOC_DAIFMT_I2S |
			SND_SOC_DAIFMT_NB_NF |
			SND_SOC_DAIFMT_CBM_CFM;
			
	if (params_rate(params) == 8000 || params_rate(params) == 11025)
		pll_out = params_rate(params) * 512;
	else
		pll_out = params_rate(params) * 256;
		
	//printk("{RD} %s - %s: params_rate(params):%d\n",__FILE__, __FUNCTION__,params_rate(params));
	//printk("{RD} %s - %s: pll_out:%d\n",__FILE__, __FUNCTION__,pll_out);
// {RD} END:

	/* Set codec DAI configuration */
	ret = snd_soc_dai_set_fmt(codec_dai, fmt);
	if (ret < 0) {
		printk(KERN_ERR "can't set codec DAI configuration\n");
		return ret;
	}

	/* Set cpu DAI configuration */
	ret = snd_soc_dai_set_fmt(cpu_dai, fmt);
	if (ret < 0) {
		printk(KERN_ERR "can't set cpu DAI configuration\n");
		return ret;
	}

	/* Set the codec system clock for DAC and ADC */
	
	//printk("{RD} %s - %s: CP1\n",__FILE__, __FUNCTION__);
	
// {RD} BEGIN:	
	/*// {PS} BEGIN:
	ret = snd_soc_dai_set_sysclk(codec_dai, WM8994_SYSCLK_MCLK1, 24576000,
				     SND_SOC_CLOCK_IN);	
	#if 0
	ret = snd_soc_dai_set_sysclk(codec_dai, 0, 26000000,
				     SND_SOC_CLOCK_IN);
	#endif
	// {PS} END:*/
	
	ret = snd_soc_dai_set_pll(codec_dai, WM8994_FLL1, WM8994_FLL_SRC_MCLK1,
					24576000, pll_out);
	if (ret < 0){
		printk(KERN_ERR "can't set snd_soc_dai_set_pll configuration\n");
		return ret;
	}

	ret = snd_soc_dai_set_sysclk(codec_dai, WM8994_SYSCLK_FLL1,
					pll_out, SND_SOC_CLOCK_IN);
	if (ret < 0){
		printk(KERN_ERR "can't set snd_soc_dai_set_sysclk configuration\n");
		return ret;
	}
// {RD} END:	

//printk("{RD} %s - %s: CP2\n",__FILE__, __FUNCTION__);

	if (ret < 0) {
		printk(KERN_ERR "can't set codec system clock\n");
		return ret;
	}

	return 0;
}

static struct snd_soc_ops omap3beagle_ops = {
	.hw_params = omap3beagle_hw_params,
};

/* {PS} BEGIN: */
#ifdef CONFIG_SND_SOC_WL1271BT
static int omap3tcbin_wl1271bt_pcm_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int ret;

	/* Set cpu DAI configuration for WL1271 Bluetooth codec */
	ret = snd_soc_dai_set_fmt(cpu_dai,
				SND_SOC_DAIFMT_DSP_B |
				SND_SOC_DAIFMT_NB_NF |
				SND_SOC_DAIFMT_CBM_CFM);
	if (ret < 0) {
		printk(KERN_ERR "Can't set cpu DAI configuration for "\
				"WL1271 Bluetooth codec\n");
		return ret;
	}

	return 0;
}

static struct snd_soc_ops omap3tcbin_wl1271bt_pcm_ops = {
	.hw_params = omap3tcbin_wl1271bt_pcm_hw_params,
};
#endif
/* {PS} END: */

/* Digital audio interface glue - connects codec <--> CPU */
static struct snd_soc_dai_link omap3beagle_dai[] = {
/* {PS} BEGIN: */
{
	.name 		 	= "WM8994 I2S",
	.stream_name 	= "WM8994 Audio",
	.cpu_dai_name 	= "omap-mcbsp-dai.1",
	.platform_name 	= "omap-pcm-audio",
	.codec_dai_name = "wm8994-aif1",
	.codec_name 	= "wm8994-codec",	
	.ops 		 	= &omap3beagle_ops,
},
#if 0		
	.name = "TWL4030",
	.stream_name = "TWL4030",
	.cpu_dai_name = "omap-mcbsp-dai.1",
	.platform_name = "omap-pcm-audio",
	.codec_dai_name = "twl4030-hifi",
	.codec_name = "twl4030-codec",
	.ops = &omap3beagle_ops,
#endif
/* {PS} END: */
	
/* {PS} BEGIN: */
#ifdef CONFIG_SND_SOC_WL1271BT
{
	.name		= "WL1271BT",
	.stream_name	= "WL1271BT",
	.cpu_dai_name	= "omap-mcbsp-dai.0",
	.codec_dai_name	= "wl1271bt",
	.platform_name	= "omap-pcm-audio",
	.codec_name	= "wl1271bt-dummy-codec",
	.ops		= &omap3tcbin_wl1271bt_pcm_ops,
},
#endif
/* {PS} END: */
};

/* Audio machine driver */
static struct snd_soc_card snd_soc_omap3beagle = {
	.name = "omap3beagle",
	.owner = THIS_MODULE,
	.dai_link = omap3beagle_dai,			/* {PS} : &omap3beagle_dai, */
	.num_links = ARRAY_SIZE(omap3beagle_dai), 	/* {PS} : 1, */
};

static struct platform_device *omap3beagle_snd_device;

static int __init omap3beagle_soc_init(void)
{
	int ret;
	//static struct snd_soc_jack tcbin_headset;
	
	//printk("{RD} %s - %s\n",__FILE__, __FUNCTION__);
	#ifdef SND_OMAP_SOC_OMAP3_BEAGLE_DEBUG
		printk("[%08u] - %s - %s\n", (unsigned int)jiffies, __FILE__, __FUNCTION__); /* {PS} */
	#endif	
	
	if (!(machine_is_omap3_beagle() || machine_is_devkit8000()))
		return -ENODEV;
	pr_info("OMAP3 Beagle/Devkit8000 SoC init\n");

	omap3beagle_snd_device = platform_device_alloc("soc-audio", -1);
	if (!omap3beagle_snd_device) {
		printk(KERN_ERR "Platform device allocation failed\n");
		return -ENOMEM;
	}

	platform_set_drvdata(omap3beagle_snd_device, &snd_soc_omap3beagle);

	ret = platform_device_add(omap3beagle_snd_device);
	if (ret)
		goto err1;

/*// {RD} BEGIN:		
	//ret = snd_soc_dai_set_sysclk(codec_dai, WM8994_SYSCLK_MCLK2,
	//			     32768, SND_SOC_CLOCK_IN);
	//if (ret < 0)
	//	return ret;
	printk("{RD} %s - %s snd_soc_omap3beagle:%p\n",__FILE__, __FUNCTION__,&snd_soc_omap3beagle);
	
	ret = snd_soc_jack_new(&snd_soc_omap3beagle, "Headset",
			       SND_JACK_HEADSET | SND_JACK_MECHANICAL |
			       SND_JACK_BTN_0 | SND_JACK_BTN_1 |
			       SND_JACK_BTN_2 | SND_JACK_BTN_3 |
			       SND_JACK_BTN_4 | SND_JACK_BTN_5,
			       &tcbin_headset);
			       
	printk("{RD} %s - %s jack done\n",__FILE__, __FUNCTION__);
	
	if (ret)
		goto err1;

	/// This will check device compatibility itself
	wm8994_mic_detect(&snd_soc_omap3beagle, &tcbin_headset, 2, SND_JACK_HEADSET, SND_JACK_HEADPHONE);
	
// {RD} END:*/

	return 0;

err1:
	printk(KERN_ERR "Unable to add platform device\n");
	platform_device_put(omap3beagle_snd_device);

	return ret;
}

static void __exit omap3beagle_soc_exit(void)
{
	//printk("{RD} %s - %s\n",__FILE__, __FUNCTION__);
	#ifdef SND_OMAP_SOC_OMAP3_BEAGLE_DEBUG
		printk("[%08u] - %s - %s\n", (unsigned int)jiffies, __FILE__, __FUNCTION__); /* {PS} */
	#endif	
	
	platform_device_unregister(omap3beagle_snd_device);
}

module_init(omap3beagle_soc_init);
module_exit(omap3beagle_soc_exit);

MODULE_AUTHOR("Steve Sakoman <steve@sakoman.com>");
MODULE_DESCRIPTION("ALSA SoC OMAP3 Beagle");
MODULE_LICENSE("GPL");
