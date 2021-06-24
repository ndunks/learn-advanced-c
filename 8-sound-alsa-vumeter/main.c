/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <alsa/asoundlib.h>
// https://github.com/certik/record/blob/master/arecord.c
void print_vu_meter_mono(int perc, int maxperc)
{
  const int bar_length = 50;
  char line[80];
  int val;

  for (val = 0; val <= perc * bar_length / 100 && val < bar_length; val++)
    line[val] = '#';
  for (; val <= maxperc * bar_length / 100 && val < bar_length; val++)
    line[val] = ' ';
  line[val] = '+';
  for (++val; val <= bar_length; val++)
    line[val] = ' ';
  if (maxperc > 99)
    sprintf(line + val, "| MAX");
  else
    sprintf(line + val, "| %02i%%", maxperc);
  fputs(line, stdout);
  if (perc > 100)
    printf(" !clip  ");
}
int main()
{
  int i, a, err, dir;
  unsigned int period_time;
  int channel = 1, graphsize;
  unsigned char *buf, zero_based = 0, avg, max_peak;
  unsigned long total;

  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  snd_pcm_uframes_t buf_frame_count = 1600, buf_size;
  snd_pcm_format_t format = SND_PCM_FORMAT_U8;
  unsigned int sample_rate = 8000, sample_rate_exact;
  unsigned char mask_silence = snd_pcm_format_silence(format);
  if ((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0)) < 0)
    return printf("%s\n", snd_strerror(err));

  if ((err = snd_pcm_hw_params_malloc(&params)) < 0)
    return printf("%s\n", snd_strerror(err));

  if ((err = snd_pcm_hw_params_any(handle, params)) < 0)
    return printf("%s\n", snd_strerror(err));

  if ((err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    return printf("%s\n", snd_strerror(err));

  if ((err = snd_pcm_hw_params_set_format(handle, params, format)) < 0)
    return printf("cannot set sample format (%s)\n", snd_strerror(err));

  sample_rate_exact = sample_rate;
  if ((err = snd_pcm_hw_params_set_rate_near(handle, params, &sample_rate_exact, 0)) < 0)
    return printf("cannot set sample rate (%s)\n", snd_strerror(err));

  if (sample_rate != sample_rate_exact)
    printf("Sample rate %d Hz --> %d Hz\n", sample_rate, sample_rate_exact);

  if ((err = snd_pcm_hw_params_set_channels(handle, params, channel)) < 0)
    return printf("cannot set channel count (%s)\n", snd_strerror(err));

  if ((err = snd_pcm_hw_params_set_period_size_near(handle, params, &buf_frame_count, NULL)) < 0)
    return printf("cannot set period (%s)\n", snd_strerror(err));

  snd_pcm_hw_params_get_period_size(params, &buf_frame_count, NULL);

  buf_size = channel * buf_frame_count * snd_pcm_format_width(format) / 8;

  if ((err = snd_pcm_hw_params_set_buffer_size_near(handle, params, &buf_size)) < 0)
    return printf("cannot buffer size (%s)\n", snd_strerror(err));

  if ((err = snd_pcm_hw_params(handle, params)) < 0)
    return printf("cannot set parameters (%s)\n", snd_strerror(err));

  snd_pcm_hw_params_get_period_time(params, &period_time, NULL);
  snd_pcm_hw_params_get_buffer_size(params, &buf_size);
  printf("%d Chanel, %d Hz, %d bit/hz. %d frames (%u ns)\n", channel, sample_rate_exact, snd_pcm_format_width(format), buf_frame_count, period_time);

  snd_pcm_hw_params_free(params);
  buf = malloc(buf_size);

  if ((err = snd_pcm_prepare(handle)) < 0)
    return printf("cannot prepare audio interface for use (%s)\n", snd_strerror(err));
  clock_t start, tick;
  int f = open("record.raw", O_CREAT | O_TRUNC | O_WRONLY);
  for (i = 0; i < 10000000 / (period_time > 0 ? period_time : 1); ++i)
  //while(1)
  {
    start = clock();
    err = snd_pcm_readi(handle, buf, buf_frame_count);
    tick = clock() - start;

    if (err != buf_frame_count)
    {
      fprintf(stderr, "read from audio interface failed %d != %d (%s)\n",
              err, buf_frame_count, snd_strerror(err));
      exit(1);
    }

    total = 0UL;
    avg = 0;
    max_peak = 0;
    for (a = 0; a < buf_frame_count; a++)
    {
      zero_based = (unsigned char)abs(buf[a] - mask_silence);
      total += zero_based;
      if (zero_based > max_peak)
      {
        max_peak = zero_based;
      }
    }

    if (total > 0)
    {
      avg = total / buf_frame_count;
    }
    if (zero_based < avg)
    {
      zero_based = avg;
    }
    putchar('\r');
    print_vu_meter_mono(avg * 100 / mask_silence, max_peak * 100 / mask_silence);
    fflush(stdout);
    write(f, buf, buf_frame_count);
  }
  printf("\n");
  close(f);
  free(buf);
  snd_pcm_drain(handle);
  snd_pcm_close(handle);

  return 0;
}
