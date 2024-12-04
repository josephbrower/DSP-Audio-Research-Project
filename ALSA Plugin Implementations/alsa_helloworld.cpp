#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

//For closing the program
#include <csignal>
#include <atomic>
#include <unistd.h>

#include <iostream>

/*====================================================================================
 *                                   Joseph Brower
 * This program will capture and playack audio from the hw:2,0 plugin device
 *
 *====================================================================================
 */


//You can search for the plugin device by searching in the terminal: aplay --list-devices
//You must install the library using apt-get install libasound2-dev
//You must compile the this code with the -lasound dependency
#define PLUGIN_DEVICE "hw:2,0"//This should be the logitech headset on the USB type A port
#define BUFFER_SIZE 1024

std::atomic<bool> running(true);

// PCM -> pulse code modulation is a common way to digitize analog audio data 

void end_program(int signal){//Used for closing the program
    if(signal == SIGINT){
        running = false;
    }
}

int main(){
    // --Create and open PCM parameters--
    /* Structure that holds all configuration parameters of the PCM stream.
     * -> Audio format
     * -> Number of channels (mono, stereo)
     * -> Sample rate
     * -> Buffer size
     * */
    snd_pcm_t *capture, *playback;
    
    //Open a PCM device and setting it to capture audio (you can output audio by calling PLAYBACK instead)
    snd_pcm_open(&capture, PLUGIN_DEVICE, SND_PCM_STREAM_CAPTURE, 0);
    
    //Check to make sure the capture successfully opened a PCM device
    if(capture == nullptr){
        std::cout << "Unable to open pcm device." << std::endl;
        return -1;
    }
    
    //Open the PCM device and set it to playback
    snd_pcm_open(&playback, PLUGIN_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
    if(playback == nullptr){
        std::cout << "unable to pen PCM device." << std::endl;
        return -1;
    }
    
    // --Configure the PCM parameters for capture--
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_hw_params_alloca(&hw_params);//Alocate memory for the hardware params
    
    snd_pcm_hw_params_any(capture, hw_params);//Set to default hardware params
    /*Tells ALSA how you intend to interact with the device (read/write/both)
     * -> SND_PCM_ACCESS_RW_INTERLEAVED -> the audio data will be interleaved in memory 
     *    (common for stereo/multi-channel audio where left and right samples are alternating)
     * -> SND_PCM_ACCESS_RW_NONINTERLEAVED -> the audio data will be non-interleaved 
     *    (each channels samples will be stored in different buffers)
     * -> SND_PCM_ACCESS_MMAP_INTERLEAVED
     * -> SND_PCM_ACCESS_MMAP_NONINTERLEAVED
     * */
    snd_pcm_hw_params_set_access(capture, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    /* Tells ALSA what audio format you want to set
     * -> SND_PCM_FORMAT_S16_LE -> 16bit singed little endian (right to left)
     * -> SND_PCM_FORMAT_U8 -> 8bit unsinged
     * -> others...
     * */
    snd_pcm_hw_params_set_format(capture, hw_params, SND_PCM_FORMAT_S16_LE);
    /*Sets the sample rate in Hz and specifies if the sample rate can change (1||0)*/
    snd_pcm_hw_params_set_rate(capture, hw_params, 44100, 0);
    snd_pcm_hw_params_set_channels(capture, hw_params, 2);//Sets the number of channels (1 mono, 2 stereo, >2...)
    snd_pcm_hw_params(capture, hw_params);//Apply hw params
    
    // --Configure the PCM parameters for playback--
    // (Literaly the same as the above except the PCM structure is the playback structure)
    snd_pcm_hw_params_any(playback, hw_params);
    snd_pcm_hw_params_set_access(playback, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(playback, hw_params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_rate(playback, hw_params, 44100, 0);
    snd_pcm_hw_params_set_channels(playback, hw_params, 2);
    snd_pcm_hw_params(playback, hw_params);
    
    // --Buffer initilization and PCM prepare--
    short* buffer = (short* )malloc(BUFFER_SIZE * 2 * sizeof(short));//Create and alocate memory for buffer that will take in audio data
    
    if(buffer == nullptr){
            std::cout << "Unable to allocate memory for buffer." << std::endl;
            return -1;
    }
    
    snd_pcm_prepare(capture);//Prepare PCM device for capture
    snd_pcm_prepare(playback);//Prepare PCM device for playback
    
    // --capture and play autio--
    signal(SIGINT, end_program);//Used for closing the program
    
    std::cout << "Capturing audio (Press Ctrl+C to stop)...\n" << std::endl;
    while(running){
        int err = snd_pcm_readi(capture, buffer, BUFFER_SIZE);//Read audio samples and store them in buffer provided
        if(err < 0){
            std::cout << "Error capturing audio: ";
            std::cout << snd_strerror(err) << std::endl;//Print error message
            break;
        }
        
        err = snd_pcm_writei(playback, buffer, BUFFER_SIZE);
        if(err < 0){
            std::cout << "Error capturing audio: ";
            std::cout << snd_strerror(err) << std::endl;//Print error message
            break;
        }
        
        usleep(1000);//Sleep for 1ms to simulate real time processing
    }
    
    free(buffer);//Do not forget to dealocate your buffer!
    snd_pcm_close(capture);//Do not forget to close your PCM device!
    snd_pcm_close(playback);
    std::cout << "\nProgram completed." << std::endl;

    
    return 0;
    
}
