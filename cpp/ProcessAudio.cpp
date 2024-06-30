#include "ProcessAudio.hpp"
#include "portaudio.h"
#include <cstdio>
#include <memory>
#include <condition_variable>
#include <queue>

std::mutex gMutex_audio_buffer;
std::queue<float> AudioBuffer;
std::mutex Mutex_enough_buffer;
std::condition_variable cond_var;
PaStream *stream;

static int patestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    std::queue<float> *data = (std::queue<float>*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;

    std::unique_lock<std::mutex> lock(Mutex_enough_buffer);
    if( data->size() < framesPerBuffer * 2)
    {
        cond_var.wait(lock);
    }

    gMutex_audio_buffer.lock();
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = data->front();  /* left */
        data->pop();

        *out++ = data->front();  /* right */
        data->pop();
    }
    gMutex_audio_buffer.unlock();

    return paContinue;
}

int PortAudio_initialize()
{
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)
    PaStreamParameters outputParameters;
    PaError err;
    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        goto error;
    }
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              NULL, /* no input */
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              patestCallback,
              &AudioBuffer );
    if( err != paNoError ) goto error;

//    sprintf( data.message, "No Message" );
//    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
//    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

//    printf("Play for %d seconds.\n", NUM_SECONDS );
//    Pa_Sleep( NUM_SECONDS * 1000 );

error:
    Pa_Terminate();
    fprintf( stderr, "An error occurred while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;

}

int PortAudio_stop_and_terminate()
{
    PaError err;
    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();
    printf("Test finished.\n");

    return err;
    
error:
    Pa_Terminate();
    fprintf( stderr, "An error occurred while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}