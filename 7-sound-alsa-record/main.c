/*

This example reads from the default PCM device
and writes to standard output for 5 seconds of data.

*/

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
int info() {
  int val;

  printf("ALSA library version: %s\n",
          SND_LIB_VERSION_STR);

  printf("\nPCM stream types:\n");
  for (val = 0; val <= SND_PCM_STREAM_LAST; val++)
    printf("  %s\n",
      snd_pcm_stream_name((snd_pcm_stream_t)val));

  printf("\nPCM access types:\n");
  for (val = 0; val <= SND_PCM_ACCESS_LAST; val++)
    printf("  %s\n",
      snd_pcm_access_name((snd_pcm_access_t)val));

  printf("\nPCM formats:\n");
  for (val = 0; val <= SND_PCM_FORMAT_LAST; val++)
    if (snd_pcm_format_name((snd_pcm_format_t)val)
      != NULL)
      printf("  %s (%s)\n",
        snd_pcm_format_name((snd_pcm_format_t)val),
        snd_pcm_format_description(
                           (snd_pcm_format_t)val));

  printf("\nPCM subformats:\n");
  for (val = 0; val <= SND_PCM_SUBFORMAT_LAST;
       val++)
    printf("  %s (%s)\n",
      snd_pcm_subformat_name((
        snd_pcm_subformat_t)val),
      snd_pcm_subformat_description((
        snd_pcm_subformat_t)val));

  printf("\nPCM states:\n");
  for (val = 0; val <= SND_PCM_STATE_LAST; val++)
    printf("  %s\n",
           snd_pcm_state_name((snd_pcm_state_t)val));

  return 0;
}

int main()
{
  long loops;
  int rc;
  int size;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int rate, exact_rate;
  int dir;
  snd_pcm_uframes_t periodsCount;
  char *buffer;
  info();
  /* Open PCM device for recording (capture). */
  rc = snd_pcm_open(&handle, "default",
                    SND_PCM_STREAM_CAPTURE, 0);
  if (rc < 0)
  {
    fprintf(stderr,
            "unable to open pcm device: %s\n",
            snd_strerror(rc));
    exit(1);
  }

  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params,
                               SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params,
                               SND_PCM_FORMAT_S16_LE);

  snd_pcm_hw_params_set_channels(handle, params, 1);

  rate = 2000;
  exact_rate = rate;
  fprintf(stderr, "1 Channel %d Hz\n", rate);
  snd_pcm_hw_params_set_rate_near(handle, params,
                                  &exact_rate, &dir);
  if (rate != exact_rate)
  {
    fprintf(stderr, "The rate %d Hz is not supported by your hardware.\n \
                       ==> Using %d Hz instead.\n",
            rate, exact_rate);
  }

  periodsCount = 16;
  snd_pcm_hw_params_set_period_size_near(handle,
                                         params, &periodsCount, &dir);
  fprintf(stderr, "%d periods (%d)\n", periodsCount, dir);
  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0)
  {
    fprintf(stderr,
            "unable to set hw parameters: %s\n",
            snd_strerror(rc));
    exit(1);
  }
  fprintf(stderr, "Device set!\n");

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params,
                                    &periodsCount, &dir);
  fprintf(stderr, "period %d\n", periodsCount);
  size = periodsCount * 2; /* 1 bytes/sample, 1 channels */
  buffer = (char *)malloc(size);

  /* We want to loop for 5 seconds */
  snd_pcm_hw_params_get_period_time(params,
                                    &rate, &dir);
  loops = 3000000 / rate;
  fprintf(stderr, "Period time %d, loops %d\n", rate, loops);
  int f = open("record.raw", O_CREAT | O_TRUNC | O_WRONLY, 0777);
  if (f == -1)
  {
    fprintf(stderr, "Can't open file\n");
    return 1;
  }

  while (loops > 0)
  {
    loops--;
    fprintf(stderr, "LOOP %d\n", loops);
    rc = snd_pcm_readi(handle, buffer, periodsCount);
    if (rc == -EPIPE)
    {
      /* EPIPE means overrun */
      fprintf(stderr, "overrun occurred\n");
      snd_pcm_prepare(handle);
    }
    else if (rc < 0)
    {
      fprintf(stderr,
              "error from read: %s\n",
              snd_strerror(rc));
    }
    else if (rc != (int)periodsCount)
    {
      fprintf(stderr, "short read, read %d frames\n", rc);
    }
    rc = write(f, buffer, size);
    if (rc != size)
      fprintf(stderr,
              "short write: wrote %d bytes\n", rc);
  }

  close(f);
  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);

  return 0;
}
