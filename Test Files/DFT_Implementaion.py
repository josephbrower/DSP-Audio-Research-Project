
import numpy as np
from scipy.io import wavfile
from scipy.fft import fft

#--- Under Development --#
def DFT(data):
    N = len(data)
    for i, (L, R) in enumerate(data):
        pass

#wav file locations
audiofile = "Test Files/Dry Audio/Song Dry.wav"

#Open and store wav files in list, and sample rate {samples per second}
samplerate, data_stereo = wavfile.read(audiofile)

data_stereo = data_stereo[:100][:100]

print(data_stereo)
fft_data = fft(data_stereo)
#print(fft_data.shape)

DFT(data_stereo)