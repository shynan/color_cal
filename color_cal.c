/*
 * @Date: 2022-09-08 17:59:45
 * @Author: tao.wang
 * @LastEditors: tao.wang
 * @LastEditTime: 2022-09-08 19:11:06
 * @FilePath: \tif_color\test.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

#define RED_VALUE 30
#define GREEN_VALUE 30
#define BLUE_VALUE 30

int main(int argc, char *argv[])
{
    int red_value = RED_VALUE;
    int green_value = GREEN_VALUE;
    int blue_value = BLUE_VALUE;
    
    if (argc < 2)
    {
        printf("please input file name xxx.tif\r\n");
        return -1;
    }

    if (strcmp(argv[1], "-h") == 0)
    {
        printf("***************************\r\n");
        printf("-r: 红色分量阈值[0-255],默认值30,可省略\r\n");
        printf("-g: 绿色分量阈值[0-255],默认值30,可省略\r\n");
        printf("-b: 蓝色分量阈值[0-255],默认值30,可省略\r\n");
        printf("***************************\r\n");
        printf("./color_cal xxx.tif -r 30 -g 30 -b 30\r\n");
        return 0;
    }

    if (argc > 2)
    {
        for (uint8_t i = 2; i < argc - 1; i++)
        {
            if (strcmp(argv[i], "-r") == 0)
            {
                i++;
                red_value = atoi(argv[i]);
            }
            else if (strcmp(argv[i], "-g") == 0)
            {
                i++;
                green_value = atoi(argv[i]);
            }
            else if (strcmp(argv[i], "-b") == 0)
            {
                i++;
                blue_value = atoi(argv[i]);
            }
            else if (strcmp(argv[i], "-h") == 0)
            {
                printf("./color_cal xxx.tif -r 30 -g 30 -b 30\r\n");
                return 0;
            }
        }
    }

    TIFF *tif = TIFFOpen(argv[1], "r");
    if (tif)
    {
        uint32_t w, h;
        uint32_t npixels;
        uint32_t *raster;

        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
        npixels = w * h;
        uint32_t red_count = 0;
        uint32_t green_count = 0;
        uint32_t yellow_count = 0;
        uint32_t blue_count = 0;

        raster = (uint32_t *)_TIFFmalloc(npixels * sizeof(uint32_t));
        if (raster != NULL)
        {
            if (TIFFReadRGBAImage(tif, w, h, raster, 0))
            {
                for (uint32_t i = 0; i < npixels; i++)
                {
                    uint8_t red = raster[i] & 0x000000ff;
                    uint8_t green = (raster[i] & 0x0000ff00) >> 8;
                    uint8_t blue = (raster[i] & 0x00ff0000) >> 16;
                    if (red > red_value && green < green_value && blue < blue_value)
                    {
                        red_count++;
                    }
                    else if (green > green_value && red < red_value && blue < blue_value)
                    {
                        green_count++;
                    }
                    else if (blue > blue_value && red < red_value && green < green_value)
                    {
                        blue_count++;
                    }
                    else if (red > red_value && green > green_value && blue < blue_value)
                    {
                        yellow_count++;
                    }
                }
                printf("red:%d green:%d blue:%d yellow:%d all:%d\r\n", red_count, green_count, blue_count, yellow_count, npixels);
            }
            _TIFFfree(raster);
        }
        else
        {
            TIFFClose(tif);
            return -1;
        }
        TIFFClose(tif);
    }
    else
    {
        return -1;
    }
    return 0;
}
